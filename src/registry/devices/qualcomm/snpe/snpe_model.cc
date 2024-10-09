#include "snpe_model.h"

#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>

#include "core/tensor/tensor.h"
#include "common/log.h"

#include "SNPE/SNPE.hpp"
#include "SNPE/SNPEFactory.hpp"
#include "SNPE/DlSystem/StringList.hpp"
#include "SNPE/DlSystem/TensorShape.hpp"
#include "SNPE/DlSystem/IUserBuffer.hpp"
#include "SNPE/DlSystem/RuntimeList.hpp"
#include "SNPE/DlSystem/IUserBufferFactory.hpp"
#include "SNPE/DlSystem/UserBufferMap.hpp"
#include "SNPE/DlContainer/IDlContainer.hpp"
#include "SNPE/DlSystem/PlatformConfig.hpp"
#include "SNPE/SNPEBuilder.hpp"

using namespace aisdk;

#include "core/utils.h"
namespace aisdk {
class SnpeModelPrivate
{
public:
    SnpeModelPrivate();
    ~SnpeModelPrivate();

    int Init();
    int Load(const std::string& model_path);
    int Unload();

    int GetInputCount()
    {
        return input_tensors_.size();
    }
    int GetOutputCount()
    {
        return output_tensors_.size();
    }
    std::vector<TensorShape> GetInputShape()
    {
        std::vector<TensorShape> input_shapes;
        for (auto& input_tensor : input_tensors_) {
            TensorShape input_shape;
            input_tensor->GetShape(input_shape);
            input_shapes.push_back(input_shape);
        }

        return input_shapes;
    }

    std::vector<TensorShape> GetOutputShape()
    {
        std::vector<TensorShape> output_shapes;
        for (auto& output_tensor : output_tensors_) {
            TensorShape output_shape;
            output_tensor->GetShape(output_shape);
            output_shapes.push_back(output_shape);
        }

        return output_shapes;
    }

    std::vector<DataType> GetInputDtype()
    {
        std::vector<DataType> data_types;
        for (auto& input_tensor : input_tensors_) {
            DataType data_type;
            input_tensor->GetDataType(data_type);
            data_types.push_back(data_type);
        }

        return data_types;
    }

    std::vector<DataType> GetOutputDtype()
    {
        std::vector<DataType> data_types;
        for (auto& output_tensor : output_tensors_) {
            DataType data_type;
            output_tensor->GetDataType(data_type);
            data_types.push_back(data_type);
        }

        return data_types;
    }
    
    std::vector<Layout> GetInputLayout()
    {
        std::vector<Layout> input_layouts;
        for (auto& input_tensor : input_tensors_) {
            Layout input_layout;
            input_tensor->GetLayout(input_layout);
            input_layouts.push_back(input_layout);
        }

        return input_layouts;
    }

    std::vector<Layout> GetOutputLayout()
    {
        std::vector<Layout> output_layouts;
        for (auto& output_tensor : output_tensors_) {
            Layout output_layout;
            output_tensor->GetLayout(output_layout);
            output_layouts.push_back(output_layout);
        }

        return output_layouts;
    }

    std::vector<std::string> GetInputName()
    {
        std::vector<std::string> input_names;
        for (auto& input_tensor : input_tensors_) {
            input_names.push_back(input_tensor->GetName());
        }

        return input_names;
    }

    std::vector<std::string> GetOutputName()
    {
        std::vector<std::string> output_names;
        for (auto& output_tensor : output_tensors_) {
            output_names.push_back(output_tensor->GetName());
        }

        return output_names;
    }

    std::vector<TensorPtr> GetInputTensors()
    {
        return input_tensors_;
    }

    std::vector<TensorPtr> GetOutputTensors()
    {
        return output_tensors_; 
    }

    std::string GetKey()
    {
        return "";
    }

    std::shared_ptr<zdl::SNPE::SNPE> GetModel()
    {
        return std::move(snpe_);
    }

private:
    std::unique_ptr<zdl::DlContainer::IDlContainer> container_;
    std::unique_ptr<zdl::SNPE::SNPE> snpe_;

    std::vector<TensorPtr> input_tensors_;
    std::vector<TensorPtr> output_tensors_;
};
}

SnpeModelPrivate::SnpeModelPrivate()
{
    
}

SnpeModelPrivate::~SnpeModelPrivate()
{
    Unload();
}



int SnpeModelPrivate::Init()
{
    AISDK_LOG_TRACE("[registry] [snpe] SnpeModelPrivate::Init");

    zdl::SNPE::SNPEFactory::initializeLogging(zdl::DlSystem::LogLevel_t::LOG_ERROR, "./Log");

    zdl::SNPE::SNPEFactory::setLogLevel(zdl::DlSystem::LogLevel_t::LOG_WARN);

    AISDK_LOG_TRACE("[registry] [snpe] SnpeModelPrivate::Init end");
    return true;
}

