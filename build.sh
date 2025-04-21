#! /bin/bash

rm -rf build_bak
mkdir build_bak

cp *.o ./build_bak || :
cp *.so ./build_bak || :
cp *.a ./build_bak || :
cp *.dylib ./build_bak || :

cp -r build ./build_bak/build || :

rm -f *.o
rm -f *.so
rm -f *.a
rm -f *.dylib

rm -f metadata.db

rm -f run_tests
rm -f xios_driver

g++ --std=c++20 ./util/reverse-https-proxy.cpp -I ~/homebrew/include/ -L ~/homebrew/Cellar/wolfssl/5.7.6/lib -lwolfssl -o ./util/reverse-https-proxy

gh release download PQC-Release --repo AbelPrivacy/wolfssl

tar -xzvf build-x86_64.tar.gz
tar -xzvf build-osx-arm64.tar.gz

g++ -std=c++17 -lwolfssl -lsqlite3 -o secure_http_client -c xios.cpp \
    -I build_x86_64/include \
    -L build_x86_64/lib \
    -I build-osx-arm64/wolfssl \
    -L build-osx-arm64/lib \
    -L ~/homebrew/Cellar/sqlite/3.49.1/lib \
    -I ~/homebrew/Cellar/sqlite/3.49.1/include \
    -o xios.o

g++ -std=c++17 ./test/test.cpp ./test/test_parseURL.cpp \
    ./test/test_get.cpp\
    xios.o \
    -I build_x86_64/include \
    -L build_x86_64/lib \
    -I build-osx-arm64/wolfssl \
    -L build-osx-arm64/lib \
    -L ~/homebrew/Cellar/sqlite/3.49.1/lib \
    -I ~/homebrew/Cellar/sqlite/3.49.1/include \
    -lwolfssl -lsqlite3 \
    -o run_tests

g++ -std=c++17 ./main.cpp xios.o \
    -I build_x86_64/include \
    -L build_x86_64/lib \
    -I build-osx-arm64/wolfssl \
    -L build-osx-arm64/lib \
    -L ~/homebrew/Cellar/sqlite/3.49.1/lib \
    -I ~/homebrew/Cellar/sqlite/3.49.1/include \
    -lwolfssl -lsqlite3 \
    -o xios_driver

g++ -std=c++17 ./test/test_server/test_server.cpp xios.o \
    -I build_x86_64/include \
    -L build_x86_64/lib \
    -I build-osx-arm64/wolfssl \
    -L build-osx-arm64/lib \
    -L ~/homebrew/Cellar/sqlite/3.49.1/lib \
    -I ~/homebrew/Cellar/sqlite/3.49.1/include \
    -lwolfssl -lsqlite3 \
    -o test_server

npm update
npm i node-gyp
npm i node-addon-api

echo 'y' | npx node-gyp configure build

source ./venv/bin/activate

echo "Starting proxy server..."
./util/reverse-https-proxy 127.0.0.1 8080 443 > proxy.log 2>&1 &
PROXY_PID=$!
sleep 2  # Wait for proxy to start

python test/test_rest_api/app.py > backend.log 2>&1 &
BACKEND_PID=$!
sleep 2 

echo "Running test client..."
if ./run_tests; then
    echo "Integration test PASSED"
    kill "$PROXY_PID"
    kill "$BACKEND_PID"
    exit 0
else
    echo "Integration test FAILED"
    cat proxy.log || true
    cat backend.log || true
    kill "$PROXY_PID"
    kill "$BACKEND_PID"
    exit 1
fi



