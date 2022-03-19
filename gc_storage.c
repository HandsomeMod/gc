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

#include "gc_storage.h"

int gc_storage_create(int argc,char *argv[],gc_generic_info info){
    if(argc < 4){
        fprintf(stderr,"chose a directory to mount! \n");
        return GC_FAILED;
    }

    usbg_gadget *gadget = gc_init(info);

    if(gadget == NULL)
        return GC_FAILED;

    usbg_function *f_mass;
    int usbg_ret;

    char *id = gc_generate_id(USBG_F_MASS_STORAGE);

    if(id == NULL)
        return GC_FAILED;

    struct usbg_f_ms_lun_attrs f_ms_luns_array[] = {
            {
                    .id = -1, /* allows to place in any position */
                    .cdrom = 0,
                    .ro = 0,
                    .nofua = 0,
                    .removable = 1,
                    .file = argv[3],
            }
    };

    struct usbg_f_ms_lun_attrs *f_ms_luns[] = {
            /*
             * When id in lun structure is below 0 we can place it in any
             * arbitrary position
             */
            &f_ms_luns_array[0],
            NULL,
    };

    struct usbg_f_ms_attrs f_attrs = {
            .stall = 0,
            .nluns = 1,
            .luns = f_ms_luns,
    };


    usbg_ret = usbg_create_function(gadget,USBG_F_MASS_STORAGE,id,&f_attrs,&f_mass);
    if(usbg_ret != USBG_SUCCESS){
        fprintf(stderr,"failed to create mass storage function!  (maybe kernel module not loaded?) \n");
        gc_clean();
        return GC_FAILED;
    }

    /* for now we only create one config file */
    usbg_config *config = gc_get_config(gadget);

    usbg_ret = usbg_add_config_function(config,id,f_mass);
    if(usbg_ret != USBG_SUCCESS){
        fprintf(stderr,"failed to bind mass storage config to function! \n");
        gc_clean();
        return GC_FAILED;
    }

    gc_clean();
    return GC_SUCCESS;
}