#! /usr/bin/bash

rm -rf build_bak
mkdir build_bak

cp -f *.o ./build_bak
cp -f *.so ./build_bak
cp -f *.a ./build_bak
cp -f *.dylib ./build_bak

cp -r build ./build_bak/build

rm -f *.o
rm -f *.so
rm -f *.a
rm -f -f *.dyl-f ib

rm -f run_tests
rm -f xios_driver


g++ -std=c++17 -lwolfssl -lsqlite3 -o secure_http_client -c xios.cpp \
    -I ~/homebrew/Cellar/wolfssl/5.7.6/include \
    -L ~/homebrew/Cellar/wolfssl/5.7.6/lib \
    -L ~/homebrew/Cellar/sqlite/3.49.1/lib \
    -I ~/homebrew/Cellar/sqlite/3.49.1/include \
    -o xios.o

g++ -std=c++17 ./test/test.cpp ./test/test_parseURL.cpp xios.o \
    -I ~/homebrew/Cellar/wolfssl/5.7.6/include \
    -L ~/homebrew/Cellar/wolfssl/5.7.6/lib \
    -L ~/homebrew/Cellar/sqlite/3.49.1/lib \
    -I ~/homebrew/Cellar/sqlite/3.49.1/include \
    -lwolfssl -lsqlite3 \
    -o run_tests

g++ -std=c++17 ./main.cpp xios.o \
    -I ~/homebrew/Cellar/wolfssl/5.7.6/include \
    -L ~/homebrew/Cellar/wolfssl/5.7.6/lib \
    -L ~/homebrew/Cellar/sqlite/3.49.1/lib \
    -I ~/homebrew/Cellar/sqlite/3.49.1/include \
    -lwolfssl -lsqlite3 \
    -o xios_driver

g++ -std=c++17 ./test/test_server/test_server.cpp xios.o \
    -I ~/homebrew/Cellar/wolfssl/5.7.6/include \
    -L ~/homebrew/Cellar/wolfssl/5.7.6/lib \
    -L ~/homebrew/Cellar/sqlite/3.49.1/lib \
    -I ~/homebrew/Cellar/sqlite/3.49.1/include \
    -lwolfssl -lsqlite3 \
    -o test_server

./run_tests

node-gyp configure build
