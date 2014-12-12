/*  =========================================================================
    zpubsub_filter - zpubsub protocol

    Codec class for zpubsub_filter.

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

/*
@header
    zpubsub_filter - zpubsub protocol
@discuss
@end
*/

#include "../include/zpubsub_filter.h"

//  Structure of our class

struct _zpubsub_filter_t {
    zframe_t *routing_id;               //  Routing_id from ROUTER, if any
    int id;                             //  zpubsub_filter message ID
    byte *needle;                       //  Read/write pointer for serialization
    byte *ceiling;                      //  Valid upper limit for read pointer
    char partition [256];               //  Partition name
    char topic [256];                   //  Topic name
};

//  --------------------------------------------------------------------------
//  Network data encoding macros

//  Put a block of octets to the frame
#define PUT_OCTETS(host,size) { \
    memcpy (self->needle, (host), size); \
    self->needle += size; \
}

//  Get a block of octets from the frame
#define GET_OCTETS(host,size) { \
    if (self->needle + size > self->ceiling) { \
        zsys_warning ("zpubsub_filter: GET_OCTETS failed"); \
        goto malformed; \
    } \
    memcpy ((host), self->needle, size); \
    self->needle += size; \
}

//  Put a 1-byte number to the frame
#define PUT_NUMBER1(host) { \
    *(byte *) self->needle = (host); \
    self->needle++; \
}

//  Put a 2-byte number to the frame
#define PUT_NUMBER2(host) { \
    self->needle [0] = (byte) (((host) >> 8)  & 255); \
    self->needle [1] = (byte) (((host))       & 255); \
    self->needle += 2; \
}

//  Put a 4-byte number to the frame
#define PUT_NUMBER4(host) { \
    self->needle [0] = (byte) (((host) >> 24) & 255); \
    self->needle [1] = (byte) (((host) >> 16) & 255); \
    self->needle [2] = (byte) (((host) >> 8)  & 255); \
    self->needle [3] = (byte) (((host))       & 255); \
    self->needle += 4; \
}

//  Put a 8-byte number to the frame
#define PUT_NUMBER8(host) { \
    self->needle [0] = (byte) (((host) >> 56) & 255); \
    self->needle [1] = (byte) (((host) >> 48) & 255); \
    self->needle [2] = (byte) (((host) >> 40) & 255); \
    self->needle [3] = (byte) (((host) >> 32) & 255); \
    self->needle [4] = (byte) (((host) >> 24) & 255); \
    self->needle [5] = (byte) (((host) >> 16) & 255); \
    self->needle [6] = (byte) (((host) >> 8)  & 255); \
    self->needle [7] = (byte) (((host))       & 255); \
    self->needle += 8; \
}

//  Get a 1-byte number from the frame
#define GET_NUMBER1(host) { \
    if (self->needle + 1 > self->ceiling) { \
        zsys_warning ("zpubsub_filter: GET_NUMBER1 failed"); \
        goto malformed; \
    } \
    (host) = *(byte *) self->needle; \
    self->needle++; \
}

//  Get a 2-byte number from the frame
#define GET_NUMBER2(host) { \
    if (self->needle + 2 > self->ceiling) { \
        zsys_warning ("zpubsub_filter: GET_NUMBER2 failed"); \
        goto malformed; \
    } \
    (host) = ((uint16_t) (self->needle [0]) << 8) \
           +  (uint16_t) (self->needle [1]); \
    self->needle += 2; \
}

//  Get a 4-byte number from the frame
#define GET_NUMBER4(host) { \
    if (self->needle + 4 > self->ceiling) { \
        zsys_warning ("zpubsub_filter: GET_NUMBER4 failed"); \
        goto malformed; \
    } \
    (host) = ((uint32_t) (self->needle [0]) << 24) \
           + ((uint32_t) (self->needle [1]) << 16) \
           + ((uint32_t) (self->needle [2]) << 8) \
           +  (uint32_t) (self->needle [3]); \
    self->needle += 4; \
}

//  Get a 8-byte number from the frame
#define GET_NUMBER8(host) { \
    if (self->needle + 8 > self->ceiling) { \
        zsys_warning ("zpubsub_filter: GET_NUMBER8 failed"); \
        goto malformed; \
    } \
    (host) = ((uint64_t) (self->needle [0]) << 56) \
           + ((uint64_t) (self->needle [1]) << 48) \
           + ((uint64_t) (self->needle [2]) << 40) \
           + ((uint64_t) (self->needle [3]) << 32) \
           + ((uint64_t) (self->needle [4]) << 24) \
           + ((uint64_t) (self->needle [5]) << 16) \
           + ((uint64_t) (self->needle [6]) << 8) \
           +  (uint64_t) (self->needle [7]); \
    self->needle += 8; \
}

//  Put a string to the frame
#define PUT_STRING(host) { \
    size_t string_size = strlen (host); \
    PUT_NUMBER1 (string_size); \
    memcpy (self->needle, (host), string_size); \
    self->needle += string_size; \
}

