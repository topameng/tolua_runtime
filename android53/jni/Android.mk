LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := liblua
LOCAL_SRC_FILES := liblua.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_FORCE_STATIC_EXECUTABLE := true
LOCAL_MODULE := tolua
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../lua53/src
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../

LOCAL_CPPFLAGS := -O2
LOCAL_CFLAGS :=  -O2 -std=gnu99
LOCAL_SRC_FILES :=	../../tolua.c \
					../../int64.c \
					../../uint64.c \
					../../pb.c \
					../../lpeg/lpcap.c \
					../../lpeg/lpcode.c \
					../../lpeg/lpprint.c \
					../../lpeg/lptree.c \
					../../lpeg/lpvm.c \
					../../struct.c \
					../../cjson/strbuf.c \
					../../cjson/lua_cjson.c \
					../../cjson/fpconv.c \
					../../luasocket/auxiliar.c \
 					../../luasocket/buffer.c \
 					../../luasocket/except.c \
 					../../luasocket/inet.c \
 					../../luasocket/io.c \
 					../../luasocket/luasocket.c \
 					../../luasocket/mime.c \
 					../../luasocket/options.c \
 					../../luasocket/select.c \
 					../../luasocket/tcp.c \
 					../../luasocket/timeout.c \
 					../../luasocket/udp.c \
 					../../luasocket/usocket.c \
 					../../luasocket/compat.c \
 					
LOCAL_WHOLE_STATIC_LIBRARIES += liblua
include $(BUILD_SHARED_LIBRARY)