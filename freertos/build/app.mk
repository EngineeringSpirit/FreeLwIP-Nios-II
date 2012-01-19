# ******************************************************************************
# *                                                                            *
# * License Agreement                                                          *
# *                                                                            *
# * Copyright (c) 2003 Altera Corporation, San Jose, California, USA.          *
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

# This makefile is included into the top level makefile for application
# projects. It is responsible for defining the rules used to build the project.
#
# In practice this file defers the rule definitions to the included file, 
# app_rules.mk. This makefile restricts itself to simply configuring some
# variables required by app_rules.mk, and including the makefile fragments
# supplied by each component.
#
# The key feature of this makefile is that it includes the auto-generated
# makefile, generated_all.mk, to obtain a list of the components built into
# the library. This file defines four lists of components being used,
# which are combined here to form the COMPONENTS variable.
#
# COMPONENTS is then used to construct: the include search path, and a list of 
# the makefile fragments supplied by the components (which are then included).

# specify the flavour of system that's being built

SYSTEM = freertos

# specify that we're an application Makefile

PROJ_TYPE = app

# The system library build directory  

SYSTEM_CONFIG_DIR = $(SYSTEM_DIR)/$(SYS_CONFIG)

# The directory in which all gtf generated files are stored

GTF_GENERATED = $(SYSTEM_CONFIG_DIR)/system_description

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

# Construct the source searchpath. 
#
# Source files are listed relative to the project directory, which is the
# parent directory for the current directory.

VPATH = ..

# Construct the include path based on the available components

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

# Include the rules used to build application projects

include $(SOPC_KIT_NIOS2)/components/altera_hal/build/app_rules.mk
