#include <map>
#include <optional>
#include <string>
#include <string_view>

class URIDescriptor {
public:
    explicit URIDescriptor(const std::string& rawURI) : _rawURI(rawURI) { parse(); }

    // Returns protocol/scheme: "rtsp", "http", "file", "device", etc.
    std::string_view scheme() const { return _scheme; }

    // Returns the base path or address without parameters and scheme
    // For example: "192.168.1.1/live" or "/dev/video0"
    std::string_view path() const { return _path; }

    // Returns the address WITH the scheme, but WITHOUT parameters
    // Example: "rtsp://192.168.1.100:8554/stream" or "twin1212f14:/dev/video0"
    std::string_view base() const {
        size_t queryStart = _rawURI.find('?');
        if (queryStart == std::string::npos)
            return _rawURI;
        return std::string_view(_rawURI).substr(0, queryStart);
    }

    // Checking for the presence of the parameter
    bool hasParam(std::string_view key) const { return _params.find(key) != _params.end(); }

    // Get the parameter as a string
    std::string_view getParam(std::string_view key, std::string_view defaultValue) const {
        auto it = _params.find(key);
        return (it != _params.end()) ? it->second : defaultValue;
    }

    // Get parameter as number (convenient for ports, angles, fps)
    double getNumParam(std::string_view key, double defaultValue = 0.0) const {
        auto it = _params.find(key);
        if (it == _params.end())
            return defaultValue;
        try {
            return std::stod(std::string(it->second));
        } catch (...) {
            return defaultValue;
        }
    }

    std::optional< std::string > getStringParam(std::string_view key) const {
        auto it = _params.find(key);
        if (it != _params.end()) {
            return std::string(it->second);
        }
        return std::nullopt;
    }

    // Returns the complete original string
    const std::string& raw() const { return _rawURI; }

private:
    std::string                                    _rawURI;
    std::string_view                               _scheme;
    std::string_view                               _path;
    std::map< std::string_view, std::string_view > _params;

    void parse() {
        std::string_view view(_rawURI);

        if (size_t schemeEnd = view.find(':'); schemeEnd != std::string_view::npos) {
            _scheme = view.substr(0, schemeEnd);
            view.remove_prefix(schemeEnd + 1);
        }

        if (view.starts_with("//")) {
            view.remove_prefix(2);
        }

        size_t queryStart = view.find('?');
        _path             = view.substr(0, queryStart);

        if (queryStart != std::string_view::npos) {
            parseParams(view.substr(queryStart + 1));
        }
    }

    void parseParams(std::string_view query) {
        size_t start = 0;
        while (start < query.size()) {
            size_t           end  = query.find('&', start);
            std::string_view pair = query.substr(start, end - start);
            if (size_t sep = pair.find('='); sep != std::string_view::npos) {
                _params[pair.substr(0, sep)] = pair.substr(sep + 1);
            }
            if (end == std::string_view::npos)
                break;
            start = end + 1;
        }
    }
};