#ifndef RL_NODE_H
#define RL_NODE_H

#include <float.h>
#include <iostream>

#include <Eigen/Dense>
#include <mujoco/mujoco.h>

#include <curl/curl.h>
#include <vector>

#include "models/cart-pole.hpp"

using Eigen::MatrixXd;
using Eigen::VectorXd;
using namespace std;

namespace safe_learning {

  static string ENDPOINT = "http://localhost:8000/";

class RlController {
private:
  RlController(CartPole cart_pole);

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
  void send_observations(vector<double *> observations);
  mjtNum * get_action();

public:
  static RlController initialize(CartPole cart_pole, string model_file_path);
};
} // namespace safe_learning

#endif