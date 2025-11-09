#include <GLFW/glfw3.h>
#include <mujoco/mujoco.h>
#include <stdio.h>
//#include <tensorflow/c/c_api.h>

#include "controllers/rl.hpp"
#include "controllers/lqr.hpp"
#include "models/cart-pole.hpp"

using safe_learning::CartPole;
using safe_learning::LqrController;
using safe_learning::RlController;
using namespace std;


// --- Globals kept tiny for GLFW callbacks ---
static mjModel *m = nullptr;
static mjData *d = nullptr;

static mjvCamera cam;  // abstract camera
static mjvOption opt;  // visualization options
static mjvScene scn;   // abstract scene
static mjrContext con; // custom GPU context
static mjtNum *scl;

static int starting_delay = 10;

double inner(double *first, double *second, int size) {
  double res = 0;
  for (int i = 0; i < size; i++) {
    res += first[i] * second[i];
  }
  return res;
}

double* get_state(mjData *d) {
  double* state = new double[4];
  state[0] = d->qpos[0];
  state[1] = d->qpos[1];
  state[2] = d->qvel[0];
  state[3] = d->qvel[1];

  return state;
}

void cartpole_controller(const mjModel *m, mjData *d) {
  double* state = get_state(d);
  const double res = inner(state, scl, m->nv);
  if (starting_delay > 0) {
    starting_delay--;
  } else {
    d->ctrl[0] = res; 
  }
}

///////////////////////////////// main //////////////////////////////////////
int main(int argc, char **argv) {
  //printf("Hello from TensorFlow C library version %s\n", TF_Version());
  printf("Hello from MuJoCo C library version %d\n", mj_version());

  CartPole model = CartPole();
  model.setup_model();

  LqrController ctrl = LqrController::initialize(model);
  RlController rl_ctrl = RlController::initialize(model);
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

  // Load model
  char error[1024] = {0};
  m = mj_loadXML(model_path, nullptr, error, sizeof(error));
  if (!m) {
    fprintf(stderr, "Could not load model: %s\n", error);
    return 1;
  }
  d = mj_makeData(m);
  d->ctrl[0] = 0.5;

  // From https://mujoco.readthedocs.io/en/3.3.7/programming/simulation.html
  // install control callback
  mjcb_control = cartpole_controller;

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
  int num_steps = 0;
  vector<double*> state_history;
  state_history.push_back(get_state(d));
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

    state_history.push_back(get_state(d));
    num_steps++;

    if (num_steps == 50) {
      rl_ctrl.send_observations(state_history);
    }
  }

  // close GLFW, free visualization storage
  glfwTerminate();
  mjv_freeScene(&scn);
  mjr_freeContext(&con);
  mj_deleteData(d);
  mj_deleteModel(m);

  return 0;
}