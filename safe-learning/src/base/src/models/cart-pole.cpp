#include "models/cart-pole.hpp"

#include <filesystem>
#include <cstdlib>

using namespace std;

namespace safe_learning {
void CartPole::setup_model() {
  // TODO: refactor
  string model_path = "assets/cartpole.xml";
  string base_path{std::filesystem::current_path().c_str()};
  string abs_model_path = base_path + "/../" + model_path;
  
  // From https://techoverflow.net/2025/09/11/mujoco-c-example-list-all-bodies/
  char error[1000] = "Could not load binary model";
  mjModel *m = mj_loadXML(abs_model_path.c_str(), nullptr, error, 1000);
  if (!m) {
    printf("%s\n", error);
  }

  this->_model = m;
  this->n_states = 2 * m->nv;
  this->n_inputs = m->nu;

  this->nstate_ = this->_model->nq + this->_model->nv + this->_model->na;
  this->ndstate_ = 2 * this->_model->nv + this->_model->na;

  // Create data
  this->_data = mj_makeData(m);

  // Free MuJoCo structures
  // mj_deleteData(d);
  // mj_deleteModel(m);
}

int CartPole::get_n_states() { return this->n_states; }

int CartPole::get_n_inputs() { return this->n_inputs; }

mjModel *CartPole::get_model() { return this->_model; }
mjData *CartPole::get_data() { return this->_data; }
} // namespace safe_learning