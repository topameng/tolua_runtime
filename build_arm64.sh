cd luajit-2.1/src

# Android/ARM, armeabi-v7a (ARMv7 VFP), Android 4.0+ (ICS)
NDK=D:/android-ndk-r16b
NDKABI=21
NDKTRIPLE=aarch64-linux-android
NDKVER=$NDK/toolchains/$NDKTRIPLE-4.9
NDKP=$NDKVER/prebuilt/windows-x86_64/bin/$NDKTRIPLE-
NDKF="-isystem $NDK/sysroot/usr/include/$NDKTRIPLE -D__ANDROID_API__=$NDKABI"
NDK_SYSROOT_BUILD=$NDK/sysroot
NDK_SYSROOT_LINK=$NDK/platforms/android-$NDKABI/arch-arm64

make clean
make HOST_CC="gcc -m64" CROSS=$NDKP TARGET_FLAGS="$NDKF" TARGET_SYS=Linux TARGET_SHLDFLAGS="--sysroot $NDK_SYSROOT_LINK"  TARGET_LDFLAGS="--sysroot $NDK_SYSROOT_LINK" TARGET_CFLAGS="--sysroot $NDK_SYSROOT_BUILD"
cp ./libluajit.a ../../android/jni/libluajit.a
make clean

if [[ "$OSTYPE" == "msys" ]]; then
	cd ../../
	# can't pass $NDK to bat
	cmd /c "link_arm64.bat"
else
	cd ../../android
	$NDK/build/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"
	$NDK/build/ndk-build APP_ABI="arm64-v8a"
	cp libs/arm64-v8a/libtolua.so ../Plugins/Android/libs/arm64-v8a
	$NDK/build/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"
fi