static DataType elementType2DataType(Snpe_UserBufferEncoding_ElementType_t elemnet_type)
{
    if (elemnet_type == SNPE_USERBUFFERENCODING_ELEMENTTYPE_FLOAT) {
        return aisdk::DataType::AISDK_DATA_TYPE_FLOAT32;
    } else if (elemnet_type == SNPE_USERBUFFERENCODING_ELEMENTTYPE_FLOAT16) {
        return aisdk::DataType::AISDK_DATA_TYPE_FLOAT16;
    } else if (elemnet_type == SNPE_USERBUFFERENCODING_ELEMENTTYPE_UINT8) {
        return aisdk::DataType::AISDK_DATA_TYPE_FLOAT32;
    } else if (elemnet_type == SNPE_USERBUFFERENCODING_ELEMENTTYPE_INT8) {
        return aisdk::DataType::AISDK_DATA_TYPE_INT8;
    } else if (elemnet_type == SNPE_USERBUFFERENCODING_ELEMENTTYPE_INT16) {
        return aisdk::DataType::AISDK_DATA_TYPE_INT16;
    } else if (elemnet_type == SNPE_USERBUFFERENCODING_ELEMENTTYPE_UINT16) {
        return aisdk::DataType::AISDK_DATA_TYPE_UINT16;
    }
    
    return aisdk::DataType::AISDK_DATA_TYPE_FLOAT32;
}

int SnpeModelPrivate::Load(const std::string& model_path)
{
    AISDK_LOG_TRACE("[registry] [snpe] SnpeModelPrivate::Load");

    std::unique_ptr<zdl::DlContainer::IDlContainer> container = zdl::DlContainer::IDlContainer::open(zdl::DlSystem::String(model_path.c_str()));
    if (container == nullptr)
    {
       return false;
    }

    AISDK_LOG_TRACE("[registry] [snpe] SnpeModelPrivate::Load container success");

    std::unique_ptr<zdl::SNPE::SNPE> snpe;
    zdl::DlSystem::PlatformConfig platformConfig;

    zdl::DlSystem::Runtime_t runtime = zdl::DlSystem::Runtime_t::CPU;
    zdl::DlSystem::RuntimeList runtimeList;
    
    zdl::SNPE::SNPEBuilder snpeBuilder(container.get());
    snpe = snpeBuilder.setOutputLayers({})
    //    .setRuntimeProcessorOrder(runtimeList)
    //    .setUseUserSuppliedBuffers(useUserSuppliedBuffers)
    //    .setPlatformConfig(platformConfig)
    //    .setInitCacheMode(useCaching)
    //    .setCpuFixedPointMode(cpuFixedPointMode)
       .build();
    if (snpe == nullptr)
    {
       return false;
    }
    AISDK_LOG_TRACE("[registry] [snpe] SnpeModelPrivate::Load snpe build success");
    
    // 获取多个input的名称
    DlSystem::StringList input_name_list = snpe->getInputTensorNames();
    for (size_t i = 0; i < input_name_list.size(); ++i) {
        const char* name = input_name_list.at(i);
        AISDK_LOG_TRACE("[registry] [snpe] input tensor name: {}", name);
        // get attributes of buffer by name
        DlSystem::IBufferAttributes* bufferAttributesOptHandle = snpe->getInputOutputBufferAttributes(name);
        if (nullptr == bufferAttributesOptHandle) {
            return false;
        }

        DlSystem::TensorShape tensor_shape = bufferAttributesOptHandle->getDims();
        
        // 循环tensor_shape，合并为“[1,2,3,4]”
        std::string tensor_shape_str;
        for (size_t j = 0; j < tensor_shape.rank(); j++) {
            tensor_shape_str += std::to_string(tensor_shape[j]);
            if (j < tensor_shape.rank() - 1) {
                tensor_shape_str += ",";
            }
        }
        AISDK_LOG_TRACE("[registry] [snpe] input tensor shape: {}", tensor_shape_str);

        aisdk::TensorShape shape;
        shape.dim = tensor_shape.rank();
        std::vector<size_t> tensorShape;
        for (size_t j = 0; j < shape.dim; j++) {
            shape.shape.push_back(tensor_shape[j]);
        }

        Snpe_UserBufferEncoding_ElementType_t elemnet_type = Snpe_IBufferAttributes_GetEncodingType(bufferAttributesOptHandle);
        DataType data_type = elementType2DataType(elemnet_type);
        AISDK_LOG_TRACE("[registry] [snpe] input tensor data type: {}", (int)data_type);

        TensorPtr input_tensor = std::make_shared<Tensor>(std::string(name), shape, data_type);
        input_tensors_.emplace_back(input_tensor);
    }

    // 获取多个output的名称
    DlSystem::StringList output_name_list = snpe->getOutputTensorNames();
    for (size_t i = 0; i < output_name_list.size(); ++i) {
        const char* name = output_name_list.at(i);
        // get attributes of buffer by name
        DlSystem::IBufferAttributes* bufferAttributesOptHandle = snpe->getInputOutputBufferAttributes(name);
        if (nullptr == bufferAttributesOptHandle) {
            return false;
        }

        DlSystem::TensorShape tensor_shape = bufferAttributesOptHandle->getDims();

        std::string tensor_shape_str;
        for (size_t j = 0; j < tensor_shape.rank(); j++) {
            tensor_shape_str += std::to_string(tensor_shape[j]);
            if (j < tensor_shape.rank() - 1) {
                tensor_shape_str += ",";
            }
        }
        AISDK_LOG_TRACE("[registry] [snpe] output tensor shape: {}", tensor_shape_str);

        aisdk::TensorShape shape;
        shape.dim = tensor_shape.rank();
        std::vector<size_t> tensorShape;
        for (size_t j = 0; j < shape.dim; j++) {
            shape.shape.push_back(tensor_shape[j]);
        }

        Snpe_UserBufferEncoding_ElementType_t elemnet_type = Snpe_IBufferAttributes_GetEncodingType(bufferAttributesOptHandle);
        DataType data_type = elementType2DataType(elemnet_type);
        AISDK_LOG_TRACE("[registry] [snpe] output tensor data type: {}", (int)data_type);
        TensorPtr output_tensor = std::make_shared<Tensor>(std::string(name), shape, data_type);
        output_tensors_.emplace_back(output_tensor);
    }

    AISDK_LOG_TRACE("[registry] [snpe] SnpeModelPrivate::Load end");

    return true;
}


