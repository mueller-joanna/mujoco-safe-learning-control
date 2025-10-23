#include "cart-pole.hpp"

#include <cstdlib>

#include <mujoco/mujoco.h>

namespace safe_learning
{
    void CartPole::setup_model()
    {
        // From https://techoverflow.net/2025/09/11/mujoco-c-example-list-all-bodies/
        char error[1000] = "Could not load binary model";
        mjModel* m = mj_loadXML("assets/cartpole.xml", nullptr, error, 1000);
        if (!m) {
            printf("%s\n", error);
        }

        // Create data
        mjData* d = mj_makeData(m);

        // Loop over all bodies
        printf("Bodies in the model:\n");
        for (int i = 0; i < m->nbody; i++) {
            const char* name = mj_id2name(m, mjOBJ_BODY, i);
            if (name)
                printf("  id %d: %s\n", i, name);
            else
                printf("  id %d: (unnamed)\n", i);
        }

        // Free MuJoCo structures
        mj_deleteData(d);
        mj_deleteModel(m);
    }
}