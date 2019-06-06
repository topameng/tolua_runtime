luacdir="lua53"
luajitdir="luajit-2.1"
luapath=""
lualibname=""

while :
do
    echo "Please choose (1¡¢luajit; 2¡¢lua5.3)"
    read input
    case $input in
        "1")
            luapath=$luajitdir
            lualibname="libluajit"
            break
        ;;
        "2")
            luapath=$luacdir
            lualibname=$luacdir
            break
        ;;
        *)
            echo "Please enter 1 or 2!!"
            continue
        ;;
    esac
done

echo "select : $luapath"
cd $luapath/src

# Android/ARM, armeabi-v7a (ARMv7 VFP), Android 4.0+ (ICS)
NDK=H:/android-ndk-r10e
NDKABI=21
NDKTRIPLE=aarch64-linux-android
NDKVER=$NDK/toolchains/aarch64-linux-android-4.9
NDKP=$NDKVER/prebuilt/windows-x86_64/bin/aarch64-linux-android-
NDKF="--sysroot $NDK/platforms/android-$NDKABI/arch-arm64" 
NDKARCH="-DLJ_ABI_SOFTFP=0 -DLJ_ARCH_HASFPU=1 -DLUAJIT_ENABLE_GC64=1"

make clean
case $luapath in 
    $luacdir)
        make mingw HOST_CC="gcc -m64" CROSS=$NDKP TARGET_SYS=Linux TARGET_FLAGS="$NDKF $NDKARCH"
    ;;
    $luajitdir)
        make HOST_CC="gcc -m64" CROSS=$NDKP TARGET_SYS=Linux TARGET_FLAGS="$NDKF $NDKARCH"
    ;;
esac
cp ./$lualibname.a ../../android/jni/libluajit.a
make clean

if [[ "$OSTYPE" == "msys" ]]; then
	cd ../../
	# can't pass $NDK to bat
	cmd /c "link_arm64.bat"
else
	cd ../../android
	$NDK/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"
	$NDK/ndk-build APP_ABI="arm64-v8a"
	cp libs/arm64-v8a/libtolua.so ../Plugins/Android/libs/arm64-v8a
	$NDK/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"
fi