#include "controllers/rl.hpp"
#include <string>

namespace safe_learning {

// Adapted from https://stackoverflow.com/a/51319043/10512964
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
 
  const string post_data = "{\"nu\" : " + to_string(model->nu) + ", \"nv\" : " + to_string(model->nv) + "}";
  if(!curl) {
    fprintf(stderr, "Failed to create CURL handle\n");
    //return false;
  } else {
    curl_slist *headerlist = NULL;
    headerlist = curl_slist_append(headerlist, "Content-Type: application/json");
    // Adapted from https://stackoverflow.com/a/51319043/10512964
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8000/init");
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{ \"nu\": 1, \"nv\": 2}");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_slist_free_all(headerlist);
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