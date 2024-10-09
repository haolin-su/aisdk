#ifndef NLP_LLAMA_H_
#define NLP_LLAMA_H_

#include "core/graph/graph.h"

namespace aisdk {

class NLPLLAMA : public Graph
{
public:
    NLPLLAMA();
    ~NLPLLAMA();

    int Init();
    int Finalize();
};

class NLPLLAMACreator : public GraphCreator
{
public:
    NLPLLAMACreator() : GraphCreator("NLPLLAMA") {}
    ~NLPLLAMACreator() {}

    std::shared_ptr<Graph> CreateGraph(const std::string& name, const std::string& config_file, ModelPtr& model)
    {
        return std::make_shared<NLPLLAMA>(name, config_file, model);
    }
};

} // namespace aisdk

#endif  // NLP_LLAMA_H_