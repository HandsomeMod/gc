// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2021-2022 HandsomeMod Project
 *
 * Handsomeyingyan <handsomeyingyan@gmail.com>
 *
 * GC(Gadget Controller) is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 */

#ifndef GC_GENERIC_H
#define GC_GENERIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/usb/ch9.h>
#include <usbg/usbg.h>

#define GC_SUCCESS 0
#define GC_FAILED -1
#define GC_CONFIG_PATH "/sys/kernel/config"

typedef enum {
    USB_HOST,
    USB_GADGET
}gc_usb_mode;

// generic info passed by cmake
typedef struct {
    char *serial_number;
    char *manufacturer;
    char *product;
    unsigned int id_vendor;
    unsigned int id_product;
}gc_generic_info;


usbg_gadget * gc_init(gc_generic_info info);

/* 获取usb模式 */
gc_usb_mode gc_get_mode();

/* 生成id */
char *gc_generate_id(usbg_function_type type);

/* 删除特定的function */
int gc_delete_function(gc_generic_info info,char *target);

/* 清除所有的gadget */
int gc_remove_all_gadgets(gc_generic_info info);

/* 销毁usb gadget */
void gc_clean();

/* 手动开启gadget */
int gc_enable_gadget(gc_generic_info info);

/* 手动关闭gadget */
int gc_disable_gadget(gc_generic_info info);

/* 得到config */
usbg_config *gc_get_config(usbg_gadget *gadget);

#endif //GC_GENERIC_H
