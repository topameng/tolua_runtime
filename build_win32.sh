#!/bin/bash
# 32 Bit Version
mkdir -p window/x86
luacdir="lua53"
luajitdir="luajit-2.1"
luapath=""
lualibname=""
outpath="Plugins"
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

while :
do
    echo "Please choose (1)luajit; (2)lua5.3"
    read input
    case $input in
        "1")
            luapath=$luajitdir
            lualibname="libluajit"
			outpath="Plugins"
            break
        ;;
        "2")
            luapath=$luacdir
            lualibname="liblua"
			outpath="Plugins53"
            break
        ;;
        *)
            echo "Please enter 1 or 2!!"
            continue
        ;;
    esac
done

echo "select : $luapath"

cd $DIR/$luapath
mingw32-make clean

case $luapath in 
    $luacdir)
        mingw32-make mingw BUILDMODE=static CC="gcc -m32 -O2"
    ;;
    $luajitdir)
        mingw32-make BUILDMODE=static CC="gcc -m32 -O2"
    ;;
esac

cp src/$lualibname.a ../window/x86/$lualibname.a
mingw32-make clean

cd ..

gcc -m32 -O2 -std=gnu99 -shared \
	int64.c \
	uint64.c \
	tolua.c \
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
	luasocket/wsocket.c \
	luasocket/compat.c \
	-o $outpath/x86/tolua.dll \
	-I./ \
 	-I$luapath/src \
	-Icjson \
	-Iluasocket \
	-lws2_32 \
 	-Wl,--whole-archive window/x86/$lualibname.a -Wl,--no-whole-archive -static-libgcc -static-libstdc++