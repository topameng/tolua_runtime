#!/usr/bin/env bash

LUALIB="${1:-luajit}"
set -e
set -x
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
LIPO="xcrun -sdk iphoneos lipo"
STRIP="xcrun -sdk iphoneos strip"
LIBTOOL="xcrun -sdk iphoneos libtool"

JITSRCDIR=$DIR/luajit-2.1/
LUASRCDIR=$DIR/lua
DESTDIR=$DIR/iOS
IXCODE=`xcode-select -print-path`
ISDK=$IXCODE/Platforms/iPhoneOS.platform/Developer
ISDKD=/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/
ISDKVER=iPhoneOS.sdk
ISDKP=$IXCODE/usr/bin/

ISDKSim=$IXCODE/Platforms/iPhoneSimulator.platform/Developer
ISDKVERSim=iPhoneSimulator.sdk

ISDKFCom="-miphoneos-version-min=7.0 -fembed-bitcode -DLJ_NO_SYSTEM=1"

if [ ! -e $ISDKP/ar ]; then 
  sudo cp $ISDKD/usr/bin/ar $ISDKP
fi

if [ ! -e $ISDKP/ranlib ]; then
  sudo cp $ISDKD/usr/bin/ranlib $ISDKP
fi

if [ ! -e $ISDKP/strip ]; then
  sudo cp $ISDKD/usr/bin/strip $ISDKP
fi

rm -f "$DESTDIR"/*.a

if [ $LUALIB = 'lua' ]
then
    cd $LUASRCDIR
    xcodebuild clean
    xcodebuild -configuration Release
    mv $LUASRCDIR/build/Release-iphoneos/liblua.a $DESTDIR/lib$LUALIB.a
else
    cd $JITSRCDIR
    make clean
    ISDKF="-arch armv7 -isysroot $ISDK/SDKs/$ISDKVER $ISDKFCom"
    make HOST_CC="gcc -m32" TARGET_FLAGS="$ISDKF" TARGET=armv7 TARGET_SYS=iOS BUILDMODE=static
    mv "$JITSRCDIR"/src/lib$LUALIB.a "$DESTDIR"/lib$LUALIB-armv7.a

    make clean
    ISDKF="-arch arm64 -isysroot $ISDK/SDKs/$ISDKVER $ISDKFCom"
    make HOST_CC="gcc " TARGET_FLAGS="$ISDKF" TARGET=arm64 TARGET_SYS=iOS BUILDMODE=static
    mv "$JITSRCDIR"/src/lib$LUALIB.a "$DESTDIR"/lib$LUALIB-arm64.a

    make clean

    cd ../iOS
    $LIPO -create "$DESTDIR"/lib$LUALIB-*.a -output "$DESTDIR"/lib$LUALIB.a
    $STRIP -S "$DESTDIR"/lib$LUALIB.a

    cd $DIR
fi 

cd $DESTDIR
xcodebuild clean
xcodebuild -configuration=Release HEADER_SEARCH_PATHS=$LUASRCDIR/src OTHER_LDFLAGS="-ObjC -l$LUALIB"

cd $LUASRCDIR
xcodebuild clean
xcodebuild -configuration Release -sdk iphonesimulator
mv $LUASRCDIR/build/Release-iphonesimulator/liblua.a $DESTDIR/libluasim.a

cd $DESTDIR
xcodebuild -configuration=Release -sdk iphonesimulator HEADER_SEARCH_PATHS=$LUASRCDIR/src OTHER_LDFLAGS="-ObjC -lluasim"

mkdir -p $DIR/Plugins/iOS

cd $DESTDIR
$LIPO -create build/Release-iphoneos/libtolua.a build/Release-iphonesimulator/libtolua.a -output $DESTDIR/libtolua.a
$LIBTOOL -static -o $DIR/Plugins/iOS/libtolua.a $DESTDIR/libtolua.a $DESTDIR/lib$LUALIB.a $DESTDIR/libluasim.a

