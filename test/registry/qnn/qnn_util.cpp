#include "qnn_util.hpp"


void logStdoutCallback(const char* fmt,
                           QnnLog_Level_t level,
                           uint64_t timestamp,
                           va_list argp) {
    const char* levelStr = "";
    switch (level) {
        case QNN_LOG_LEVEL_ERROR:
            levelStr = " ERROR ";
            break;
        case QNN_LOG_LEVEL_WARN:
            levelStr = "WARNING";
            break;
        case QNN_LOG_LEVEL_INFO:
            levelStr = "  INFO ";
            break;
        case QNN_LOG_LEVEL_DEBUG:
            levelStr = " DEBUG ";
            break;
        case QNN_LOG_LEVEL_VERBOSE:
            levelStr = "VERBOSE";
            break;
        case QNN_LOG_LEVEL_MAX:
            levelStr = "UNKNOWN";
            break;
    }

//   fprintf(stdout, "%8.1fms [%-7s] ", ms, levelStr);

    vfprintf(stdout, fmt, argp);
    fprintf(stdout, "\n");
}

size_t getFileSize(std::string filePath) {
    std::ifstream in(filePath, std::ifstream::binary);
    if (!in) {
        printf("Failed to open input file: %s\n", filePath.c_str());
        return 0;
    }

    in.seekg(0, in.end);
    const size_t length = in.tellg();
    in.seekg(0, in.beg);

    return  length;
}


int freeQnnTensor(Qnn_Tensor_t &tensor) {
    // free all pointer allocations in struct
    free((void *)QNN_TENSOR_GET_NAME(tensor));
    free(QNN_TENSOR_GET_DIMENSIONS(tensor));

    if (QNN_TENSOR_GET_IS_DYNAMIC_DIMENSIONS(tensor)) {
        free(QNN_TENSOR_GET_IS_DYNAMIC_DIMENSIONS(tensor));
    }

    return MODEL_NO_ERROR;
}

int freeQnnTensors(Qnn_Tensor_t *&tensors, uint32_t numTensors) {
    // free all pointer allocations in struct
    for (size_t i = 0; i < numTensors; i++) {
        freeQnnTensor(tensors[i]);
    }
    free(tensors);
    return MODEL_NO_ERROR;
}

int freeGraphsInfo(GraphInfoPtr_t **graphsInfo, uint32_t numGraphs) {
    if (graphsInfo == nullptr || *graphsInfo == nullptr) {
        return MODEL_TENSOR_ERROR;
    }
    for (uint32_t i = 0; i < numGraphs; i++) {
        free((*graphsInfo)[i]->graphName);
        freeQnnTensors((*graphsInfo)[i]->inputTensors, (*graphsInfo)[i]->numInputTensors);
        freeQnnTensors((*graphsInfo)[i]->outputTensors, (*graphsInfo)[i]->numOutputTensors);
    }

    free(**graphsInfo);
    free(*graphsInfo);
    *graphsInfo = nullptr;
    return MODEL_NO_ERROR;
}

int memscpy(void *dst, size_t dstSize, const void *src, size_t copySize) {
    if (!dst || !src || !dstSize || !copySize) 
        return 0;

    size_t minSize = dstSize < copySize ? dstSize : copySize;

    memcpy(dst, src, minSize);

    return minSize;
}

