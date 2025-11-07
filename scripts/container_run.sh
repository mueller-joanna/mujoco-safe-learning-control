#! /bin/bash

# From https://stackoverflow.com/a/246128
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR/..

user_id=$(id -rg)

# Adapted from 
# - https://discussion.fedoraproject.org/t/cannot-run-wayland-gui-app-in-podman/105151/2
# - https://stackoverflow.com/a/51209546/10512964
podman run -it -e "DISPLAY" \
--net host \
-e WAYLAND_DISPLAY=$WAYLAND_DISPLAY -e XDG_RUNTIME_DIR=$XDG_RUNTIME_DIR \
-v "${XAUTHORITY:-$HOME/.Xauthority}:/root/.Xauthority:Z" --privileged \
-v /tmp/.X11-unix:/tmp/.X11-unix:Z  \
-v ./:/app:Z -v "/run/user/$user_id/wayland-0:/tmp/wayland-0:Z" --gpus all \
safe_learning:0.0.1 bash
