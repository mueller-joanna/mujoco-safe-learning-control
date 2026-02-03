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

#include "controllers/base.hpp"

using Eigen::MatrixXd;
using Eigen::VectorXd;
using namespace std;

namespace safe_learning {

class RlController : public Controller {

private:
  vector<int> action_space;

  VectorXd desired_set_point_x;
  VectorXd desired_set_point_u;

  // Jacobian
  std::vector<double> A;
  std::vector<double> B;

  MatrixXd K_mat;

private:
  void set_K(MatrixXd K_mat) override { this->K_mat = K_mat; }

public:
  mjtNum *neg_K() override;
  int init_model(std::shared_ptr<rclcpp::Node> node, string model_path) override;
  int request_train(std::shared_ptr<rclcpp::Node> node) override;
  float request_action(std::shared_ptr<rclcpp::Node> node) override;

public:
  static std::unique_ptr<RlController> initialize();
};

} // namespace safe_learning

#endif