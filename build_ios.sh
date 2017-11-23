#!/usr/bin/env bash
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

if [ ! -e $ISDKP/ar ]; then 
  sudo cp $ISDKD/usr/bin/ar $ISDKP
fi

if [ ! -e $ISDKP/ranlib ]; then
  sudo cp $ISDKD/usr/bin/ranlib $ISDKP
fi

if [ ! -e $ISDKP/strip ]; then
  sudo cp $ISDKD/usr/bin/strip $ISDKP
fi

rm "$DESTDIR"/*.a
cd $SRCDIR

make clean
ISDKF="-arch armv7 -isysroot $ISDK/SDKs/$ISDKVER -miphoneos-version-min=8.0 -fembed-bitcode"
make HOST_CC="gcc -m32" TARGET_FLAGS="$ISDKF" TARGET=armv7 TARGET_SYS=iOS BUILDMODE=static
mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit-armv7.a

make clean
ISDKF="-arch armv7s -isysroot $ISDK/SDKs/$ISDKVER -miphoneos-version-min=8.0 -fembed-bitcode"
make HOST_CC="gcc -m32" TARGET_FLAGS="$ISDKF" TARGET=armv7s TARGET_SYS=iOS BUILDMODE=static 
mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit-armv7s.a

make clean
ISDKF="-arch arm64 -isysroot $ISDK/SDKs/$ISDKVER -miphoneos-version-min=8.0 -fembed-bitcode"
make HOST_CC="gcc " TARGET_FLAGS="$ISDKF" TARGET=arm64 TARGET_SYS=iOS BUILDMODE=static
mv "$SRCDIR"/src/libluajit.a "$DESTDIR"/libluajit-arm64.a
make clean

cd ../iOS
$LIPO -create "$DESTDIR"/libluajit-*.a -output "$DESTDIR"/libluajit.a
$STRIP -S "$DESTDIR"/libluajit.a
xcodebuild clean
xcodebuild -configuration=Release
cp -f ./build/Release-iphoneos/libtolua.a ../Plugins/iOS/