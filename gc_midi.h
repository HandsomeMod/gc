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


#ifndef GC_MIDI_H
#define GC_MIDI_H
#include "gc_generic.h"
#include <usbg/function/midi.h>

int gc_midi_create(int argc,char *argv[],gc_generic_info info);
#endif //GC_MIDI_H
