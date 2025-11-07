#include "controllers/rl.hpp"

namespace safe_learning {

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

RlController::RlController(CartPole cart_pole) {
  mjModel *model = cart_pole.get_model_data();
  mjData *data_ = mj_makeData(model);

  for (int i = 0; i < 2 * model->nu; i++)
    action_space.push_back(i);

  CURL *curl = curl_easy_init();
  CURLcode res;
  std::string readBuffer;
 
  if(!curl) {
    fprintf(stderr, "Failed to create CURL handle\n");
    //return false;
  } else {
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8000");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    std::cout << readBuffer << std::endl;
  }
}
mjtNum *RlController::neg_K() {
  return nullptr;
}

RlController RlController::initialize(CartPole cart_pole) {
  return RlController(cart_pole);
}
} // namespace safe_learning