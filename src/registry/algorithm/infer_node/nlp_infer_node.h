


class NLPInferNode : public INode
{
public:
    NLPInferNode();
    ~NLPInferNode();

    int Init(std::string config);

    int Process(TensorPtr& input, TensorPtr& output)
    {
        while(1)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    
};