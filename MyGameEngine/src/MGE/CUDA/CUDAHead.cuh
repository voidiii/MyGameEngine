#pragma once
#include <cuda.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include "MathAlias.h"

__global__ void ElasticCollisions_Verlet_GPU(const glm::vec2* a, const glm::vec2* b, glm::vec2* c, int N);
// CUDA kernel using Physics2D
