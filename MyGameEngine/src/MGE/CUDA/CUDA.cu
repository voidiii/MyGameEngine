#include "MGEpch.h"
#include "CUDAHead.cuh"

__global__ void ElasticCollisions_Verlet_GPU(const glm::vec2* a, const glm::vec2* b, glm::vec2* c, int N)
{
    int tid =  threadIdx.x;
    
    // Boundary check
    if (tid < N) {
        // Each thread adds a single element
        glm::vec2 hit_distance = a[tid] - b[tid];
    
        if (glm::length(hit_distance) < 0.0001f)
        {
            c[tid] = glm::vec2(0);
            return;
        };
    
        glm::vec2 hit_direction = glm::normalize(hit_distance);
    
        c[tid] = (1.0f - glm::length(hit_distance)) / 2.0f * hit_direction;
    }
}

void ElasticCollisions_Verlet_GPU_Jumper(const glm::vec2* a, const glm::vec2* b, glm::vec2* c, int N)
{
    ElasticCollisions_Verlet_GPU<< <1, 256>> >(a, b, c, N);
}