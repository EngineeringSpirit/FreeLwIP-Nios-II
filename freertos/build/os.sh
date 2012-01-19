#! /bin/sh

# set -x
#
# Copyright (C) 2005 Altera Corporation
#
# Altera gives unlimited permission to copy, distribute and modify
# this script.

# This file is included into the auto-generated shell script: generated.sh. It is
# used to provide operating system specific parameters for code download and debug. 

# Symbol names used to follow target thread structures
# TODO incomplete and working...
thread_vars='list:current=*pxCurrentTCB,first=*pxReadyTasksLists[0],id=($T->uxTaskNumber)+1'
