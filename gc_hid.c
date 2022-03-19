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


#include "gc_hid.h"

static char report_desc[] = {
        0x05, 0x01,	/* USAGE_PAGE (Generic Desktop)	          */
        0x09, 0x06,	/* USAGE (Keyboard)                       */
        0xa1, 0x01,	/* COLLECTION (Application)               */
        0x05, 0x07,	/*   USAGE_PAGE (Keyboard)                */
        0x19, 0xe0,	/*   USAGE_MINIMUM (Keyboard LeftControl) */
        0x29, 0xe7,	/*   USAGE_MAXIMUM (Keyboard Right GUI)   */
        0x15, 0x00,	/*   LOGICAL_MINIMUM (0)                  */
        0x25, 0x01,	/*   LOGICAL_MAXIMUM (1)                  */
        0x75, 0x01,	/*   REPORT_SIZE (1)                      */
        0x95, 0x08,	/*   REPORT_COUNT (8)                     */
        0x81, 0x02,	/*   INPUT (Data,Var,Abs)                 */
        0x95, 0x01,	/*   REPORT_COUNT (1)                     */
        0x75, 0x08,	/*   REPORT_SIZE (8)                      */
        0x81, 0x03,	/*   INPUT (Cnst,Var,Abs)                 */
        0x95, 0x05,	/*   REPORT_COUNT (5)                     */
        0x75, 0x01,	/*   REPORT_SIZE (1)                      */
        0x05, 0x08,	/*   USAGE_PAGE (LEDs)                    */
        0x19, 0x01,	/*   USAGE_MINIMUM (Num Lock)             */
        0x29, 0x05,	/*   USAGE_MAXIMUM (Kana)                 */
        0x91, 0x02,	/*   OUTPUT (Data,Var,Abs)                */
        0x95, 0x01,	/*   REPORT_COUNT (1)                     */
        0x75, 0x03,	/*   REPORT_SIZE (3)                      */
        0x91, 0x03,	/*   OUTPUT (Cnst,Var,Abs)                */
        0x95, 0x06,	/*   REPORT_COUNT (6)                     */
        0x75, 0x08,	/*   REPORT_SIZE (8)                      */
        0x15, 0x00,	/*   LOGICAL_MINIMUM (0)                  */
        0x25, 0x65,	/*   LOGICAL_MAXIMUM (101)                */
        0x05, 0x07,	/*   USAGE_PAGE (Keyboard)                */
        0x19, 0x00,	/*   USAGE_MINIMUM (Reserved)             */
        0x29, 0x65,	/*   USAGE_MAXIMUM (Keyboard Application) */
        0x81, 0x00,	/*   INPUT (Data,Ary,Abs)                 */
        0xc0		/* END_COLLECTION                         */
};

int gc_hid_create(int argc,char *argv[],gc_generic_info info)
{
    usbg_gadget *gadget = gc_init(info);

    if(gadget == NULL)
        return GC_FAILED;

    usbg_function *f_hid;
    int usbg_ret;

    char *id = gc_generate_id(USBG_F_HID);

    if(id == NULL)
        return GC_FAILED;

    struct usbg_f_hid_attrs f_attrs = {
            .protocol = 1,
            .report_desc = {
                    .desc = report_desc,
                    .len = sizeof(report_desc),
            },
            .report_length = 8,
            .subclass = 0,
    };

    usbg_ret = usbg_create_function(gadget,USBG_F_HID,id,&f_attrs,&f_hid);
    if(usbg_ret != USBG_SUCCESS){
        fprintf(stderr,"failed to create hid function! (maybe kernel module not loaded?)\n");
        gc_clean();
        return GC_FAILED;
    }

    /* for now we only create one config file in */
    usbg_config *config = gc_get_config(gadget);

    usbg_ret = usbg_add_config_function(config,id,f_hid);
    if(usbg_ret != USBG_SUCCESS){
        fprintf(stderr,"failed to bind hid config to function! \n");
        gc_clean();
        return GC_FAILED;
    }

    gc_clean();
    return GC_SUCCESS;
}
