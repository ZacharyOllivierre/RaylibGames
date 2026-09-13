#include "VulkanCompute.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "simulation.h"

// Ai File

static std::vector<char> readFile(const std::string &filename)
{
    std::ifstream file(
        filename,
        std::ios::ate | std::ios::binary);

    if (!file)
        throw std::runtime_error(
            "Failed to open shader file");

    size_t size = file.tellg();

    std::vector<char> buffer(size);

    file.seekg(0);
    file.read(buffer.data(), size);

    return buffer;
}

void VulkanCompute::initialize(Simulation &simulation)
{
    VkApplicationInfo appInfo{};

    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Particle Life";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Particle Life";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_2;

    const char *extensions[] = {VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME};

    VkInstanceCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = 1;
    createInfo.ppEnabledExtensionNames = extensions;

    if (vkCreateInstance(
            &createInfo,
            nullptr,
            &instance) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Failed to create Vulkan instance");
    }

    std::cout << "Vulkan instance created successfully\n";

    uint32_t deviceCount = 0;

    vkEnumeratePhysicalDevices(
        instance,
        &deviceCount,
        nullptr);

    if (deviceCount == 0)
        throw std::runtime_error(
            "No Vulkan GPU found");

    std::vector<VkPhysicalDevice> devices(
        deviceCount);

    vkEnumeratePhysicalDevices(
        instance,
        &deviceCount,
        devices.data());

    physicalDevice = devices[0];

    VkPhysicalDeviceProperties properties{};

    vkGetPhysicalDeviceProperties(
        physicalDevice,
        &properties);

    std::cout
        << "GPU: "
        << properties.deviceName
        << '\n';

    uint32_t queueFamilyCount = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(
        physicalDevice,
        &queueFamilyCount,
        nullptr);

    std::vector<VkQueueFamilyProperties>
        queueFamilies(queueFamilyCount);

    vkGetPhysicalDeviceQueueFamilyProperties(
        physicalDevice,
        &queueFamilyCount,
        queueFamilies.data());

    bool foundComputeQueue = false;

    for (uint32_t i = 0;
         i < queueFamilyCount;
         i++)
    {
        if (queueFamilies[i].queueFlags &
            VK_QUEUE_COMPUTE_BIT)
        {
            computeQueueFamily = i;
            foundComputeQueue = true;
            break;
        }
    }

    if (!foundComputeQueue)
        throw std::runtime_error(
            "No compute queue found");

    std::cout << "Compute queue family: "
              << computeQueueFamily
              << '\n';

    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueCreateInfo{};

    queueCreateInfo.sType =
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

    queueCreateInfo.queueFamilyIndex =
        computeQueueFamily;

    queueCreateInfo.queueCount = 1;

    queueCreateInfo.pQueuePriorities =
        &queuePriority;

    VkDeviceCreateInfo deviceCreateInfo{};

    deviceCreateInfo.sType =
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    deviceCreateInfo.queueCreateInfoCount = 1;

    deviceCreateInfo.pQueueCreateInfos =
        &queueCreateInfo;

    if (vkCreateDevice(physicalDevice, &deviceCreateInfo,
                       nullptr, &device) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Failed to create logical device");
    }

    std::cout << "Logical device created successfully\n";

    vkGetDeviceQueue(
        device,
        computeQueueFamily,
        0,
        &computeQueue);

    std::cout << "Compute queue obtained successfully\n";

    createParticleBuffer(
        simulation.getParticles().size());

    createSimulationBuffers(
        simulation);

    createComputePipeline();

    createDescriptorResources();

    createCommandResources();

    std::cout << "Vulkan initialization complete\n";
}

