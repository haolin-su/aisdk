# API 说明文档

## 接口类型

算法分类接口：  
1）CV类接口：CV类接口用于处理图像、视频、摄像头等相关内容。  
2）NLP类接口：NLP类接口用于处理文本，文件等相关内容。  
3）Audio类接口：Audio类接口用于处理音频类相关内容。  

多语言接口  
1）C++接口：C++接口用于处理图像、视频、摄像、文本、语音等相关内容。  
2）C接口：C接口用于处理图像、视频、摄像、文本、语音等相关内容。  
3）Python接口：Python接口用于处理图像、视频、摄像、文本、语音等相关内容。  
4）Java接口：Java接口用于处理图像、视频、摄像、文本、语音相关内容。  

## 接口列表

1）通用接口
- 初始化函数
- 释放函数
- 错误码函数
- 版本号函数
- 日志函数

2）CV类接口
- 图像处理接口
- 视频处理接口

3）NLP类接口：
- 文本处理接口

4）Audio类接口：
- 音频处理接口

5）多模态类接口
- 多模态处理接口

6）硬件资源接口
- 硬件资源控制接口
- 硬件资源状态接口

7）算法指标接口
- 模型推理时延
- 模型推理吞吐量

## C++接口

### 结构体说明

1）通用结构体

授权结构体
```C++
struct FIBO_License_t {
    std::string app_id; // 应用ID
    std::string app_key; // 应用Key
    std::string app_secret; // 应用Secret
    std::string token; // Token
}
```

日志结构体
```C++
enum FIBO_LogLevel {
    LOG_LEVEL_DEBUG = 0, // 调试
    LOG_LEVEL_INFO = 1, // 信息
    LOG_LEVEL_WARNING = 2, // 警告#
    LOG_LEVEL_ERROR = 3, // 错误
    LOG_LEVEL_FATAL = 4, // 严重错误
};

struct FIBO_Log_t {
    int level; // 日志级别，参考FIBO_LogLevel
    std::string file_path; // 文件路径
    int max_size; // 日志文件最大大小， 单位：MB，取值范围[0, 100MB]
    int max_count; // 日志文件最大数量， 单位：个，取值范围[1, 10]
    bool enable_console; // 是否输出到控制台
    bool enable_file; // 是否输出到文件
};
```

错误码结构体
```C++
struct FIBO_Error_t {
    int code; // 错误码
    std::string message; // 错误信息
};
```

版本信息结构体
```C++
struct FIBO_Version_t {
    std::string major_version; // 主版本号
    std::string minor_version; // 次版本号
    std::string patch_version; // 修订版本号
    std::string time; // 时间
};
// 版本号格式：主版本号.次版本号.修订版本号
// 时间格式：yyyy-MM-dd HH:mm:ss
// 例如：1.0.0 2021-01-01 00:00:00
```

算法信息结构体
```C++
struct FIBO_Algorithm_t {
    std::string name; // 算法名称，如CV_CLASSIFIER、NLP_SUMMARIZE等
    std::string type; // 算法类型，如CV、NLP等
    std::string version; // 算法版本，如1.0.0等
    std::string path; // 算法路径
};

模型信息结构体
```C++
struct FIBO_Model_t {
    std::string name; // 模型名称，如CV_CLASSIFIER_RESNET50、NLP_SUMMARIZE_T5等
    std::string type; // 模型类型，如yolov5、t5等
    std::string version; // 模型版本，如1.0.0等
    std::string path; // 模型路径
};

错误码定义如下：
```C++
enum ErrorCode {
    ERROR_CODE_OK                   = 0, // 成功
    ERROR_CODE_INIT_FAILED          = 1, // 初始化失败
    ERROR_CODE_INVALID_PARAM        = 2, // 参数错误
    ERROR_CODE_INVALID_NO_INIT      = 3, // 未初始化

    ERROR_CODE_INVALID_APP_ID       = 100, // 无效的应用ID
    ERROR_CODE_INVALID_APP_KEY      = 101, // 无效的应用Key
    ERROR_CODE_INVALID_APP_SECRET   = 102, // 无效的应用Secret
    ERROR_CODE_INVALID_LICENSE      = 103, // 无效的License

    ERROR_CODE_CV_INIT_FAILED       = 200, // CV初始化失败
    ERROR_CODE_CV_INVALID_PARAM     = 201, // CV参数错误
    ERROR_CODE_CV_INVALID_IMAGE     = 202, // CV无效的图像
    ERROR_CODE_CV_INVALID_MODEL     = 203, // CV无效的模型
    ERROR_CODE_CV_INVALID_RESULT    = 204, // CV无效的结果

    ERROR_CODE_NLP_INIT_FAILED      = 300, // NLP初始化失败
    ERROR_CODE_NLP_INVALID_PARAM    = 301, // NLP参数错误
    ERROR_CODE_NLP_INVALID_TEXT     = 302, // NLP无效的文本
    ERROR_CODE_NLP_INVALID_RESPONSE = 303, // NLP无效的响应

    ERROR_CODE_AUDIO_INIT_FAILED    = 400, // Audio初始化失败
    ERROR_CODE_AUDIO_INVALID_PARAM  = 401, // Audio参数错误
    ERROR_CODE_AUDIO_INVALID_AUDIO  = 402, // Audio无效的音频
    ERROR_CODE_AUDIO_INVALID_SPEECH = 403, // Audio无效的语音

    ERROR_CODE_RESOURCE_INIT_FAILED     = 500, // 资源初始化失败
    ERROR_CODE_RESOURCE_INVALID_PARAM   = 501, // 资源参数错误
    
    ERROR_CODE_OTHER_ERROR           = 1000, // 其他错误
};
```

