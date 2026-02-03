#include <chrono>
#include <memory>
#include <functional>
#include <filesystem>
#include <mujoco/mujoco.h>
#include <stdio.h>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"
#include "sl_interfaces/srv/string.hpp"

//#include "client.hpp"
#include "controllers/base.hpp"
#include "controllers/controller_factory.hpp"
#include "controllers/lqr.hpp"
#include "controllers/rl.hpp"
#include "visualization/mujoco_sim.hpp"

using safe_learning::ControllerFactory;
using safe_learning::LqrFactory;
using safe_learning::RlFactory;
using safe_learning::Controller;
using safe_learning::LqrController;
using safe_learning::RlController;

using namespace std;
using namespace std::chrono_literals;

// TODO: replace by setting
/*
 * 1: RL
 * 2: LQR
*/
static int CONTROLLER_TYPE = 2;

// --- Globals kept tiny for GLFW callbacks ---
mjModel *m = nullptr;
mjData *d = nullptr;
static mjtNum *scl;

static std::shared_ptr<rclcpp::Node> node;
std::unique_ptr<Controller> ctrl = nullptr;

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

void cartpole_controller_lqr(const mjModel *m, mjData *d) {
  double *state = get_state(d);
  const double res = inner(state, scl, m->nv);
  if (starting_delay > 0) {
    starting_delay--;
  } else {
    d->ctrl[0] = res;
  }
}


void cartpole_controller_rl(const mjModel *m, mjData *d) {
  d->ctrl[0] = ctrl->request_action(node);
}


///////////////////////////////// main //////////////////////////////////////
int main(int argc, char ** argv)
{
  printf("Hello from MuJoCo C library version %d\n", mj_version());
  
  // TODO: refactor
  string model_path = "assets/cartpole.xml";
  string base_path{std::filesystem::current_path().c_str()};
  string abs_model_path = base_path + "/../" + model_path;
  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Model path: %s", abs_model_path.c_str());
  
  // Load model
  char error[1024] = {0};
  m = mj_loadXML(abs_model_path.c_str(), nullptr, error, sizeof(error));
  if (!m) {
    fprintf(stderr, "Could not load model: %s\n", error);
    return 1;
  }
  d = mj_makeData(m);
  d->ctrl[0] = 0.5;
  
  rclcpp::init(argc, argv);

  node = rclcpp::Node::make_shared("add_two_ints_server");

  int status = 0;
  switch (CONTROLLER_TYPE)
  {
  case 1:
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Using RL controller.");
    ctrl = RlFactory().create(m, d);
    status = ctrl->init_model(node, abs_model_path);
    if (status == 0) {
      status = ctrl->request_train(node);
    }
    if (status == 0) {
      float action = ctrl->request_action(node);
      safe_learning::run_simulation(cartpole_controller_rl, m, d);
    }
    break;

  case 2:
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Using LQR controller.");
    ctrl = LqrFactory().create(m, d);
    scl = ctrl->neg_K();
    safe_learning::run_simulation(cartpole_controller_lqr, m, d);
    break;
  
  default:
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "No valid controller type was chosen.");
    break;
  }
  

  rclcpp::spin(node);
  rclcpp::shutdown();

  return 0;
}