int SnpeModelPrivate::Unload()
{
    return true;
}

SnpeModel::SnpeModel(const std::string& model_path)
{
    AISDK_LOG_TRACE("[registry] [snpe] SnpeModel::SnpeModel {}", model_path);
    priv_ = std::shared_ptr<SnpeModelPrivate>(new SnpeModelPrivate());
    if (priv_->Init() != 0) {
        AISDK_LOG_ERROR("[registry] [snpe] SnpeModel::SnpeModel init failed");
    }

    if (priv_->Load(model_path) != 0) {
        AISDK_LOG_ERROR("[registry] [snpe] SnpeModel::SnpeModel load failed");
    }

    AISDK_LOG_TRACE("[registry] [snpe] SnpeModel::SnpeModel end");
}

SnpeModel::~SnpeModel()
{
    AISDK_LOG_TRACE("[registry] [snpe] SnpeModel::~SnpeModel");
    priv_->Unload();
}

int SnpeModel::Load(const std::string& model_path)
{
    AISDK_LOG_TRACE("[registry] [snpe] SnpeModel::Load {}", model_path);
    return priv_->Load(model_path);
}

int SnpeModel::Unload()
{
    AISDK_LOG_TRACE("[registry] [snpe] SnpeModel::Unload");
    return priv_->Unload();
}

// 获取模型输入输出的个数
int SnpeModel::GetInputCount()
{
    return priv_->GetInputCount();
}

int SnpeModel::GetOutputCount()
{
    return priv_->GetOutputCount();
}

// 获取模型输入输出的shape
std::vector<TensorShape> SnpeModel::GetInputShape()
{
    return priv_->GetInputShape();
}

std::vector<TensorShape> SnpeModel::GetOutputShape()
{
    return priv_->GetOutputShape();
}

// 获取模型输入输出的dtype
std::vector<DataType> SnpeModel::GetInputDtype()
{
    return priv_->GetInputDtype();
}

std::vector<DataType> SnpeModel::GetOutputDtype()
{
    return priv_->GetOutputDtype();
}

// 获取模型输入输出的layout
std::vector<Layout> SnpeModel::GetInputLayout()
{
    return priv_->GetInputLayout();
}

std::vector<Layout> SnpeModel::GetOutputLayout()
{
    return priv_->GetOutputLayout();
}

// 获取模型输入输出的name
std::vector<std::string> SnpeModel::GetInputName()
{
    return priv_->GetInputName();
}

std::vector<std::string> SnpeModel::GetOutputName()
{
    return priv_->GetOutputName();
}

std::vector<TensorPtr> SnpeModel::GetInputTensors() 
{
    return priv_->GetInputTensors();
}

std::vector<TensorPtr> SnpeModel::GetOutputTensors()
{
    return priv_->GetOutputTensors();
}

int SnpeModel::GetLabels(std::map<int, std::string>& labels)
{
    return 0;
}

// 获取模型的key
std::string SnpeModel::GetKey()
{
    return priv_->GetKey();
}

any SnpeModel::GetModel()
{
    return priv_->GetModel();
}
