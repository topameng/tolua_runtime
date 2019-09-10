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
NDK=D:/android-ndk-r16b
NDKABI=21
NDKTRIPLE=aarch64-linux-android
NDKVER=$NDK/toolchains/$NDKTRIPLE-4.9
NDKP=$NDKVER/prebuilt/windows-x86_64/bin/$NDKTRIPLE-
NDKF="-isystem $NDK/sysroot/usr/include/$NDKTRIPLE -D__ANDROID_API__=$NDKABI -D_FILE_OFFSET_BITS=64"
NDK_SYSROOT_BUILD=$NDK/sysroot
NDK_SYSROOT_LINK=$NDK/platforms/android-$NDKABI/arch-arm64
NDKARCH="-DLJ_ABI_SOFTFP=0 -DLJ_ARCH_HASFPU=1 -DLUAJIT_ENABLE_GC64=1"

case $luapath in 
    $luacdir)        
        $NDK/ndk-build.cmd clean APP_ABI="armeabi-v7a,x86,arm64-v8a" APP_PLATFORM=android-$NDKABI
        $NDK/ndk-build.cmd APP_ABI="arm64-v8a" APP_PLATFORM=android-$NDKABI
        cp obj/local/arm64-v8a/$lualibname.a ../../android53/jni/
        $NDK/ndk-build.cmd clean APP_ABI="armeabi-v7a,x86,arm64-v8a" APP_PLATFORM=android-$NDKABI        	        
    ;;
    $luajitdir)        
        make clean        
        make HOST_CC="gcc -m64" CROSS=$NDKP TARGET_FLAGS="$NDKF $NDKARCH" TARGET_SYS=Linux TARGET_SHLDFLAGS="--sysroot $NDK_SYSROOT_LINK"  TARGET_LDFLAGS="--sysroot $NDK_SYSROOT_LINK" TARGET_CFLAGS="--sysroot $NDK_SYSROOT_BUILD"
        cp ./$lualibname.a ../../android/jni/$lualibname.a
        make clean    	
    ;;
esac

cd ../../$lualinkpath
$NDK/ndk-build.cmd clean APP_ABI="armeabi-v7a,x86,arm64-v8a" APP_PLATFORM=android-$NDKABI
$NDK/ndk-build.cmd APP_ABI="arm64-v8a" APP_PLATFORM=android-$NDKABI
cp libs/arm64-v8a/libtolua.so ../$outpath/Android/libs/arm64-v8a
$NDK/ndk-build.cmd clean APP_ABI="armeabi-v7a,x86,arm64-v8a" APP_PLATFORM=android-$NDKABI