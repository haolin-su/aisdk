#include "registry.h"

#include "model/model.h"
#include "graph/graph.h"
#include "node/node.h"
#include "mem/mem.h"  
#include "tensor/tensor.h"

#include "common/log.h"

namespace aisdk {

int RegisterAll()
{
    // 注册模型创建器
    ModelManager::GetInstance()->RegisterAllModelCreator();

    // 注册图创建器
    GraphManager::GetInstance()->RegisterAllGraphsCreator();

    // 注册节点创建器
    NodeManager::GetInstance()->RegisterAllNodesCreator();

    // 注册内存管理器
    MemoryManager::GetInstance()->RegisterAllMemoryCreators();

    // // 注册张量创建器
    // TensorManager::GetInstance()->RegisterTensorCreatorAll();
    
    return 0;
}

}