// 拷贝tensor，一般是从graph的输入输出的tensor中拷贝对应的字段到Qnn_Tensor_t结构体中
bool deepCopyQnnTensorInfo(Qnn_Tensor_t *dst, const Qnn_Tensor_t *src) {
    if (nullptr == dst || nullptr == src) {
        printf("Received nullptr\n");
        return -1;
    }
    // set tensor.version before using QNN_TENSOR_SET macros, as they require the version to be set
    // to correctly assign values
    dst->version           = src->version;
    const char *tensorName = QNN_TENSOR_GET_NAME(src);
    if (!tensorName) {
        QNN_TENSOR_SET_NAME(dst, nullptr);
    } else {
        QNN_TENSOR_SET_NAME(dst, strndup(tensorName, strlen(tensorName)));
    }

    QNN_TENSOR_SET_ID(dst, QNN_TENSOR_GET_ID(src));
    QNN_TENSOR_SET_TYPE(dst, QNN_TENSOR_GET_TYPE(src));
    QNN_TENSOR_SET_DATA_FORMAT(dst, QNN_TENSOR_GET_DATA_FORMAT(src));
    QNN_TENSOR_SET_DATA_TYPE(dst, QNN_TENSOR_GET_DATA_TYPE(src));
    Qnn_QuantizeParams_t qParams = QNN_QUANTIZE_PARAMS_INIT;
    qParams.encodingDefinition   = QNN_TENSOR_GET_QUANT_PARAMS(src).encodingDefinition;
    qParams.quantizationEncoding = QNN_QUANTIZATION_ENCODING_UNDEFINED;
    if (QNN_TENSOR_GET_QUANT_PARAMS(src).quantizationEncoding ==
        QNN_QUANTIZATION_ENCODING_SCALE_OFFSET) {
        qParams.quantizationEncoding = QNN_TENSOR_GET_QUANT_PARAMS(src).quantizationEncoding;
        qParams.scaleOffsetEncoding  = QNN_TENSOR_GET_QUANT_PARAMS(src).scaleOffsetEncoding;
    } else if (QNN_TENSOR_GET_QUANT_PARAMS(src).quantizationEncoding ==
                QNN_QUANTIZATION_ENCODING_AXIS_SCALE_OFFSET) {
        qParams.quantizationEncoding = QNN_TENSOR_GET_QUANT_PARAMS(src).quantizationEncoding;
        qParams.axisScaleOffsetEncoding.axis =
            QNN_TENSOR_GET_QUANT_PARAMS(src).axisScaleOffsetEncoding.axis;
        qParams.axisScaleOffsetEncoding.numScaleOffsets =
            QNN_TENSOR_GET_QUANT_PARAMS(src).axisScaleOffsetEncoding.numScaleOffsets;
        if (QNN_TENSOR_GET_QUANT_PARAMS(src).axisScaleOffsetEncoding.numScaleOffsets > 0) {
            qParams.axisScaleOffsetEncoding.scaleOffset = (Qnn_ScaleOffset_t *)malloc(
                QNN_TENSOR_GET_QUANT_PARAMS(src).axisScaleOffsetEncoding.numScaleOffsets *
                sizeof(Qnn_ScaleOffset_t));
            if (qParams.axisScaleOffsetEncoding.scaleOffset) {
                for (size_t idx = 0;
                        idx < QNN_TENSOR_GET_QUANT_PARAMS(src).axisScaleOffsetEncoding.numScaleOffsets;
                        idx++) {
                    qParams.axisScaleOffsetEncoding.scaleOffset[idx].scale =
                        QNN_TENSOR_GET_QUANT_PARAMS(src).axisScaleOffsetEncoding.scaleOffset[idx].scale;
                    qParams.axisScaleOffsetEncoding.scaleOffset[idx].offset =
                        QNN_TENSOR_GET_QUANT_PARAMS(src).axisScaleOffsetEncoding.scaleOffset[idx].offset;
                }
            }
        }
    }

    QNN_TENSOR_SET_QUANT_PARAMS(dst, qParams);
    QNN_TENSOR_SET_RANK(dst, QNN_TENSOR_GET_RANK(src));
    QNN_TENSOR_SET_DIMENSIONS(dst, nullptr);

    if (QNN_TENSOR_GET_RANK(src) > 0) {
        QNN_TENSOR_SET_DIMENSIONS(dst, (uint32_t *)malloc(QNN_TENSOR_GET_RANK(src) * sizeof(uint32_t)));
        if (QNN_TENSOR_GET_DIMENSIONS(dst)) {
            memscpy(QNN_TENSOR_GET_DIMENSIONS(dst),
                                    QNN_TENSOR_GET_RANK(src) * sizeof(uint32_t),
                                    QNN_TENSOR_GET_DIMENSIONS(src),
                                    QNN_TENSOR_GET_RANK(src) * sizeof(uint32_t));
        }
        if (QNN_TENSOR_GET_IS_DYNAMIC_DIMENSIONS(src)) {
            QNN_TENSOR_SET_IS_DYNAMIC_DIMENSIONS(
                dst, (uint8_t *)malloc(QNN_TENSOR_GET_RANK(src) * sizeof(uint8_t)));
            memscpy(QNN_TENSOR_GET_IS_DYNAMIC_DIMENSIONS(dst),
                                    QNN_TENSOR_GET_RANK(src) * sizeof(uint8_t),
                                    QNN_TENSOR_GET_IS_DYNAMIC_DIMENSIONS(src),
                                    QNN_TENSOR_GET_RANK(src) * sizeof(uint8_t));
        }
    }

    QNN_TENSOR_SET_SPARSE_PARAMS(dst, QNN_TENSOR_GET_SPARSE_PARAMS(src));

    return 0;
}

bool copyTensorsInfo(const Qnn_Tensor_t *tensorsInfoSrc,
                                 Qnn_Tensor_t *&tensorWrappers,
                                 uint32_t tensorsCount) {
    auto returnStatus = true;
    tensorWrappers    = (Qnn_Tensor_t *)calloc(tensorsCount, sizeof(Qnn_Tensor_t));
    if (nullptr == tensorWrappers) {
        printf("Failed to allocate memory for tensorWrappers.\n");
        return -1;
    }

    if (returnStatus) {
        for (size_t tIdx = 0; tIdx < tensorsCount; tIdx++) {
            printf("Extracting tensorInfo for tensor Idx: %ld\n", tIdx);
            tensorWrappers[tIdx] = QNN_TENSOR_INIT;
            deepCopyQnnTensorInfo(&tensorWrappers[tIdx], &tensorsInfoSrc[tIdx]);
        }
    }

    return returnStatus;
}

bool copyGraphsInfoV1(const QnnSystemContext_GraphInfoV1_t *graphInfoSrc,
                                  GraphInfo_t *graphInfoDst) {
    graphInfoDst->graphName = nullptr;
    if (graphInfoSrc->graphName) {
        graphInfoDst->graphName =
            strndup(graphInfoSrc->graphName, strlen(graphInfoSrc->graphName));
    }
    graphInfoDst->inputTensors    = nullptr;
    graphInfoDst->numInputTensors = 0;
    if (graphInfoSrc->graphInputs) {
        if (!copyTensorsInfo(
                graphInfoSrc->graphInputs, graphInfoDst->inputTensors, graphInfoSrc->numGraphInputs)) {
            return -1;
        }
        graphInfoDst->numInputTensors = graphInfoSrc->numGraphInputs;
    }
    graphInfoDst->outputTensors    = nullptr;
    graphInfoDst->numOutputTensors = 0;
    if (graphInfoSrc->graphOutputs) {
        if (!copyTensorsInfo(graphInfoSrc->graphOutputs,
                                graphInfoDst->outputTensors,
                                graphInfoSrc->numGraphOutputs)) {
            return -1;
        }
        graphInfoDst->numOutputTensors = graphInfoSrc->numGraphOutputs;
    }
  return 0;
}

