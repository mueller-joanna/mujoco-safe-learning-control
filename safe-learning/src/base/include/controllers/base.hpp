#ifndef BASE_NODE_H
#define BASE_NODE_H

#include <float.h>
#include <iostream>

#include <GLFW/glfw3.h>
#include <Eigen/Dense>
#include <mujoco/mujoco.h>

#include "rclcpp/rclcpp.hpp"

using Eigen::MatrixXd;
using namespace std;

namespace safe_learning {

class Controller {
public:
  virtual ~Controller() = default;

protected:
  virtual void set_K(MatrixXd K_mat) = 0;

public:
  virtual mjtNum *neg_K() = 0;
  virtual int init_model(std::shared_ptr<rclcpp::Node> node, string model_path) = 0;
  virtual int request_train(std::shared_ptr<rclcpp::Node> node) = 0;
  virtual float request_action(std::shared_ptr<rclcpp::Node> node) = 0;
};
} // namespace safe_learning

#endif
