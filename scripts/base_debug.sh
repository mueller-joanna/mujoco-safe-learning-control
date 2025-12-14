#! /bin/bash

# From https://stackoverflow.com/a/246128
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
cd $SCRIPT_DIR/../safe-learning

echo "Source ROS 2 if needed..."
if [ ! -e ros2 ]; then
    source /opt/ros/$ROS_DISTRO/setup.bash
fi

build_dir=./install
rm -r build/ install/ log/

colcon build --symlink-install --packages-select sl_interfaces

# Ensure that interfaces are available at next build step
. install/local_setup.bash

colcon build --symlink-install --cmake-args -DCMAKE_BUILD_TYPE=Debug --packages-select safe_learning
. install/local_setup.bash

ros2 run --prefix 'gdbserver localhost:3000' safe_learning base
