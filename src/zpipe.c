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

/*
@header
    zpipe - Encapsulates a pair of sockets connected by inproc.
@discuss
@end
*/

#include <czmq.h>

//  Structure of our class
struct _zpipe_t {
    zsock_t *write;
    zsock_t *read;
    char endpoint[256];
};

#include "../include/zlabs_library.h"

//  --------------------------------------------------------------------------
//  Create a new zpipe

zpipe_t *
zpipe_new ()
{
    zpipe_t *self = (zpipe_t *) zmalloc (sizeof (zpipe_t));
    assert (self);

    //  Initialize properties
    zuuid_t *uuid = zuuid_new ();
    char *str = zsys_sprintf ("inproc://%s", zuuid_formatted_str (uuid));
    strncpy (self->endpoint, str, 255);
    free (str);
    zuuid_destroy (&uuid);

    char *attach = zsys_sprintf ("@%s", self->endpoint);
    self->write = zsock_new_pair (attach);
    free (attach);
    assert (self->write);
    attach = zsys_sprintf (">%s", self->endpoint);
    self->read = zsock_new_pair (attach);
    free (attach);
    assert (self->read);

    return self;
}


//  --------------------------------------------------------------------------
//  Destroy the zpipe

void
zpipe_destroy (zpipe_t **self_p)
{
    assert (self_p);
    if (*self_p) {
        zpipe_t *self = *self_p;

        //  Free class properties
        zsock_destroy (&self->read);
        zsock_destroy (&self->write);

        //  Free object itself
        free (self);
        *self_p = NULL;
    }
}


//  --------------------------------------------------------------------------
//  Get write socket

zsock_t *
zpipe_write_socket (zpipe_t *self)
{
    assert (self);
    return self->write;
}


//  --------------------------------------------------------------------------
//  Get read socket

zsock_t *
zpipe_read_socket (zpipe_t *self)
{
    assert (self);
    return self->read;
}


//  --------------------------------------------------------------------------
//  Write to pipe

int
zpipe_write (zpipe_t *self, const char *picture, ...)
{
    assert (self);
    assert (picture);

    va_list argptr;
    va_start (argptr, picture);
    zmsg_t *msg = zmsg_new ();
    while (*picture) {
        if (*picture == 'i')
            zmsg_addstrf (msg, "%d", va_arg (argptr, int));
        else
        if (*picture == 'u')
            zmsg_addstrf (msg, "%ud", va_arg (argptr, uint));
        else
        if (*picture == 's')
            zmsg_addstr (msg, va_arg (argptr, char *));
        else
        if (*picture == 'b') {
            //  Note function arguments may be expanded in reverse order,
            //  so we cannot use va_arg macro twice in a single call
            byte *data = va_arg (argptr, byte *);
            zmsg_addmem (msg, data, va_arg (argptr, size_t));
        }
        else
        if (*picture == 'c') {
            zchunk_t *chunk = va_arg (argptr, zchunk_t *);
            assert (zchunk_is (chunk));
            zmsg_addmem (msg, zchunk_data (chunk), zchunk_size (chunk));
        }
        else
        if (*picture == 'f') {
            zframe_t *frame = va_arg (argptr, zframe_t *);
            assert (zframe_is (frame));
            zmsg_addmem (msg, zframe_data (frame), zframe_size (frame));
        }
        else
        if (*picture == 'p') {
            void *pointer = va_arg (argptr, void *);
            zmsg_addmem (msg, &pointer, sizeof (void *));
        }
        else
        if (*picture == 'h') {
            zhashx_t *hash = va_arg (argptr, zhashx_t *);
            zframe_t *frame = zhashx_pack (hash);
            zmsg_append (msg, &frame);
        }
        else
        if (*picture == 'm') {
            zframe_t *frame;
            zmsg_t *zmsg = va_arg (argptr, zmsg_t *);
            for (frame = zmsg_first (zmsg); frame; frame = zmsg_next (zmsg) ) {
                zframe_t *frame_dup = zframe_dup (frame);
                zmsg_append (msg, &frame_dup);
            }
        }
        else
        if (*picture == 'z')
            zmsg_addmem (msg, NULL, 0);
        else {
            zsys_error ("zpipe: invalid picture element '%c'", *picture);
            assert (false);
        }
        picture++;
    }
    va_end (argptr);
    return zmsg_send (&msg, self->write);
}


//  --------------------------------------------------------------------------
//  Read from pipe

