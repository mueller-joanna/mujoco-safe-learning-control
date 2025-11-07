import mujoco
from pydantic import BaseModel

class MujocoModel(BaseModel):
    nu: int
    nv: int