bool copyGraphsInfo(const QnnSystemContext_GraphInfo_t *graphsInput,
                                const uint32_t numGraphs,
                                GraphInfo_t **&graphsInfo) {
  
    if (!graphsInput) {
        printf("Received nullptr for graphsInput.\n");
        return -1;
    }
    bool returnStatus = true;
    graphsInfo =
        (GraphInfo_t **)calloc(numGraphs, sizeof(GraphInfo_t *));
    GraphInfo_t *graphInfoArr =
        (GraphInfo_t *)calloc(numGraphs, sizeof(GraphInfo_t));

    if (nullptr == graphsInfo || nullptr == graphInfoArr) {
        printf("Failure to allocate memory for *graphInfo\n");
        returnStatus = false;
    }

    if (true == returnStatus) {
        for (size_t gIdx = 0; gIdx < numGraphs; gIdx++) {
            printf("Extracting graphsInfo for graph Idx: %ld\n", gIdx);
            if (graphsInput[gIdx].version == QNN_SYSTEM_CONTEXT_GRAPH_INFO_VERSION_1) {
                copyGraphsInfoV1(&graphsInput[gIdx].graphInfoV1, &graphInfoArr[gIdx]);
            }
            graphsInfo[gIdx] = graphInfoArr + gIdx;
        }
    }

    if (true != returnStatus) {
        printf("Received an ERROR during extractGraphsInfo. Freeing resources.\n");
        if (graphsInfo) {
            for (uint32_t gIdx = 0; gIdx < numGraphs; gIdx++) {
                if (graphsInfo[gIdx]) {
                    if (nullptr != graphsInfo[gIdx]->graphName) {
                        free(graphsInfo[gIdx]->graphName);
                        graphsInfo[gIdx]->graphName = nullptr;
                    }
                    freeQnnTensors(graphsInfo[gIdx]->inputTensors,
                                                    graphsInfo[gIdx]->numInputTensors);
                    freeQnnTensors(graphsInfo[gIdx]->outputTensors,
                                                    graphsInfo[gIdx]->numOutputTensors);
                }
            }
            free(*graphsInfo);
        }
        free(graphsInfo);
        graphsInfo = nullptr;
    }

    return true;
}