2）CV类结构体
单帧图像结构体
```C++
struct FIBO_CV_Img_t {
    // 图像宽度，取值范围：[1, 4096]
    int width; 
    // 图像高度，取值范围：[1, 4096]
    int height; 
    // 图像通道数，取值范围：[1, 4]
    int channels; 
    // 图像格式，取值范围：[FIBO_CV_FORMAT_RGB, FIBO_CV_FORMAT_BGR, FIBO_CV_FORMAT_RGBA, FIBO_CV_FORMAT_BGRA, FIBO_CV_FORMAT_GRAY, FIBO_CV_FORMAT_YUV420P，FIBO_CV_FORMAT_NV12, FIBO_CV_FORMAT_NV21]
    int format;     
    // 图像大小，单位：字节，取值范围：[width * height * channels]
    int size; 
    // 图像数据
    unsigned char* data; 
};
```

多帧图像结构体
```C++
struct FIBO_CV_Imgs_t {
    // 图像数量, 取值范围：[1, 32]
    int count; 
    // 图像数组
    FIBO_CV_Img_t* imgs; 
};
```

图像推理结果结构体
```C++
TODO：分类/检测/姿态/分割/OCR/超分

struct ResultCV_t {
    // 结果数量, 取值范围：[1, 32]  
    int count; 
    // 标签数组
    std::vector<std::string> labels; 
    // 得分数组
    float* scores; 
};
```

CV模型信息结构体
```C++
struct FIBO_CV_Model_t {    
    // 模型名称, 如CV_CLASSIFIER_RESNET50、NLP_SUMMARIZE_YOLOV5等
    std::string name; 
    // 模型类型，如yolov5、resnet50等，不一定要
    std::string type; 
    // 模型版本
    std::string version; 
    // 模型路径
    std::string path; 
    // 标签数组
    std::vector<std::string> labels; 
    // 模型meta配置，预留字段
    std::string meta_config; 
};
```


CV参数结构体
```C++
// TODO：分类/检测/姿态/分割/OCR/超分
struct CVAPI_Param {
    // 置信度阈值，取值范围：[0, 1]
    float threshold = 0.5; 
}

// 检测类算法参数结构体
struct CVAPI_Detect_Param {
    // 置信度阈值，取值范围：[0, 1]
    float threshold = 0.5; 
    // 最多返回的结果数，取值范围：[1, 1000]
    int top_k = 50; 
    // 保留的结果数，取值范围：[1, 1000]
    int keep_top_k = 50; 
    // nms阈值，取值范围：[0, 1]
    float nms_threshold = 0.45; 
    // 类别数，取值范围：[1, 10000]
    int num_classes = 80; 
    // 输入高度，取值范围：[1, 4096]
    int input_width = 416; 
    // 输入宽度，取值范围：[1, 4096]
    int input_height = 416; 
    // 标签数组
    std::vector<std::string> labels; 
    // 模型meta配置，预留字段
    std::string meta_config; 
}

// 分类类算法参数结构体
struct CVAPI_Classifier_Param {
    // 置信度阈值，取值范围：[0, 1]
    float threshold = 0.5; 
    // 最多返回的结果数，取值范围：[1, 1000]
    int top_k = 50; 
    // 输入高度，取值范围：[1, 4096]
    int input_width = 416; 
    // 输入宽度，取值范围：[1, 4096]
    int input_height = 416; 
    // 标签数组
    std::vector<std::string> labels; 
    // 模型meta配置，预留字段
    std::string meta_config; 
}

// 其他算法参数结构体待定

```


3）NLP类结构体
初始化参数结构体：
```C++
struct FIBO_NLP_Param {
    // 置信度阈值，取值范围：[0, 1]
    float top_p = 0.95; 
    // 最多返回的结果数，取值范围：[1, 1000]
    int top_k = 50;     
    // 最大长度，取值范围：[1, 1024]
    int max_length = 512;  
    // beam search宽度，取值范围：[1, 100]
    int num_beams = 1;      
    // 是否采样
    bool do_sample = false;     
    // beam search组数，取值范围：[1, 100]
    int num_return_sequences = 1;   
    // beam search返回结果数，取值范围：[1, 100]
    bool output_scores = true;      
    // 重复惩罚，取值范围：[0, 1]
    float repetition_penalty = 1.0; 
    // 最大新token数，取值范围：[1, 1024]
    int max_new_tokens = 256;      
    // 多样性惩罚，取值范围：[0, 1]
    float diversity_penalty = 1.5; 
    // 长度惩罚，取值范围：[0, 1]
    float length_penalty = 1.0;     
    // 结束符token id，取值范围：[-1, 1000000]
    int eos_token_id = -1;  
    // 屏蔽词id列表，非必须，可传入空列表
    std::vector<int> bad_words_ids;  
    // 是否返回logprobs
    bool logprobs = false;
    // 返回logprobs的数量
    int top_logprobs = 0;
    // 前缀允许token函数，非必须，可传入nullptr
    std::function<std::vector<int>(const std::vector<int>&)> prefix_allowed_tokens_fn; 
};



```