int
zpipe_read (zpipe_t *self, const char *picture, ...)
{
    assert (self);
    assert (picture);
    zmsg_t *msg = zmsg_recv (self->read);
    if (!msg)
        return -1;              //  Interrupted

    int rc = 0;
    va_list argptr;
    va_start (argptr, picture);
    while (*picture) {
        if (*picture == 'i') {
            char *string = zmsg_popstr (msg);
            int *int_p = va_arg (argptr, int *);
            if (int_p)
                *int_p = string ? atoi (string) : 0;
            free (string);
        }
        else
        if (*picture == 'u') {
            char *string = zmsg_popstr (msg);
            uint *uint_p = va_arg (argptr, uint *);
            if (uint_p)
                *uint_p = string ? (uint) atol (string) : 0;
            free (string);
        }
        else
        if (*picture == 's') {
            char *string = zmsg_popstr (msg);
            char **string_p = va_arg (argptr, char **);
            if (string_p)
                *string_p = string;
            else
                free (string);
        }
        else
        if (*picture == 'b') {
            zframe_t *frame = zmsg_pop (msg);
            byte **data_p = va_arg (argptr, byte **);
            size_t *size = va_arg (argptr, size_t *);
            if (data_p) {
                if (frame) {
                    *size = zframe_size (frame);
                    *data_p = (byte *) malloc (*size);
                    memcpy (*data_p, zframe_data (frame), *size);
                }
                else {
                    *data_p = NULL;
                    *size = 0;
                }
            }
            zframe_destroy (&frame);
        }
        else
        if (*picture == 'c') {
            zframe_t *frame = zmsg_pop (msg);
            zchunk_t **chunk_p = va_arg (argptr, zchunk_t **);
            if (chunk_p) {
                if (frame)
                    *chunk_p = zchunk_new (zframe_data (frame), zframe_size (frame));
                else
                    *chunk_p = NULL;
            }
            zframe_destroy (&frame);
        }
        else
        if (*picture == 'f') {
            zframe_t *frame = zmsg_pop (msg);
            zframe_t **frame_p = va_arg (argptr, zframe_t **);
            if (frame_p)
                *frame_p = frame;
            else
                zframe_destroy (&frame);
        }
        else
        if (*picture == 'p') {
            zframe_t *frame = zmsg_pop (msg);
            void **pointer_p = va_arg (argptr, void **);
            if (pointer_p) {
                if (frame) {
                    if (zframe_size (frame) == sizeof (void *))
                        *pointer_p = *((void **) zframe_data (frame));
                    else
                        rc = -1;
                }
                else
                    *pointer_p = NULL;
            }
            zframe_destroy (&frame);
        }
        else
        if (*picture == 'h') {
            zframe_t *frame = zmsg_pop (msg);
            zhashx_t **hash_p = va_arg (argptr, zhashx_t **);
            if (hash_p) {
                if (frame)
                    *hash_p = zhashx_unpack (frame);
                else
                    *hash_p = NULL;
            }
            zframe_destroy (&frame);
        }
        else
        if (*picture == 'm') {
            zmsg_t **zmsg_p = va_arg (argptr, zmsg_t **);
            if (zmsg_p) {
                *zmsg_p = zmsg_new ();
                zframe_t *frame;
                while ((frame = zmsg_pop (msg)))
                    zmsg_append (*zmsg_p, &frame);
            }
        }
        else
        if (*picture == 'z') {
            zframe_t *frame = zmsg_pop (msg);
            if (frame && zframe_size (frame) != 0)
                rc = -1;
            zframe_destroy (&frame);
        }
        else {
            zsys_error ("zpipe: invalid picture element '%c'", *picture);
            assert (false);
        }
        picture++;
    }
    va_end (argptr);
    zmsg_destroy (&msg);
    return rc;
}


//  --------------------------------------------------------------------------
//  Print properties of object

void
zpipe_print (zpipe_t *self)
{
    assert (self);

    zsys_debug ("zpipe @ %s", self->endpoint);
}


//  --------------------------------------------------------------------------
//  Selftest

int
zpipe_test (bool verbose)
{
    printf (" * zpipe: ");

    //  @selftest
    zpipe_t *self = zpipe_new (NULL);
    assert (self);

    if (verbose)
        zpipe_print (self);

    int rc = zpipe_write (self, "s", "Test");
    assert (rc == 0);
    char *msg;
    rc = zpipe_read (self, "s", &msg);
    assert (rc == 0);
    assert (msg != NULL);
    assert (streq (msg, "Test"));

    zpipe_destroy (&self);
    //  @end

    printf ("OK\n");
    return 0;
}
