#pragma once
#include <cuda.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#define GLM_FORCE_CUDA
#include <stdio.h>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <vector>
#include "../vendor/glm/glm/glm.hpp"

__global__ void ElasticCollisions_Verlet_GPU(const glm::vec2* a, const glm::vec2* b, glm::vec2* c, int N);
void ElasticCollisions_Verlet_GPU_Jumper(const glm::vec2* a, const glm::vec2* b, glm::vec2* c, int N);
//CUDA kernel using Physics2D
