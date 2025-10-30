#include <GLFW/glfw3.h>
#include <mujoco/mujoco.h>
#include <stdio.h>
#include <tensorflow/c/c_api.h>

#include "controllers/lqr.hpp"
#include "models/cart-pole.hpp"

using safe_learning::CartPole;
using safe_learning::LqrController;
using namespace std;

// --- Globals kept tiny for GLFW callbacks ---
static mjModel *m = nullptr;
static mjData *d = nullptr;

static mjvCamera cam;  // abstract camera
static mjvOption opt;  // visualization options
static mjvScene scn;   // abstract scene
static mjrContext con; // custom GPU context
static mjtNum *scl;

static int min_duration = 5;

double inner(double *first, double *second, int size) {
  double res = 0;
  for (int i = 0; i < size; i++) {
    res += first[i] * second[i];
  }
  return res;
}

// simple controller applying damping to each dof
void mycontroller(const mjModel *m, mjData *d) {
  const double res = inner(d->qvel, scl, m->nv);
  const double res_pos = inner(d->qpos, scl, m->nv);
  if (m->nu == m->nv) {
    mju_scl(d->ctrl, &res, 1.0, m->nv);
  } else {
    if (min_duration > 0) {
      min_duration--;
    } else {
      d->ctrl[0] = res_pos; // + res_pos;
      // d->ctrl[1] = res_pos;
    }
  }
}

///////////////////////////////// main //////////////////////////////////////
int main(int argc, char **argv) {
  printf("Hello from TensorFlow C library version %s\n", TF_Version());
  printf("Hello from MuJoCo C library version %d\n", mj_version());

  CartPole model = CartPole();
  model.setup_model();

  LqrController ctrl = LqrController(model);
  scl = ctrl.neg_K();

  // Usage
  const char *model_path;
  if (argc < 2) {
    printf("Usage: %s model.xml\n", argv[0]);
    model_path = "assets/cartpole.xml";
    // return 1;
  } else {
    model_path = argv[1];
  }

  // (Open-source MuJoCo >= 2.2 has no license activation step)
  // Load model
  char error[1024] = {0};
  m = mj_loadXML(model_path, nullptr, error, sizeof(error));
  if (!m) {
    fprintf(stderr, "Could not load model: %s\n", error);
    return 1;
  }
  d = mj_makeData(m);
  d->ctrl[0] = 0.25;
  // d->ctrl[1] = 0.5;

  // install control callback
  mjcb_control = mycontroller;

  // init GLFW, create window, make OpenGL context current, request v-sync
  glfwInit();
  GLFWwindow *window = glfwCreateWindow(1200, 900, "Demo", NULL, NULL);
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

  // ... install GLFW keyboard and mouse callbacks

  // run main loop, target real-time simulation and 60 fps rendering
  while (!glfwWindowShouldClose(window)) {
    // advance interactive simulation for 1/60 sec
    //  Assuming MuJoCo can simulate faster than real-time, which it usually
    //  can, this loop will finish on time for the next frame to be rendered at
    //  60 fps. Otherwise add a cpu timer and exit this loop when it is time to
    //  render.
    mjtNum simstart = d->time;
    while (d->time - simstart < 1.0 / 60.0) {
      // mycontroller(m, d);
      // const double res = inner(d->qvel, scl, m->nv);
      // const double res = inner(scl, scl, m->nv);
      // d->ctrl[0] = res;
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

  // close GLFW, free visualization storage
  glfwTerminate();
  mjv_freeScene(&scn);
  mjr_freeContext(&con);
  mj_deleteData(d);
  mj_deleteModel(m);

  return 0;
}