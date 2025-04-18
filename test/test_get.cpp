#include "test_get.h"
#include <iostream>


using namespace std;

void testGet() {
  cout << 0 << endl;
    std::string db =  "./metadata.db";
    std::vector<std::string> proto;
    proto.push_back("TLSv1.3");
    std::vector<std::string> algo;
    algo.push_back("TLS13-AES256-GCM-SHA384");
    std::vector<std::string> empty;
    cout << 1 << endl;
    SecureHttpClient::initialize(
      db,
      proto,
      algo,
      empty,
      empty // or ['KYBER_LEVEL1'] if supported
    );
    cout << 2 << endl;
    std::string response = SecureHttpClient::get("https://127.0.0.1:8080/request/");
    cout << 3 << endl;
    cout << response << endl;
    assert(response.find("HTTP/1.1 200 OK") == 0);
    assert(response.find("Server: Werkzeug/3.1.3 Python/3.13.3") != -1);
    assert(response.find("Content-Type: text/html; charset=utf-8") != -1);
    assert(response.find("Connection: close") != -1);
    assert(response.find("GET /requests/") != -1);
    std::cout << "✅ All get() tests passed.\n";
}