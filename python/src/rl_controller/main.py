import mujoco
import uvicorn
import json
import asyncio
from typing import Union

from fastapi import FastAPI, Request
from contextlib import asynccontextmanager
from dataclasses import dataclass
from concurrent.futures import ProcessPoolExecutor
import time

from communication.model import MujocoModel
from communication.observation import Observation
from rl.agent import rl_agent

# Async stuff from https://stackoverflow.com/a/77862153/10512964
async def process_requests(q: asyncio.Queue, pool: ProcessPoolExecutor):
    while True:
        observations = await q.get()  # Get a request from the queue
        loop = asyncio.get_running_loop()
        r = await loop.run_in_executor(pool, rl_agent.train, observations)
        q.task_done()  # tell the queue that the processing on the task is completed

@asynccontextmanager
async def lifespan(app: FastAPI):
    q = asyncio.Queue()  # note that asyncio.Queue() is not thread safe
    pool = ProcessPoolExecutor()
    asyncio.create_task(process_requests(q, pool))  # Start the requests processing task
    yield {'q': q, 'pool': pool}
    pool.shutdown()  # free any resources that the pool is using when the currently pending futures are done executing


app = FastAPI(lifespan=lifespan)

@app.get("/")
def read_root():
    return {"Hello": "World"}


@app.post("/init")
def init(model: MujocoModel):
    rl_agent.init_model(model)
    return { "msg": "OK!" }
    #return {"agent": rl_agent.to_json() }

@app.post("/observation")
async def post_observation(request: Request, observations: Observation):
    request.state.q.put_nowait(observations)  # Add request to the queue
    return { "msg": "Started training!" }


# Adapted from https://stackoverflow.com/a/65850100/10512964
def start():
    """Launched with `poetry run ...` at root level"""
    uvicorn.run("rl_controller.main:app", host="0.0.0.0", port=8000, reload=True)
