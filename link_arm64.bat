@echo off
set ndkPath=D:/android-ndk-r10e
cd ./android
call %ndkPath%/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"
call %ndkPath%/ndk-build APP_ABI="arm64-v8a"
copy libs\arm64-v8a\libtolua.so ..\Plugins\Android\libs\arm64-v8a
call %ndkPath%/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"
echo Successfully linked
exit