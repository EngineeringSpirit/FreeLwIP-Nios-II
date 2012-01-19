#!/bin/sh

# search the altera install path
ALT_PATH="/opt/altera";

if ! ls "${ALT_PATH}" > /dev/null >2 /dev/null; then
    read -p "altera path: [/opt/altera] " ALT_PATH;
fi

if ! ls "${ALT_PATH}" > /dev/null >2 /dev/null; then
    echo "Error: No altera directory found!";
    exit 1;
fi

# get the latest version
ALT_VERSION=`ls "${ALT_PATH}" | tail -1`;

# build arguments
M_NIOS="${ALT_PATH}/${ALT_VERSION}";
M_INST=`pwd`;

echo "Launching the Nios II Command Shell, please standby....";

# launce Nios command shell
"${M_NIOS}/nios2eds/nios2_command_shell.sh" "${M_INST}/alt-freertos-lwip-do-install.sh" "${M_NIOS}" "${M_INST}"
