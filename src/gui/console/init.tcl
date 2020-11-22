#!/bin/sh
# ---
# tclreadline -- gnu readline for tcl
# https://github.com/flightaware/tclreadline/
# Copyright (c) 1998 - 2014, Johannes Zellner <johannes@zellner.org>
# This software is copyright under the BSD license.
# ---


if {$tcl_interactive} {

    package require tclreadline

    set tclreadline::historyLength 0

    set tclreadline::autosave 1

    # go to tclrealdine's main loop.
    #
    tclreadline::Loop
}

# vim:set ft=tcl:
