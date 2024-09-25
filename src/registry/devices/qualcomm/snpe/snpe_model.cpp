#include "snpe_model.h"

#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>

#include "core/tensor/tensor.h"

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

// // {
// // {
// //     #ifdef __ANDROID__
// //     // Initialize Logs with level LOG_ERROR.
// //     zdl::SNPE::SNPEFactory::initializeLogging(zdl::DlSystem::LogLevel_t::LOG_ERROR);
// // #else
// //     // Initialize Logs with specified log level as LOG_ERROR and log path as "./Log".
// //     zdl::SNPE::SNPEFactory::initializeLogging(zdl::DlSystem::LogLevel_t::LOG_ERROR, "./Log");
// // #endif

// //     // Update Log Level to LOG_WARN.
// //     zdl::SNPE::SNPEFactory::setLogLevel(zdl::DlSystem::LogLevel_t::LOG_WARN);

// std::unique_ptr<zdl::DlContainer::IDlContainer> container = loadContainerFromFile(dlc);
//     if (container == nullptr)
//     {
//        std::cerr << "Error while opening the container file." << std::endl;
//        return EXIT_FAILURE;
//     }

// std::unique_ptr<zdl::SNPE::SNPE> snpe;
//     zdl::DlSystem::PlatformConfig platformConfig;

//      snpe = setBuilderOptions(container, runtime, runtimeList,
//                              useUserSuppliedBuffers, platformConfig,
//                              usingInitCaching, cpuFixedPointMode);
//     if (snpe == nullptr)
//     {
//        std::cerr << "Error while building SNPE object." << std::endl;
//        return EXIT_FAILURE;
//     }

//     zdl::DlSystem::TensorShape tensorShape;
//     tensorShape = snpe->getInputDimensions();
//     size_t batchSize = tensorShape.getDimensions()[0];

//     // Open the input file listing and group input files into batches
//     std::vector<std::vector<std::string>> inputs = preprocessInput(inputFile, batchSize);
//     zdl::DlSystem::UserBufferMap inputMap, outputMap;
//        std::vector <std::unique_ptr<zdl::DlSystem::IUserBuffer>> snpeUserBackedInputBuffers, snpeUserBackedOutputBuffers;
//        std::unordered_map <std::string, std::vector<uint8_t>> applicationOutputBuffers;

//        if(bufferType == ITENSOR)
//     {
//         // A tensor map for SNPE execution outputs
//         zdl::DlSystem::TensorMap outputTensorMap;
//         //Get input names and number
//         const auto& inputTensorNamesRef = snpe->getInputTensorNames();
//         if (!inputTensorNamesRef) throw std::runtime_error("Error obtaining Input tensor names");
//         const auto &inputTensorNames = *inputTensorNamesRef;

//         for (size_t i = 0; i < inputs.size(); i++) {
//             // Load input/output buffers with Tensor
//             if(batchSize > 1)
//                 std::cout << "Batch " << i << ":" << std::endl;
//             if (inputTensorNames.size() == 1)
//             {
//                 // Load input/output buffers with Tensor
//                 std::unique_ptr<zdl::DlSystem::Tensor> inputTensor = loadInputTensor(snpe, inputs[i], inputTensorNames);
//                 if(!inputTensor)
//                 {
//                     return EXIT_FAILURE;
//                 }
//                 // Execute the input tensor on the model with SNPE
//                 execStatus = snpe->execute(inputTensor.get(), outputTensorMap);
//             }
//             else
//             {
//                 std::vector<std::unique_ptr<zdl::DlSystem::Tensor>> inputTensors(inputTensorNames.size());
//                 zdl::DlSystem::TensorMap inputTensorMap;
//                 bool inputLoadStatus = false;
//                 // Load input/output buffers with TensorMap
//                 std::tie(inputTensorMap, inputLoadStatus) = loadMultipleInput(snpe, inputs[i], inputTensorNames, inputTensors);
//                 if(!inputLoadStatus)
//                 {
//                     return EXIT_FAILURE;
//                 }
//                 // Execute the multiple input tensorMap on the model with SNPE
//                 execStatus = snpe->execute(inputTensorMap, outputTensorMap);
//             }
//             // Save the execution results if execution successful
//             if (execStatus == true)
//             {
//                if(!saveOutput(outputTensorMap, OutputDir, i * batchSize, batchSize))
//                {
//                    return EXIT_FAILURE;
//                }
//             }
//             else
//             {
//                std::cerr << "Error while executing the network." << std::endl;
//             }
//         }
// // }
#include "core/utils.h"
namespace aisdk {
class SnpeModelPrivate
{
public:
    SnpeModelPrivate();
    ~SnpeModelPrivate();

