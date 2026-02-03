#ifndef LQR_NODE_H
#define LQR_NODE_H

// Contains Eigen
#include "controllers/base.hpp"

#include <float.h>
#include <iostream>
#include <memory>

#include <GLFW/glfw3.h>
#include <mujoco/mujoco.h>

using Eigen::VectorXd;
using RowMatrix4d = Eigen::Matrix<double, 4, 4, Eigen::RowMajor>;

// From
// https://github.com/TakaHoribe/Riccati_Solver/blob/master/riccati_solver.cpp
#define PRINT_MAT(X) std::cout << #X << ":\n" << X << std::endl << std::endl

// Adapted from
// https://jamiemair.github.io/MuJoCo.jl/dev/examples/cartpole_balance/
namespace safe_learning {
// Finite-difference parameters
static float EPS = std::pow(1, -6);
static bool CENTERED = true;

class LqrController : public Controller {
// TODO: How to private/protect constructor while using factory

private:
  VectorXd desired_set_point_x;
  VectorXd desired_set_point_u;

  // Jacobian
  std::vector<double> A;
  std::vector<double> B;

  MatrixXd K_mat;

private:
  // From
  // https://github.com/TakaHoribe/Riccati_Solver/blob/master/riccati_solver.cpp#L38C1-L65C2
  static bool solveRiccati(const MatrixXd &Ad, const MatrixXd &Bd,
                           const MatrixXd &Q, const MatrixXd &R, MatrixXd &P,
                           const double &tolerance = 1.E-5,
                           const uint iter_max = 100000);

private:
  void set_K(MatrixXd K_mat) override { this->K_mat = K_mat; }

public:
  mjtNum *neg_K() override;
  static std::unique_ptr<LqrController> initialize(const mjModel *m, mjData *d);
  // Because of mutual base class with RlController
  // TODO: find better solution
  int init_model(std::shared_ptr<rclcpp::Node> node, string model_path) override {};
  int request_train(std::shared_ptr<rclcpp::Node> node) override {};
  float request_action(std::shared_ptr<rclcpp::Node> node) override {};
};
} // namespace safe_learning

#endif