int load_qnn_function_pointers(QnnFunctionPointers* qnnFunctionPointers, const char* qnn_backend_file, 
                               const char* qnn_model_file, const char* qnn_systefile, const char* qnn_modeldlc_file) {

    void* libBackendHandle = dlopen(qnn_backend_file, RTLD_NOW | RTLD_LOCAL);
    if (nullptr == libBackendHandle) {
        printf("Unable to load backend. dlerror(): %s\n",  dlerror());
        return -1;
    }
    printf("Loaded backend library: %s\n", qnn_backend_file);

    /* Resolve the symbol for Qnn_ErrorHandle_t QnnInterface_getProviders(const QnnInterface_t*** providerList,
                                                                          uint32_t* numProviders)
       API */
    typedef Qnn_ErrorHandle_t (*QnnInterfaceGetProvidersFn_t)(const QnnInterface_t*** providerList,
                                                              uint32_t* numProviders);
    
    
    QnnInterfaceGetProvidersFn_t getInterfaceProviders {nullptr};
    
    getInterfaceProviders = resolveSymbol<QnnInterfaceGetProvidersFn_t>(libBackendHandle, "QnnInterface_getProviders");
    if (nullptr == getInterfaceProviders) {
        return -1;
    }
    printf("Found QnnInterface_getProviders function in backend library.\n");

    // 3. Use QNN Interface to obtain function pointers
    QnnInterface_t** interfaceProviders{nullptr};
    uint32_t numProviders{0};
    // Query for al available interfaces
    if (QNN_SUCCESS != getInterfaceProviders((const QnnInterface_t***)&interfaceProviders, &numProviders)) {
        printf("Failed to get interface providers.\n");
        return -1;
    }
    // Check for validity of returned interfaces
    if (nullptr == interfaceProviders) {
        printf("Failed to get interface providers: null interface providers received.\n");
        return -1;
    }
    if (0 == numProviders) {
        printf("Failed to get interface providers: 0 interface providers.\n");
        return -1;
    }
    printf("Found %d interface providers in the backend library.\n", numProviders);

    bool foundValidInterface{false};
    // Loop through all available interface providers and pick the one that suits the current API
    // version
    for (size_t pIdx = 0; pIdx < numProviders; pIdx++) {
        if (QNN_API_VERSION_MAJOR == interfaceProviders[pIdx]->apiVersion.coreApiVersion.major && \
            QNN_API_VERSION_MINOR <= interfaceProviders[pIdx]->apiVersion.coreApiVersion.minor) {
            foundValidInterface                 = true;
            qnnFunctionPointers = new QnnFunctionPointers();
            qnnFunctionPointers->qnnInterface = interfaceProviders[pIdx]->QNN_INTERFACE_VER_NAME;
            printf("Found a valid interface, major version: %d, minor version: %d\n",
                    interfaceProviders[pIdx]->apiVersion.coreApiVersion.major, interfaceProviders[pIdx]->apiVersion.coreApiVersion.minor);
            break;
        }
    }
    if (!foundValidInterface) {
        printf("Unable to find a valid interface.\n");
        libBackendHandle = nullptr;
        return -1;
    }
    printf("Found a valid interface\n");
    
    // load model相关的接口
    void* libModelHandle = dlopen(qnn_model_file, RTLD_NOW | RTLD_LOCAL);

    if (nullptr == libModelHandle) {
        printf("Unable to load model. dlerror(): %s\n", dlerror());
        return -1;
    }
    printf("Loaded model library: %s\n", qnn_model_file);
    
    #if 0
    std::string modelPrepareFunc = "QnnModel_composeGraphs";
    qnnFunctionPointers->composeGraphsFnHandle =
        resolveSymbol<ComposeGraphsFnHandleType_t>(libModelHandle, modelPrepareFunc.c_str());
    if (nullptr == qnnFunctionPointers->composeGraphsFnHandle) {
        return -1;
    }
    printf("Found QnnModel_composeGraphs function in model library.\n");

    std::string modelFreeFunc = "QnnModel_freeGraphsInfo";
    qnnFunctionPointers->freeGraphInfoFnHandle =
        resolveSymbol<FreeGraphInfoFnHandleType_t>(libModelHandle, modelFreeFunc.c_str());
    if (nullptr == qnnFunctionPointers->freeGraphInfoFnHandle) {
        return -1;
    }
    printf("Found QnnModel_freeGraphsInfo function in model library.\n");
    #endif

    // QnnModelDlc.so
    void* libmodelDlcHandle = dlopen(qnn_modeldlc_file, RTLD_NOW | RTLD_LOCAL);
    if (nullptr == libmodelDlcHandle) {
        printf("Unable to load modeldlc library. dlerror(): %s\n", dlerror());
        return -1;
    }

    std::string modelDlcFunc = "QnnModel_composeGraphs";
    qnnFunctionPointers->qnnModelComposeGraphsFromDlc =
        resolveSymbol<QnnModel_composeGraphsFromDlc_t>(libmodelDlcHandle, modelDlcFunc.c_str());
    if (nullptr == qnnFunctionPointers->freeGraphInfoFnHandle) {
        printf("Failed to resolve QnnModel_composeGraphsFromDlc function in modeldlc library.\n");
        return -1;
    }
    printf("Found QnnModelDlc_freeModelInfo function in model library.\n");

    // QNN System Interface can be used to resolve all symbols related to QNN System APIs as shown below
    void* systemLibraryHandle = dlopen(qnn_systefile, RTLD_NOW | RTLD_LOCAL);
    if (nullptr == systemLibraryHandle) {
        printf("Unable to load system library. dlerror(): %s\n", dlerror());
        return -1;
    }

    typedef Qnn_ErrorHandle_t (*QnnSystemInterfaceGetProvidersFn_t)(const QnnSystemInterface_t*** providerList, uint32_t* numProviders);

    QnnSystemInterfaceGetProvidersFn_t getSystemInterfaceProviders{nullptr};
    getSystemInterfaceProviders = resolveSymbol<QnnSystemInterfaceGetProvidersFn_t>(
            systemLibraryHandle, "QnnSystemInterface_getProviders");
    if (nullptr == getSystemInterfaceProviders) {
        return -1;
    }


    QnnSystemInterface_t** systemInterfaceProviders{nullptr};
    
    if (QNN_SUCCESS != getSystemInterfaceProviders(
                         (const QnnSystemInterface_t***)&systemInterfaceProviders, &numProviders)) {
        printf("Failed to get system interface providers.\n");
        return -1;
    }

    if (nullptr == systemInterfaceProviders) {
        printf("Failed to get system interface providers: null interface providers received.\n");
        return -1;
    }

    if (0 == numProviders) {
        printf("Failed to get interface providers: 0 interface providers.\n");
        return -1;
    }

    bool foundValidSystemInterface{false};
    for (size_t pIdx = 0; pIdx < numProviders; pIdx++) {
        if (QNN_SYSTEM_API_VERSION_MAJOR == systemInterfaceProviders[pIdx]->systemApiVersion.major &&
            QNN_SYSTEM_API_VERSION_MINOR <= systemInterfaceProviders[pIdx]->systemApiVersion.minor) {
            foundValidSystemInterface = true;
            qnnFunctionPointers->qnnSystemInterface =
                systemInterfaceProviders[pIdx]->QNN_SYSTEM_INTERFACE_VER_NAME;
                
            printf("Found a valid system interface, major version: %d, minor version: %d\n",  
                   systemInterfaceProviders[pIdx]->systemApiVersion.major, systemInterfaceProviders[pIdx]->systemApiVersion.minor);
            break;
        }
    }

    return 0;
}


int Init(QnnFunctionPointers* qnnFunctionPointers, Qnn_LogHandle_t &logHandle, 
            Qnn_BackendHandle_t &backendHandle, Qnn_DeviceHandle_t &deviceHandle,
             Qnn_ContextHandle_t &context, Qnn_GraphHandle_t &graphHandle, 
             Qnn_ProfileHandle_t &profileBackendHandle, const char* cache_model_file)
{
    // 4. Set Up logging 
    if (QNN_SUCCESS !=
          qnnFunctionPointers->qnnInterface.logCreate(logStdoutCallback, QNN_LOG_LEVEL_DEBUG, &logHandle)) {
        printf("Unable to initialize logging in the backend.\n");
        return -1;
    }
    
    // 5. Init backend
    // const QnnBackend_Config_t* backendConfigs;
    QnnBackend_Config_t **backendConfigs = nullptr;
    /* Set up any necessary backend configurations */
    if (QNN_BACKEND_NO_ERROR != qnnFunctionPointers->qnnInterface.backendCreate(logHandle,
                                                                                 (const QnnBackend_Config_t**)backendConfigs,
                                                                                 &backendHandle)) {
        printf("Could not initialize backend\n");
        return -1;
    }

    // 6. Init profiling
    if (QNN_PROFILE_NO_ERROR != qnnFunctionPointers->qnnInterface.profileCreate(
                                      backendHandle, QNN_PROFILE_LEVEL_BASIC, &profileBackendHandle)) {
        printf("Unable to create profile handle in the backend.\n");
        return -1;
    }

    // 7. create device
    auto qnnStatus = qnnFunctionPointers->qnnInterface.propertyHasCapability(QNN_PROPERTY_GROUP_DEVICE);
    if (QNN_PROPERTY_NOT_SUPPORTED == qnnStatus) {
        printf("Device property is not supported\n");
    }
    QnnDevice_Config_t devConfig;
    // devConfig.option = QNN_DEVICE_OPTION_RUNTIME;
        const QnnDevice_Config_t* devConfigArray[] = {&devConfig, nullptr};
    if (QNN_PROPERTY_ERROR_UNKNOWN_KEY == qnnStatus) {
        printf("Backend does not support device creation. Status: %ld\n", qnnStatus);
        return -1;
    } else {
        Qnn_ErrorHandle_t qnnStatus = qnnFunctionPointers->qnnInterface.deviceCreate(logHandle, devConfigArray, &deviceHandle);
        if (QNN_SUCCESS != qnnStatus && QNN_DEVICE_ERROR_UNSUPPORTED_FEATURE != qnnStatus) {
            printf("Failed to create device: %ld\n", qnnStatus);
            return -1;
        }
    }
    printf("Created device\n");

    // 8. register op package
    // uint32_t opPackageCount;
    // char* opPackagePath[opPackageCount];
    // char* opPackageInterfaceProvider[opPackageCount];
    // /* Set up required op package paths and interface providers as necessary */
    // for(uint32_t idx = 0; idx < opPackageCount; idx++) {
    //     if (QNN_BACKEND_NO_ERROR !=
    //         qnnFunctionPointers->qnnInterface.backendRegisterOpPackage(backendHandle,
    //                                                                     opPackagePath[idx],
    //                                                                     opPackageInterfaceProvider[idx])) {
    //         printf("Could not register Op Package: %s and interface provider: %s",
    //                 opPackagePath[idx],
    //                 opPackageInterfaceProvider[idx]);
    //         return -1;
    //     }
    // }
    return 0;
}

