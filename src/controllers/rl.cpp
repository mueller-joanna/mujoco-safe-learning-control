#include "controllers/rl.hpp"

namespace safe_learning {

RlController::RlController(CartPole cart_pole) {
  mjModel *model = cart_pole.get_model_data();
  mjData *data_ = mj_makeData(model);

  for (int i = 0; i < 2 * model->nu; i++)
    action_space.push_back(i);
}
mjtNum *RlController::neg_K() {
  return nullptr;
}

RlController RlController::initialize(CartPole cart_pole) {
  return RlController(cart_pole);
}
} // namespace safe_learning