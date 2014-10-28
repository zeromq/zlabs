/*  =========================================================================
    zlabs_selftest.c - run selftests

    Runs all selftests.

    -------------------------------------------------------------------------
    Copyright (c) the Contributors as noted in the AUTHORS file.       
    This file is part of CZMQ, the high-level C binding for 0MQ:       
    http://czmq.zeromq.org.                                            
                                                                       
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.           

    #################################################################
    #   GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY    #
    #   Please read the README.txt file in the model directory.     #
    #################################################################
    =========================================================================
*/

#include "../include/zlabs.h"

int
main (int argc, char *argv [])
{
    bool verbose;
    if (argc == 2 && streq (argv [1], "-v"))
        verbose = true;
    else
        verbose = false;

    printf ("Running ZLABS selftests...\n");

    zpubsub_test (verbose); 
    zpubsub_option_test (verbose); 
    ztimeout_test (verbose); 
    zpubsub_filter_test (verbose); 

    printf ("Tests passed OK\n");
    return 0;
}
/*
    #################################################################
    #   GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY    #
    #   Please read the README.txt file in the model directory.     #
    #################################################################
*/
