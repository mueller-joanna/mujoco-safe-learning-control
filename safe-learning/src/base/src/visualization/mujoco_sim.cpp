#include "visualization/mujoco_sim.hpp"
#include "mujoco/mjdata.h"


namespace safe_learning
{
    static mjvCamera cam;  // abstract camera
    static mjvOption opt;  // visualization options
    static mjvScene scn;   // abstract scene
    static mjrContext con; // custom GPU context
    static GLFWwindow *window;

    static mjtNum *scl;

    static mjModel *m;
    static mjData *d;

    void init_window() {
        // init GLFW, create window, make OpenGL context current, request v-sync
        glfwInit();
        window = glfwCreateWindow(1200, 900, "Demo", NULL, NULL);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        // initialize visualization data structures
        mjv_defaultCamera(&cam);
        // mjv_defaultPerturb(&pert);
        mjv_defaultOption(&opt);
        mjr_defaultContext(&con);

        // create scene and context
        mjv_makeScene(m, &scn, 1000);
        mjr_makeContext(m, &con, mjFONTSCALE_100);
    }

    void loop() {
        // run main loop, target real-time simulation and 60 fps rendering
        while (!glfwWindowShouldClose(window)) {
            // advance interactive simulation for 1/60 sec
            //  Assuming MuJoCo can simulate faster than real-time, which it usually
            //  can, this loop will finish on time for the next frame to be rendered at
            //  60 fps. Otherwise add a cpu timer and exit this loop when it is time to
            //  render.
            mjtNum simstart = d->time;
            while (d->time - simstart < 1.0 / 60.0) {
            mj_step(m, d);
            }

            // get framebuffer viewport
            mjrRect viewport = {0, 0, 0, 0};
            glfwGetFramebufferSize(window, &viewport.width, &viewport.height);

            // update scene and render
            mjv_updateScene(m, d, &opt, NULL, &cam, mjCAT_ALL, &scn);
            mjr_render(viewport, &scn, &con);

            // swap OpenGL buffers (blocking call due to v-sync)
            glfwSwapBuffers(window);

            // process pending GUI events, call GLFW callbacks
            glfwPollEvents();
        }
    }
    
    void close_window(){
        // close GLFW, free visualization storage
        glfwTerminate();
        mjv_freeScene(&scn);
        mjr_freeContext(&con);
    }
    
    void free_mujoco_model() {
        mj_deleteData(d);
        mj_deleteModel(m);
    }
    
    int run_simulation(mjfGeneric ctrl, mjModel *_model, mjData *_data) {
        m = _model;
        d = _data;
        init_window();

        // From https://mujoco.readthedocs.io/en/3.3.7/programming/simulation.html
        // install control callback
        mjcb_control = ctrl;

        // TODO: add a window
        loop();
        close_window();
        free_mujoco_model();

        return 0;
    }
}