int load_model(QnnFunctionPointers* qnnFunctionPointers, const char* cache_model_file,
               Qnn_BackendHandle_t backendHandle, Qnn_DeviceHandle_t deviceHandle, Qnn_ProfileHandle_t &profileBackendHandle,
               Qnn_GraphHandle_t* graphHandle, 
               Qnn_ContextHandle_t &context, QnnContext_Config_t **contextConfigs,
               GraphInfo_t** graphsInfo, uint32_t graphsCount,
               const char* dlc_model_file, bool used_cache) 
{


  // 检查cache_model_file是否存在
    std::string cachedBinaryPath(cache_model_file);
    if (!std::ifstream(cachedBinaryPath).good()) {
        used_cache = false;    
    } else {
        used_cache = true;
    }
    // 9. create context
    if (!used_cache){
        /* Set up any context configs that are necessary */
        if (QNN_CONTEXT_NO_ERROR !=
            qnnFunctionPointers->qnnInterface.contextCreate(backendHandle,
                                                            deviceHandle,
                                                            (const QnnContext_Config_t**)contextConfigs,
                                                            &context)) {
            printf("Could not create context\n");
            return -1;
        }
        printf("Created context\n");
        
        // 10. prepare graphs
        /* Structure to retrieve information about graphs, like graph name,
        details about input and output tensors preset in libQnnSampleModel.so */
        
        // No. of graphs present in libQnnSampleModel.so
        // true to enable intermediate outputs, false for network outputs only
        bool debug = true;
        QnnLog_Level_t logLevel = QNN_LOG_LEVEL_DEBUG;
        GraphConfigInfo_t **graphConfigsInfo = nullptr;
        #if 0
        if (ModelError_t::MODEL_NO_ERROR !=
                qnnFunctionPointers->composeGraphsFnHandle(backendHandle,
                                                            qnnFunctionPointers->qnnInterface,
                                                            context,
                                                            (const GraphConfigInfo_t**)graphConfigsInfo,
                                                            1,
                                                            &graphsInfo,
                                                            &graphsCount,
                                                            debug,
                                                            logStdoutCallback,
                                                            logLevel)) {
            printf("Failed in composeGraphs()\n");
            return -1;
        }
        #else
        // QnnModel_composeGraphsFromDlc(Qnn_BackendHandle_t backendHandle,
        //                                 QNN_INTERFACE_VER_TYPE interface,
        //                                 Qnn_ContextHandle_t contextHandle,
        //                                 const GraphConfigInfo_t **graphsConfigInfo,
        //                                 const char *dlcPath,
        //                                 const uint32_t numGraphsConfigInfo,
        //                                 GraphInfoPtr_t **graphsInfo,
        //                                 uint32_t *numGraphsInfo,
        //                                 bool debug,
        //                                 QnnLog_Callback_t logCallback,
        //                                 QnnLog_Level_t maxLogLevel)
        // GraphConfigInfo_t **graphConfigsInfo = nullptr;
        if (QNN_SUCCESS != qnnFunctionPointers->qnnModelComposeGraphsFromDlc(
            backendHandle,
            qnnFunctionPointers->qnnInterface,
            context,
            (const GraphConfigInfo_t**)graphConfigsInfo,
            cache_model_file,
            1,
            &graphsInfo,
            &graphsCount,
            true,
            logStdoutCallback,
            QNN_LOG_LEVEL_DEBUG)) {
            printf("Failed to get context binary info\n");
            return -1;
        }
        #endif

        // 11. Finalize graphs
        for (size_t graphIdx = 0; graphIdx < graphsCount; graphIdx++) {
            if (QNN_GRAPH_NO_ERROR != qnnFunctionPointers->qnnInterface.graphFinalize(
                                         (*graphsInfo)[graphIdx].graph, profileBackendHandle, nullptr)) {
                printf("Could not finalize graph\n");
                return -1;
            }
            /* Extract profiling information if desired and if a valid handle was supplied to finalize
                graphs API */
        }
        // 12. Save context int to a binary file
        // Get the expected size of the buffer from the backend in which the context can be saved
        unsigned long requiredBufferSize{0};
        if (QNN_CONTEXT_NO_ERROR != qnnFunctionPointers->qnnInterface.contextGetBinarySize(context, &requiredBufferSize)) {
            printf("Could not get the required binary size.\n");
            return -1;
        }
        printf("Required buffer size: %ld bytes\n", requiredBufferSize);
        
        // Allocate a buffer of the required size
        uint8_t* saveBuffer = (uint8_t*)malloc(requiredBufferSize * sizeof(uint8_t));
        if (nullptr == saveBuffer) {
            printf("Could not allocate buffer to save binary.\n");
            return -1;
        }
        
        auto status = StatusCode::SUCCESS;
        unsigned long writtenBufferSize{0};
        // Pass the allocated buffer and obtain a copy of the context binary written into the buffer
        if (QNN_CONTEXT_NO_ERROR !=
        qnnFunctionPointers->qnnInterface.contextGetBinary(context,
                                                            reinterpret_cast<void*>(saveBuffer),
                                                            requiredBufferSize,
                                                            &writtenBufferSize)) {
            printf("Could not get binary.\n");
            return -1;
        }
        printf("Written buffer size: %ld bytes\n", writtenBufferSize);
        
        // Check if the supplied buffer size is at least as big as the amount of data witten by the backend
        if (requiredBufferSize < writtenBufferSize) {
            printf("Illegal written buffer size [%ld] bytes. Cannot exceed allocated memory of [%ld] bytes.\n",
                                                            writtenBufferSize,
                                                            requiredBufferSize);
                return -1;
        }
        
        // Use caching utility to save metadata along with the binary buffer from the backend
        const std::string outputPath(cache_model_file);
        std::ofstream os(outputPath, std::ofstream::binary);
        if (!os) {
            printf("Failed to open output file for writing: %s\n", outputPath.c_str());
            return -1;
        }
        os.write(reinterpret_cast<char*>(saveBuffer), writtenBufferSize);
        os.close();
        printf("Saved binary to file: %s\n", outputPath.c_str());
    } else {
        printf("Loading context from cache file: %s\n", cache_model_file);
        // 13. Load context from a binary file
        auto returnStatus   = StatusCode::SUCCESS;
        
        uint32_t bufferSize = getFileSize(cache_model_file);
        printf("bufferSize: %d\n", bufferSize);

        auto buffer = std::shared_ptr<uint8_t>(new uint8_t[bufferSize], std::default_delete<uint8_t[]>());
        if (!buffer) {
            printf("Failed to allocate memory.\n");
            return -1;
        }

        std::ifstream in(cachedBinaryPath, std::ifstream::binary);
        if (!in) {
            printf("Failed to open input file: %s\n", cachedBinaryPath.c_str());
            return -1;
        }
        if (!in.read(reinterpret_cast<char*>(buffer.get()), bufferSize)) {
            printf("Failed to read the contents of: %s\n", cachedBinaryPath.c_str());
            return -1;
        }
        printf("Read binary from file: %s\n", cachedBinaryPath.c_str());
        // in.close();

        /* Create a QnnSystemContext handle to access system context APIs. */
        QnnSystemContext_Handle_t sysCtxHandle{nullptr};
        if (QNN_SUCCESS != qnnFunctionPointers->qnnSystemInterface.systemContextCreate(&sysCtxHandle)) {
            printf("Could not create system handle.\n");
            return -1;
        }
        printf("Created system handle\n");

        /* Retrieve metadata from the context binary through QNN System Context API. */
        const QnnSystemContext_BinaryInfo_t* binaryInfo{nullptr};
        Qnn_ContextBinarySize_t binaryInfoSize{0};
        if (QNN_SUCCESS != qnnFunctionPointers->qnnSystemInterface.systemContextGetBinaryInfo(
                            sysCtxHandle,
                            static_cast<void*>(buffer.get()),
                            bufferSize,
                            &binaryInfo,
                            &binaryInfoSize)) {
            printf("Failed to get context binary info\n");
            return -1;
        }
        /* Make a copy of the metadata. */
        if (binaryInfo->version == QNN_SYSTEM_CONTEXT_BINARY_INFO_VERSION_1) {
            if (binaryInfo->contextBinaryInfoV1.graphs) {
                if (!copyGraphsInfo(binaryInfo->contextBinaryInfoV1.graphs,
                                    binaryInfo->contextBinaryInfoV1.numGraphs,
                                    graphsInfo)) {
                    printf("Failed while copying graphs Info.\n");
                    return -1;
                }
                graphsCount = binaryInfo->contextBinaryInfoV1.numGraphs;
            }
        } else if (binaryInfo->version == QNN_SYSTEM_CONTEXT_BINARY_INFO_VERSION_2) {
            if (binaryInfo->contextBinaryInfoV2.graphs) {
                if (!copyGraphsInfo(binaryInfo->contextBinaryInfoV2.graphs,
                                    binaryInfo->contextBinaryInfoV2.numGraphs,
                                    graphsInfo)) {
                    printf("Failed while copying graphs Info.\n");
                    return -1;
                }
                graphsCount = binaryInfo->contextBinaryInfoV2.numGraphs;
            }
        }
        printf("Binary info version: %d\n", binaryInfo->version);
        printf("Binary info size: %ld\n", binaryInfoSize);
        
        printf("Successfully loaded context from cache file: %s\n", cache_model_file);
        
        /* Release resources associated with previously created QnnSystemContext handle. */
        qnnFunctionPointers->qnnSystemInterface.systemContextFree(sysCtxHandle);
        sysCtxHandle = nullptr;
        
        /* readBuffer contains the binary data that was previously obtained from a backend. Pass this
        cached binary data to the backend to recreate the same context. */
        if (qnnFunctionPointers->qnnInterface.contextCreateFromBinary(backendHandle,
                                                                    deviceHandle,
                                                                    (const QnnContext_Config_t**)&contextConfigs,
                                                                    reinterpret_cast<void*>(buffer.get()),
                                                                    bufferSize,
                                                                    &context,
                                                                    profileBackendHandle)) {
            printf("Could not create context from binary.\n");
            return -1;
        }
        printf("Successfully created context from cache file: %s\n", cache_model_file);
        
        /* Obtain and save graph handles for each graph present in the context based on the saved graph
        names in the metadata */
        for (size_t graphIdx = 0; graphIdx < graphsCount; graphIdx++) {
            if (QNN_SUCCESS != qnnFunctionPointers->qnnInterface.graphRetrieve(
                    context, (*graphsInfo)[graphIdx].graphName, &((*graphsInfo)[graphIdx].graph))) {
                printf("Unable to retrieve graph handle for graph Idx: %ld\n", graphIdx);
                return -1;
            }
        }
        printf("Successfully retrieved graph handles from cache file: %s\n", cache_model_file);
    }

    // 打印计算图信息
    for (size_t graphIdx = 0; graphIdx < graphsCount; graphIdx++) {
        printf("Graph Name: %s\n", (*graphsInfo)[graphIdx].graphName);
        printf("Number of input tensors: %d\n", (*graphsInfo)[graphIdx].numInputTensors);
        printf("Number of output tensors: %d\n", (*graphsInfo)[graphIdx].numOutputTensors);
        for (size_t inputIdx = 0; inputIdx < (*graphsInfo)[graphIdx].numInputTensors; inputIdx++) {
            // printf("Input tensor %d: %s\n", inputIdx, (*graphsInfo)[graphIdx].inputTensors[inputIdx].name);
            if ((*graphsInfo)[graphIdx].inputTensors[inputIdx].version == 1) {
                printf("Input tensor %ld: %s, shape: ", inputIdx, (*graphsInfo)[graphIdx].inputTensors[inputIdx].v1.name);
                for (size_t i = 0; i < (*graphsInfo)[graphIdx].inputTensors[inputIdx].v1.rank; i++) {
                    printf("%d ", (*graphsInfo)[graphIdx].inputTensors[inputIdx].v1.dimensions[i]);
                }
                printf("\n");
            } else if ((*graphsInfo)[graphIdx].inputTensors[inputIdx].version == 2) {
                printf("Input tensor %ld: %s, shape: ", inputIdx, (*graphsInfo)[graphIdx].inputTensors[inputIdx].v2.name);
                for (size_t i = 0; i < (*graphsInfo)[graphIdx].inputTensors[inputIdx].v2.rank; i++) {
                    printf("%d ", (*graphsInfo)[graphIdx].inputTensors[inputIdx].v2.dimensions[i]);
                }
                printf("\n");
            }
        }
        for (size_t outputIdx = 0; outputIdx < (*graphsInfo)[graphIdx].numOutputTensors; outputIdx++) {
            if ((*graphsInfo)[graphIdx].outputTensors[outputIdx].version == 1) {
                printf("Output tensor %ld: %s, shape: ", outputIdx, (*graphsInfo)[graphIdx].outputTensors[outputIdx].v1.name);
                for (size_t i = 0; i < (*graphsInfo)[graphIdx].outputTensors[outputIdx].v1.rank; i++) {
                    printf("%d ", (*graphsInfo)[graphIdx].outputTensors[outputIdx].v1.dimensions[i]);
                }
                printf("\n");
            } else if ((*graphsInfo)[graphIdx].outputTensors[outputIdx].version == 2) {
                printf("Output tensor %ld: %s, shape: ", outputIdx, (*graphsInfo)[graphIdx].outputTensors[outputIdx].v2.name);
                for (size_t i = 0; i < (*graphsInfo)[graphIdx].outputTensors[outputIdx].v2.rank; i++) {
                    printf("%d ", (*graphsInfo)[graphIdx].outputTensors[outputIdx].v2.dimensions[i]);
                }
                printf("\n");
            }
        }
    }

    return 0;
}