void VulkanCompute::createParticleBuffer(size_t particleCount)
{
    size_t bufferCount =
        particleCount;

    if (bufferCount == 0)
        bufferCount = 1;

    if (particleBuffer != VK_NULL_HANDLE)
    {
        vkDestroyBuffer(
            device,
            particleBuffer,
            nullptr);

        particleBuffer =
            VK_NULL_HANDLE;
    }

    if (particleMemory != VK_NULL_HANDLE)
    {
        vkFreeMemory(
            device,
            particleMemory,
            nullptr);

        particleMemory =
            VK_NULL_HANDLE;
    }

    VkDeviceSize bufferSize = sizeof(GPUParticle) * bufferCount;

    VkBufferCreateInfo bufferInfo{};

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

    bufferInfo.size = bufferSize;

    bufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo,
                       nullptr, &particleBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create particle buffer");
    }

    VkMemoryRequirements memoryRequirements{};

    vkGetBufferMemoryRequirements(
        device,
        particleBuffer,
        &memoryRequirements);

    VkMemoryAllocateInfo allocateInfo{};

    allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

    allocateInfo.allocationSize = memoryRequirements.size;

    allocateInfo.memoryTypeIndex =
        findMemoryType(
            physicalDevice,
            memoryRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(device, &allocateInfo,
                         nullptr, &particleMemory) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Failed to allocate particle buffer memory");
    }

    if (vkBindBufferMemory(
            device,
            particleBuffer,
            particleMemory,
            0) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Failed to bind particle buffer memory");
    }

    particleCapacity = bufferCount;

    std::cout << "Particle buffer created: " << bufferCount << " particle slots\n";

    if (descriptorSet != VK_NULL_HANDLE)
        updateDescriptorSet();
}

void VulkanCompute::createSimulationBuffers(Simulation &simulation)
{
    VkBufferCreateInfo bufferInfo{};

    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

    bufferInfo.size = sizeof(GPUSimulation);

    bufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(
            device,
            &bufferInfo,
            nullptr,
            &simulationBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Failed to create simulation buffer");
    }

    VkMemoryRequirements memoryRequirements{};

    vkGetBufferMemoryRequirements(
        device,
        simulationBuffer,
        &memoryRequirements);

    VkMemoryAllocateInfo allocateInfo{};

    allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

    allocateInfo.allocationSize = memoryRequirements.size;

    allocateInfo.memoryTypeIndex =
        findMemoryType(
            physicalDevice,
            memoryRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(
            device,
            &allocateInfo,
            nullptr,
            &simulationMemory) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Failed to allocate simulation memory");
    }

    if (vkBindBufferMemory(device, simulationBuffer,
                           simulationMemory, 0) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to bind simulation memory");
    }

    uploadSimulation(simulation);
}

uint32_t VulkanCompute::findMemoryType(
    VkPhysicalDevice physicalDevice,
    uint32_t typeFilter,
    VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memoryProperties{};

    vkGetPhysicalDeviceMemoryProperties(
        physicalDevice,
        &memoryProperties);

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) &&
            (memoryProperties.memoryTypes[i]
                 .propertyFlags &
             properties) == properties)
        {
            return i;
        }
    }

    throw std::runtime_error("Failed to find suitable memory type");
}

void VulkanCompute::uploadParticles(const std::vector<Particle> &particles)
{
    if (particles.size() > particleCapacity)
    {
        vkDeviceWaitIdle(device);

        size_t newCapacity = particleCapacity;

        if (newCapacity == 0)
            newCapacity = 1;

        while (newCapacity <
               particles.size())
        {
            newCapacity *= 2;
        }

        createParticleBuffer(newCapacity);
    }

    if (particles.empty())
        return;

    void *mappedMemory = nullptr;

    if (vkMapMemory(
            device,
            particleMemory,
            0,
            sizeof(GPUParticle) * particles.size(),
            0,
            &mappedMemory) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Failed to map particle memory");
    }

    GPUParticle *gpuParticles = static_cast<GPUParticle *>(mappedMemory);

    for (size_t i = 0; i < particles.size(); i++)
    {
        const ParticleData &data = particles[i].getDataConst();

        gpuParticles[i].typeId = data.typeId;
        gpuParticles[i].size = data.size;

        gpuParticles[i].positionX = data.position.x;
        gpuParticles[i].positionY = data.position.y;

        gpuParticles[i].velocityX = data.velocity.x;
        gpuParticles[i].velocityY = data.velocity.y;
    }

    vkUnmapMemory(device, particleMemory);
}

