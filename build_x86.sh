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
NDK=D:/android-ndk-r16b
NDKABI=21
NDKVER=$NDK/toolchains/x86-4.9
NDKP=$NDKVER/prebuilt/windows-x86_64/bin/i686-linux-android-
NDKF="-isystem $NDK/sysroot/usr/include/i686-linux-android -D__ANDROID_API__=$NDKABI -D_FILE_OFFSET_BITS=32"
NDK_SYSROOT_BUILD=$NDK/sysroot
NDK_SYSROOT_LINK=$NDK/platforms/android-$NDKABI/arch-x86

case $luapath in 
    $luacdir)
        $NDK/ndk-build.cmd clean APP_ABI="armeabi-v7a,x86,arm64-v8a" APP_PLATFORM=android-$NDKABI
        $NDK/ndk-build.cmd APP_ABI="x86" APP_PLATFORM=android-$NDKABI
        cp obj/local/x86/$lualibname.a ../../android53/jni/
        $NDK/ndk-build.cmd clean APP_ABI="armeabi-v7a,x86,arm64-v8a" APP_PLATFORM=android-$NDKABI
    ;;
    $luajitdir)
        make clean
        make HOST_CC="gcc -m32" CROSS=$NDKP TARGET_SYS=Linux TARGET_FLAGS="$NDKF" TARGET_SHLDFLAGS="--sysroot $NDK_SYSROOT_LINK" TARGET_LDFLAGS="--sysroot $NDK_SYSROOT_LINK" TARGET_CFLAGS="--sysroot $NDK_SYSROOT_BUILD"
        cp ./$lualibname.a ../../android/jni/$lualibname.a
        make clean
    ;;
esac

cd ../../$lualinkpath
$NDK/ndk-build.cmd clean APP_ABI="armeabi-v7a,x86,arm64-v8a" APP_PLATFORM=android-$NDKABI
$NDK/ndk-build.cmd APP_ABI="x86" APP_PLATFORM=android-$NDKABI
cp libs/x86/libtolua.so ../$outpath/Android/libs/x86
$NDK/ndk-build.cmd clean APP_ABI="armeabi-v7a,x86,arm64-v8a" APP_PLATFORM=android-$NDKABI