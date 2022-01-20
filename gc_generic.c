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


#include "gc_generic.h"
static usbg_state *state = NULL;

#define LUGX_CALL(c) \
{ \
    int usbg_ret; \
    usbg_ret = c; \
    if (usbg_ret != USBG_SUCCESS) { \
        fprintf(stderr, "In: %s (Function: %s)\n", #c, __func__); \
        fprintf(stderr, "Error: %s : %s\n", usbg_error_name(usbg_ret), \
                usbg_strerror(usbg_ret)); \
        return GC_FAILED; \
    } \
}


usbg_gadget * gc_init(gc_generic_info info)
{
    usbg_gadget *g;
    int usbg_ret;

    struct usbg_gadget_attrs g_attrs = {
            .bcdUSB = 0x0200,
            .bDeviceClass =	USB_CLASS_PER_INTERFACE,
            .bDeviceSubClass = 0x00,
            .bDeviceProtocol = 0x00,
            .bMaxPacketSize0 = 64, /* Max allowed ep0 packet size */
            .idVendor = info.id_vendor,
            .idProduct = info.id_product,
            .bcdDevice = 0x0001, /* Verson of device */
    };

    struct usbg_gadget_strs g_strs = {
            .serial = info.serial_number, /* Serial number */
            .manufacturer = info.manufacturer, /* Manufacturer */
            .product = info.product /* Product string */
    };

    usbg_ret = usbg_init(GC_CONFIG_PATH, &state);
    if(usbg_ret != USBG_SUCCESS){
        fprintf(stderr,"failed to init gadget. \n");
        state = NULL;
        return NULL;
    }

    /* Now we just support one otg controller */
    /* if one gadget been created just use it */
    g = usbg_get_first_gadget(state);
    if(g == NULL){
        usbg_ret = usbg_create_gadget(state, "g1", &g_attrs, &g_strs, &g);
        if (usbg_ret != USBG_SUCCESS) {
            fprintf(stderr,"failed to create gadget. \n");
            state = NULL;
            return NULL;
        }
    }

    /* disable gadget for now to apply new config */
    usbg_disable_gadget(g);

    return g;
}

int gc_delete_function(gc_generic_info info,char *target)
{
    usbg_gadget *g;
    gc_init(info);
    g = usbg_get_first_gadget(state);
    if(g == NULL || target == NULL)
        return GC_FAILED;

    usbg_function  *f = usbg_get_first_function(g);
    while (f != NULL) {
        usbg_function *next = usbg_get_next_function(f);
        if(strcmp(usbg_get_function_instance(f),target) == 0){
            usbg_rm_function(f,USBG_RM_RECURSE);
            return GC_SUCCESS;
        }
        f = next;
    }
    return GC_FAILED;
}

static int remove_gadget(usbg_gadget *g)
{
    usbg_udc *u;

    u = usbg_get_gadget_udc(g);
    if (u)
        LUGX_CALL(usbg_disable_gadget(g));

    LUGX_CALL(usbg_rm_gadget(g, USBG_RM_RECURSE));

    return 0;
}

int gc_remove_all_gadgets(gc_generic_info info)
{
    if(gc_init(info) == NULL)
        return GC_FAILED;
    usbg_gadget *g;
    g = usbg_get_first_gadget(state);
    while (g != NULL) {
        usbg_gadget *next = usbg_get_next_gadget(g);
        LUGX_CALL(remove_gadget(g));
        g = next;
    }

    return GC_SUCCESS;
}

/* id generate helper */
static int gc_get_functions()
{
    if(state == NULL)
        return -1;

    usbg_gadget *gadget = usbg_get_first_gadget(state);

    if(gadget == NULL)
        return -1;

    usbg_function *last_function  = usbg_get_first_function(gadget);
    int ci = 0;
    if(last_function == NULL){
        return 0;
    }else{
        while(last_function != NULL){
            if(usbg_get_next_function(last_function) == NULL)
                break;
            last_function = usbg_get_next_function(last_function);
            ci++;
        }
    }

    return ci;

}

/* get first config from gadget */
/* if not find create a new one */
usbg_config *gc_get_config(usbg_gadget *gadget)
{
    if(gadget == NULL)
        return NULL;

    /* default config */
    struct usbg_config_strs config_strs = {
            .configuration = "c1"
    };

    usbg_config *result = usbg_get_first_config(gadget);

    if(result == NULL){
        int usbg_ret = usbg_create_config(gadget,1,"c1",NULL,&config_strs,&result);
        if(usbg_ret != USBG_SUCCESS){
            fprintf(stderr,"failed to get config! \n");
            gc_clean();
            return NULL;
        }
    }

    return result;
}



char *gc_generate_id(usbg_function_type type)
{
    /*
     *  FORMAT : {function}.{number}
     *  the number not order by the number of same function
     *  by number of all exist usb gadgets function.
     *  if gc -l result like this.
     *  ffs.1
     *  ffs.2
     *  mass.4
     *
     *  And you add a new ffs function you will get this.
     *  ffs.4
     *  if some gadget function had been deleted. id will be the number of functions +1.
     */

    char *result = malloc(sizeof(char) * 20);
    int id = gc_get_functions() + 1;

    if(id < 1){
        fprintf(stderr,"failed to get gadgets. \n");
        return NULL;
    }

    if(type == USBG_F_HID){
        sprintf(result,"hid.%d",id);
    }else if(type == USBG_F_ACM){
        sprintf(result,"acm.%d",id);
    }else if(type == USBG_F_ECM){
        sprintf(result,"ecm.%d",id);
    }else if(type == USBG_F_EEM){
        sprintf(result,"eem.%d",id);
    }else if(type == USBG_F_FFS){
        sprintf(result,"ffs.%d",id);
    }else if(type == USBG_F_LOOPBACK){
        sprintf(result,"loopback.%d",id);
    }else if(type == USBG_F_MASS_STORAGE){
        sprintf(result,"mass.%d",id);
    }else if(type == USBG_F_MIDI){
        sprintf(result,"midi.%d",id);
    }else if(type == USBG_F_NCM){
        sprintf(result,"ncm.%d",id);
    }else if(type == USBG_F_OBEX){
        sprintf(result,"obex.%d",id);
    }else if(type == USBG_F_PHONET){
        sprintf(result,"hid.%d",id);
    }else if(type == USBG_F_PRINTER){
        sprintf(result,"printer.%d",id);
    }else if(type == USBG_F_RNDIS){
        sprintf(result,"rndis.%d",id);
    }else if(type == USBG_F_SERIAL){
        sprintf(result,"serial.%d",id);
    }else if(type == USBG_F_SUBSET){
        sprintf(result,"subset.%d",id);
    }else if(type == USBG_F_UAC2){
        sprintf(result,"uac.%d",id);
    }else if(type == USBG_F_UVC){
        sprintf(result,"uvc.%d",id);
    }else{
        return NULL;
    }

    return result;
}

void gc_clean()
{
    usbg_cleanup(state);
}

