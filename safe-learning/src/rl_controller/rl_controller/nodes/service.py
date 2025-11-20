from example_interfaces.srv import AddTwoInts
# TODO: refactor
from sl_interfaces.srv import String

import rclpy
from rclpy.node import Node

from ..rl.agent import Agent

class MinimalService(Node):

    def __init__(self, agent: Agent, servic_name = 'minimal_service'):
        super().__init__(servic_name)
        self.agent = agent
        self.init_srv = self.create_service(String, 'init', self._init_model)
        # TODO: problably better of as topic
        self.train_srv = self.create_service(String, 'train', self._train_model)
        self.action_srv = self.create_service(String, 'action', self._get_action)


    def _init_model(self, request, response):
        model_path = request.model_path
        self.get_logger().info('Got path: %s' % (model_path))

        self.agent.init_model(model_path)

        response.response = 'OK!'
        return response

    def _train_model(self, request, response):
        model_path = request.model_path
        self.get_logger().info('Train: %s' % (model_path))

        self.agent.train()

        response.response = 'OK!'
        return response

    def _get_action(self, request, response):
        model_path = request.model_path
        self.get_logger().info('Action: %f' % (self.agent.get_action_as_float()))

        response.response = 'OK!'
        return response

