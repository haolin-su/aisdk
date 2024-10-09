#include "node.h"

#include "registry/algorithm/postproc_node/cv_classify_resnet_postproc.h"
#include "registry/algorithm/preproc_node/cv_classify_resnet_preproc.h"
#include "registry/devices/qualcomm/snpe/snpe_infer.h"

namespace aisdk {

NodeManager* NodeManager::instance_ = nullptr;


int NodeManager::RegisterAllNodesCreator()
{
    auto cv_classify_resnet_preproc_creator = std::make_shared<ResnetPreProcNodeCreator>();
    NodeManager::GetInstance()->RegisterNode("cv_classify_resnet_preproc", cv_classify_resnet_preproc_creator);

    auto cv_classify_resnet_postproc_creator = std::make_shared<ResnetPostProcNodeCreator>();
    NodeManager::GetInstance()->RegisterNode("cv_classify_resnet_postproc", cv_classify_resnet_postproc_creator);

    auto snpe_infer_creator = std::make_shared<SnpeInferNodeCreator>();
    NodeManager::GetInstance()->RegisterNode("snpe_infer", snpe_infer_creator);

    return 0;
}

} // namespace aisdk