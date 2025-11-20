#! /bin/bash

# From https://stackoverflow.com/a/246128
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR/../safe-learning

colcon build --symlink-install
. install/setup.bash
(trap 'kill 0' SIGINT;
ros2 run safe_learning base)
