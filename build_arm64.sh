luacdir="lua53"
luajitdir="luajit-2.1"
luapath=""
lualibname=""
lualinker=""
outpath=""
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

while :
do
    echo "Please choose (1)luajit; (2)lua5.3"
    read input
    case $input in
        "1")
            luapath=$luajitdir
            lualibname="libluajit"
            lualinker="link_arm64.bat"
            outpath="Plugins"
            break
        ;;
        "2")
            luapath=$luacdir
            lualibname="liblua"
            lualinker="link_Lua53_arm64.bat"
            outpath="Plugins53"
            break
        ;;
        *)
            echo "Please enter 1 or 2!!"
            continue
        ;;
    esac
done

echo "select : $luapath"
# Android/ARM, armeabi-v7a (ARMv7 VFP), Android 4.0+ (ICS)
NDK=D:/android-ndk-r10e
NDKABI=21
NDKTRIPLE=aarch64-linux-android
NDKVER=$NDK/toolchains/aarch64-linux-android-4.9
NDKP=$NDKVER/prebuilt/windows-x86_64/bin/aarch64-linux-android-
NDKF="--sysroot $NDK/platforms/android-$NDKABI/arch-arm64" 
NDKARCH="-DLJ_ABI_SOFTFP=0 -DLJ_ARCH_HASFPU=1 -DLUAJIT_ENABLE_GC64=1"

case $luapath in 
    $luacdir)
        if [[ "$OSTYPE" == "msys" ]]; then
            cd $DIR
            cmd /c "build_Lua53_arm64.bat"
        else
            cd $DIR/$luapath/src
            $NDK/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"
            $NDK/ndk-build APP_ABI="arm64-v8a"
            cp obj/local/arm64-v8a/$lualibname.a ../../android53/jni/
            $NDK/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"        
	        cd ../../
        fi
    ;;
    $luajitdir)
        cd $DIR/$luapath/src
        make clean
        make HOST_CC="gcc -m64" CROSS=$NDKP TARGET_SYS=Linux TARGET_FLAGS="$NDKF $NDKARCH"
        cp ./$lualibname.a ../../android/jni/$lualibname.a
        make clean
    	cd ../../
    ;;
esac

if [[ "$OSTYPE" == "msys" ]]; then
	# can't pass $NDK to bat
	cmd /c $lualinker
else
    cd ./android
	$NDK/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"
	$NDK/ndk-build APP_ABI="arm64-v8a"
	cp libs/arm64-v8a/libtolua.so ../$outpath/Android/libs/arm64-v8a
	$NDK/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"
fi