int ready_tensors(Qnn_Tensor_t* input_tensors, Qnn_Tensor_t* output_tensors,
                  uint32_t inputSize, uint32_t outputSize,
                  Qnn_DataType_t input_dataType, Qnn_DataType_t output_dataType,
                  uint32_t inputDataSize, uint32_t outputDataSize,
                  Qnn_GraphHandle_t graphHandle, Qnn_ContextHandle_t context,
                  GraphInfo_t** graphsInfo) 
{
  uint32_t graphIdx;
    auto graphInfo = (*graphsInfo)[graphIdx];
    uint32_t tensorCount = graphInfo.numInputTensors + graphInfo.numOutputTensors;
    uint32_t inputTensorCount = graphInfo.numInputTensors;
    uint32_t outputTensorCount = graphInfo.numOutputTensors;

    std::vector<int> inputDims;
    std::vector<int> outputDims;
    for (size_t i = 0; i < inputTensorCount; i++) {
        if (graphInfo.inputTensors[i].version == 1) {
            inputDims.insert(inputDims.end(), graphInfo.inputTensors[i].v1.dimensions, graphInfo.inputTensors[i].v1.dimensions + graphInfo.inputTensors[i].v1.rank);
            input_dataType = graphInfo.inputTensors[i].v1.dataType;
        } else if (graphInfo.inputTensors[i].version == 2) {
            inputDims.insert(inputDims.end(), graphInfo.inputTensors[i].v2.dimensions, graphInfo.inputTensors[i].v2.dimensions + graphInfo.inputTensors[i].v2.rank);
            input_dataType = graphInfo.inputTensors[i].v2.dataType;
        }
    }

    for (size_t i = 0; i < outputTensorCount; i++) {
        if (graphInfo.outputTensors[i].version == 1) {
            outputDims.insert(outputDims.end(), graphInfo.outputTensors[i].v1.dimensions, graphInfo.outputTensors[i].v1.dimensions + graphInfo.outputTensors[i].v1.rank);
            output_dataType = graphInfo.outputTensors[i].v1.dataType;
        } else if (graphInfo.outputTensors[i].version == 2) {
            outputDims.insert(outputDims.end(), graphInfo.outputTensors[i].v2.dimensions, graphInfo.outputTensors[i].v2.dimensions + graphInfo.outputTensors[i].v2.rank);
            output_dataType = graphInfo.outputTensors[i].v2.dataType;
        }
    }

    inputDataSize = 1;
    if (input_dataType == QNN_DATATYPE_INT_8) {
        inputDataSize = sizeof(uint8_t);
    } else if (input_dataType == QNN_DATATYPE_INT_16) {
        inputDataSize = sizeof(int16_t);
    } else if (input_dataType == QNN_DATATYPE_INT_32) {
        inputDataSize = sizeof(int32_t);
    } else if (input_dataType == QNN_DATATYPE_FLOAT_32) {
        inputDataSize = sizeof(float);
    }   

    outputDataSize = 1;
    if (output_dataType == QNN_DATATYPE_INT_8) {
        outputDataSize = sizeof(uint8_t);
    } else if (output_dataType == QNN_DATATYPE_INT_16) {
        outputDataSize = sizeof(int16_t);
    } else if (output_dataType == QNN_DATATYPE_INT_32) {
        outputDataSize = sizeof(int32_t);
    } else if (output_dataType == QNN_DATATYPE_FLOAT_32) {
        outputDataSize = sizeof(float);
    }   

    inputSize = 1;
    for (size_t i = 0; i < inputDims.size(); i++) {
        inputSize *= inputDims[i];
    }

    printf("inputSize: %d, dataSize: %d\n", inputSize, inputDataSize);
    outputSize = 1;
    for (size_t i = 0; i < outputDims.size(); i++) {
        outputSize *= outputDims[i];
    }
    printf("outputSize: %d, dataSize: %d\n", outputSize, outputDataSize);

    input_tensors  =  (Qnn_Tensor_t*)calloc(1, inputTensorCount * sizeof(Qnn_Tensor_t));
    output_tensors =  (Qnn_Tensor_t*)calloc(1, outputTensorCount * sizeof(Qnn_Tensor_t));

    // deepCopyQnnTensorInfo(((*tensors) + tensorIdx), &wrapperTensor)
    for (size_t i = 0; i < inputTensorCount; i++) {
        deepCopyQnnTensorInfo(&input_tensors[i], ((*graphsInfo)[graphIdx].inputTensors + i));
    }

    for (size_t i = 0; i < outputTensorCount; i++) {
        deepCopyQnnTensorInfo(&output_tensors[i], ((*graphsInfo)[graphIdx].outputTensors + i));\
    }

    // 给inputs和outputs分配空间
    for (size_t i = 0; i < inputTensorCount; i++) {
        // QNN_TENSOR_GET_CLIENT_BUF(input_tensors).data = (char *)malloc(inputSize * inputDataSize);
        Qnn_ClientBuffer_t clientBuffer = QNN_CLIENT_BUFFER_INIT;
        clientBuffer.data = (char *)malloc(inputSize * inputDataSize);
        clientBuffer.dataSize = inputSize * inputDataSize;
        QNN_TENSOR_SET_CLIENT_BUF(input_tensors[i], clientBuffer);
    }
    for (size_t i = 0; i < outputTensorCount; i++) {
        // QNN_TENSOR_GET_CLIENT_BUF(output_tensors).data = (char *)malloc(outputSize * outputDataSize);
        Qnn_ClientBuffer_t clientBuffer = QNN_CLIENT_BUFFER_INIT;
        clientBuffer.data = (char *)malloc(outputSize * outputDataSize);
        clientBuffer.dataSize = outputSize * outputDataSize;
        QNN_TENSOR_SET_CLIENT_BUF(output_tensors[i], clientBuffer);
    }
    
    return 0;
}

