#! /bin/bash

# From https://stackoverflow.com/a/246128
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR/../safe-learning

if [ ! -e ros2 ]; then
    echo "Source ROS 2..."
    source /opt/ros/$ROS_DISTRO/setup.bash
fi

colcon build --symlink-install
. install/setup.bash
(trap 'kill 0' SIGINT;
ros2 run safe_learning base &
ros2 run rl_controller control
)
