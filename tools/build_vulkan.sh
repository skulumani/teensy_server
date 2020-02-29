#!/bin/bash

VULKAN_VERSION=1.2.131.2
WORK_DIR=$(mktemp -d)

VULKAN_URL="https://vulkan.lunarg.com/sdk/home#sdk/downloadConfirm/1.2.131.2/linux/vulkansdk-linux-x86_64-${VULKAN_VERSION}.tar.gz"

# make sure tmp dir was actually created
if [[ ! -d "$WORK_DIR" ]]; then
    echo "Could not create temp directory"
    exit 1
fi

# delete temp dir
cleanup () {
    rm -rf "$WORK_DIR"
    echo "Deleted temp working directory: $WORK_DIR"
}

set +e
options=("Yes" "No" "Quit")
prompt () {
    echo "Are you sure you want to $1"
    select yn in "${options[@]}"; do
        case $yn in
            Yes ) eval "$2";
                break;;
            No ) break;;
            Quit ) exit;;
        esac
    done
}

# register cleanup function to be called on exit
trap cleanup EXIT

sudo apt install libxcb1-dev xorg-dev

cd $WORK_DIR

