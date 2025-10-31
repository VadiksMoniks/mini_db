#ifndef VALIDATOR
#define VALIDATOR
#include <functional>
#include <string>
#include <stdexcept>
#include <regex>

class Validator
{
    private:
        using FactoryFunc = std::function<bool(const std::string&)>;
        
        inline static const std::unordered_map<std::string, FactoryFunc> datatypes = {
            {
                "int", [](const std::string& v) {
                    static const std::regex pattern(R"(^[-+]?\d+$)");
                    return std::regex_match(v, pattern);
                }
            },
            {
                "double", [](const std::string& v) {
                    static const std::regex pattern(R"(^[-+]?\d*\.?\d+$)");
                    return std::regex_match(v, pattern);
                }
            },
            {
                "string", [](const std::string& v){
                    static const std::regex pattern(R"(^[A-Za-zА-Яа-я0-9 _.,!?\'\"-]*$)");
                    return std::regex_match(v, pattern);
                }
            },
            {
                "char", [](const std::string& v){
                    static const std::regex pattern(R"(^[A-Za-zА-Яа-я0-9 _.,!?\'\"-]{1}$)");
                    return std::regex_match(v, pattern);
                }
            },
            {
                "bool", [](const std::string& v) {
                    static const std::regex pattern(R"(^(true|false|1|0)$)", std::regex_constants::icase);
                    return std::regex_match(v, pattern);
                }
            }
        };

    public:
        Validator() = delete;
        ~Validator() = delete;

        static bool is_valid(const std::string& value, const std::string& type){
            auto it = datatypes.find(type);
            if(it != datatypes.end()){
                return it->second(value);
            }
            else{
                throw std::runtime_error("Unsupported data type " + type);
            }
        }
};

#endif
