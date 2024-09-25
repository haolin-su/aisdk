/******************************************************************************
  Copyright (C), 2024, Shenzhen G&T Industrial Development Co., Ltd

  File:      model_nlp.h

  Author:  leo.li         
  Version: 1.0        
  Date:  2024.09
  
  Description:   NLP model APIs 

** History:
**Author (core ID)                Date          Number     Description of Changes
**-----------------------------------------------------------------------------
** leo.li                      09-Sep-2024     LIBtt54238   Init 
** -----------------------------------------------------------------------------
******************************************************************************/

#ifndef FIBO_MODEL_NLP_H_
#define FIBO_MODEL_NLP_H_

#include <string>
#include <vector>

namespace aisdk {

typedef struct {
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
    // std::function<std::vector<int>(const std::vector<int>&)> prefix_allowed_tokens_fn; 
    } FIBO_NLP_Param;

typedef struct {
    // 模型名称
    std::string model_name;
    // 模型版本
    std::string model_version;
    // 模型描述
    std::string model_description;
    // 模型路径
    std::string path; 
    // 模型md5
    std::string md5; 
    // 模型参数
    FIBO_NLP_Param param;
} FIBO_NLP_Model;

}  // namespace aisdk

#endif  // FIBO_MODEL_NLP_H_