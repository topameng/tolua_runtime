#!/usr/bin/env bash
lua53dir="macnojit53/"
lua51dir="macnojit/"
luapath=""
lualibname="liblua"
outpath=""

while :
do
    echo "Please choose (1)lua5.1; (2)lua5.3"
    read input
    case $input in
        "1")
            luapath=$lua51dir
            outpath="Plugins"
            break
        ;;
        "2")
            luapath=$lua53dir
            outpath="Plugins53"
            break
        ;;
        *)
            echo "Please enter 1 or 2!!"
            continue
        ;;
    esac
done

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $DIR/$luapath
xcodebuild clean
xcodebuild -configuration=Release
cp -r build/Release/tolua.bundle ../$outpath/
