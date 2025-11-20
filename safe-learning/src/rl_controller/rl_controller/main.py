import rclpy
from rclpy.node import Node

from std_msgs.msg import String

from .nodes.service import MinimalService

def main(args=None):
    rclpy.init(args=args)

    service = MinimalService()

    rclpy.spin(service)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    service.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()