int read_input_data(Qnn_Tensor_t* input_tensors, uint32_t inputSize, uint32_t inputDataSize, const char* input_file) {
    // 读取输入文件
    std::vector<float> inputData;

    std::ifstream inputFile(input_file, std::ifstream::binary);
    if (!inputFile) {
        printf("Failed to open input file: %s\n", input_file);
        return -1;
    }

    inputFile.seekg(0, std::ios::end);
    size_t inputFileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);
    inputData.resize(inputFileSize / sizeof(float));
    inputFile.read(reinterpret_cast<char*>(inputData.data()), inputFileSize);
    inputFile.close();

    // 塞到inputs
    for (size_t i = 0; i < inputSize; i++) {
        memcpy((char *)QNN_TENSOR_GET_CLIENT_BUF(input_tensors[i]).data, &inputData[i], inputDataSize);
    }
    printf("Copied input data to tensor\n");

    return 0;
}

int save_output_data(Qnn_Tensor_t* output_tensors, uint32_t outputSize, uint32_t outputDataSize, const char* output_file) {
    // 保存output_file
    char* bufferToWrite = reinterpret_cast<char*>(QNN_TENSOR_GET_CLIENT_BUF(output_tensors[0]).data);
    std::ofstream outputFile(output_file, std::ofstream::binary);
    if (!outputFile) {
        printf("Failed to open output file: %s\n", output_file);
        return -1;
    }

    outputFile.write(reinterpret_cast<char*>(bufferToWrite), outputSize * outputDataSize);
    outputFile.close();
    printf("Wrote output data to file: %s\n", output_file);

    return 0;
}