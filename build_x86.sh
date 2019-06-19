luacdir="lua53"
luajitdir="luajit-2.1"
luapath=""
lualibname=""
lualinkpath=""
outpath="Plugins"
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

# Android/x86, x86 (i686 SSE3), Android 4.0+ (ICS)
NDK=D:/android-ndk-r10e
NDKABI=19
NDKVER=$NDK/toolchains/x86-4.9
NDKP=$NDKVER/prebuilt/windows-x86_64/bin/i686-linux-android-
NDKF="--sysroot $NDK/platforms/android-$NDKABI/arch-x86"

case $luapath in 
    $luacdir)
        $NDK/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"
        $NDK/ndk-build APP_ABI="x86"
        cp obj/local/x86/$lualibname.a ../../android53/jni/
        $NDK/ndk-build clean APP_ABI="armeabi-v7a,x86,arm64-v8a"
    ;;
    $luajitdir)
        make clean
        make HOST_CC="gcc -m32" CROSS=$NDKP TARGET_SYS=Linux TARGET_FLAGS="$NDKF"
        cp ./$lualibname.a ../../android/jni/$lualibname.a
        make clean
    ;;
esac

cd ../../$lualinkpath
$NDK/ndk-build clean APP_ABI="armeabi-v7a,x86"
$NDK/ndk-build APP_ABI="x86"
cp libs/x86/libtolua.so ../$outpath/Android/libs/x86
$NDK/ndk-build clean APP_ABI="armeabi-v7a,x86"