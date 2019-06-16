@echo off
set ndkPath=D:/android-ndk-r10e
cd ./lua53/src
call %ndkPath%/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"
call %ndkPath%/ndk-build APP_ABI="arm64-v8a"
copy obj\local\arm64-v8a\liblua.a ..\..\android53\jni
call %ndkPath%/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"
echo Successfully builded
exit