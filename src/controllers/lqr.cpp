#include "controllers/lqr.hpp"

namespace safe_learning {
LqrController::LqrController(CartPole cart_pole) {
  int n_states = cart_pole.get_n_states();
  int n_inputs = cart_pole.get_n_inputs();

  mjModel *model = cart_pole.get_model_data();
  mjData *data_ = mj_makeData(model);

  this->desired_set_point_x = VectorXd::Zero(n_states);
  this->desired_set_point_u = VectorXd::Zero(n_inputs);

  // dimension
  int nstate_ = model->nq + model->nv + model->na;
  int ndstate_ = 2 * model->nv + model->na;

  A.resize(ndstate_ * ndstate_);
  B.resize(n_inputs * ndstate_);

  mjd_transitionFD(model, data_, this->eps, this->centred, A.data(), B.data(),
                   NULL, NULL);

  std::vector<double> Q = {1, 0, 0, 0, 0, 10, 0, 0, 0, 0, 1, 0, 0, 0, 0, 5};

  std::vector<double> R = {1};

  std::vector<double> S;
  S.resize(n_inputs * ndstate_);

  // From https://stackoverflow.com/a/66221941/10512964
  MatrixXd A_mat = Eigen::Map<const MatrixXd>(A.data(), ndstate_, ndstate_);
  MatrixXd B_mat = Eigen::Map<const MatrixXd>(B.data(), ndstate_, n_inputs);
  MatrixXd Q_mat = Eigen::Map<const MatrixXd>(Q.data(), ndstate_, ndstate_);
  MatrixXd R_mat = Eigen::Map<const MatrixXd>(R.data(), n_inputs, 1);
  MatrixXd P_mat = MatrixXd::Zero(ndstate_, ndstate_);

  solveRiccati(A_mat, B_mat, Q_mat, R_mat, P_mat);
  // PRINT_MAT(P_mat);

  MatrixXd BT = B_mat.transpose();
  K_mat = (R_mat + BT * P_mat * B_mat).inverse() * BT * P_mat * A_mat;
  // PRINT_MAT(K_mat);
}

// From
// https://github.com/TakaHoribe/Riccati_Solver/blob/master/riccati_solver.cpp#L38C1-L65C2
bool LqrController::solveRiccati(const Eigen::MatrixXd &Ad,
                                 const Eigen::MatrixXd &Bd,
                                 const Eigen::MatrixXd &Q,
                                 const Eigen::MatrixXd &R, Eigen::MatrixXd &P,
                                 const double &tolerance,
                                 const uint iter_max) {
  P = Q; // initialize

  Eigen::MatrixXd P_next;

  Eigen::MatrixXd AdT = Ad.transpose();
  Eigen::MatrixXd BdT = Bd.transpose();
  Eigen::MatrixXd Rinv = R.inverse();

  double diff;
  for (uint i = 0; i < iter_max; ++i) {
    // -- discrete solver --
    P_next = AdT * P * Ad;
    P_next =
        P_next - AdT * P * Bd * (R + BdT * P * Bd).inverse() * BdT * P * Ad;
    P_next = P_next + Q;

    diff = fabs((P_next - P).maxCoeff());
    P = P_next;
    if (diff < tolerance) {
      std::cout << "iteration mumber = " << i << std::endl;
      return true;
    }
  }
  return false; // over iteration limit
}

mjtNum *LqrController::neg_K() {
  int rows = K_mat.rows();
  int cols = K_mat.cols();
  double *K = new double[rows * cols];
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      // K[i * rows + j] = -K_mat.coeff(i, j);
      K[i * rows + j] = K_mat.coeff(i, j);
    }
  }
  return K;
}

static LqrController initialize(CartPole cart_pole) {
  LqrController ctrl = LqrController(cart_pole);

  return ctrl;
}
} // namespace safe_learning