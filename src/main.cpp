#include <stdio.h>
#include <tensorflow/c/c_api.h>

#include "models/cart-pole.hpp"

using safe_learning::CartPole;

int main() {
  printf("Hello from TensorFlow C library version %s\n", TF_Version());

  CartPole model = CartPole();
  model.setup_model();

  return 0;
}