推理输入结构体：
```C++
struct FIBO_NLP_Data_t {
    // 文本，取值范围：任意字符串
    std::string text; 
    // 语言，取值范围：zh/en/...
    std::string language; 
    // Token，取值范围：任意字符串
    std::string token; 
    // URL，取值范围：任意字符串
    std::string url; 
    // 文件路径
    std::string file_path; 
    // 音频路径
    std::string audio_path; 
    // 语音路径
    std::string speech_path; 
    // 响应
    std::string response; 
};
```

4）Audio类结构体
```C++
struct FIBO_Audio_t {
    // 音频采样率，取值范围：[8000, 48000]
    int32_t audio_sample_rate; 
    // 音频通道数，取值范围：[1, 2]
    int32_t audio_channel; 
    // 音频存储格式，取值范围：[FIBO_AUDIO_FORMAT_PCM, FIBO_AUDIO_FORMAT_MP3, FIBO_AUDIO_FORMAT_AAC, FIBO_AUDIO_FORMAT_WAV]     
    int32_t audio_format; 
    // 音频chunk 大小，取值范围：[1, 4096]
    int32_t audio_buffer_bytes_size; 
    // 音频时长，单位：秒，取值范围：[0, 1000]
    int32_t audio_duration; 
    // 音频原始数据引用
    std::vector<uint8_t> &audio_buffer;
};

struct FIBO_Result_Audio_Transalte_t {
    // 翻译token            
    std::vector<std::string> speech_tokens; 
    // 翻译开始时间，单位：微妙，取值范围：[0, 1000000]
    int64_t speech_start; 
    // 翻译结束时间，单位：微妙，取值范围：[0, 1000000]
    int64_t speech_end; 
    // 翻译置信度，取值范围：[0, 1]
    float speech_confence; 
};
```

5）多模态类接口
```C++
struct FIBO_MultiModal_t {
    // 图像数据
    FIBO_CV_Img_t img; 
    // 文本数据
    std::string text; 
    // 语音数据
    FIBO_Audio_t audio; 
};
```

6） 硬件资源接口
```C++

struct FIBO_Resource_CPU_t {
    // CPU核心数
    int core_num; 
    // CPU频率，单位：MHz
    int frequency; 
    // CPU使用率，取值范围：[0, 1]
    float usage; 
};

struct FIBO_Resource_System_Memory_t {
    // 内存总量，单位：MB
    int total; 
    // 内存使用量，单位：MB
    int used; 
    // 内存剩余量，单位：MB
    int free; 
    // 内存使用率，取值范围：[0, 1]
    float usage; 
};

struct FIBO_Resource_NPU_Memory_t {
    // NPU内存总量，单位：MB
    int total; 
    // NPU内存使用量，单位：MB
    int used; 
    // NPU内存剩余量，单位：MB
    int free; 
    // NPU内存使用率，取值范围：[0, 1]
    float usage; 
};

struct FIBO_Resource_GPU_t {
    // GPU名称
    std::string name; 
    // GPU核心数
    int core_num; 
    // GPU频率，单位：MHz
    int frequency; 
    // GPU内存总量，单位：MB
    int total; 
    // GPU内存使用量，单位：MB
    int used; 
    // GPU内存剩余量，单位：MB
    int free; 
    // GPU使用率，取值范围：[0, 1]
    float usage; 
};

struct FIBO_Resource_NPU_t {
    // NPU名称
    std::string name; 
    // NPU核心数
    int core_num; 
    // NPU频率，单位：MHz
    int frequency; 
    // NPU内存信息
    FIBO_Resource_NPU_Memory_t memory; 
};

struct FIBO_Resource_DSP_t {
    // DSP名称
    std::string name; 
    // DSP核心数
    int core_num; 
    // DSP频率，单位：MHz
    int frequency; 
    // DSP使用率，取值范围：[0, 1]
    float usage; 
};

struct FIBO_Resource_Memory_t {
    // 系统内存信息
    FIBO_Resource_System_Memory_t system; 
    // NPU内存信息
    FIBO_Resource_NPU_Memory_t npu; 
};

struct FIBO_Resource_System_t {
    // 系统名称
    std::string name; 
    // 系统版本
    std::string version; 
};

struct FIBO_Resource_Info_t {
    // CPU信息
    FIBO_Resource_CPU_t cpu; 
    // 内存信息
    FIBO_Resource_Memory_t memory; 
    // GPU信息
    FIBO_Resource_GPU_t gpu; 
    // NPU信息
    FIBO_Resource_NPU_t npu; 
    // 系统信息
    FIBO_Resource_System_t system; 
};
```

