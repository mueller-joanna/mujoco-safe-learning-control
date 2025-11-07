from communication.model import MujocoModel

class Agent(object):

    def __init__(self, model: MujocoModel):
        self.model = model
