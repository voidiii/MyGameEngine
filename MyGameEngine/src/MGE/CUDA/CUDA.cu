#include "CUDAHead.cuh"

__global__ void ElasticCollisions_Verlet_GPU(const glm::vec2* a, const glm::vec2* b, glm::vec2* c, int N)
{
    //int tid = (blockIdx.x * blockDim.x) + threadIdx.x;
    //
    //// Boundary check
    //if (tid < N) {
    //    // Each thread adds a single element
    //    glm::vec2 hit_distance = a[tid] - b[tid];
    //
    //    if (glm::length(hit_distance) < 0.0001f)
    //    {
    //        c[tid] = glm::vec2(0);
    //    };
    //
    //    glm::vec2 hit_direction = glm::normalize(hit_distance);
    //
    //    c[tid] = (1.0f - glm::length(hit_distance)) / 2.0f * hit_direction;
    //}
}