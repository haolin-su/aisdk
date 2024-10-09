#include "mem.h"
#include <stdlib.h>
#include <string.h>

#include "registry/devices/cpu/cpu_mem.h"

namespace aisdk {
std::shared_ptr<MemoryManager> MemoryManager::instance_ = nullptr;


int MemoryManager::RegisterAllMemoryCreators()
{
    auto cpu_mem_creator = std::make_shared<CPUMemoryCreator>();
    MemoryManager::GetInstance()->RegisterMemory(cpu_mem_creator);


    return 0;
}

}


