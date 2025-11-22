#include "DB/Validator.hpp"

bool Validator::is_valid(const std::string& value, const std::string& type){
    auto it = datatypes.find(type);
    if(it != datatypes.end()){
        return it->second(value);
    }
    else{
        throw std::runtime_error("Unsupported data type " + type);
    }
}