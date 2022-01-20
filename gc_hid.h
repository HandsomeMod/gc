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


#ifndef GC_HID_H
#define GC_HID_H

#include "gc_generic.h"
#include <usbg/function/hid.h>

int gc_hid_create(int argc,char *argv[],gc_generic_info info);

#endif //GC_GC_HID_H
