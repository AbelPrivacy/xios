
- Compile xios with g++ (g++ -std=c++17 -lwolfssl -lsqlite3 -o secure_http_client -c xios.cpp -I ~/homebrew/Cellar/wolfssl/5.7.6/include -L ~/homebrew/Cellar/wolfssl/5.7.6/lib/ -L ~/homebrew/Cellar/sqlite/3.49.1/lib -I ~/homebrew/Cellar/sqlite/3.49.1/include -o xios.o)
- Compile napi module with node-gyp (node-gyp build)
