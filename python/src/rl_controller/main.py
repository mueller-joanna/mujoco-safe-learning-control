import mujoco
import uvicorn
from typing import Union

from fastapi import FastAPI

from communication.model import MujocoModel
from rl.agent import Agent

app = FastAPI()


@app.get("/")
def read_root():
    return {"Hello": "World"}


@app.post("/init/")
def init(model: MujocoModel):
    agent = Agent(model)
    return { msg: "OK!" }
    #return {"agent": agent}

@app.post("/observation/")
def post_observation(observation: int):
    pass

# Adapted from https://stackoverflow.com/a/65850100/10512964
def start():
    """Launched with `poetry run ...` at root level"""
    uvicorn.run("rl_controller.main:app", host="0.0.0.0", port=8000, reload=True)
