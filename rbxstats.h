#ifndef RBXSTATS_API_H
#define RBXSTATS_API_H

#include <windows.h>
#include <wininet.h>
#include <string>
#include <stdexcept>
#include <map>
#include <vector>
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
    std::map<std::string, std::string> parse_json(const std::string &response) {
        std::map<std::string, std::string> result;
        std::string key, value;
        bool inQuotes = false;
        
        for (size_t i = 0; i < response.size(); ++i) {
            char ch = response[i];

            if (ch == '\"') {
                inQuotes = !inQuotes;
                if (!inQuotes && !key.empty()) {
                    // We finished a key
                    result[key] = value;
                    key.clear();
                    value.clear();
                }
            } else if (inQuotes) {
                if (key.empty() && ch == ':') {
                    // Switch to value
                    continue; // Skip the colon
                }
                (key.empty() ? key : value) += ch;
            }
        }

        return result;
    }

public:
    RbxStatsClient(const std::string &key) : api_key(key) {}

    // Nested Offsets class
    class Offsets {
    private:
        RbxStatsClient &client;

        std::string api_url(const std::string &endpoint) {
            return "https://api.rbxstats.xyz/api/" + endpoint + "?api=" + client.api_key;
        }

    public:
        Offsets(RbxStatsClient &client) : client(client) {}

        // Fetches all offsets
        std::map<std::string, std::string> get_all() {
            return fetch("offsets");
        }

        // Fetches specific offset by name
        std::map<std::string, std::string> get_offset_by_name(const std::string &name) {
            return fetch("offsets/search/" + name);
        }

        // Fetches offsets by prefix
        std::map<std::string, std::string> get_offsets_by_prefix(const std::string &prefix) {
            return fetch("offsets/prefix/" + prefix);
        }

        // Fetches camera-related offsets
        std::map<std::string, std::string> get_camera() {
            return fetch("offsets/camera");
        }

    private:
        std::map<std::string, std::string> fetch(const std::string &endpoint) {
            std::string url = api_url(endpoint);
            std::string response = client.perform_get_request(url);
            return parse_json(response);
        }
    };

    // Nested Exploits class
    class Exploits {
    private:
        RbxStatsClient &client;

        std::string api_url(const std::string &endpoint) {
            return "https://api.rbxstats.xyz/api/" + endpoint + "?api=" + client.api_key;
        }

    public:
        Exploits(RbxStatsClient &client) : client(client) {}

        // Fetches all exploits
        std::map<std::string, std::string> get_all() {
            return fetch("exploits");
        }

        // Fetches Windows exploits
        std::map<std::string, std::string> get_windows() {
            return fetch("exploits/windows");
        }

        // Fetches Mac exploits
        std::map<std::string, std::string> get_mac() {
            return fetch("exploits/mac");
        }

        // Fetches undetected exploits
        std::map<std::string, std::string> get_undetected() {
            return fetch("exploits/undetected");
        }

        // Fetches detected exploits
        std::map<std::string, std::string> get_detected() {
            return fetch("exploits/detected");
        }

        // Fetches free exploits
        std::map<std::string, std::string> get_free() {
            return fetch("exploits/free");
        }

    private:
        std::map<std::string, std::string> fetch(const std::string &endpoint) {
            std::string url = api_url(endpoint);
            std::string response = client.perform_get_request(url);
            return parse_json(response);
        }
    };

    // Nested Versions class
    class Versions {
    private:
        RbxStatsClient &client;

        std::string api_url(const std::string &endpoint) {
            return "https://api.rbxstats.xyz/api/" + endpoint + "?api=" + client.api_key;
        }

    public:
        Versions(RbxStatsClient &client) : client(client) {}

        // Fetches the latest version
        std::map<std::string, std::string> get_latest() {
            return fetch("versions/latest");
        }

        // Fetches the future version
        std::map<std::string, std::string> get_future() {
            return fetch("versions/future");
        }

    private:
        std::map<std::string, std::string> fetch(const std::string &endpoint) {
            std::string url = api_url(endpoint);
            std::string response = client.perform_get_request(url);
            return parse_json(response);
        }
    };

    // Nested Game class
    class Game {
    private:
        RbxStatsClient &client;

        std::string api_url(const std::string &endpoint) {
            return "https://api.rbxstats.xyz/api/" + endpoint + "?api=" + client.api_key;
        }

    public:
        Game(RbxStatsClient &client) : client(client) {}

        // Fetches game information by ID
        std::map<std::string, std::string> get_game_by_id(int game_id) {
            return fetch("game/" + std::to_string(game_id));
        }

    private:
        std::map<std::string, std::string> fetch(const std::string &endpoint) {
            std::string url = api_url(endpoint);
            std::string response = client.perform_get_request(url);
            return parse_json(response);
        }
    };

    Offsets offsets{*this};
    Exploits exploits{*this};
    Versions versions{*this};
    Game game{*this};
};

#endif // RBXSTATS_API_H