7）算法指标
```C++
// 性能指标结构体
struct FIBO_Algorithm_Performance_t {
    // 算法名称
    std::string name; 
    // 算法类型
    std::string type; 
    // 算法版本
    std::string version; 
    // 算法时延，单位：微秒，取值范围：[0, 1000000]
    float latency; 
    // 算法内存占用，单位：KB，取值范围：[0, 1000000]
    int memory_usage; 
    // 算法吞吐量，单位：帧/秒，取值范围：[0, 1000000]
    int throughput; 
    // 大模型指标，token速度，单位：token/秒，取值范围：[0, 1000000]
    int token_speed; 
};
```



### 接口说明
1）通用接口
一般为程序开始时调用，主要用于激活license、获取一些通用新鲜、设置日志等功能，程序运行中不调用。

- 初始化函数
```C++
// 初始化函数
// 功能说明：激活license，目前暂时不使用
// 参数说明：
// app_id：应用ID
// app_key：应用Key
// app_secret：应用Secret
// license：license文件路径
// 返回值：0表示成功，其他表示失败
int ActivateLicense(std::string app_id, std::string app_key, std::string app_secret, std::string license);

```


- 错误码函数
```C++
// 错误码函数
// 功能说明：获取错误码
// 参数说明：
// 返回值：错误码描述结构体
FiboCommon_ErrorCode GetErrorCode();

```

- 版本号函数
```C++
// 版本号函数
// 功能说明：获取版本号
// 参数说明：
// 返回值：版本号
FIBO_Version_t GetVersion();
```

- 日志函数
```C++
// 日志函数
// 功能说明：设置日志
// 参数说明：
// log：日志结构体，必须传入，非必须每个字段都设置
// 返回值：0表示成功，其他表示失败
int SetLog(FIBO_Log_t log);
```

```
// 获取日志函数
// 功能说明：获取日志
// 参数说明：
// log：日志结构体，必须传入，非必须每个字段都设置
// 返回值：0表示成功，其他表示失败
int GetLog(FIBO_Log_t &log);
```



2）CV类接口：

适用范围：
纯CV算法使用，比如分类、检测、分割等。  
目前支持算法：
- 分类算法
支持网络：resnet50
- 检测算法
支持网络：yolov5



调用流程示例代码：
```C++
// 回调函数
void InferAsyncCallback(std::string& tag, int status, std::string& response)
{
    std::cout << "InferAsyncCallback: tag=" << tag << ", status=" << status << ", response=" << response << std::endl;
}

void test_cv() {
    const std::string model_path = "path/to/model";
    const std::string config_path = "path/to/config";
    const std::string img_path = "path/to/img";
    // 1. 初始化
    CVAPI cvapi;
    int ret = cvapi.Init(config_path, model_path);
    if (ret != 0) {
        std::cerr << "Init failed, error code: " << ret << std::endl;
        return;
    }

    // 获取模型相关的信息
    FIBO_CV_Model_t model_info = cvapi.GetModelInfo();
    std::cout << "Model name: " << model_info.name << std::endl;
    std::cout << "Model type: " << model_info.type << std::endl;
    std::cout << "Model version: " << model_info.version << std::endl;
    std::cout << "Model path: " << model_info.path << std::endl;

    // 获取标签
    std::vector<std::string> labels = cvapi.GetLabels();
    std::cout << "Labels: " << labels.size() << std::endl;
    for (const auto& label : labels) {
        std::cout << label << std::endl;
    }

    // 2. 同步推理
    // 2.1 使用FIBO_CV_Img_t结构体
    FIBO_CV_Img_t img;
    cv::Mat mat = cv::imread(img_path);
    img.data = mat.data;
    img.width = mat.cols;
    img.height = mat.rows;
    img.channels = mat.channels();
    img.format = FIBO_CV_FORMAT_RGB;
    img.size = mat.total() * mat.elemSize();

    ResultCV_t result;
    ret = cvapi.InferSync(img, result);
    if (ret != 0) {
        std::cerr << "InferSync failed, error code: " << ret << std::endl;
        return;
    }

    // 2.2 使用图片文件夹路径接口
    ret = cvapi.InferSync(img_path, result);
    if (ret != 0) {
        std::cerr << "InferSync failed, error code: " << ret << std::endl;
        return;
    }

    // 2.3 使用异步接口
    ret = cvapi.InferAsync(img, InferAsyncCallback);
    if (ret != 0) {
        std::cerr << "InferAsync failed, error code: " << ret << std::endl;
        return;
    }
    cvapi.WaitTaskDone();

    // 3. 释放
    cvapi.Release();
}
```

