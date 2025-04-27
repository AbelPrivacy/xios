#! /bin/bash

echo "Cleaning up old build artifacts..."

rm -rf build_bak > /dev/null
mkdir build_bak

cp ./*.o ./build_bak || :
cp ./*.so ./build_bak || :
cp ./*.a ./build_bak || :
cp ./*.dylib ./build_bak || :

cp -r build ./build_bak/build || :

rm -f ./*.o > /dev/null
rm -f ./*.so > /dev/null
rm -f ./*.a > /dev/null
rm -f ./*.dylib > /dev/null

rm -f metadata.db > /dev/null

rm -f run_tests > /dev/null
rm -f xios_driver > /dev/null

# Find Node.js include path from NVM
if [[ -z "$NODE_INCLUDE_PATH" ]]; then
    NODE_INCLUDE_PATH=$(find "$HOME/.nvm/versions/node" -type d -path "*/include/node" | head -n 1)
fi

if [[ -z "$NODE_INCLUDE_PATH" ]]; then
    echo "❌ node_api.h not found under ~/.nvm. Is Node.js installed via NVM?"
else
    echo "✅ Node.js include path found: $NODE_INCLUDE_PATH" ...
fi

echo "Downloading prebuilt wolfssl binaries..."

gh release download PQC-Release --repo AbelPrivacy/wolfssl

tar -xzf build-x86_64.tar.gz
tar -xzf build-osx-arm64.tar.gz

echo "Building catch2 testing framework..."

g++ -std=c++20 -o test/catch2/src/libcatch2.o test/catch2/src/catch_amalgamated.cpp -Wnan-infinity-disabled


echo "Building reverse https proxy..."

g++ --std=c++20 ./util/reverse-https-proxy.cpp  \
	-ffast-math \
	-Llib -lwolfssl \
	-Iinclude \
	-o ./util/reverse-https-proxy \
	-pthread \
    -framework CoreFoundation -framework Security

echo "Building xios library..."

g++ -std=c++20 \
	-ffast-math \
	-I include \
	-I ./node_modules/node-addon-api/ \
	-I "$NODE_INCLUDE_PATH"  \
	-c src/xios.cpp \
	-pthread -static
	
rm -rf ./lib/libwolfssl.a.dir/
mkdir ./lib/libwolfssl.a.dir/
cp lib/libwolfssl.a ./lib/libwolfssl.a.dir/

cd ./lib/libwolfssl.a.dir/
ar x libwolfssl.a
cd ../../

ar rvs ./lib/libxios.a xios.o lib/sqlite3.o ./lib/libwolfssl.a.dir/*.o > xios.a.log

echo "Building test driver..."

g++ -std=c++20 ./test/test_main.cpp ./test/test_parseURL.cpp \
	./test/test_get.cpp ./test/test_post.cpp \
	-ffast-math \
	-I ./src/ \
	./src/xios.cpp \
	-I test/catch2/include \
	-pthread \
	./test/catch2/src/catch_amalgamated.cpp \
	-o run_tests \
	-L lib \
	-I include \
	-I include/wolfssl \
	-lwolfssl -lsqlite3 -lxios \
    -framework CoreFoundation -framework Security -w


echo "Installing Node dependencies..."

npm update
npm i node-gyp
npm i node-addon-api

echo "Building N-API node module with node-gyp..."

echo 'y' | npx node-gyp configure build  >node-gyp.configure.build.log 2>&1

echo "Activating Python venv..."
source ./venv/bin/activate

echo "Starting reverse https proxy server..."
./util/reverse-https-proxy 127.0.0.1 8080 127.0.0.1 1443 >proxy.log 2>&1 &
PROXY_PID=$!
sleep 2

echo "Running test REST API..."
python test/test_rest_api/app.py >backend.log 2>&1 &
BACKEND_PID=$!
sleep 2

echo "Running test client..."
if ./run_tests; then
	echo "Integration test PASSED"
	kill "${PROXY_PID}"
	kill "${BACKEND_PID}"
	exit 0
else
	echo "Integration test FAILED"
	kill "${PROXY_PID}"
	kill "${BACKEND_PID}"
	exit 1
fi
