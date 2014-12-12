/*  =========================================================================
    zpubsub_filter - zpubsub protocol
    
    Codec header for zpubsub_filter.

    ** WARNING *************************************************************
    THIS SOURCE FILE IS 100% GENERATED. If you edit this file, you will lose
    your changes at the next build cycle. This is great for temporary printf
    statements. DO NOT MAKE ANY CHANGES YOU WISH TO KEEP. The correct places
    for commits are:

     * The XML model used for this code generation: zpubsub_filter.xml, or
     * The code generation script that built this file: zproto_codec_c
    ************************************************************************
    Copyright (c) the Contributors as noted in the AUTHORS file.       
    This file is part of CZMQ, the high-level C binding for 0MQ:       
    http://czmq.zeromq.org.                                            
                                                                       
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.           
    =========================================================================
*/

#ifndef __ZPUBSUB_FILTER_H_INCLUDED__
#define __ZPUBSUB_FILTER_H_INCLUDED__

/*  These are the zpubsub_filter messages:

    FILTER - A zpubsub filter.
        magic               number 2    Magic number
        version             number 2    Version
        partition           string      Partition name
        topic               string      Topic name
*/

#define ZPUBSUB_FILTER_MAGIC_NUMBER         666
#define ZPUBSUB_FILTER_VERSION              2

#define ZPUBSUB_FILTER_FILTER               1

#include <czmq.h>

#ifdef __cplusplus
extern "C" {
#endif

//  Opaque class structure
#ifndef ZPUBSUB_FILTER_T_DEFINED
typedef struct _zpubsub_filter_t zpubsub_filter_t;
#define ZPUBSUB_FILTER_T_DEFINED
#endif

//  @interface
//  Create a new empty zpubsub_filter
zpubsub_filter_t *
    zpubsub_filter_new (void);

//  Destroy a zpubsub_filter instance
void
    zpubsub_filter_destroy (zpubsub_filter_t **self_p);

//  Receive a zpubsub_filter from the socket. Returns 0 if OK, -1 if
//  there was an error. Blocks if there is no message waiting.
int
    zpubsub_filter_recv (zpubsub_filter_t *self, zsock_t *input);

//  Send the zpubsub_filter to the output socket, does not destroy it
int
    zpubsub_filter_send (zpubsub_filter_t *self, zsock_t *output);
    
//  Print contents of message to stdout
void
    zpubsub_filter_print (zpubsub_filter_t *self);

//  Get/set the message routing id
zframe_t *
    zpubsub_filter_routing_id (zpubsub_filter_t *self);
void
    zpubsub_filter_set_routing_id (zpubsub_filter_t *self, zframe_t *routing_id);

//  Get the zpubsub_filter id and printable command
int
    zpubsub_filter_id (zpubsub_filter_t *self);
void
    zpubsub_filter_set_id (zpubsub_filter_t *self, int id);
const char *
    zpubsub_filter_command (zpubsub_filter_t *self);

//  Get/set the partition field
const char *
    zpubsub_filter_partition (zpubsub_filter_t *self);
void
    zpubsub_filter_set_partition (zpubsub_filter_t *self, const char *value);

//  Get/set the topic field
const char *
    zpubsub_filter_topic (zpubsub_filter_t *self);
void
    zpubsub_filter_set_topic (zpubsub_filter_t *self, const char *value);

//  Self test of this class
int
    zpubsub_filter_test (bool verbose);
//  @end

//  For backwards compatibility with old codecs
#define zpubsub_filter_dump  zpubsub_filter_print

#ifdef __cplusplus
}
#endif

#endif