详细接口说明：
```C++
class CVAPI {
public:
    // 初始化函数
    // 功能说明：初始化模型，主要为初始化模型路径，加载模型
    // 参数说明：
    //  config：配置文件路径，一般为json格式文件，后续制定字段，非必须，可传入""
    //  model_path：模型路径，必须传入
    // 返回值：0表示成功，其他表示失败
    int Init(std::string config, std::string model_path);

    // 初始化函数
    // 功能说明：初始化模型，主要为初始化模型路径，加载模型
    // 参数说明：
    //  config：配置文件路径，一般为json格式文件，后续制定字段，非必须，可传入""
    //  parames：参数结构体，非必须，可传入默认参数
    //  model_path：模型路径，必须传入
    // 返回值：0表示成功，其他表示失败
    int Init(std::string config, const CVAPI_Param &parames, std::string model_path);

    // 获取默认参数函数
    // 功能说明：获取默认参数
    // 参数说明：
    // 返回值：默认参数结构体
    CVAPI_Param GetDefaultParam();

    // 释放函数
    // 功能说明：释放模型
    // 参数说明：
    // 返回值：0表示成功，其他表示失败
    int Release();

    // 同步推理函数
    // 功能说明：同步推理，同步执行，需要等待推理完成
    // 参数说明：
    //  img：图像结构体，为解码后图片数据，必须传入
    //  result：推理结果，空变量，推理完成后，将推理结果填充到result中
    // 返回值：0表示成功，其他表示失败
    int InferSync(const FIBO_CV_Img_t& img, ResultCV_t& result);

    // 同步推理函数
    // 功能说明：同步推理，同步执行，需要等待推理完成
    // 参数说明：
    //  img：图像文件路径，为解码前图片文件，目前支持jpg/png/等opencv可支持的格式，必须传入
    //  result：推理结果，空变量，推理完成后，将推理结果填充到result中
    // 返回值：0表示成功，其他表示失败
    int InferSync(const std::string& img, ResultCV_t& result);


    // 异步推理回调函数
    // 功能说明：异步推理回调函数，异步执行，需要配合回调函数和WaitTaskDone函数使用
    // 参数说明：
    //  tag：任务标签，非必须，从中获取值
    //  status：任务状态，非必须，从中获取值
    //  response：任务响应，非必须，从中获取值
    // 返回值：0表示成功，其他表示失败
    int InferAsyncCallback(std::string& tag, int status, std::string& response);

    // 异步推理函数
    // 功能说明：异步推理，异步执行，需要配合回调函数和WaitTaskDone函数使用 
    // 参数说明：
    //  img：图像结构体，为解码后图片数据，必须传入
    //  cb：回调函数，非必须，��传入nullptr
    //  timeout：超时时间，单位：毫秒，非必须，可传入0
    //  tag：任务标签，非必须
    // 返回值：0表示成功，其他表示失败
    int InferAsync(const FIBO_CV_Img_t& img, InferAsyncCallback cb = nullptr, int timeout = 0, std::string tag = "");

    // 异步推理函数
    // 功能说明：异步推理，异步执行，需要配合回调函数和WaitTaskDone函数使用 
    // 参数说明：
    //  img：图像文件路径，为解码前图片文件，目前支持jpg/png/等opencv可支持的格式，必须传入
    //  cb：回调函数，非必须，可传入nullptr
    //  timeout：超时时间，单位：毫秒，非必须，可传入0
    //  tag：任务标签，非必须
    // 返回值：0表示成功，其他表示失败
    int InferAsync(const std::string& img, InferAsyncCallback cb = nullptr, int timeout = 0, std::string tag = "");

    // 等待推理完成函数
    // 功能说明：等待推理完成，等待异步推理完成
    // 参数说明：
    // tag：任务标签，非必须，可传入""
    // 返回值：0表示成功，其他表示失败
    int WaitTaskDone(std::string& tag="");

    // 获取模型类型函数
    // 参数说明：
    // 返回值：模型类型，比如classify、detect、segment等
    std::string GetModelType();

    // 获取标签函数
    // 参数说明：
    // 返回值：标签数组
    std::vector<std::string> GetLabels();

    // 获取模型信息函数
    // 参数说明：
    // 返回值：模型信息结构体
    FIBO_CV_Model_t GetModelInfo();

private:
};

```


3) NLP类接口：

适用范围，
纯NLP算法使用，比如文本生成、文本摘要、文本翻译等。

目前支持算法：
- 文本生成算法
支持模型：QWen2/LLama3-8B



调用流程示例代码：

```C++
// 回调函数
void InferAsyncCallback(std::string& tag, int status, std::string& response)
{
    std::cout << "InferAsyncCallback: tag=" << tag << ", status=" << status << ", response=" << response << std::endl;
    // push data to display，注意非阻塞操作
}

void test_nlp() {
    const std::string model_path = "path/to/model";
    const std::string config_path = "path/to/config";
    const std::string text = "Hello, world!";

    // 1. 初始化
    NLPAPI nlpapi;
    int ret = nlpapi.Init(config_path, model_path);
    if (ret != 0) {
        std::cerr << "Init failed, error code: " << ret << std::endl;
        return;
    }

    // 2. 同步推理
    ret = nlpapi.Generate(text, InferAsyncCallback);
    if (ret != 0) {
        std::cerr << "Generate failed, error code: " << ret << std::endl;
        return;
    }

    // 3. 等待推理完成
    ret = nlpapi.WaitTaskDone();
    if (ret != 0) {
        std::cerr << "WaitTaskDone failed, error code: " << ret << std::endl;
        return;
    }

    // 3. 释放
    nlpapi.Release();
}
```


详细接口说明：

