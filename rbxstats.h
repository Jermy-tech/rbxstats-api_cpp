#ifndef RBXSTATS_API_H
#define RBXSTATS_API_H

#include <string>
#include <stdexcept>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class RbxStatsClient {
private:
    std::string api_key;

    // Helper function to perform the HTTP GET request and get the response as a string
    std::string perform_get_request(const std::string &url) const {
        CURL *curl = curl_easy_init();
        if (!curl) throw std::runtime_error("Failed to initialize CURL");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            curl_easy_cleanup(curl);
            throw std::runtime_error("Request failed: " + std::string(curl_easy_strerror(res)));
        }

        curl_easy_cleanup(curl);
        return response;
    }

    // CURL callback to write response data into a string
    static size_t write_callback(void *contents, size_t size, size_t nmemb, std::string *output) {
        size_t total_size = size * nmemb;
        output->append(static_cast<char *>(contents), total_size);
        return total_size;
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

        json get_all() { return fetch("offsets"); }
        std::string get_all_plain() { return fetch_text("offsets/plain"); }
        json get_offset_by_name(const std::string &name) { return fetch("offsets/search/" + name); }
        std::string get_offset_by_name_plain(const std::string &name) { return fetch_text("offsets/search/" + name + "/plain"); }
        json get_offsets_by_prefix(const std::string &prefix) { return fetch("offsets/prefix/" + prefix); }
        json get_camera() { return fetch("offsets/camera"); }

    private:
        json fetch(const std::string &endpoint) {
            std::string url = api_url(endpoint);
            std::string response = client.perform_get_request(url);
            return json::parse(response);
        }

        std::string fetch_text(const std::string &endpoint) {
            std::string url = api_url(endpoint);
            return client.perform_get_request(url);
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

        json get_all() { return fetch("exploits"); }
        json get_windows() { return fetch("exploits/windows"); }
        json get_mac() { return fetch("exploits/mac"); }
        json get_undetected() { return fetch("exploits/undetected"); }
        json get_detected() { return fetch("exploits/detected"); }
        json get_free() { return fetch("exploits/free"); }

    private:
        json fetch(const std::string &endpoint) {
            std::string url = api_url(endpoint);
            std::string response = client.perform_get_request(url);
            return json::parse(response);
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

        json get_latest() { return fetch("versions/latest"); }
        json get_future() { return fetch("versions/future"); }

    private:
        json fetch(const std::string &endpoint) {
            std::string url = api_url(endpoint);
            std::string response = client.perform_get_request(url);
            return json::parse(response);
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

        json get_game_by_id(int game_id) {
            return fetch("offsets/game/" + std::to_string(game_id));
        }

        std::string get_game_by_id_plain(int game_id) {
            return fetch_text("offsets/game/" + std::to_string(game_id) + "/plain");
        }

    private:
        json fetch(const std::string &endpoint) {
            std::string url = api_url(endpoint);
            std::string response = client.perform_get_request(url);
            return json::parse(response);
        }

        std::string fetch_text(const std::string &endpoint) {
            std::string url = api_url(endpoint);
            return client.perform_get_request(url);
        }
    };

    Offsets offsets{*this};
    Exploits exploits{*this};
    Versions versions{*this};
    Game game{*this};
};

#endif // RBXSTATS_API_H