void VulkanCompute::uploadSimulation(Simulation &simulation)
{
    const SimData &data = simulation.getData();

    GPUSimulation gpuData{};

    gpuData.innateRepulsionArea = data.innateRepulsionArea;
    gpuData.innateRepulsion = data.innateRepulsion;
    gpuData.maxAttractionArea = data.maxAttractionArea;
    gpuData.maxAttraction = data.maxAttraction;
    gpuData.maxSpeed = data.maxSpeed;
    gpuData.damping = data.damping;
    gpuData.densityLimit = data.densityLimit;
    gpuData.width = data.dimensions.x;
    gpuData.height = data.dimensions.y;
    gpuData.numTypes = static_cast<uint32_t>(data.numTypes);

    for (int i = 0; i < data.numTypes; i++)
    {
        for (int j = 0; j < data.numTypes; j++)
        {
            size_t index = static_cast<size_t>(i) * data.numTypes + j;

            if (index < 256)
            {
                gpuData.attraction[index] = data.attraction[i][j];
            }
        }
    }

    void *mapped = nullptr;

    if (vkMapMemory(device, simulationMemory, 0,
                    sizeof(GPUSimulation), 0, &mapped) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to map simulation memory");
    }

    std::memcpy(
        mapped,
        &gpuData,
        sizeof(GPUSimulation));

    vkUnmapMemory(
        device,
        simulationMemory);
}