```C++
class NLPAPI {
public:
    // 初始化参数
    // 功能说明：初始化模型，主要为初始化模型路径，加载模型
    // 参数说明：
    //  nlp_param：NLP参数结构体，非必须，可传入默认参数
    //  model_path：模型路径，必须传入
    // 返回值：0表示成功，其他表示失败
    int Init(const FIBO_NLP_Param& nlp_param, std::string& model_path);

    // 初始化参数
    // 功能说明：初始化模型，主要为初始化模型路径，加载模型
    // 参数说明：
    //  config：配置文件路径，json格式文件，非必须，可传入""
    //  model_path：模型路径，必须传入
    // 返回值：0表示成功，其他表示失败
    int Init(std::string &config, std::string& model_path);

    // 释放函数
    // 功能说明：释放模型
    // 参数说明：
    // 返回值：0表示成功，其他表示失败
    int Release();

    // 回调函数
    // 功能说明：异步推理回调函数，异步执行，需要配合回调函数和WaitTaskDone函数使用
    // 参数说明：
    //  tag：任务标签，非必须，从中获取值
    //  status：任务状态，非必须，从中获取值
    //  response：任务响应，非必须，从中获取值
    // 返回值：0表示成功，其他表示失败
    int InferAsyncCallback(std::string& tag, int status, std::string& response);


    // 文本生成
    // 功能说明：根据输入的text进行文本生成，异步执行，需要配合回调函数和WaitTaskDone函数使用
    // 参数说明：
    //  text：输入文本, 必须传入
    //  cb：回调函数, 非必须，可传入nullptr
    //  timeout：超时时间，单位：毫秒，非必须，可传入0
    //  tag：任务标签，非必须
    // 返回值：0表示成功，其他表示失败
    int Generate(std::vector<int>& text, InferAsyncCallback cb, int timeout = 0, std::string tag = "");

    // 获取模型信息
    // 功能说明：获取模型信息
    // 参数说明：
    // 返回值：模型信息结构体
    FIBO_NLP_Model_t GetModelInfo();

    // 等待任务完成
    // 功能说明：等待推理完成，等待异步推理完成
    // 参数说明：
    //  tag：任务标签，非必须，可传入""
    // 返回值：0表示成功，其他表示失败
    int WaitTaskDone(std::string& tag="");

private:
};


```

4）Audio类接口：

适用范围：
纯Audio算法使用，比如音频翻译、音频转文字等。

目前支持算法：
- 音频翻译算法
支持模型：whisper-large-v3
- 音频转文字算法
支持模型：whisper-large-v3


调用流程示例代码：

```C++
// 回调函数
void InferAsyncCallback(std::string& tag, int status, std::string& response)
{
    std::cout << "InferAsyncCallback: tag=" << tag << ", status=" << status << ", response=" << response << std::endl;
}


void test_audio() {
    const std::string model_path = "path/to/model";
    const std::string config_path = "path/to/config";
    const std::string wav_audio_path = "path/to/wav/audio";

    // 1. 初始化
    AudioAPI audioapi;
    int ret = audioapi.Init(config_path, model_path);

    // 2. 异步推理
    ret = audioapi.Transcribe(wav_audio_path, InferAsyncCallback);
    if (ret != 0) {
        std::cerr << "transcribe failed, error code: " << ret << std::endl;
        return;
    }

    // 2.2 使用audio的chunk接口
    // 封装chunk接口，将音频文件分片，每次传入10s音频数据，分片处理
    ret = audioapi.Transcribe(wav_audio_path, InferAsyncCallback);
    if (ret != 0) {
        std::cerr << "transcribe failed, error code: " << ret << std::endl;
        return;
    }

    // 3. 等待推理完成
    ret = audioapi.WaitTaskDone();  
    if (ret != 0) {
        std::cerr << "WaitTaskDone failed, error code: " << ret << std::endl;
        return;
    }

    // 4. 释放
    audioapi.Release();
}
```

详细接口说明：

```C++
class AudioAPI {
public:
    // 初始化参数
    // 功能说明：初始化模型，主要为初始化模型路径，加载模型
    // 参数说明：
    // config：配置文件路径，json格式文件，非必须，可传入""，目前不可设置
    // model_path：模型路径，必须传入
    // 返回值：0表示成功，其他表示失败
    int Init(std::string &config, std::string& model_path);


    // 释放函数
    // 功能说明：释放模型
    // 参数说明：
    // 返回值：0表示成功，其他表示失败
    int Release();

    // 回调函数
    // 功能说明：异步推理回调函数，异步执行，需要配合回调函数和WaitTaskDone函数使用
    // 参数说明：
    //  tag：任务标签，非必须，从中获取值
    //  status：任务状态，非必须，从中获取值
    //  response：任务响应，非必须，从中获取值
    // 返回值：0表示成功，其他表示失败
    int InferAsyncCallback(std::string& tag, int status, FIBO_Result_Audio_Transcribe_t &response, std::string metrice_str);


    // 音频翻译
    // 功能说明：根据输入的wav音频文件进行音频翻译，异步执行，需要配合回调函数和WaitTaskDone函数使用
    // 参数说明：
    //  audio 可以传wav 文件路径或音频原始数组, 传文件则在transcribe 加载音频数据,传音频原始数据则由用户给定
    //  result 是转录的结果
    // 返回值：错误码
    int Transcribe(FIBO_Audio_t audio , InferAsyncCallback cb, int timeout = 0, std::string tag = "");

    // 获取模型信息
    // 功能说明：获取模型信息
    // 参数说明：
    // 返回值：模型信息结构体
    FIBO_Algorithm_t GetModelInfo();

    // 等待任务完成
    // 参数说明：
    //  tag：任务标签，非必须，可传入""
    // 返回值：0表示成功，其他表示失败
    int WaitTaskDone(std::string& tag="");

private:
};

```


