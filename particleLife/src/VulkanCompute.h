#pragma once

#include <vulkan/vulkan.h>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "../src/particle.h"

// AI file

class Simulation;

struct GPUParticle
{
    int typeId;
    float size;

    float positionX;
    float positionY;

    float velocityX;
    float velocityY;
};

struct GPUSimulation
{
    float innateRepulsionArea;
    float innateRepulsion;
    float maxAttractionArea;
    float maxAttraction;
    float maxSpeed;
    float damping;
    float densityLimit;

    float width;
    float height;

    uint32_t numTypes;

    float attraction[256];
};

class VulkanCompute
{
public:
    ~VulkanCompute();

    void initialize(Simulation &simulation);

    void uploadParticles(const std::vector<Particle> &particles);

    void uploadSimulation(Simulation &simulation);

    void dispatch(size_t particleCount);

    void readbackParticles(std::vector<Particle> &particles);

private:
    void createParticleBuffer(size_t particleCount);

    void createSimulationBuffers(Simulation &simulation);

    void createComputePipeline();

    void createDescriptorResources();

    void updateDescriptorSet();

    void createCommandResources();

    uint32_t findMemoryType(VkPhysicalDevice physicalDevice,
                            uint32_t typeFilter,
                            VkMemoryPropertyFlags properties);

private:
    VkInstance instance = VK_NULL_HANDLE;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    VkDevice device = VK_NULL_HANDLE;

    VkQueue computeQueue = VK_NULL_HANDLE;

    uint32_t computeQueueFamily = 0;

    VkBuffer particleBuffer = VK_NULL_HANDLE;

    VkDeviceMemory particleMemory = VK_NULL_HANDLE;

    size_t particleCapacity = 0;

    VkBuffer simulationBuffer = VK_NULL_HANDLE;

    VkDeviceMemory simulationMemory = VK_NULL_HANDLE;

    VkShaderModule computeShader = VK_NULL_HANDLE;

    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;

    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

    VkPipeline computePipeline = VK_NULL_HANDLE;

    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;

    VkDescriptorSet descriptorSet = VK_NULL_HANDLE;

    VkCommandPool commandPool = VK_NULL_HANDLE;

    VkCommandBuffer commandBuffer = VK_NULL_HANDLE;

    VkFence computeFence = VK_NULL_HANDLE;
};