#!/usr/bin/env bash
set -e
set -x
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
LIPO="xcrun -sdk iphoneos lipo"
STRIP="xcrun -sdk iphoneos strip"

SRCDIR=$DIR/luajit-2.1/
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
cd $SRCDIR

make clean
ISDKF="-arch armv7 -isysroot $ISDK/SDKs/$ISDKVER $ISDKFCom"
make HOST_CC="gcc -m32" TARGET_FLAGS="$ISDKF" TARGET=armv7 TARGET_SYS=iOS BUILDMODE=static
mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit-armv7.a

make clean
ISDKF="-arch arm64 -isysroot $ISDK/SDKs/$ISDKVER $ISDKFCom"
make HOST_CC="gcc " TARGET_FLAGS="$ISDKF" TARGET=arm64 TARGET_SYS=iOS BUILDMODE=static
mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit-arm64.a

#make clean
#ISDKF="-arch x86_64 -isysroot $ISDKSim/SDKs/$ISDKVERSim $ISDKFCom -mios-simulator-version-min=11.1 -DLUAJIT_ENABLE_GC64"
#make HOST_CC="gcc " TARGET_FLAGS="$ISDKF" TARGET=x86_64 TARGET_SYS=iOS BUILDMODE=static
#mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit-x86_64.a

#make clean
#ISDKF="-arch i386 -isysroot $ISDKSim/SDKs/$ISDKVERSim $ISDKFCom -mios-simulator-version-min=10"
#make HOST_CC="gcc -m32" TARGET_FLAGS="$ISDKF" TARGET=i386 TARGET_SYS=iOS BUILDMODE=static
#mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit-i386.a

make clean

cd ../iOS
$LIPO -create "$DESTDIR"/libluajit-*.a -output "$DESTDIR"/libluajit.a
$STRIP -S "$DESTDIR"/libluajit.a

conf=Release
xcodebuild clean
#xcodebuild -configuration=conf -sdk iphonesimulator
xcodebuild -configuration=Release

mkdir -p ../Plugins/iOS
cp ./build/Release-iphoneos/libtolua.a ../Plugins/iOS/libtolua.a

#$LIPO -create ./build/$conf-iphoneos/libtolua.a build/$conf-iphonesimulator/libtolua.a -output ../Plugins/iOS/libtolua.a

xcodebuild clean
