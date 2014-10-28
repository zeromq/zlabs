/*  =========================================================================
    zpubsub_option - options for the zpubsub class

    Copyright (c) the Contributors as noted in the AUTHORS file.
    This file is part of CZMQ, the high-level C binding for 0MQ:
    http://czmq.zeromq.org.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

#ifndef __ZPUBSUB_OPTION_H_INCLUDED__
#define __ZPUBSUB_OPTION_H_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

//  @interface
//  Get the currently installed function for calculating port number from
//  domain id. If NULL, the default implementation in the zpubsub class
//  will be used.
ZLABS_EXPORT zpubsub_port_fn *
    zpubsub_port_function (void);


//  Set the function for calculating port number from domain id. If NULL,
//  the default implementation in the zpubsub class will be used.
ZLABS_EXPORT void
    zpubsub_set_port_function (zpubsub_port_fn *port_fn);


//  Get the beacon port number.
ZLABS_EXPORT dbyte
    zpubsub_beacon_port (void);


//  Set the beacon port number (1025-65535).
ZLABS_EXPORT void
    zpubsub_set_beacon_port (dbyte port);


//  Get the beacon interval.
ZLABS_EXPORT int
    zpubsub_beacon_interval (void);


//  Set the beacon interval (1000-60000).
ZLABS_EXPORT void
    zpubsub_set_beacon_interval (int interval);


//  Selftest
ZLABS_EXPORT void
    zpubsub_option_test (bool verbose);
//  @end

#ifdef __cplusplus
}
#endif

#endif
