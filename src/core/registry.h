#ifndef FIBO_REGISTRY_H
#define FIBO_REGISTRY_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <typeinfo>

namespace aisdk {

class IRegistry {
public:
    virtual ~IRegistry() {}
    virtual void registerFactory(const std::string& name, std::function<std::unique_ptr<void>()> factory) = 0;
    virtual std::unique_ptr<void> createInstance(const std::string& name) = 0;
};    

int RegisterAll();

}

// 这里需要两级注册，比如node/tensor/model概念的注册，然后再是具体的子类实现注册
#define DEFINE_REGISTER(register_name) \
    class Registry##register_name : public IRegistry { \
    public: \
        Registry##register_name() { \
            factories.clear(); \
        } \
        ~Registry##register_name() { \
            factories.clear(); \
        } \
        void registerFactory(const std::string& name, std::function<std::unique_ptr<void>()> factory) override { \
            factories[name] = factory; \
        } \
        std::string getRegisterName() { \
            return #register_name; \
        } \
        std::function<std::unique_ptr<void>()> getFactory(const std::string& name) { \
            auto it = factories.find(name); \
            if (it != factories.end()) { \
                return it->second; \
            } \
            return nullptr; \
        } \
        std::unique_ptr<void> createInstance(const std::string& name) override { \
            auto it = factories.find(name); \
            if (it != factories.end()) { \
                return it->second(); \
            } \
            return nullptr; \
        } \
    private: \
        std::unordered_map<std::string, std::function<std::unique_ptr<void>()>> factories; \
    }; \
    static Registry##register_name registry;

#define REGISTER_CREATOR(register_name, creator_name) \
    static std::unique_ptr<void> createInstance##creator_name() { \
        return std::make_unique<creator_name>(); \
    } \
    static void register##creator_name() { \
        static IRegistry* registry = new Registry##register_name(); \
        registry->registerFactory(#creator_name, createInstance##creator_name); \
    } \
    static void initialize##creator_name() { \
        register##creator_name(); \
        registry.createInstance(#creator_name); \
    } \
    static void destroy##creator_name() { \
        delete registry; \
    } \
    static std::unique_ptr<void> create##creator_name() { \
        return registry.createInstance(#creator_name); \
    } \
    static void register##creator_name##Instance() { \
        static bool initialized = false; \
        if (!initialized) { \
            initialize##creator_name(); \
            initialized = true; \
        } \
    } \
    static void unregister##creator_name##Instance() { \
        static bool initialized = true; \
        if (initialized) { \
            destroy##creator_name(); \
            initialized = false; \
        } \
    } \
    static std::unique_ptr<void> get##creator_name##Instance() { \
        register##creator_name##Instance(); \
        return create##creator_name(); \
    } \


#endif // FIBO_REGISTRY_H