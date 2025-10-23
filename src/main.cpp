#include <stdio.h>
#include <tensorflow/c/c_api.h>

#include "cart-pole.hpp"

using safe_learning::CartPole;

int main() {
  printf("Hello from TensorFlow C library version %s\n", TF_Version());

  CartPole model = CartPole();
  model.setup_julia();

  return 0;
}