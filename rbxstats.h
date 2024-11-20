#ifndef RBXSTATS_API_H
#define RBXSTATS_API_H

#include <windows.h>
#include <wininet.h>
#include <string>
#include <stdexcept>
#include <map>
#include <sstream>
#include <iostream>

#pragma comment(lib, "wininet.lib")

class RbxStatsClient {
private:
    std::string api_key;

    // Helper function to perform the HTTP GET request and get the response as a string
    std::string perform_get_request(const std::string &url) const {
        HINTERNET hInternet = InternetOpen(L"RbxStatsClient", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (!hInternet) throw std::runtime_error("Failed to open internet handle");

        HINTERNET hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (!hConnect) {
            InternetCloseHandle(hInternet);
            throw std::runtime_error("Failed to open URL: " + std::to_string(GetLastError()));
        }

        std::string response;
        char buffer[4096];
        DWORD bytesRead;

        while (InternetReadFile(hConnect, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead != 0) {
            buffer[bytesRead] = '\0'; // Null-terminate the buffer
            response.append(buffer);
        }

        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return response;
    }

    // Simple JSON-like parsing function to extract data
    static std::map<std::string, std::string> parse_json(const std::string &response) {
        std::map<std::string, std::string> result;
        std::string key, value;
        bool inQuotes = false;
        bool readingValue = false;

        for (size_t i = 0; i < response.size(); ++i) {
            char ch = response[i];

            if (ch == '\"') {
                inQuotes = !inQuotes;
                if (!inQuotes && !key.empty()) {
                    if (!readingValue) {
                        readingValue = true;
                    } else {
                        result[key] = value;
                        key.clear();
                        value.clear();
                        readingValue = false;
                    }
                }
            } else if (inQuotes) {
                if (!readingValue) {
                    key += ch;
                } else {
                    value += ch;
                }
            }
        }

        return result;
    }

public:
    explicit RbxStatsClient(const std::string &key) : api_key(key) {}

    class NestedBase {
    protected:
        RbxStatsClient &client;

        NestedBase(RbxStatsClient &client) : client(client) {}

        std::string api_url(const std::string &endpoint) const {
            return "https://api.rbxstats.xyz/api/" + endpoint + "?api=" + client.api_key;
        }

        std::map<std::string, std::string> fetch(const std::string &endpoint) const {
            std::string url = api_url(endpoint);
            std::string response = client.perform_get_request(url);
            return parse_json(response);
        }
    };

    class Offsets : public NestedBase {
    public:
        explicit Offsets(RbxStatsClient &client) : NestedBase(client) {}

        std::map<std::string, std::string> get_all() { return fetch("offsets"); }
        std::map<std::string, std::string> get_offset_by_name(const std::string &name) { return fetch("offsets/search/" + name); }
        std::map<std::string, std::string> get_offsets_by_prefix(const std::string &prefix) { return fetch("offsets/prefix/" + prefix); }
        std::map<std::string, std::string> get_camera() { return fetch("offsets/camera"); }
    };

    class Exploits : public NestedBase {
    public:
        explicit Exploits(RbxStatsClient &client) : NestedBase(client) {}

        std::map<std::string, std::string> get_all() { return fetch("exploits"); }
        std::map<std::string, std::string> get_windows() { return fetch("exploits/windows"); }
        std::map<std::string, std::string> get_mac() { return fetch("exploits/mac"); }
        std::map<std::string, std::string> get_undetected() { return fetch("exploits/undetected"); }
        std::map<std::string, std::string> get_detected() { return fetch("exploits/detected"); }
        std::map<std::string, std::string> get_free() { return fetch("exploits/free"); }
    };

    class Versions : public NestedBase {
    public:
        explicit Versions(RbxStatsClient &client) : NestedBase(client) {}

        std::map<std::string, std::string> get_latest() { return fetch("versions/latest"); }
        std::map<std::string, std::string> get_future() { return fetch("versions/future"); }
    };

    class Game : public NestedBase {
    public:
        explicit Game(RbxStatsClient &client) : NestedBase(client) {}

        std::map<std::string, std::string> get_game_by_id(int game_id) { return fetch("game/" + std::to_string(game_id)); }
    };

    Offsets offsets{*this};
    Exploits exploits{*this};
    Versions versions{*this};
    Game game{*this};
};

#endif // RBXSTATS_API_H
