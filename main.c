// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2021 HandsomeMod Project
 *
 * Handsomeyingyan <handsomeyingyan@gmail.com>
 *
 * GC(Gadget Controller) is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gc_config.h"
#include "gc_generic.h"
#include "gc_ffs.h"
#include "gc_hid.h"
#include "gc_midi.h"
#include "gc_printer.h"
#include "gc_serial.h"
#include "gc_uvc.h"
#include "gc_storage.h"
#include "gc_rndis.h"
#include "gc_list.h"
#include "gc_ecm.h"
#include "gc_acm.h"

static int is_root(){
    if(getuid() == 0){
        return 1;
    }else{
        return 0;
    }
}
static void print_help()
{
    printf("Usage : gc [options....] \n");
    printf("-h          Show this help. \n");
    printf("-l          Show active gadget functions. \n");
    printf("-c          Clean all active gadget. \n");
    printf("-a <function> [configs ...]   Add a gadget function. \n");
    printf("-d <name>      Delete a gadget function by name in list (-l). \n");
}

int main(int argc,char *argv[])
{

    if(argc < 2) {
        print_help();
        return 0;
    }

    /* generate vendor info */
    gc_generic_info info;
    info.id_product = strtoul(ID_PRODUCT,0,16);
    info.id_vendor = strtoul(ID_VENDOR,0,16);
    info.manufacturer = MANUFACTURER;
    info.product = PRODUCT;
    info.serial_number = SERIAL_NUMBER;

    /* deal with args */
    if(strcmp(argv[1],"-h") == 0) {
        /* help */
        print_help();
    } else if(strcmp(argv[1],"-l") == 0) {
        /* list gadget functions */
        gc_show_list();
    } else if(strcmp(argv[1],"-a") == 0) {
        /* add a gadget function */
        if(!is_root()){
            printf("you need root to set usb gadget!\n");
            return -1;
        }

        if(argc < 3) {
            printf("You need a function to add ! \n");
            return -1;
        }

        if(strcmp(argv[2],"ffs") == 0){
            if(gc_ffs_create(argc,argv,info) == GC_FAILED)
                return -1;
        }else if(strcmp(argv[2],"hid") == 0){
            if(gc_hid_create(argc,argv,info) == GC_FAILED)
                return -1;
        }else if(strcmp(argv[2],"midi") == 0){
            if(gc_midi_create(argc,argv,info) == GC_FAILED)
                return -1;
        }else if(strcmp(argv[2],"printer") == 0){
            if(gc_printer_create(argc,argv,info) == GC_FAILED)
                return -1;
        }else if(strcmp(argv[2],"serial") == 0){
            if(gc_serial_create(argc,argv,info) == GC_FAILED)
                return -1;
        }else if(strcmp(argv[2],"uvc") == 0){
            if(gc_uvc_create(argc,argv,info) == GC_FAILED)
                return -1;
        }else if(strcmp(argv[2],"mass") == 0){
            if(gc_storage_create(argc,argv,info) == GC_FAILED)
                return -1;
        }else if(strcmp(argv[2],"rndis") == 0){
            if(gc_rndis_create(argc,argv,info) == GC_FAILED)
                return -1;
        }else if(strcmp(argv[2],"ecm") == 0){
            if(gc_ecm_create(argc,argv,info) == GC_FAILED)
                return -1;
        }else if(strcmp(argv[2],"acm") == 0){
            if(gc_acm_create(argc,argv,info) == GC_FAILED)
                return -1;
        }
    } else if(strcmp(argv[1],"-d") == 0) {
        /* delete a gadget function */
        if(argc < 3) {
            printf("You need a gadget to remove ! \n");
            return -1;
        }

        if(gc_delete_function(info,argv[2]) == GC_FAILED){
            fprintf(stderr,"failed to remove gadget function!\n");
            return -1;
        }

        if(!is_root()){
            printf("you need root to delete usb gadget!\n");
            return -1;
        }
    } else if(strcmp(argv[1],"-c") == 0){
        if(!is_root()){
            printf("you need root to clear usb gadget!\n");
            return -1;
        }

        if(gc_remove_all_gadgets(info) != GC_SUCCESS) {
            fprintf(stderr,"failed to remove all gadgets. \n");
            return -1;
        }

    } else if(strcmp(argv[1],"playing") == 0 ){
        /* eastern egg :p */
        /* 'gc playing' is a command in bhuman simrobot */
        printf("Hi ! I'm XBMUAttila1.  \n");
    } else{
        fprintf(stderr,"Invalid Argument!\n");
        return -1;
    }

    return 0;
}
