#pragma once

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <chrono>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "Common/Linux/Common.hpp"

class PipeWriter {
private:
    pid_t pid      = -1;
    int   write_fd = -1;

    static void ignoreSigpipeOnce() {
        // Ignore SIGPIPE globally: write() will return -1/EPIPE instead of terminating the process.
        static bool done = false;
        if (!done) {
            struct sigaction sa{};
            sa.sa_handler = SIG_IGN;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sigaction(SIGPIPE, &sa, nullptr);
            done = true;
        }
    }

    static void setCloExec(int fd) {
        int flags = fcntl(fd, F_GETFD);
        if (flags < 0)
            throw std::runtime_error("'fcntl(F_GETFD)' failed");
        if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) < 0)
            throw std::runtime_error("'fcntl(F_SETFD, FD_CLOEXEC)' failed");
    }

    static bool isPidAlive(pid_t p) {
        if (p <= 0)
            return false;
        // kill(pid, 0) checks existence/permissions without sending a signal.
        if (kill(p, 0) == 0)
            return true;
        return errno != ESRCH;
    }

public:
    PipeWriter() { ignoreSigpipeOnce(); }
    ~PipeWriter() { stop(); }

    void start(const std::string& pipeline) {
        stop();// Ensure clean state before starting.

        // Build argv in parent process (safer than doing allocations in child after fork).
        std::vector< std::string > argsStr = lineCmdToArgs(pipeline);
        if (argsStr.empty() || argsStr[0].empty())
            throw std::runtime_error("Pipeline command is empty");

        std::vector< char* > argv;
        argv.reserve(argsStr.size() + 1);
        for (auto& s : argsStr)
            argv.push_back(const_cast< char* >(s.c_str()));
        argv.push_back(nullptr);

        int stdin_pipe[2];
        int exec_pipe[2];

        if (pipe(stdin_pipe) < 0)
            throw std::runtime_error("'pipe(stdin_pipe)' failed");

        if (pipe(exec_pipe) < 0) {
            close(stdin_pipe[0]);
            close(stdin_pipe[1]);
            throw std::runtime_error("'pipe(exec_pipe)' failed");
        }

        // Make exec_pipe[1] close-on-exec: if execvp succeeds, parent will see EOF on exec_pipe[0].
        setCloExec(exec_pipe[1]);

        pid = fork();
        if (pid < 0) {
            close(stdin_pipe[0]);
            close(stdin_pipe[1]);
            close(exec_pipe[0]);
            close(exec_pipe[1]);
            throw std::runtime_error("'fork' failed");
        }

        if (pid == 0) {
            // --- Child process ---
            // Redirect stdin to the read end of stdin_pipe.
            close(stdin_pipe[1]);
            if (dup2(stdin_pipe[0], STDIN_FILENO) < 0) {
                int err = errno;
                (void)!write(exec_pipe[1], &err, sizeof(err));
                _exit(127);
            }
            close(stdin_pipe[0]);

            // Close parent side of exec pipe read end.
            close(exec_pipe[0]);

            // Execute the program.
            execvp(argv[0], argv.data());

            // If execvp returns, it failed.
            int err = errno;
            (void)!write(exec_pipe[1], &err, sizeof(err));
            _exit(127);
        }

        // --- Parent process ---
        close(stdin_pipe[0]);
        close(exec_pipe[1]);

        // Wait for exec result: EOF => success, or errno payload => failure.
        int     child_errno = 0;
        ssize_t r           = read(exec_pipe[0], &child_errno, sizeof(child_errno));
        close(exec_pipe[0]);

        if (r > 0) {
            // Child failed to exec. Clean up and throw a useful error.
            close(stdin_pipe[1]);
            stdin_pipe[1] = -1;

            int status = 0;
            (void)waitpid(pid, &status, 0);
            pid = -1;

            throw std::runtime_error(std::string("execvp failed: ") + strerror(child_errno) +
                                     " (errno=" + std::to_string(child_errno) + ")");
        }

        write_fd = stdin_pipe[1];
    }

    void stop() {
        // Never throw from stop/destructor.
        if (write_fd >= 0) {
            // Closing stdin signals EOF to the child pipeline (often the cleanest stop).
            close(write_fd);
            write_fd = -1;
        }

        if (pid > 0) {
            // Try graceful stop first.
            kill(pid, SIGTERM);

            // Wait a bit; then force kill if still alive.
            for (int i = 0; i < 50; ++i) {// ~500ms total
                int   status = 0;
                pid_t w      = waitpid(pid, &status, WNOHANG);
                if (w == pid) {
                    pid = -1;
                    return;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }

            kill(pid, SIGKILL);
            (void)waitpid(pid, nullptr, 0);
            pid = -1;
        }
    }

    bool isReady() const { return (write_fd >= 0) && isPidAlive(pid); }

protected:
    void writeData(const void* data, size_t size, int timeout_ms = 2000) {
        if (write_fd < 0)
            throw std::runtime_error("Write file descriptor is invalid");

        if (!isPidAlive(pid))
            throw std::runtime_error("Child process is not alive");

        const uint8_t* p    = static_cast< const uint8_t* >(data);
        size_t         left = size;

        while (left > 0) {
            struct pollfd pfd{};
            pfd.fd     = write_fd;
            pfd.events = POLLOUT | POLLERR | POLLHUP;

            int pr = poll(&pfd, 1, timeout_ms);
            if (pr == 0) {
                throw std::runtime_error("writeData timeout: pipe is not writable (consumer may not read stdin)");
            }
            if (pr < 0) {
                if (errno == EINTR)
                    continue;
                throw std::runtime_error(std::string("'poll' failed: ") + strerror(errno));
            }

            if (pfd.revents & (POLLERR | POLLHUP)) {
                throw std::runtime_error("Pipe is broken/hung up (child may have exited or closed stdin)");
            }

            ssize_t w = ::write(write_fd, p, left);
            if (w < 0) {
                if (errno == EINTR)
                    continue;
                if (errno == EPIPE) {
                    throw std::runtime_error("Broken pipe (EPIPE): child closed stdin or exited");
                }
                throw std::runtime_error(std::string("'write' failed: ") + strerror(errno));
            }

            p += static_cast< size_t >(w);
            left -= static_cast< size_t >(w);
        }
    }
};
