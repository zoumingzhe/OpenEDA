# FILE: tclreadlineInit.tcl.in
# $Id: 4d98a82179464f209f4bb5c85a10cd0e958799a9 $
# ---
# tclreadline -- gnu readline for tcl
# https://github.com/flightaware/tclreadline/
# Copyright (c) 1998 - 2014, Johannes Zellner <johannes@zellner.org>
# This software is copyright under the BSD license.
# ---

package provide tclreadline 2.3.8

namespace eval tclreadline:: {
    namespace export Init
}

proc ::tclreadline::Init {} {
    uplevel #0 {
        if {![info exists tclreadline::library]} {
            set msg ""
            foreach dirname {$tclreadline::installPath/src/console/lib [file dirname [info script]]} {
                if {[catch {load [file join $dirname libtclreadline[info sharedlibextension]]} msg] == 0} {
                    set msg ""
                    break
                }
            }
            if {$msg != ""} {
                puts stderr $msg
                exit 2
            }
        }
    }
}

tclreadline::Init
::tclreadline::readline customcompleter ::tclreadline::ScriptCompleter
source [file join [file dirname [info script]] tclreadlineSetup.tcl]


set auto_index(::tclreadline::ScriptCompleter) \
    [list source [file join [file dirname [info script]] tclreadlineCompleter.tcl]]
