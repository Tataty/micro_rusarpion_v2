#pragma once

#include <functional>
#include <mutex>
#include <vector>

template < typename TData > class DataPool {
public:
    struct Context {
        TData data;
        bool  used;
    };

private:
    std::vector< Context* > buffers; /* TODO: optimization to O(1) by std::queue */
    std::mutex              mutex;

public:
    DataPool(int count, std::function< TData() > factory) {
        buffers.reserve(count);
        for (int i = 0; i < count; i++) {
            buffers.push_back(new Context{ factory(), false });
        }
    }

    ~DataPool() {
        for (auto ctx : buffers) {
            delete ctx;
        }
    }

    DataPool(const DataPool&)            = delete;
    DataPool& operator=(const DataPool&) = delete;

    Context* acquire() {
        std::lock_guard< std::mutex > lock(mutex);
        for (auto ctx : buffers) {
            if (!ctx->used) {
                ctx->used = true;
                return ctx;
            }
        }
        return nullptr;
    }

    void release(Context* ctx) {
        if (!ctx)
            return;
        std::lock_guard< std::mutex > lock(mutex);
        ctx->used = false;
    }
};