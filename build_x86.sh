cd luajit/src

# Android/x86, x86 (i686 SSE3), Android 4.0+ (ICS)
NDK=D:/android-ndk-r10e
NDKABI=21
NDKVER=$NDK/toolchains/x86-4.9
NDKP=$NDKVER/prebuilt/windows-x86_64/bin/i686-linux-android-
NDKF="--sysroot $NDK/platforms/android-$NDKABI/arch-x86"

make clean
make HOST_CC="gcc -m32" CROSS=$NDKP TARGET_SYS=Linux TARGET_FLAGS="$NDKF"
cp ./libluajit.a ../../android/jni/libluajit.a
make clean

cd ../../android
ndk-build clean APP_ABI="armeabi-v7a,x86"
ndk-build APP_ABI="x86"
cp libs/x86/libtolua.so ../Plugins/Android/libs/x86
ndk-build clean APP_ABI="armeabi-v7a,x86"