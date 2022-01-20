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


#include "gc_uvc.h"

int gc_uvc_create(int argc,char *argv[],gc_generic_info info)
{
    usbg_gadget *gadget = gc_init(info);

    if(gadget == NULL)
        return GC_FAILED;

    usbg_function *f_uvc;
    int usbg_ret;

    char *id = gc_generate_id(USBG_F_UVC);

    if(id == NULL)
        return GC_FAILED;

    struct usbg_f_uvc_frame_attrs uvc_frame_attrs_array[] = {
            {
                    .bFrameIndex = 1,
                    .dwFrameInterval = 2000000,
                    .wHeight = 480,
                    .wWidth = 640,
            }, {
                    .bFrameIndex = 2,
                    .dwFrameInterval = 2000000,
                    .wHeight = 1080,
                    .wWidth = 1920,
            }, {
                    .bFrameIndex = 3,
                    .dwFrameInterval = 333333,
                    .wHeight = 1080,
                    .wWidth = 1920,
            }, {
                    .bFrameIndex = 4,
                    .dwFrameInterval = 333333,
                    .wHeight = 2160,
                    .wWidth = 3840,
            }
    };

    struct usbg_f_uvc_frame_attrs *uvc_frame_mjpeg_attrs[] = {
            &uvc_frame_attrs_array[0],
            &uvc_frame_attrs_array[1],
            &uvc_frame_attrs_array[2],
            &uvc_frame_attrs_array[3],
            NULL,
    };

    struct usbg_f_uvc_frame_attrs *uvc_frame_uncompressed_attrs[] = {
            &uvc_frame_attrs_array[0],
            &uvc_frame_attrs_array[1],
            &uvc_frame_attrs_array[2],
            &uvc_frame_attrs_array[3],
            NULL,
    };

    struct usbg_f_uvc_format_attrs uvc_format_attrs_array[] = {
            {
                    .frames = uvc_frame_mjpeg_attrs,
                    .format = "mjpeg/m",
                    .bDefaultFrameIndex = 3,
            }, {
                    .frames = uvc_frame_uncompressed_attrs,
                    .format = "uncompressed/u",
                    .bDefaultFrameIndex = 2,
            }
    };

    struct usbg_f_uvc_format_attrs *uvc_format_attrs[] = {
            &uvc_format_attrs_array[0],
            &uvc_format_attrs_array[1],
            NULL,
    };

    struct usbg_f_uvc_attrs uvc_attrs = {
            .formats = uvc_format_attrs,
    };

    struct usbg_config_strs config_strs = {
            .configuration = id
    };

    usbg_ret = usbg_create_function(gadget,USBG_F_UVC,id,&uvc_attrs,&f_uvc);
    if(usbg_ret != USBG_SUCCESS){
        fprintf(stderr,"failed to create uvc function!  (maybe kernel module not loaded?) \n");
        gc_clean();
        return GC_FAILED;
    }

    /* for now we only create one config file */
    usbg_config *config = gc_get_config(gadget);

    usbg_ret = usbg_add_config_function(config,id,f_uvc);
    if(usbg_ret != USBG_SUCCESS){
        fprintf(stderr,"failed to bind uvc config to function! \n");
        gc_clean();
        return GC_FAILED;
    }

    usbg_ret = usbg_enable_gadget(gadget, DEFAULT_UDC);
    if(usbg_ret != USBG_SUCCESS){
        fprintf(stderr,"failed to enable gadget! \n");
        gc_clean();
        return GC_FAILED;
    }

    gc_clean();
    return GC_SUCCESS;
}

