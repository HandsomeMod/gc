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

#ifndef GC_CONFIG_H
#define GC_CONFIG_H

/* #undef SERIAL_NUMBER */
/* #undef MANUFACTURER */
/* #undef PRODUCT */
/* #undef ID_VENDOR */
/* #undef ID_PRODUCT */

#ifndef SERIAL_NUMBER
#define SERIAL_NUMBER "0123456789"
#endif

#ifndef MANUFACTURER
#define MANUFACTURER "HandsomeTech"
#endif

#ifndef PRODUCT
#define PRODUCT "HandsomeMod Device"
#endif

#ifndef ID_VENDOR
#define ID_VENDOR "0x18d1"
#endif

#ifndef ID_PRODUCT
#define ID_PRODUCT "0xd001"
#endif

#endif //GC_CONFIG_H
