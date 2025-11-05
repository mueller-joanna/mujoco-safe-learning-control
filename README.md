# Safe Learning-based Control with MuJoCo

- [Prerequisites](#prerequisites)
- [Getting started](#getting-started)
- [Example](#example)
- [Implementation](#implementation)
- [Debugging](#debugging)
- [Q&A](#qa)
- [References](#references)

## Prerequisites
| Package | Version requirements | Licensing | Notes |
| ------- | -------------------- | --------- | ----- |
| [CMake](https://cmake.org/) | | | |
| [Clang](https://clang.llvm.org/) | | | |
| [Clang-Tidy](https://clang.llvm.org/extra/clang-tidy/)| | | |
| [TensorFlow for C](https://www.tensorflow.org/install/lang_c) | | | |
| [libfdeep-dev]() | | | |
| [libmujoco](https://github.com/google-deepmind/mujoco) | | | |
| [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) | | | |
| [Drake](https://github.com/RobotLocomotion/drake/) | | | |
| [Googletest](https://github.com/google/googletest) | | | |

## Getting started
1. Build project by using 
```
./scripts/project_build.sh
```
2. Run with
```
./build/safe_learning
```

### Linting
```
clang-tidy-14 ./src/**/* ./include/**/*
```

## Example
- Cart-pole/Inverted pendulum (similar to [1](#references))
    - Model file from [google-deepmind / dm_control (Github)](https://github.com/google-deepmind/dm_control/blob/main/dm_control/mujoco/testing/assets/cartpole.xml)

## Implementation
- Combining reinforcement learning and `<TODO>` control

## Debugging
- [How to debug a CMake / Make project in VS Code?](https://stackoverflow.com/a/49583462/10512964)

## Q&A

### Telling CMake where to find (object) libraries
If some functions are not found after installing, check `LD_LIBRARY_PATH`
```
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:<missing_path>
```

### Telling ClangD where to find headers
You can do this (in the root directory of the project) by 
```
ln -s build/compile_commands.json ./
```

## References
1. Brunke, Lukas, et al. "Safe learning in robotics: From learning-based control to safe reinforcement learning." Annual Review of Control, Robotics, and Autonomous Systems 5.1 (2022): 411-444. 
2. https://techoverflow.net/2025/09/11/mujoco-c-example-list-all-bodies/
3. https://techoverflow.net/2025/09/11/minimal-c-mujoco-scene-viewer-with-mouse-navigation-simulation/
3. https://deepwiki.com/google-deepmind/mujoco/3.2-forward-dynamics-pipeline
4. https://github.com/google-deepmind/mujoco_mpc/tree/main
5. https://devtoolhub.com/github-templates-pull-requests-issues-discussions/ (issue templates)
6. [Underactuated Robotics (MIT) - Chapter 8 Linear Quadratic Regulators](https://underactuated.mit.edu/lqr.html)

## TODOS
- [ ] Automize drake installation in container
- [ ] Clean up container files
- [ ] Set up embed python
- [ ] Set up RL controller
- [ ] Replace libcurl by ROS2?
