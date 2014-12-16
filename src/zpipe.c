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
    char *uuid_str = zuuid_formatted_str (uuid);
    char *str = zsys_sprintf ("inproc://%s", uuid_str);
    strncpy (self->endpoint, str, 255);
    free (uuid_str);
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
    int rc = zsock_vsend (self->write, picture, argptr);
    va_end (argptr);
    return rc;
}


//  --------------------------------------------------------------------------
//  Read from pipe

int
zpipe_read (zpipe_t *self, const char *picture, ...)
{
    assert (self);
    assert (picture);
    va_list argptr;
    va_start (argptr, picture);
    int rc = zsock_vrecv (self->read, picture, argptr);
    va_end (argptr);
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
    if (verbose)
        printf ("\n");

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
    free (msg);

    zpipe_destroy (&self);
    //  @end

    printf ("OK\n");
    return 0;
}
