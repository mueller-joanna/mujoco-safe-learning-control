#ifndef RL_NODE_H
#define RL_NODE_H

#include <float.h>
#include <iostream>

#include <Eigen/Dense>
#include <mujoco/mujoco.h>

#include <vector>

#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "sl_interfaces/srv/string.hpp"
#include "sl_interfaces/srv/action.hpp"

using Eigen::MatrixXd;
using Eigen::VectorXd;
using namespace std;

namespace safe_learning {

class RlController {
private:
  RlController();

private:
  vector<int> action_space;

  VectorXd desired_set_point_x;
  VectorXd desired_set_point_u;

  // Jacobian
  std::vector<double> A;
  std::vector<double> B;

  MatrixXd K_mat;

private:
  void set_K(MatrixXd K_mat) { this->K_mat = K_mat; }

public:
  mjtNum *neg_K();
  int init_model(std::shared_ptr<rclcpp::Node> node, string model_path);
  int request_train(std::shared_ptr<rclcpp::Node> node);
  float request_action(std::shared_ptr<rclcpp::Node> node);

public:
  static RlController initialize(string model_file_path);
};

} // namespace safe_learning

#endif