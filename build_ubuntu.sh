#!/bin/bash
# 64 Bit Version
# build for Ubuntu18.04
mkdir -p ubuntu

cd luajit-2.1
make clean

make BUILDMODE=static CC="gcc -fPIC -m64 -O2" XCFLAGS=-DLUAJIT_ENABLE_GC64
cp src/libluajit.a ../ubuntu/libluajit.a
make clean

echo -e "\n[MAINTAINCE] build libluajit.a done\n"

cd ..

gcc -m64 -O2 -std=gnu99 -shared \
 tolua.c \
 int64.c \
 uint64.c \
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
 luasocket/mime.c \
 luasocket/options.c \
 luasocket/select.c \
 luasocket/tcp.c \
 luasocket/timeout.c \
 luasocket/udp.c \
 luasocket/usocket.c \
 -fPIC\
 -o Plugins/ubuntu/libtolua.so \
 -I./ \
 -Iluajit-2.1/src \
 -Iluasocket \
 -Wl,--whole-archive ubuntu/libluajit.a -Wl,--no-whole-archive -static-libgcc -static-libstdc++

if [ "$?" = "0" ]; then
	echo -e "\n[MAINTAINCE] build libtolua.so success"
else
	echo -e "\n[MAINTAINCE] build libtolua.so failed"
fi
