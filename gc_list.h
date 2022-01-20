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

#ifndef GC_LIST_H
#define GC_LIST_H

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/sysmacros.h>
#include <netinet/ether.h>
#include <usbg/usbg.h>
#include <usbg/function/ms.h>
#include <usbg/function/net.h>
#include <usbg/function/ffs.h>
#include <usbg/function/phonet.h>
#include <usbg/function/midi.h>
#include <usbg/function/hid.h>
#include <usbg/function/uac2.h>

#include "gc_generic.h"

void gc_show_list();

#endif //GC_LIST_H
