luacdir="lua53"
luajitdir="luajit-2.1"
luapath=""
lualibname=""
lualinkpath=""
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
            lualinkpath="android"
            outpath="Plugins"
            break
        ;;
        "2")
            luapath=$luacdir
            lualibname="liblua"
            lualinkpath="android53"
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
cd $DIR/$luapath/src

# Android/ARM, armeabi-v7a (ARMv7 VFP), Android 4.0+ (ICS)
NDK=D:/android-ndk-r10e
NDKABI=19
NDKVER=$NDK/toolchains/arm-linux-androideabi-4.9
NDKP=$NDKVER/prebuilt/windows-x86_64/bin/arm-linux-androideabi-
NDKF="--sysroot $NDK/platforms/android-$NDKABI/arch-arm" 
NDKARCH="-march=armv7-a -mfloat-abi=softfp -Wl,--fix-cortex-a8"

case $luapath in 
    $luacdir)
        $NDK/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"
        $NDK/ndk-build APP_ABI="armeabi-v7a"
        cp obj/local/armeabi-v7a/$lualibname.a ../../android53/jni/
        $NDK/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"
    ;;
    $luajitdir)
        make clean
        make HOST_CC="gcc -m32" CROSS=$NDKP TARGET_SYS=Linux TARGET_FLAGS="$NDKF $NDKARCH"
        cp ./$lualibname.a ../../android/jni/libluajit.a
        make clean
    ;;
esac

cd ../../$lualinkpath
$NDK/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"
$NDK/ndk-build APP_ABI="armeabi-v7a"
cp libs/armeabi-v7a/libtolua.so ../$outpath/Android/libs/armeabi-v7a
$NDK/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"