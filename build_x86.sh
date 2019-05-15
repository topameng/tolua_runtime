cd luajit-2.1/src

# Android/x86, x86 (i686 SSE3), Android 4.0+ (ICS)
NDK=D:/android-ndk-r16b
NDKABI=19
NDKVER=$NDK/toolchains/x86-4.9
NDKP=$NDKVER/prebuilt/windows-x86_64/bin/i686-linux-android-
NDKF="-isystem $NDK/sysroot/usr/include/i686-linux-android -D__ANDROID_API__=$NDKABI -D_FILE_OFFSET_BITS=32"
NDK_SYSROOT_BUILD=$NDK/sysroot
NDK_SYSROOT_LINK=$NDK/platforms/android-$NDKABI/arch-x86

make clean
make HOST_CC="gcc -m32" CROSS=$NDKP TARGET_FLAGS="$NDKF" TARGET_SYS=Linux TARGET_SHLDFLAGS="--sysroot $NDK_SYSROOT_LINK"  TARGET_LDFLAGS="--sysroot $NDK_SYSROOT_LINK" TARGET_CFLAGS="--sysroot $NDK_SYSROOT_BUILD"
cp ./libluajit.a ../../android/jni/libluajit.a
make clean

cd ../../android
$NDK/build/ndk-build clean APP_ABI="armeabi-v7a,x86"
$NDK/build/ndk-build APP_ABI="x86"
cp libs/x86/libtolua.so ../Plugins/Android/libs/x86
$NDK/build/ndk-build clean APP_ABI="armeabi-v7a,x86"