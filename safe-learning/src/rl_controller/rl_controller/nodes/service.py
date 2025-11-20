from example_interfaces.srv import AddTwoInts
# TODO: refactor
from sl_interfaces.srv import String, Action

import rclpy
from rclpy.node import Node

from ..rl.agent import Agent, rl_agent

class MinimalService(Node):

    def __init__(self, servic_name = 'minimal_service'):
        super().__init__(servic_name)
        self.init_srv = self.create_service(String, 'init', self._init_model)
        # TODO: problably better of as topic
        self.train_srv = self.create_service(String, 'train', self._train_model)
        self.action_srv = self.create_service(Action, 'action', self._get_action)


    def _init_model(self, request, response):
        model_path = request.model_path
        self.get_logger().info('Got path: %s' % (model_path))

        rl_agent.init_model(model_path)

        response.response = 'OK!'
        return response

    def _train_model(self, request, response):
        model_path = request.model_path
        self.get_logger().info('Train: %s' % (model_path))

        rl_agent.train()

        response.response = 'OK!'
        return response

    def _get_action(self, request, response):
        action = rl_agent.get_action_as_float()
        response.action = action

        self.get_logger().info('Action: %f' % (rl_agent.get_action_as_float()))
        return response

