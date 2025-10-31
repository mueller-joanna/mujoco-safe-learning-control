# Safe Learning-based Control with MuJoCo

<contents>
- [Prerequisites](#prerequisites)
- [Getting started](#getting-started)
- [Example](#example)
- [Implementation](#implementation)
- [Debugging](#debugging)
- [Q&A](#qa)
- [References](#references)
...


## Prerequisites
| Package | Version requirements | Licensing | Notes |
| ------- | -------------------- | --------- | ----- |
| [TensorFlow for C](https://www.tensorflow.org/install/lang_c) | | | |
| [libmujoco](https://github.com/google-deepmind/mujoco) | | | |
| [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) | | | |
| [Drake](https://github.com/RobotLocomotion/drake/) | | | |
| [Googletest](https://github.com/google/googletest) | | | |

## Getting started
1. Build project by using 
```
./scripts/install-project.sh
```
2. Run with
```
./build/safe_learning
```

## Example
- Cart-pole/Inverted pendulum (similar to [1](#references))
    - Model file from [google-deepmind / dm_control (Github)](https://github.com/google-deepmind/dm_control/blob/main/dm_control/mujoco/testing/assets/cartpole.xml)

## Implementation
- Combining reinforcement learning and `<TODO>` control

## Debugging
- [How to debug a CMake / Make project in VS Code?](https://stackoverflow.com/a/49583462/10512964)

## Q&A
If some functions are not found after installing, check `LD_LIBRARY_PATH`
```
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:<missing_path>
```

## References
1. Brunke, Lukas, et al. "Safe learning in robotics: From learning-based control to safe reinforcement learning." Annual Review of Control, Robotics, and Autonomous Systems 5.1 (2022): 411-444. 
2. https://techoverflow.net/2025/09/11/mujoco-c-example-list-all-bodies/
3. https://deepwiki.com/google-deepmind/mujoco/3.2-forward-dynamics-pipeline
4. https://github.com/google-deepmind/mujoco_mpc/tree/main
