/*  =========================================================================
    zlabs.h - zlabs wrapper

    Copyright (c) the Contributors as noted in the AUTHORS file.
    This file is part of zlabs, an experimentation project for CZMQ:
    http://czmq.zeromq.org.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

#ifndef __ZLABS_H_INCLUDED__
#define __ZLABS_H_INCLUDED__

//  Set up environment for the application

#include "czmq.h"

//  zlabs version macros for compile-time API detection

#define ZLABS_VERSION_MAJOR 1
#define ZLABS_VERSION_MINOR 0
#define ZLABS_VERSION_PATCH 0

#define ZLABS_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define ZLABS_VERSION \
    ZLABS_MAKE_VERSION(ZLABS_VERSION_MAJOR, ZLABS_VERSION_MINOR, ZLABS_VERSION_PATCH)

//  Opaque class structures to allow forward references
typedef struct _zpubsub_t zpubsub_t;
typedef struct _zpubsub_filter_t zpubsub_filter_t;
typedef struct _ztimeout_t ztimeout_t;
typedef struct _ztimeout_node_t ztimeout_node_t;

//  Public API classes
#include "zpubsub_filter.h"
#include "zpubsub.h"
#include "zpubsub_option.h"
#include "ztimeout.h"

#endif