void VulkanCompute::createComputePipeline()
{
    std::vector<char> shaderCode = readFile("shaders/particle.comp.spv");

    VkShaderModuleCreateInfo shaderInfo{};

    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

    shaderInfo.codeSize = shaderCode.size();

    shaderInfo.pCode =
        reinterpret_cast<const uint32_t *>(
            shaderCode.data());

    if (vkCreateShaderModule(
            device,
            &shaderInfo,
            nullptr,
            &computeShader) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Failed to create shader module");
    }

    VkDescriptorSetLayoutBinding bindings[2]{};

    bindings[0].binding = 0;

    bindings[0].descriptorType =
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

    bindings[0].descriptorCount = 1;

    bindings[0].stageFlags =
        VK_SHADER_STAGE_COMPUTE_BIT;

    bindings[1].binding = 1;

    bindings[1].descriptorType =
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

    bindings[1].descriptorCount = 1;

    bindings[1].stageFlags =
        VK_SHADER_STAGE_COMPUTE_BIT;

    VkDescriptorSetLayoutCreateInfo
        layoutInfo{};

    layoutInfo.sType =
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

    layoutInfo.bindingCount = 2;

    layoutInfo.pBindings =
        bindings;

    if (vkCreateDescriptorSetLayout(
            device,
            &layoutInfo,
            nullptr,
            &descriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Failed to create descriptor set layout");
    }

    VkPushConstantRange pushConstant{};

    pushConstant.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    pushConstant.offset = 0;

    pushConstant.size = sizeof(uint32_t);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    pipelineLayoutInfo.setLayoutCount = 1;

    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

    pipelineLayoutInfo.pushConstantRangeCount = 1;

    pipelineLayoutInfo.pPushConstantRanges = &pushConstant;

    if (vkCreatePipelineLayout(
            device,
            &pipelineLayoutInfo,
            nullptr,
            &pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error(
            "Failed to create pipeline layout");
    }

    VkPipelineShaderStageCreateInfo shaderStage{};

    shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

    shaderStage.stage = VK_SHADER_STAGE_COMPUTE_BIT;

    shaderStage.module = computeShader;

    shaderStage.pName = "main";

    VkComputePipelineCreateInfo pipelineInfo{};

    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;

    pipelineInfo.stage = shaderStage;

    pipelineInfo.layout = pipelineLayout;

    if (vkCreateComputePipelines(
            device,
            VK_NULL_HANDLE,
            1,
            &pipelineInfo,
            nullptr,
            &computePipeline) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create compute pipeline");
    }

    std::cout << "Compute pipeline created\n";
}

void VulkanCompute::createDescriptorResources()
{
    VkDescriptorPoolSize poolSize{};

    poolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSize.descriptorCount = 2;

    VkDescriptorPoolCreateInfo poolInfo{};

    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.maxSets = 1;
    poolInfo.poolSizeCount = 1;

    poolInfo.pPoolSizes = &poolSize;

    if (vkCreateDescriptorPool(
            device,
            &poolInfo,
            nullptr,
            &descriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create descriptor pool");
    }

    VkDescriptorSetAllocateInfo allocateInfo{};

    allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;

    allocateInfo.descriptorPool = descriptorPool;

    allocateInfo.descriptorSetCount = 1;

    allocateInfo.pSetLayouts = &descriptorSetLayout;

    if (vkAllocateDescriptorSets(
            device,
            &allocateInfo,
            &descriptorSet) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate descriptor set");
    }

    updateDescriptorSet();

    std::cout << "Descriptor resources created\n";
}

void VulkanCompute::updateDescriptorSet()
{
    VkDescriptorBufferInfo particleInfo{};

    particleInfo.buffer = particleBuffer;

    particleInfo.offset = 0;

    particleInfo.range = VK_WHOLE_SIZE;

    VkDescriptorBufferInfo simulationInfo{};

    simulationInfo.buffer = simulationBuffer;

    simulationInfo.offset = 0;

    simulationInfo.range = sizeof(GPUSimulation);

    VkWriteDescriptorSet writes[2]{};

    writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[0].dstSet = descriptorSet;
    writes[0].dstBinding = 0;
    writes[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    writes[0].descriptorCount = 1;
    writes[0].pBufferInfo = &particleInfo;

    writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writes[1].dstSet = descriptorSet;
    writes[1].dstBinding = 1;
    writes[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    writes[1].descriptorCount = 1;
    writes[1].pBufferInfo = &simulationInfo;

    vkUpdateDescriptorSets(
        device,
        2,
        writes,
        0,
        nullptr);
}

void VulkanCompute::createCommandResources()
{
    VkCommandPoolCreateInfo poolInfo{};

    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    poolInfo.queueFamilyIndex = computeQueueFamily;

    if (vkCreateCommandPool(
            device,
            &poolInfo,
            nullptr,
            &commandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create command pool");
    }

    VkCommandBufferAllocateInfo bufferInfo{};

    bufferInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    bufferInfo.commandPool = commandPool;
    bufferInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    bufferInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(
            device,
            &bufferInfo,
            &commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate command buffer");
    }

    VkFenceCreateInfo fenceInfo{};

    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    if (vkCreateFence(
            device,
            &fenceInfo,
            nullptr,
            &computeFence) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create compute fence");
    }

    std::cout << "Command resources created\n";
}

void VulkanCompute::dispatch(size_t particleCount)
{
    if (particleCount == 0)
        return;

    if (particleCount >
        particleCapacity)
    {
        throw std::runtime_error("Particle count exceeds GPU buffer capacity");
    }

    vkResetFences(
        device,
        1,
        &computeFence);

    vkResetCommandBuffer(
        commandBuffer,
        0);

    VkCommandBufferBeginInfo beginInfo{};

    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin command buffer");
    }

    vkCmdBindPipeline(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_COMPUTE,
        computePipeline);

    vkCmdBindDescriptorSets(
        commandBuffer,
        VK_PIPELINE_BIND_POINT_COMPUTE,
        pipelineLayout,
        0,
        1,
        &descriptorSet,
        0,
        nullptr);

    uint32_t count = static_cast<uint32_t>(particleCount);

    vkCmdPushConstants(
        commandBuffer,
        pipelineLayout,
        VK_SHADER_STAGE_COMPUTE_BIT,
        0,
        sizeof(uint32_t),
        &count);

    uint32_t workgroups = static_cast<uint32_t>((particleCount + 63) / 64);

    vkCmdDispatch(
        commandBuffer,
        workgroups,
        1,
        1);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to end command buffer");
    }

    VkSubmitInfo submitInfo{};

    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    submitInfo.commandBufferCount = 1;

    submitInfo.pCommandBuffers = &commandBuffer;

    if (vkQueueSubmit(computeQueue, 1, &submitInfo, computeFence) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to submit compute command");
    }

    if (vkWaitForFences(
            device,
            1,
            &computeFence,
            VK_TRUE,
            UINT64_MAX) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed waiting for compute fence");
    }
}

void VulkanCompute::readbackParticles(std::vector<Particle> &particles)
{
    if (particles.empty())
        return;

    if (particles.size() > particleCapacity)
    {
        throw std::runtime_error("Particle count exceeds GPU buffer capacity");
    }

    void *mappedMemory = nullptr;

    if (vkMapMemory(
            device,
            particleMemory,
            0,
            sizeof(GPUParticle) *
                particles.size(),
            0,
            &mappedMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to map particle memory for readback");
    }

    GPUParticle *gpuParticles = static_cast<GPUParticle *>(mappedMemory);

    for (size_t i = 0; i < particles.size(); i++)
    {
        ParticleData &data = particles[i].getDataRef();

        data.typeId = gpuParticles[i].typeId;
        data.size = gpuParticles[i].size;
        data.position.x = gpuParticles[i].positionX;
        data.position.y = gpuParticles[i].positionY;
        data.velocity.x = gpuParticles[i].velocityX;
        data.velocity.y = gpuParticles[i].velocityY;
    }

    vkUnmapMemory(device, particleMemory);
}

VulkanCompute::~VulkanCompute()
{
    if (device != VK_NULL_HANDLE)
    {
        vkDeviceWaitIdle(device);

        if (computeFence != VK_NULL_HANDLE)
            vkDestroyFence(
                device,
                computeFence,
                nullptr);

        if (commandPool != VK_NULL_HANDLE)
            vkDestroyCommandPool(
                device,
                commandPool,
                nullptr);

        if (descriptorPool != VK_NULL_HANDLE)
            vkDestroyDescriptorPool(
                device,
                descriptorPool,
                nullptr);

        if (computePipeline != VK_NULL_HANDLE)
            vkDestroyPipeline(
                device,
                computePipeline,
                nullptr);

        if (pipelineLayout != VK_NULL_HANDLE)
            vkDestroyPipelineLayout(
                device,
                pipelineLayout,
                nullptr);

        if (descriptorSetLayout != VK_NULL_HANDLE)
            vkDestroyDescriptorSetLayout(
                device,
                descriptorSetLayout,
                nullptr);

        if (computeShader != VK_NULL_HANDLE)
            vkDestroyShaderModule(
                device,
                computeShader,
                nullptr);

        if (particleBuffer != VK_NULL_HANDLE)
            vkDestroyBuffer(
                device,
                particleBuffer,
                nullptr);

        if (particleMemory != VK_NULL_HANDLE)
            vkFreeMemory(
                device,
                particleMemory,
                nullptr);

        if (simulationBuffer != VK_NULL_HANDLE)
            vkDestroyBuffer(
                device,
                simulationBuffer,
                nullptr);

        if (simulationMemory != VK_NULL_HANDLE)
            vkFreeMemory(
                device,
                simulationMemory,
                nullptr);

        vkDestroyDevice(
            device,
            nullptr);
    }

    if (instance != VK_NULL_HANDLE)
        vkDestroyInstance(
            instance,
            nullptr);
}