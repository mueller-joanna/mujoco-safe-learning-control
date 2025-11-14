import mujoco
from pydantic import BaseModel

class MujocoModel(BaseModel):
    model: str
    nu: int
    nv: int
