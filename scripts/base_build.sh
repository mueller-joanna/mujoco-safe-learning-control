#! /bin/bash

# From https://stackoverflow.com/a/246128
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR/../safe-learning

echo "Source ROS 2 if needed..."
if [ ! -e ros2 ]; then
    source /opt/ros/$ROS_DISTRO/setup.bash
fi

build_dir=./install
if [ -d "$build_dir" ]; then
    read -p "Remove previous build files? [y/n] " -n 1 -r
    if [[ $REPLY =~ ^[Yy]$ ]]
    then
        rm -r build/ install/ log/
    fi
fi

echo ""
read -p "Do you want to build the custom interface? Needed to run project. [y/n] " -n 1 -r
if [[ $REPLY =~ ^[Yy]$ ]]
then
    colcon build --symlink-install --packages-select sl_interfaces
fi
# Ensure that interfaces are available at next build step
. install/setup.bash

colcon build --symlink-install --packages-select safe_learning
. install/setup.bash
(trap 'kill 0' SIGINT;
ros2 run safe_learning base)
