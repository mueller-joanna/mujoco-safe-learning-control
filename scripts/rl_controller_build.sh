#! /bin/bash

# From https://stackoverflow.com/a/246128
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR/../safe-learning

echo "Source ROS 2 if needed..."
if [ ! -e ros2 ]; then
    source /opt/ros/$ROS_DISTRO/setup.bash
fi

read -p "Do you want to build the custom interface (needed to run project)? [y/n] " -n 1 -r
if [[ $REPLY =~ ^[Yy]$ ]]
then
    colcon build --symlink-install --packages-select sl_interfaces
fi
. install/setup.bash

colcon build --symlink-install --packages-select rl_controller
. install/setup.bash
(trap 'kill 0' SIGINT;
ros2 run rl_controller control)
