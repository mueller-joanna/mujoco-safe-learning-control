#ifndef MUJOCO_SIM_NODE_H
#define MUJOCO_SIM_NODE_H

#include <functional>

#include <GLFW/glfw3.h>
#include <mujoco/mujoco.h>

namespace safe_learning
{
    int run_simulation(mjfGeneric ctrl, mjModel *_model, mjData *_data);
}

#endif
