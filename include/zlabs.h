/*  =========================================================================
    zlabs - ZLABS wrapper

    Copyright (c) the Contributors as noted in the AUTHORS file.       
    This file is part of CZMQ, the high-level C binding for 0MQ:       
    http://czmq.zeromq.org.                                            
                                                                       
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.           

    #################################################################
    #   GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY    #
    #   Please read the README.txt file                             #
    #################################################################
    =========================================================================
*/

#ifndef __ZLABS_H_INCLUDED__
#define __ZLABS_H_INCLUDED__

//  External dependencies
#include <zmq.h>
#include <czmq.h>

//  Set up environment for the application
#include "zlabs_prelude.h"

//  ZLABS version macros for compile-time API detection

#define ZLABS_VERSION_MAJOR 1
#define ZLABS_VERSION_MINOR 0
#define ZLABS_VERSION_PATCH 0

#define ZLABS_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define ZLABS_VERSION \
    ZLABS_MAKE_VERSION((ZLABS_VERSION_MAJOR, ZLABS_VERSION_MINOR, ZLABS_VERSION_PATCH)

//  Opaque class structures to allow forward references
typedef struct _zpubsub_t zpubsub_t;
typedef struct _zpubsub_option_t zpubsub_option_t;
typedef struct _ztimeout_t ztimeout_t;
typedef struct _zpubsub_filter_t zpubsub_filter_t;

//  Public API classes
#include "zpubsub.h"
#include "zpubsub_option.h"
#include "ztimeout.h"
#include "zpubsub_filter.h"

#endif
/*
    #################################################################
    #   GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY    #
    #   Please read the README.txt file in the model directory.     #
    #################################################################
*/    
