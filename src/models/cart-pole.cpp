#include "models/cart-pole.hpp"

#include <cstdlib>

namespace safe_learning {
void CartPole::setup_model() {
  // From https://techoverflow.net/2025/09/11/mujoco-c-example-list-all-bodies/
  char error[1000] = "Could not load binary model";
  mjModel *m = mj_loadXML("assets/cartpole.xml", nullptr, error, 1000);
  if (!m) {
    printf("%s\n", error);
  }

  this->model_data = m;
  this->n_states = 2 * m->nv;
  this->n_inputs = m->nu;

  // Create data
  mjData *d = mj_makeData(m);

  // Free MuJoCo structures
  mj_deleteData(d);
  mj_deleteModel(m);
}

int CartPole::get_n_states() { return this->n_states; }

int CartPole::get_n_inputs() { return this->n_inputs; }

mjModel *CartPole::get_model_data() { return this->model_data; }
} // namespace safe_learning