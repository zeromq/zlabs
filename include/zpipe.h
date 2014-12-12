/*  =========================================================================
    zpipe - Encapsulates a pair of sockets connected by inproc.

    Copyright (c) the Contributors as noted in the AUTHORS file.
    This file is part of CZMQ, the high-level C binding for 0MQ:
    http://czmq.zeromq.org.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
    =========================================================================
*/

#ifndef __ZPIPE_H_INCLUDED__
#define __ZPIPE_H_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif


//  @interface
//  Create a new zpipe
ZLABS_EXPORT zpipe_t *
    zpipe_new ();

//  Destroy the zpipe
ZLABS_EXPORT void
    zpipe_destroy (zpipe_t **self_p);

//  Get write socket
ZLABS_EXPORT zsock_t *
    zpipe_write_socket (zpipe_t *self);

//  Get read socket
ZLABS_EXPORT zsock_t *
    zpipe_read_socket (zpipe_t *self);

//  Write to pipe
ZLABS_EXPORT int
    zpipe_write (zpipe_t *self, const char *picture, ...);

//  Read from pipe
ZLABS_EXPORT int
    zpipe_read (zpipe_t *self, const char *picture, ...);

//  Print properties of object
ZLABS_EXPORT void
    zpipe_print (zpipe_t *self);

//  Self test of this class
ZLABS_EXPORT int
    zpipe_test (bool verbose);
//  @end

#ifdef __cplusplus
}
#endif

#endif
