LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := lua
LOCAL_CFLAGS := -DLUA_ANSI
LOCAL_SRC_FILES :=	../lapi.c \
					../lbitlib.c \
					../lcode.c \
					../lcorolib.c \
					../lctype.c \
					../ldebug.c \
					../ldo.c \
					../ldump.c \
					../lfunc.c \
					../lgc.c \
					../llex.c \
					../lmem.c \
					../lobject.c \
					../lopcodes.c \
					../lparser.c \
					../lstate.c \
					../lstring.c \
					../ltable.c \
					../ltm.c \
					../lundump.c \
					../lvm.c \
					../lzio.c \
					../lauxlib.c \
					../lbaselib.c \
					../ldblib.c \
					../liolib.c \
					../lmathlib.c \
					../loslib.c \
					../lstrlib.c \
					../lutf8lib.c \
					../ltablib.c \
					../loadlib.c \
					../linit.c 
include $(BUILD_STATIC_LIBRARY)