//  Get a string from the frame
#define GET_STRING(host) { \
    size_t string_size; \
    GET_NUMBER1 (string_size); \
    if (self->needle + string_size > (self->ceiling)) { \
        zsys_warning ("zpubsub_filter: GET_STRING failed"); \
        goto malformed; \
    } \
    memcpy ((host), self->needle, string_size); \
    (host) [string_size] = 0; \
    self->needle += string_size; \
}

//  Put a long string to the frame
#define PUT_LONGSTR(host) { \
    size_t string_size = strlen (host); \
    PUT_NUMBER4 (string_size); \
    memcpy (self->needle, (host), string_size); \
    self->needle += string_size; \
}

//  Get a long string from the frame
#define GET_LONGSTR(host) { \
    size_t string_size; \
    GET_NUMBER4 (string_size); \
    if (self->needle + string_size > (self->ceiling)) { \
        zsys_warning ("zpubsub_filter: GET_LONGSTR failed"); \
        goto malformed; \
    } \
    free ((host)); \
    (host) = (char *) malloc (string_size + 1); \
    memcpy ((host), self->needle, string_size); \
    (host) [string_size] = 0; \
    self->needle += string_size; \
}


//  --------------------------------------------------------------------------
//  Create a new zpubsub_filter

zpubsub_filter_t *
zpubsub_filter_new (void)
{
    zpubsub_filter_t *self = (zpubsub_filter_t *) zmalloc (sizeof (zpubsub_filter_t));
    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the zpubsub_filter

void
zpubsub_filter_destroy (zpubsub_filter_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        zpubsub_filter_t *self = *self_p;

        //  Free class properties
        zframe_destroy (&self->routing_id);

        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}


//  --------------------------------------------------------------------------
//  Receive a zpubsub_filter from the socket. Returns 0 if OK, -1 if
//  there was an error. Blocks if there is no message waiting.

int
zpubsub_filter_recv (zpubsub_filter_t *self, zsock_t *input)
{
    assert (input);
    
    if (zsock_type (input) == ZMQ_ROUTER) {
        zframe_destroy (&self->routing_id);
        self->routing_id = zframe_recv (input);
        if (!self->routing_id || !zsock_rcvmore (input)) {
            zsys_warning ("zpubsub_filter: no routing ID");
            return -1;          //  Interrupted or malformed
        }
    }
    zmq_msg_t frame;
    zmq_msg_init (&frame);
    int size = zmq_msg_recv (&frame, zsock_resolve (input), 0);
    if (size == -1) {
        zsys_warning ("zpubsub_filter: interrupted");
        goto malformed;         //  Interrupted
    }
    //  Get and check protocol signature
    self->needle = (byte *) zmq_msg_data (&frame);
    self->ceiling = self->needle + zmq_msg_size (&frame);
    
    uint16_t signature;
    GET_NUMBER2 (signature);
    if (signature != (0xAAA0 | 7)) {
        zsys_warning ("zpubsub_filter: invalid signature");
        //  TODO: discard invalid messages and loop, and return
        //  -1 only on interrupt
        goto malformed;         //  Interrupted
    }
    //  Get message id and parse per message type
    GET_NUMBER1 (self->id);

    switch (self->id) {
        case ZPUBSUB_FILTER_FILTER:
            {
                uint16_t magic;
                GET_NUMBER2 (magic);
                if (magic != ZPUBSUB_FILTER_MAGIC_NUMBER) {
                    zsys_warning ("zpubsub_filter: magic is invalid");
                    goto malformed;
                }
            }
            {
                uint16_t version;
                GET_NUMBER2 (version);
                if (version != ZPUBSUB_FILTER_VERSION) {
                    zsys_warning ("zpubsub_filter: version is invalid");
                    goto malformed;
                }
            }
            GET_STRING (self->partition);
            GET_STRING (self->topic);
            break;

        default:
            zsys_warning ("zpubsub_filter: bad message ID");
            goto malformed;
    }
    //  Successful return
    zmq_msg_close (&frame);
    return 0;

    //  Error returns
    malformed:
        zsys_warning ("zpubsub_filter: zpubsub_filter malformed message, fail");
        zmq_msg_close (&frame);
        return -1;              //  Invalid message
}


//  --------------------------------------------------------------------------
//  Send the zpubsub_filter to the socket. Does not destroy it. Returns 0 if
//  OK, else -1.

int
zpubsub_filter_send (zpubsub_filter_t *self, zsock_t *output)
{
    assert (self);
    assert (output);

    if (zsock_type (output) == ZMQ_ROUTER)
        zframe_send (&self->routing_id, output, ZFRAME_MORE + ZFRAME_REUSE);

    size_t frame_size = 2 + 1;          //  Signature and message ID
    switch (self->id) {
        case ZPUBSUB_FILTER_FILTER:
            frame_size += 2;            //  magic
            frame_size += 2;            //  version
            frame_size += 1 + strlen (self->partition);
            frame_size += 1 + strlen (self->topic);
            break;
    }
    //  Now serialize message into the frame
    zmq_msg_t frame;
    zmq_msg_init_size (&frame, frame_size);
    self->needle = (byte *) zmq_msg_data (&frame);
    PUT_NUMBER2 (0xAAA0 | 7);
    PUT_NUMBER1 (self->id);
    size_t nbr_frames = 1;              //  Total number of frames to send
    
    switch (self->id) {
        case ZPUBSUB_FILTER_FILTER:
            PUT_NUMBER2 (ZPUBSUB_FILTER_MAGIC_NUMBER);
            PUT_NUMBER2 (ZPUBSUB_FILTER_VERSION);
            PUT_STRING (self->partition);
            PUT_STRING (self->topic);
            break;

    }
    //  Now send the data frame
    zmq_msg_send (&frame, zsock_resolve (output), --nbr_frames? ZMQ_SNDMORE: 0);
    
    return 0;
}


//  --------------------------------------------------------------------------
//  Print contents of message to stdout

void
zpubsub_filter_print (zpubsub_filter_t *self)
{
    assert (self);
    switch (self->id) {
        case ZPUBSUB_FILTER_FILTER:
            zsys_debug ("ZPUBSUB_FILTER_FILTER:");
            zsys_debug ("    magic=zpubsub_filter_magic_number");
            zsys_debug ("    version=zpubsub_filter_version");
            if (self->partition)
                zsys_debug ("    partition='%s'", self->partition);
            else
                zsys_debug ("    partition=");
            if (self->topic)
                zsys_debug ("    topic='%s'", self->topic);
            else
                zsys_debug ("    topic=");
            break;
            
    }
}


//  --------------------------------------------------------------------------
//  Get/set the message routing_id

zframe_t *
zpubsub_filter_routing_id (zpubsub_filter_t *self)
{
    assert (self);
    return self->routing_id;
}

void
zpubsub_filter_set_routing_id (zpubsub_filter_t *self, zframe_t *routing_id)
{
    if (self->routing_id)
        zframe_destroy (&self->routing_id);
    self->routing_id = zframe_dup (routing_id);
}


//  --------------------------------------------------------------------------
//  Get/set the zpubsub_filter id

int
zpubsub_filter_id (zpubsub_filter_t *self)
{
    assert (self);
    return self->id;
}

void
zpubsub_filter_set_id (zpubsub_filter_t *self, int id)
{
    self->id = id;
}

//  --------------------------------------------------------------------------
//  Return a printable command string

const char *
zpubsub_filter_command (zpubsub_filter_t *self)
{
    assert (self);
    switch (self->id) {
        case ZPUBSUB_FILTER_FILTER:
            return ("FILTER");
            break;
    }
    return "?";
}

//  --------------------------------------------------------------------------
//  Get/set the partition field

const char *
zpubsub_filter_partition (zpubsub_filter_t *self)
{
    assert (self);
    return self->partition;
}

void
zpubsub_filter_set_partition (zpubsub_filter_t *self, const char *value)
{
    assert (self);
    assert (value);
    if (value == self->partition)
        return;
    strncpy (self->partition, value, 255);
    self->partition [255] = 0;
}


//  --------------------------------------------------------------------------
//  Get/set the topic field

const char *
zpubsub_filter_topic (zpubsub_filter_t *self)
{
    assert (self);
    return self->topic;
}

void
zpubsub_filter_set_topic (zpubsub_filter_t *self, const char *value)
{
    assert (self);
    assert (value);
    if (value == self->topic)
        return;
    strncpy (self->topic, value, 255);
    self->topic [255] = 0;
}



//  --------------------------------------------------------------------------
//  Selftest

int
zpubsub_filter_test (bool verbose)
{
    printf (" * zpubsub_filter: ");

    //  @selftest
    //  Simple create/destroy test
    zpubsub_filter_t *self = zpubsub_filter_new ();
    assert (self);
    zpubsub_filter_destroy (&self);

    //  Create pair of sockets we can send through
    zsock_t *input = zsock_new (ZMQ_ROUTER);
    assert (input);
    zsock_connect (input, "inproc://selftest-zpubsub_filter");

    zsock_t *output = zsock_new (ZMQ_DEALER);
    assert (output);
    zsock_bind (output, "inproc://selftest-zpubsub_filter");

    //  Encode/send/decode and verify each message type
    int instance;
    self = zpubsub_filter_new ();
    zpubsub_filter_set_id (self, ZPUBSUB_FILTER_FILTER);

    zpubsub_filter_set_partition (self, "Life is short but Now lasts for ever");
    zpubsub_filter_set_topic (self, "Life is short but Now lasts for ever");
    //  Send twice
    zpubsub_filter_send (self, output);
    zpubsub_filter_send (self, output);

    for (instance = 0; instance < 2; instance++) {
        zpubsub_filter_recv (self, input);
        assert (zpubsub_filter_routing_id (self));
        assert (streq (zpubsub_filter_partition (self), "Life is short but Now lasts for ever"));
        assert (streq (zpubsub_filter_topic (self), "Life is short but Now lasts for ever"));
    }

    zpubsub_filter_destroy (&self);
    zsock_destroy (&input);
    zsock_destroy (&output);
    //  @end

    printf ("OK\n");
    return 0;
}
