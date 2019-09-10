# tolua_runtime
**Build**<br>
**lua5.1.5**<br>
windows pc: build_win32.sh build_win64.h  (mingw + luajit2.1.0-beta3) choose 1<br>
linux pc(**ubuntu18.04 amd64**): build_linux32.sh build_linux64.h  (gcc + luajit2.1.0-beta3) choose 1<br>
android: build_arm.sh build_x86.sh (mingw + luajit2.1.0-beta3) choose 1<br>
mac: build_osx.sh (xcode + luac5.1.5 for luajit can't run on unity5) choose 1<br>
ios: build_ios.sh (xcode + luajit2.1.0-beta3) choose 1<br>
**lua5.3**<br>
windows pc: build_win32.sh build_win64.h  (mingw + lua5.3) choose 2<br>
linux pc: build_linux32.sh(**ubuntu16.04 i386**) build_linux64.h(**ubuntu18.04 amd64**)  (gcc + lua5.3) choose 2<br>
android: build_arm.sh build_x86.sh (mingw + lua5.3) choose 2<br>
mac: build_osx.sh (xcode + lua5.3 ) choose 2<br>
ios: build_ios.sh (xcode + lua5.3 ) choose 2<br>

NDK 版本:android-ndk-r16b 默认安装到 D:/android-ndk-r16b<br>
https://dl.google.com/android/repository/android-ndk-r16b-windows-x86_64.zip<br>
Msys2配置说明<br>
https://github.com/topameng/tolua_runtime/wiki<br>
配置好的Msys2下载<br>
https://pan.baidu.com/s/1c2JzvDQ<br>

# Libs
**cjson**<br>
https://github.com/mpx/lua-cjson<br>
**protoc-gen-lua**<br>
https://github.com/topameng/protoc-gen-lua<br>
**LuaSocket** <br>
https://github.com/diegonehab/luasocket<br>
**struct**<br>
http://www.inf.puc-rio.br/~roberto/struct/<br>
**lpeg**<br>
http://www.inf.puc-rio.br/~roberto/lpeg/lpeg.html

# 关于mac上编译iOS 32位runtime库
* 苹果系统升级到xcode10后，移除了编译32位lib的支持，如果有需求编译支持32位版本的runtime库，请用xcode9.4.1编译，目前默认原生的编译好的是同时支持32、64位的runtime

# [runtime库编译文档](https://github.com/NewbieGameCoder/tolua_runtime/wiki)