5）多模态接口：

适用范围：
多模态算法使用，比如图像VQA

目前支持算法：
- 图像生成算法
支持模型：LLama-Interleave-QWen-7B


调用流程示例代码：

```C++
void test_multimodal() {
    const std::string model_path = "path/to/model";
    const std::string config_path = "path/to/config";
    const std::string img_path = "path/to/img";
    const std::string text = "Hello, world!";

    // 1. 初始化
    MultimodalAPI multimodalapi;

    int ret = multimodalapi.Init(config_path, model_path);
    if (ret != 0) {
        std::cerr << "Init failed, error code: " << ret << std::endl;
        return;
    }

    // 2. 同步推理
    ret = multimodalapi.Generate(text, img_path, InferAsyncCallback);
    if (ret != 0) {
        std::cerr << "Generate failed, error code: " << ret << std::endl;
        return;
    }

    // 3. 等待推理完成
    ret = multimodalapi.WaitTaskDone();
    if (ret != 0) {
        std::cerr << "WaitTaskDone failed, error code: " << ret << std::endl;
        return;
    }   

    // 4. 释放
    multimodalapi.Release();
}

```



详细接口说明：


```C++
class MultimodalAPI {
public:
    // 初始化参数
    // 功能说明：初始化模型，主要为初始化模型路径，加载模型
    // 参数说明：
    //  nlp_param：NLP参数结构体，非必须，可传入默认参数
    //  model_path：模型路径，必须传入
    // 返回值：0表示成功，其他表示失败
    int Init(const FIBO_NLP_Param& nlp_param, std::string& model_path);

    // 初始化参数
    // 功能说明：初始化模型，主要为初始化模型路径，加载模型
    // 参数说明：
    //  config：配置文件路径，json格式文件，非必须，可传入""
    //  model_path：模型路径，必须传入
    // 返回值：0表示成功，其他表示失败
    int Init(std::string &config, std::string& model_path);


    // 释放函数
    // 参数说明：
    // 返回值：0表示成功，其他表示失败
    int Release();

    // TODO: 补充多张图片，再运行Generate接口
    // void SetImgList(std::vector<std::string>& img_list);

    // 回调函数
    // 功能说明：异步推理回调函数，异步执行，需要配合回调函数和WaitTaskDone函数使用
    // 参数说明：
    // tag：任务标签，非必须，从中获取值
    // status：任务状态，非必须，从中获取值
    // response：任务响应，非必须，从中获取值
    // 返回值：0表示成功，其他表示失败
    void InferAsyncCallback(std::string& tag, int status, std::string& response);

    // 异步生成接口
    // 功能说明：根据输入的text和img进行图像生成，异步执行，需要配合回调函数和WaitTaskDone函数使用
    // 参数说明：
    //  text：输入文本，必须传入
    //  img_path：输入图像路径，非必须，可传入""
    //  cb：回调函数，非必须，可传入nullptr
    //  timeout：超时时间，单位：毫秒，非必须，可传入0
    //  tag：任务标签，非必须
    // 返回值：0表示成功，其他表示失败
    int Generate(std::string& text, std::string& img_path, InferAsyncCallback cb, int timeout = 0, std::string tag = "");

    int Generate(std::string& text, std::vector<std::string>& img_paths, InferAsyncCallback cb, int timeout = 0, std::string tag = "");

    // 文本生成
    // 功能说明：根据输入的text和img进行图像生成，异步执行，需要配合回调函数和WaitTaskDone函数使用
    // 参数说明：
    //  text：输入文本，必须传入
    //  img：输入图像结构体，非必须，可传入""
    //  cb：回调函数，非必须，可传入nullptr
    //  timeout：超时时间，单位：毫秒，非必须，可传入0
    //  tag：任务标签，非必须
    // 返回值：0表示成功，其他表示失败
    int Generate(std::string& text, const FIBO_CV_Img_t& img, InferAsyncCallback cb, int timeout = 0, std::string tag = "");

    // 获取模型信息
    // 参数说明：
    // 返回值：模型信息结构体
    FIBO_Algorithm_t GetModelInfo();

    // 等待任务完成
    // 参数说明：
    //  tag：任务标签，非必须，可传入"" 
    // 返回值：0表示成功，其他表示失败
    int WaitTaskDone(std::string& tag="");

private:
};
```

6）硬件资源接口

适用范围：

