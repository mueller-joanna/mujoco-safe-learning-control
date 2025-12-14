#include <chrono>
#include <memory>
#include <functional>
#include <filesystem>
#include <GLFW/glfw3.h>
#include <mujoco/mujoco.h>
#include <stdio.h>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"
#include "sl_interfaces/srv/string.hpp"

//#include "client.hpp"
#include "controllers/lqr.hpp"
#include "controllers/rl.hpp"
#include "models/cart-pole.hpp"
#include "visualization/mujoco_sim.hpp"

using safe_learning::CartPole;
using safe_learning::LqrController;
using safe_learning::RlController;

using namespace std;
using namespace std::chrono_literals;

// --- Globals kept tiny for GLFW callbacks ---
mjModel *m = nullptr;
mjData *d = nullptr;
static mjtNum *scl;

// //static RlController* ctrl;

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
  //scl = ctrl->get_action();
  const double res = inner(state, scl, m->nv);
  if (starting_delay > 0) {
    starting_delay--;
  } else {
    d->ctrl[0] = res;
  }
}


///////////////////////////////// main //////////////////////////////////////
int main(int argc, char ** argv)
{
  printf("Hello from MuJoCo C library version %d\n", mj_version());
  
  
  // mjModel *m = nullptr;
  // mjData *d = nullptr;
  
  // CartPole model = CartPole();
  // model.setup_model();
  
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
  
  //LqrController ctrl = LqrController::initialize(model);
  //LqrController ctrl = LqrController::initialize(m, d);
  RlController ctrl = RlController::initialize(model_path);
  scl = ctrl.neg_K();
  
  d->ctrl[0] = 0.5;
  
  rclcpp::init(argc, argv);

  std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("add_two_ints_server");

  int status = ctrl.init_model(node, abs_model_path);
  if (status == 0) {
    status = ctrl.request_train(node);
  }
  if (status == 0) {
    float action = ctrl.request_action(node);
  }

  //rclcpp::executors::MultiThreadedExecutor executor;

  // <node>: std::variant<std::shared_ptr<custom_class>>
  // std::variant<std::shared_ptr<MinimalPublisher>> node = 
  //   std::make_shared<MinimalPublisher>();
  // std::visit([&executor](auto&& arg) { executor.add_node(arg); }, node);

  // executor.spin();
  //rclcpp::spin(node);
  rclcpp::shutdown();
  //safe_learning::run_simulation(cartpole_controller_lqr, m, d);

  // rclcpp::executors::MultiThreadedExecutor executor;

  // executor.spin();
  // rclcpp::shutdown();

  return 0;
}
