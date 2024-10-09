#include "snpe_mem.h"

using namespace aisdk;

#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>

#include "common/log.h"

#include "SNPE/SNPE.hpp"
#include "SNPE/SNPEFactory.hpp"
#include "SNPE/SNPE.h"
#include "SNPE/SNPEUtil.h"
#include "SNPE/SNPEBuilder.h"
#include "DlSystem/DlVersion.h"
#include "DlSystem/DlEnums.h"
#include "DlSystem/DlError.h"
#include "DlSystem/TensorShape.h"
#include "DlContainer/DlContainer.h"

#include "DlSystem/IUserBuffer.hpp"
#include "DlSystem/UserBufferMap.hpp"

#include "utils/createUser_buffer.hpp"

// class SnpeMemoryPrivate
// {
// public:


//     zdl::DlSystem::IUserBufferFactory ubFactory;

//     std::map<std::string, std::vector<uint8_t>> applicationBuffers;
//     std::vector<std::unique_ptr<zdl::DlSystem::IUserBuffer>> snpeUserBackedBuffers;
//     std::unique_ptr<zdl::DlSystem::UserBufferEncoding> userBufferEncoding;
//     std::unique_ptr<zdl::DlSystem::UserBufferMap> userBufferMap;

//     Snpe_UserBufferMap_Handle_t m_inputUserBufferMap;
//     Snpe_UserBufferMap_Handle_t m_outputUserBufferMap;
// };


// SnpeMemPrivate::SnpeMemPrivate()
// {
//     // create SNPE user buffer from the user-backed buffer
//     ubFactory = zdl::SNPE::SNPEFactory::getUserBufferFactory();
// }


int SnpeMemory::Alloc(size_t size)
{
    AISDK_LOG_TRACE("[registry] [snpe] SnpeMemory::Alloc begin");
    // // create user-backed storage to load input data onto it
    // applicationBuffers.emplace(name, std::vector<uint8_t>(bufSize));

    // snpeUserBackedBuffers.push_back(ubFactory.createUserBuffer(applicationBuffers.at(name).data(),
    //                                                             bufSize,
    //                                                             strides,
    //                                                             userBufferEncoding.get()));
    // if (snpeUserBackedBuffers.back() == nullptr)
    // {
    //     std::cerr << "Error while creating user buffer." << std::endl;
    // }
    // // add the user-backed buffer to the inputMap, which is later on fed to the network for execution
    // userBufferMap.add(name, snpeUserBackedBuffers.back().get());

    std::unique_ptr<zdl::SNPE::SNPE> snpe;
    bool staticQuantization = false;
    int bitWidth = 32;
    zdl::DlSystem::UserBufferMap inputMap;
    std::vector <std::unique_ptr<zdl::DlSystem::IUserBuffer>> snpeUserBackedInputBuffers;
    std::unordered_map <std::string, std::vector<uint8_t>> applicationInputBuffers;
    createInputBufferMap(inputMap, applicationInputBuffers, snpeUserBackedInputBuffers, snpe, true, staticQuantization, bitWidth);

    AISDK_LOG_TRACE("[registry] [snpe] SnpeMemory::Alloc end");

    return 0;
}
int SnpeMemory::Free() 
{
    return 0;
}
int SnpeMemory::DevAlloc(size_t size)
{
    return 0;
}
int SnpeMemory::DevFree()
{
    return 0;
}
int SnpeMemory::Memcpy(void* dst, const void* src, size_t size)
{
    return 0;
}
int SnpeMemory::Memset(void* ptr, int value, size_t size)
{
    return 0;
}
int SnpeMemory::MemcpyAsync(void* dst, const void* src, size_t size, MemcpyDirection direction)
{
    return 0;
}
int SnpeMemory::MemsetAsync(void* ptr, int value, size_t size, MemcpyDirection direction)
{
    return 0;
}
int SnpeMemory::Sync()
{
    return 0;
}
int SnpeMemory::Mmap(void* ptr, size_t size)
{
    return 0;
}
int SnpeMemory::Munmap(void* ptr)
{
    return 0;
}
int SnpeMemory::GetMemSize()
{
    return 0;
}
void* SnpeMemory::GetMemPtr()
{
    return nullptr;
}

MemoryType SnpeMemory::GetMemoryType()
{
    return MemoryType::AISDK_MEM_DEVICE_QUALCOMM;
}