#include <GLFW/glfw3.h>
#include <filesystem>
#include <mujoco/mujoco.h>
#include <stdio.h>

#include "controllers/lqr.hpp"
#include "controllers/rl.hpp"
#include "models/cart-pole.hpp"
#include "visualization/mujoco_sim.hpp"

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

double *get_state(mjData *d) {
  double *state = new double[4];
  state[0] = d->qpos[0];
  state[1] = d->qpos[1];
  state[2] = d->qvel[0];
  state[3] = d->qvel[1];

  return state;
}

void cartpole_controller(const mjModel *m, mjData *d) {
  double *state = get_state(d);
  const double res = inner(state, scl, m->nv);
  if (starting_delay > 0) {
    starting_delay--;
  } else {
    d->ctrl[0] = res;
  }
}

///////////////////////////////// main //////////////////////////////////////
int main(int argc, char **argv) {
  printf("Hello from MuJoCo C library version %d\n", mj_version());

  CartPole model = CartPole();
  model.setup_model();

  LqrController ctrl = LqrController::initialize(model);
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
  string base_path{std::filesystem::current_path().c_str()};
  string abs_model_path = base_path + "/" + model_path;
  RlController rl_ctrl = RlController::initialize(model, abs_model_path);
  rl_ctrl.send_observations(vector<double*>());

  // Load model
  char error[1024] = {0};
  m = mj_loadXML(model_path, nullptr, error, sizeof(error));
  if (!m) {
    fprintf(stderr, "Could not load model: %s\n", error);
    return 1;
  }
  d = mj_makeData(m);
  d->ctrl[0] = 0.5;

  safe_learning::run_simulation(cartpole_controller, m, d);

  return 0;
}