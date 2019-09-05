LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libtest

LOCAL_SDCLANG := false
LOCAL_MODULE_RELATIVE_PATH := soundfx

LOCAL_SRC_FILES := \
	test.c
LOCAL_CFLAGS += -Wno-unused-parameter

include $(BUILD_SHARED_LIBRARY)
