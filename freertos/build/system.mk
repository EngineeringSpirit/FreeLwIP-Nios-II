# ******************************************************************************
# *                                                                            *
# * License Agreement                                                          *
# *                                                                            *
# * Copyright (c) 2004 Altera Corporation, San Jose, California, USA.          *
# * All rights reserved.                                                       *
# *                                                                            *
# * Permission is hereby granted, free of charge, to any person obtaining a    *
# * copy of this software and associated documentation files (the "Software"), *
# * to deal in the Software without restriction, including without limitation  *
# * the rights to use, copy, modify, merge, publish, distribute, sublicense,   *
# * and/or sell copies of the Software, and to permit persons to whom the      *
# * Software is furnished to do so, subject to the following conditions:       *
# *                                                                            *
# * The above copyright notice and this permission notice shall be included in *
# * all copies or substantial portions of the Software.                        *
# *                                                                            *
# * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR *
# * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   *
# * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE*
# * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER     *
# * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    *
# * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        *
# * DEALINGS IN THE SOFTWARE.                                                  *
# *                                                                            *
# * This agreement shall be governed in all respects by the laws of the State  *
# * of California and by the laws of the United States of America.             *
# *                                                                            *
# * Altera does not recommend, suggest or require that this reference design   *
# * file be used in conjunction or combination with any other product.         *
# ******************************************************************************


# This makefile is included into the top level makefile for system library
# projects. It is responsible for defining the rules used to build the project.
#
# In practice this file defers the rule definitions to the included file, 
# system_rules.mk. This makefile restricts itself to simply configuring some
# variables required by system_rules.mk, and including the makefile fragments
# supplied by each component.
#
# The key feature of this makefile is that it includes the auto-generated
# makefile, generated_all.mk, to obtain a list of the components to build into
# the library. This file defines four lists of components being used,
# which are combined here to form the COMPONENTS variable.
#
# COMPONENTS is then used to construct: the source search path, the include 
# search path, and a list of the makefile fragments supplied by the components 
# (which are then included).

# specify the flavour of system that's being built

SYSTEM = freertos

# specify that we're a system Makefile

PROJ_TYPE = system

# The system library build directory  

SYSTEM_CONFIG_DIR = $(SYSTEM_DIR)/$(SYS_CONFIG)

# The directory in which all gtf generated files are stored

GTF_GENERATED = system_description

# Include the auto-generated list of components.

-include $(GTF_GENERATED)/generated_all.mk

# combine all these that were set in generated_all.mk

COMPONENTS = $(COMPONENTS_SOFTWARE) \
             $(COMPONENTS_DEVICE_DRIVERS) \
             $(COMPONENTS_PROCESSOR) \
             $(COMPONENTS_OS)

# Add on the HAL component

CYGPATH_U = $(shell if [ "X`which cygpath.exe 2> /dev/null`" != "X" ]; \
              then echo "cygpath -u"; else echo "echo"; fi)

COMPONENTS += $(shell $(CYGPATH_U) "$(SOPC_KIT_NIOS2)/components/altera_hal")

# Add the auto-generated C file to the source list

C_SRCS += alt_sys_init.c

# Construct the source searchpath. There is a strict ordering here which is:
#
# 1. current project,
# 2. all components other than the HAL component,
# 3. the HAL component.
#
# This ordering allows the processor component to override the generic HAL in 
# order to provide the processor port of the HAL. The user can then override 
# this with files in their own directory. For example, this allows the user to 
# supply their own device drivers to replace those provided by default by the 
# components.
#
# This construction of the search path uses wildcard expansion to ensure that
# only those directories that exist make it onto the search path, which 
# improves build time.

SRC_DIRS = $(wildcard $(foreach component,$(COMPONENTS), \
                          $(component)/FreeRTOS/src $(component)/HAL/src))
VPATH = \
  ..:$(GTF_GENERATED):$(foreach dir,$(SRC_DIRS),:$(dir))

# Construct the include path. This follows the same search path rules described
# above.

INC_DIRS = $(wildcard $(foreach component, $(COMPONENTS), \
                           $(component)/FreeRTOS/inc $(component)/HAL/inc $(component)/inc))

CPPFLAGS += -I.. -I$(GTF_GENERATED)                     \
            $(foreach dir, $(INCLUDE_PATH), -I$(dir))   \
            $(foreach dir, $(INC_DIRS), -I$(dir))

# Construct a list of the component makefiles

COMPONENT_MAKEFILES = $(wildcard $(foreach component,$(COMPONENTS), \
                                 $(component)/FreeRTOS/src/component.mk  \
                                 $(component)/HAL/src/component.mk))


# Say that we still support the HAL API

CPPFLAGS += -D__hal__

# Include the rules used to build system projects

include $(SOPC_KIT_NIOS2)/components/altera_hal/build/system_rules.mk
