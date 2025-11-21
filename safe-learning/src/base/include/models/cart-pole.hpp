#ifndef CART_POLE_NODE_H
#define CART_POLE_NODE_H

#include <float.h>
#include <math.h>

#include <mujoco/mujoco.h>

#include <Eigen/Dense>
using Eigen::MatrixXd;
using Eigen::VectorXd;

namespace safe_learning {
class CartPole {
public:
  CartPole() {}
  ~CartPole() {
    mj_deleteData(this->_data);
    mj_deleteModel(this->_model);
  }

private:
  mjModel *_model;
  mjData *_data;
  int n_states = 0;
  int n_inputs = 0;

public:
  int nstate_ = 0;
  int ndstate_ = 0;

public:
  void setup_model();
  int get_n_states();
  int get_n_inputs();
  mjModel *get_model();
  mjData *get_data();
};
} // namespace safe_learning

#endif
