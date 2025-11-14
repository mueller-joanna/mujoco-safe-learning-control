from typing import Optional
import numpy as np
import gymnasium as gym
from gymnasium import utils
from gymnasium.envs.mujoco import MujocoEnv
from gymnasium import spaces

import mujoco as mj

from communication.model import MujocoModel

class TemplateEnv(MujocoEnv, utils.EzPickle):
    """
    TODO
    """

    metadata = {
        "render_modes": [
            "human",
            "rgb_array",
            "depth_array",
        ],
        #"render_fps": 25,
        "render_fps": 50,
    }

    def __init__(self, file_path: str, n_states: int = 4, **kwargs):
        utils.EzPickle.__init__(self, **kwargs)
        observation_space = spaces.Box(low=-np.inf, high=np.inf, shape=(n_states,), dtype=np.float64)
        MujocoEnv.__init__(
            self,
            file_path,
            2,
            observation_space=observation_space,
            **kwargs
        )

        # TODO temporary
        # MujocoEnv assumes spaces.Box(low=low, high=high, dtype=np.float32)
        self.action_space = spaces.Discrete(2)


    def step(self, a):
        reward = 1.0
        if not (type(a) in [type(np.array([])), type([])]):
            a = [a]
        self.do_simulation(a, self.frame_skip)
        ob = self._get_obs()
        terminated = bool(not np.isfinite(ob).all() or (np.abs(ob[1]) > 0.2))
        if self.render_mode == "human":
            self.render()
        return ob, reward, terminated, False, {}

    def reset_model(self):
        qpos = self.init_qpos + self.np_random.uniform(
            size=self.model.nq, low=-0.01, high=0.01
        )
        qvel = self.init_qvel + self.np_random.uniform(
            size=self.model.nv, low=-0.01, high=0.01
        )
        self.set_state(qpos, qvel)
        return self._get_obs()

    def _get_obs(self):
        return np.concatenate([self.data.qpos, self.data.qvel]).ravel()

    def viewer_setup(self):
        assert self.viewer is not None
        v = self.viewer
        v.cam.trackbodyid = 0
        v.cam.distance = self.model.stat.extent
