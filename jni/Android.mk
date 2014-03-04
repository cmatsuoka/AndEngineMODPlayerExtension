LOCAL_PATH	:= $(call my-dir)
#LOCAL_ARM_MODE	:= arm

include $(CLEAR_VARS)

include $(LOCAL_PATH)/src/Makefile
include $(LOCAL_PATH)/src/loaders/Makefile

SRC_SOURCES	:= $(addprefix src/,$(SRC_OBJS))
LOADERS_SOURCES := $(addprefix src/loaders/,$(LOADERS_OBJS))

LOCAL_MODULE    := xmp-coremod
LOCAL_CFLAGS	:= -O3 -DHAVE_CONFIG_H -I$(LOCAL_PATH)/include/coremod \
		   -I$(LOCAL_PATH)/src -DLIBXMP_CORE_PLAYER \
		   -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
LOCAL_LDLIBS	:=
LOCAL_SRC_FILES := interface.c \
	$(SRC_SOURCES:.o=.c) \
	$(LOADERS_SOURCES:.o=.c)

include $(BUILD_SHARED_LIBRARY)
