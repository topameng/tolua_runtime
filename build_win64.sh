#!/bin/bash
#
# Windows 32-bit/64-bit

# Copyright (C) polynation games ltd - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Christopher Redden, December 2013

# 62 Bit Version
mkdir -p window/x86_64

cd luajit
mingw32-make clean

mingw32-make BUILDMODE=static CC="gcc -m64 -O3"
cp src/libluajit.a ../window/x86_64/libluajit.a
mingw32-make clean

cd ..

gcc tolua.c \
 pb.c \
 lpeg.c \
 struct.c \
 cjson/strbuf.c \
 cjson/lua_cjson.c \
 cjson/fpconv.c \
 luasocket/auxiliar.c \
 luasocket/buffer.c \
 luasocket/except.c \
 luasocket/inet.c \
 luasocket/io.c \
 luasocket/luasocket.c \
 luasocket/luasocket_scripts.c \
 luasocket/mime.c \
 luasocket/options.c \
 luasocket/select.c \
 luasocket/tcp.c \
 luasocket/timeout.c \
 luasocket/udp.c \
 luasocket/wsocket.c \
 -o Plugins/x86_64/tolua.dll \
 -m64 -O3 -std=gnu99 -shared \
 -I./ \
 -Iluajit/src \
 -Iluasocket \
 -lws2_32 \
 -Wl,--whole-archive window/x86_64/libluajit.a -Wl,--no-whole-archive -static-libgcc -static-libstdc++