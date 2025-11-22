#ifndef COMMAND
#define COMMAND
#include <functional>
//#include <any>
#include "Database.hpp"
#include "Table.hpp"
/**
 * @brief класс консольной комманды для работы программы
 */
class Command
{
    Database* db = nullptr;
    using FactoryFunc = std::function<void(const std::optional<std::string>&)>;
    std::unordered_map<std::string, FactoryFunc> commands;
    std::map<std::string, std::string> commads_description;
    /**
     * @brief разбивает по разделителю на передаваемые параметры в другие методы
     */
    std::vector<std::string> split_args(std::string const& line, const std::string& delimeter);
    
    public: 
        Command();

        ~Command();

        void run(std::string command, const std::optional<std::string>& params);


};

#endif