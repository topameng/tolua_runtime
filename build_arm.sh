cd luajit-2.1/src

# Android/ARM, armeabi-v7a (ARMv7 VFP), Android 4.0+ (ICS)
NDK=D:/android-ndk-r17c
NDKABI=19
NDKTRIPLE=arm-linux-androideabi
NDKVER=$NDK/toolchains/$NDKTRIPLE-4.9
NDKP=$NDKVER/prebuilt/windows-x86_64/bin/$NDKTRIPLE-
NDKF="-isystem $NDK/sysroot/usr/include/$NDKTRIPLE -D__ANDROID_API__=$NDKABI -D_FILE_OFFSET_BITS=32"
NDK_SYSROOT_BUILD=$NDK/sysroot
NDK_SYSROOT_LINK=$NDK/platforms/android-$NDKABI/arch-arm
NDKARCH="-march=armv7-a -mfloat-abi=softfp -Wl,--fix-cortex-a8"

make clean
make HOST_CC="gcc -m32" CROSS=$NDKP TARGET_FLAGS="$NDKF $NDKARCH" TARGET_SYS=Linux TARGET_SHLDFLAGS="--sysroot $NDK_SYSROOT_LINK"  TARGET_LDFLAGS="--sysroot $NDK_SYSROOT_LINK" TARGET_CFLAGS="--sysroot $NDK_SYSROOT_BUILD"
cp ./libluajit.a ../../android/jni/libluajit.a
make clean

cd ../../android
$NDK/build/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"
$NDK/build/ndk-build APP_ABI="armeabi-v7a"
cp libs/armeabi-v7a/libtolua.so ../Plugins/Android/libs/armeabi-v7a
$NDK/build/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"