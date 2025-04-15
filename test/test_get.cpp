#include "test_get.h"


void testGet() {
    std::string db =  "./metadata.db";
    std::vector<std::string> proto;
    proto.push_back("TLSv1.3");
    std::vector<std::string> algo;
    algo.push_back("TLS13-AES256-GCM-SHA384");
    std::vector<std::string> empty;
    SecureHttpClient::initialize(
      db,
      proto,
      algo,
      empty,
      empty // or ['KYBER_LEVEL1'] if supported
    );
    std::string response = SecureHttpClient::get("https://127.0.0.1:8080/request/");
    assert(response.find("HTTP/1.1 200 OK") == 0);
    assert(response.find("Server: Werkzeug/3.1.3 Python/3.13.3") != -1);
    assert(response.find("Content-Type: text/html; charset=utf-8") != -1);
    assert(response.find("Connection: close") != -1);
    assert(response.find("GET /requests/") != -1);
    std::cout << "✅ All get() tests passed.\n";
}