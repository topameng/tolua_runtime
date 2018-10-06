#!/usr/bin/env bash

cd macnojit/
xcodebuild clean
xcodebuild -configuration=Release
cp -r build/Release/tolua.bundle ../Plugins/
