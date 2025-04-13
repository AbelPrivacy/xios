#include "xios.hpp"
#include <iostream>
#include <vector>
#include <string>

int main() {
    try {
        // Define security policy
        std::string sqlitePath = "metadata.db";
        std::vector<std::string> allowedProtocols = {"TLSv1.3"};
        std::vector<std::string> allowedImplementations = {"TLS13-AES256-GCM-SHA384"};
        std::vector<std::string> blockedImplementations = {};
        std::vector<std::string> allowedGroups = {}; // Optional: e.g., {"KYBER_LEVEL1"}

        // Initialize the HTTP client
        SecureHttpClient::initialize(
            sqlitePath,
            allowedProtocols,
            allowedImplementations,
            blockedImplementations,
            allowedGroups
        );

        std::cout << "Making GET request...\n";
        // Perform GET request
        std::string url = "127.0.0.1:11111";
        std::string response = SecureHttpClient::get(url);

        // Output response (truncated if needed)
        std::cout << "Response:\n" << response.substr(0, 1000) << "...\n";

    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}