    bool Init();
    bool Load(const std::string& model_path);
    bool Unload();

    int GetInputCount();
    int GetOutputCount();
    std::vector<TensorShape> GetInputShape();
    std::vector<TensorShape> GetOutputShape();
    std::vector<DataType> GetInputDtype();
    std::vector<DataType> GetOutputDtype();
    std::vector<Layout> GetInputLayout();
    std::vector<Layout> GetOutputLayout();
    std::vector<std::string> GetInputName();
    std::vector<std::string> GetOutputName();

private:
    std::unique_ptr<zdl::DlContainer::IDlContainer> container_;
    std::unique_ptr<zdl::SNPE::SNPE> snpe_;

    std::vector<Tensor> input_tensors_;
    std::vector<Tensor> output_tensors_;
};
}

SnpeModelPrivate::SnpeModelPrivate()
{
    
}

SnpeModelPrivate::~SnpeModelPrivate()
{
    Unload();
}


bool SnpeModelPrivate::Init()
{
    zdl::SNPE::SNPEFactory::initializeLogging(zdl::DlSystem::LogLevel_t::LOG_ERROR, "./Log");

    zdl::SNPE::SNPEFactory::setLogLevel(zdl::DlSystem::LogLevel_t::LOG_WARN);
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

bool SnpeModelPrivate::Load(const std::string& model_path)
{
    std::unique_ptr<zdl::DlContainer::IDlContainer> container = zdl::DlContainer::IDlContainer::open(zdl::DlSystem::String(model_path.c_str()));
    if (container == nullptr)
    {
       return false;
    }

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

    // 获取多个input的名称
    DlSystem::StringList input_name_list = snpe->getInputTensorNames();
    for (size_t i = 0; i < input_name_list.size(); ++i) {
        const char* name = input_name_list.at(i);
        // get attributes of buffer by name
        DlSystem::IBufferAttributes* bufferAttributesOptHandle = snpe->getInputOutputBufferAttributes(name);
        if (nullptr == bufferAttributesOptHandle) {
            return false;
        }

        DlSystem::TensorShape tensor_shape = bufferAttributesOptHandle->getDims();

        aisdk::TensorShape shape;
        shape.dim = tensor_shape.rank();
        std::vector<size_t> tensorShape;
        for (size_t j = 0; j < shape.dim; j++) {
            shape.shape.push_back(tensor_shape[j]);
        }

        Snpe_UserBufferEncoding_ElementType_t elemnet_type = Snpe_IBufferAttributes_GetEncodingType(bufferAttributesOptHandle);
        DataType data_type = elementType2DataType(elemnet_type);

        Tensor input_tensor(std::string(name), shape, data_type);
        input_tensors_.emplace_back(input_tensor);
    }



    return true;
}


bool SnpeModelPrivate::Unload()
{
    return true;
}

SnpeModel::SnpeModel(const std::string& model_path)
{
    priv_ = std::shared_ptr<SnpeModelPrivate>(new SnpeModelPrivate());
}

SnpeModel::~SnpeModel()
{
    priv_->Unload();
}

bool SnpeModel::Init()
{   
    return priv_->Init();
}

bool SnpeModel::Load(const std::string& model_path)
{
    return priv_->Load(model_path);
}

bool SnpeModel::Unload()
{
    return priv_->Unload();
}

// 获取模型输入输出的个数
bool SnpeModel::GetInputCount(int& input_count)
{
    return true;
}

bool SnpeModel::GetOutputCount(int& output_count)
{
    return true;
}

// 获取模型输入输出的shape
bool SnpeModel::GetInputShape(std::vector<TensorShape>& input_shapes)
{
    return true;
}

bool SnpeModel::GetOutputShape(std::vector<TensorShape>& output_shapes)
{
    return true;
}

// 获取模型输入输出的dtype
bool SnpeModel::GetInputDtype(std::vector<DataType>& input_dtypes)
{
    return true;
}

bool SnpeModel::GetOutputDtype(std::vector<DataType>& output_dtypes)
{
    return true;
}

// 获取模型输入输出的layout
bool SnpeModel::GetInputLayout(std::vector<Layout>& input_layouts)
{
    return true;
}

bool SnpeModel::GetOutputLayout(std::vector<Layout>& output_layouts)
{
    return true;
}

// 获取模型输入输出的name
bool SnpeModel::GetInputName(std::vector<std::string>& input_names)
{
    return true;
}

bool SnpeModel::GetOutputName(std::vector<std::string>& output_names)
{
    return true;
}

// 获取模型的key
bool SnpeModel::GetKey(std::string& key)
{
    return true;
}

