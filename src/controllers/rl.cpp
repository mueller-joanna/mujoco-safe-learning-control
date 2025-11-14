#include "controllers/rl.hpp"
#include "models/cart-pole.hpp"
#include "mujoco/mujoco.h"
#include <cstddef>
#include <cstdio>
#include <string>

namespace safe_learning {

// Adapted from https://stackoverflow.com/a/51319043/10512964
static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

RlController::RlController(CartPole cart_pole) {}
mjtNum *RlController::neg_K() { return nullptr; }

string to_str(double *arr) {
  size_t n = sizeof(arr);

  string res = "[";
  for (int i = 0; i < n; i++) {
    res += to_string(arr[i]) + ",";
  }
  res.pop_back();

  return res + "],";
}

void RlController::send_observations(vector<double *> observations) {
  CURL *curl = curl_easy_init();
  CURLcode res;
  std::string readBuffer;

  string post_data = "{ \"observations\": [";
  for (double *&obs : observations) {
    post_data += to_str(obs);
  }
  post_data.pop_back();
  post_data += "]}";

  if (!curl) {
    fprintf(stderr, "Failed to create CURL handle\n");
    // return false;
  } else {
    curl_slist *headerlist = NULL;
    headerlist =
        curl_slist_append(headerlist, "Content-Type: application/json");
    // Adapted from https://stackoverflow.com/a/51319043/10512964
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8000/observation");
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    // curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    // curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_slist_free_all(headerlist);
    curl_easy_cleanup(curl);

    std::cout << readBuffer << std::endl;
  }
}

RlController RlController::initialize(CartPole cart_pole,
                                      string model_file_path) {
  RlController ctrl = RlController(cart_pole);

  mjModel *model = cart_pole.get_model_data();
  mjData *data_ = mj_makeData(model);

  int n_states = cart_pole.get_n_states();
  int n_inputs = cart_pole.get_n_inputs();

  for (int i = 0; i < 2 * n_inputs; i++)
    ctrl.action_space.push_back(i);

  CURL *curl = curl_easy_init();
  CURLcode res;
  std::string readBuffer;

  const string post_data = "{\"nu\" : " + to_string(n_inputs) +
                           ", \"nv\" : " + to_string(n_states) +
                           ", \"model\" : \"" + model_file_path + "\"}";

  if (!curl) {
    fprintf(stderr, "Failed to create CURL handle\n");
    // return false;
  } else {
    curl_slist *headerlist = NULL;
    headerlist =
        curl_slist_append(headerlist, "Content-Type: application/json");
    // Adapted from https://stackoverflow.com/a/51319043/10512964
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8000/init");
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_slist_free_all(headerlist);
    curl_easy_cleanup(curl);

    std::cout << readBuffer << std::endl;
  }
  return ctrl;
}
} // namespace safe_learning