cd luajit-2.1/src

# Android/x86, x86 (i686 SSE3), Android 4.0+ (ICS)
NDK=D:/Android/android-ndk-r13b
NDKABI=19
NDKVER=$NDK/toolchains/x86-4.9
NDKP=$NDKVER/prebuilt/windows-x86_64/bin/i686-linux-android-
NDKF="--sysroot $NDK/platforms/android-$NDKABI/arch-x86"

make clean
make HOST_CC="gcc -m32" CROSS=$NDKP TARGET_SYS=Linux TARGET_FLAGS="$NDKF"
cp ./libluajit.a ../../android/jni/libluajit.a
make clean

cd ../../android
D:/Android/android-ndk-r13b/ndk-build.cmd clean APP_ABI="armeabi-v7a,x86"
D:/Android/android-ndk-r13b/ndk-build.cmd APP_ABI="x86"
cp libs/x86/libtolua.so ../Plugins/Android/libs/x86
D:/Android/android-ndk-r13b/ndk-build.cmd clean APP_ABI="armeabi-v7a,x86"