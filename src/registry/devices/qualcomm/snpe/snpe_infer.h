#ifndef FIBO_SNPE_INFER_H_
#define FIBO_SNPE_INFER_H_

#include "core/node/node.h"

class SnpeInfer : public Node {
public:
    SnpeInfer();
    ~SnpeInfer();

    bool Init(const std::string& model_path){
        auto model = ModelManager::GetInstance().GetModel(model_path);
        if (model == nullptr) {
            model = std::make_shared<SnpeModel>(model_path);
            ModelManager::GetInstance().RegisterModel(model_path, model);
        }
        return true;

        // context
    }
    bool Unload();
    
};

#endif // FIBO_SNPE_INFER_H_