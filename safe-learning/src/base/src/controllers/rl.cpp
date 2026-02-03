#include "controllers/rl.hpp"
#include "mujoco/mujoco.h"
#include <cstddef>
#include <cstdio>
#include <string>
#include <unistd.h>

namespace safe_learning {

template <typename T> int wait_for_service(T client) {
  while (!client->wait_for_service(1s)) {
    if (!rclcpp::ok()) {
      RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
      return 0;
    }
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
  }

  return 1;
}

float RlController::request_action(std::shared_ptr<rclcpp::Node> node) {
  string service_name = "action";
  rclcpp::Client<sl_interfaces::srv::Action>::SharedPtr client = 
    node->create_client<sl_interfaces::srv::Action>(service_name);
   
  auto request = std::make_shared<sl_interfaces::srv::Action::Request>();
  request->id = "";

  wait_for_service<rclcpp::Client<sl_interfaces::srv::Action>::SharedPtr>(client);

  auto result = client->async_send_request(request);
  float action = 0.f;
  // Wait for the result.
  if (rclcpp::spin_until_future_complete(node, result) ==
    rclcpp::FutureReturnCode::SUCCESS)
  {
    action = result.get()->action;
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Action response: %f", action);
  } else {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service %s", service_name);
  }

  return action;
}

int RlController::request_train(std::shared_ptr<rclcpp::Node> node) {
  rclcpp::Client<sl_interfaces::srv::String>::SharedPtr client = 
    node->create_client<sl_interfaces::srv::String>("train");
   
  auto request = std::make_shared<sl_interfaces::srv::String::Request>();
  request->model_path = "";

  wait_for_service<rclcpp::Client<sl_interfaces::srv::String>::SharedPtr>(client);

  auto result = client->async_send_request(request);
  // Wait for the result.
  if (rclcpp::spin_until_future_complete(node, result) ==
    rclcpp::FutureReturnCode::SUCCESS)
  {
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Train response: %s", result.get()->response.c_str());
    return 0;
  } else {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service add_two_ints");
  }

  return 1;
}

int RlController::init_model(std::shared_ptr<rclcpp::Node> node, string model_path) {
  rclcpp::Client<sl_interfaces::srv::String>::SharedPtr client =
    node->create_client<sl_interfaces::srv::String>("init");
   
  auto request = std::make_shared<sl_interfaces::srv::String::Request>();
  request->model_path = model_path.c_str();

  wait_for_service<rclcpp::Client<sl_interfaces::srv::String>::SharedPtr>(client);
  
  auto result = client->async_send_request(request);
  // Wait for the result.
  if (rclcpp::spin_until_future_complete(node, result) ==
  rclcpp::FutureReturnCode::SUCCESS)
  {
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Init: %s", result.get()->response.c_str());
    return 0;
  } else {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service add_two_ints");
  }

  return 1;
}

mjtNum *RlController::neg_K() { return nullptr; }

std::unique_ptr<RlController> RlController::initialize() {
  std::unique_ptr<RlController> ctrl = std::make_unique<RlController>();

  return ctrl;
}

} // namespace safe_learning