- 获取CPU使用率
- 获取GPU使用率
- 获取DSP使用率
- 获取NPU使用率
- 获取内存使用率
- 获取显存使用率

调用流程示例代码：

```C++
void test_hardware() {
    // 1. 初始化
    HardwareAPI hardwareapi;

    // 2. 获取CPU使用率
    FIBO_Resource_CPU_t cpu_usage = hardwareapi.GetCpuUsage();
    std::cout << "CPU usage: " << cpu_usage.usage << "%" << std::endl;

    // 3. 获取GPU使用率
    FIBO_Resource_GPU_t gpu_usage = hardwareapi.GetGpuUsage();
    std::cout << "GPU usage: " << gpu_usage.usage << "%" << std::endl;

    // 4. 获取DSP使用率
    FIBO_Resource_DSP_t dsp_usage = hardwareapi.GetDspUsage();
    std::cout << "DSP usage: " << dsp_usage.usage << "%" << std::endl;

    // 5. 获取NPU使用率
    FIBO_Resource_NPU_t npu_usage = hardwareapi.GetNpuUsage();
    std::cout << "NPU usage: " << npu_usage.usage << "%" << std::endl;

    // 6. 获取内存使用率
    FIBO_Resource_Memory_t memory_usage = hardwareapi.GetMemoryUsage();
    std::cout << "Memory usage: " << memory_usage.usage << "%" << std::endl;

    // 7. 获取显存使用率
    FIBO_Resource_Memory_t memory_usage = hardwareapi.GetMemoryUsage();
    std::cout << "Memory usage: " << memory_usage.usage << "%" << std::endl;

    // 8. 释放
    hardwareapi.Release();
}
```


详细接口说明：

```C++
class HardwareAPI {
public:
    // 初始化参数
    // 功能说明：初始化模型，主要为初始化模型路径，加载模型
    // 参数说明：
    //  config：配置文件路径，json格式文件，非必须，可传入""
    // 返回值：0表示成功，其他表示失败
    int Init(std::string &config);

    // 获取CPU使用率
    // 参数说明：
    // 返回值：CPU资源结构体
    int GetCpuUsage(FIBO_Resource_CPU_t& cpu_usage);

    // 获取GPU使用率
    // 参数说明：
    // 返回值：GPU资源结构体
    int GetGpuUsage(FIBO_Resource_GPU_t& gpu_usage);

    // 获取DSP使用率
    // 参数说明：
    // 返回值：DSP资源结构体
    int GetDspUsage(FIBO_Resource_DSP_t& dsp_usage);

    // 获取NPU使用率
    // 参数说明：
    // 返回值：NPU资源结构体
    int GetNpuUsage(FIBO_Resource_NPU_t& npu_usage);

    // 获取内存使用率
    // 参数说明：
    // 返回值：内存资源结构体
    int GetMemoryUsage(FIBO_Resource_Memory_t& memory_usage);

    // 获取显存使用率
    // 参数说明：
    // 返回值：显存资源结构体
    int GetMemoryUsage(FIBO_Resource_Memory_t& memory_usage);

    // 释放函数
    // 参数说明：
    // 返回值：0表示成功，其他表示失败
    int Release();

private:
};
```

7）算法指标接口

适用范围：

算法指标包含：
- 时延
- 吞吐量


调用流程示例代码：

```C++
void test_metrics() {
    // 1. 初始化
    MetricsAPI metricsapi;

    // 2. 模型推理时延

    // 5. 释放
    metricsapi.Release();
}
```


详细接口说明：


```C++
class MetricsAPI {
public:
    // 初始化参数
    // 功能说明：初始化模型，主要为初始化模型路径，加载模型
    // 参数说明：
    //  config：配置文件路径，json格式文件，非必须，可传入""
    //  model_path：模型路径，必须传入
    // 返回值：0表示成功，其他表示失败
    int Init(std::string &config, std::string& model_path);

    // 释放函数
    // 参数说明：
    // 返回值：0表示成功，其他表示失败
    int Release();


    // 模型推理时延
    // 参数说明：
    //  warmup_time：预热时间，单位：毫秒
    //  infer_time：模型推理时延，单位：毫秒
    //  batch_size：推理batch大小
    //  num_samples：推理样本数
    // 返回值：模型推理时延，单位：毫秒
    int Latency(int warmup_time, int infer_time, int batch_size, int num_samples, float& latency);

    // 模型推理吞吐量
    // 参数说明：
    //  warmup_time：预热时间，单位：毫秒
    //  infer_time：模型推理时延，单位：毫秒
    //  batch_size：推理batch大小
    //  num_samples：推理样本数
    // 返回值：模型推理吞吐量，单位：样本/秒
    int Throughput(int warmup_time, int infer_time, int batch_size, int num_samples, float& throughput);

    // 大模型推理速度
    // 参数说明：
    //  warmup_time：预热时间，单位：毫秒
    //  infer_time：模型推理时延，单位：毫秒
    //  batch_size：推理batch大小
    //  num_samples：推理样本数
    // 返回值：模型推理token数/秒
    int Speed(int warmup_time, int infer_time, int batch_size, int num_samples, float& speed);


private:    
};
```



## Python接口










