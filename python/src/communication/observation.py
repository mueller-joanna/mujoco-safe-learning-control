import mujoco
from pydantic import BaseModel

class Observation(BaseModel):
    observations: list
