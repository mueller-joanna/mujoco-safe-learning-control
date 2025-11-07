import mujoco
from pydantic import BaseModel

class Observation(BaseModel):
    model: mujoco.MjModel
