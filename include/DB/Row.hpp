#ifndef ROW
#define ROW
#include <vector>
#include <memory>
#include <functional>
#include "Value.hpp"
#include "ValueBase.hpp"
#include <stdexcept>
#include "Validator.hpp"

/**
 * @brief класс строки данных
 * @param row_data вектор строки данных
 * @param is_deleted флаг, указывающий, что строка удалена, и ее не нужно записывать обратно в файл
 * @param factories - фабрика, которая хранит в себе лямбда функции - способы преобразования данных, чтобы потом записать их в вектор
 */
class Row//это класс отдельной строки записи, он будет состоять из отдельных записей типа Value
{
    std::vector<std::unique_ptr<ValueBase>> row_data;
    using FactoryFunc = std::function<std::unique_ptr<ValueBase>(const std::string&)>;
    std::unordered_map<std::string, FactoryFunc> factories;
    bool is_deleted = false;

    public:
    
        Row();
        
        // Копирование запрещено (из-за unique_ptr)
        Row(const Row&) = delete;
        Row& operator=(const Row&) = delete;

        // Перемещение разрешено
        Row(Row&&) = default;
        Row& operator=(Row&&) = default;

        ~Row() = default;

        void add_to_row(const std::string& data_type, const std::string& value);
        void update_value(int index, std::string data_type, const::std::string& value); 
        size_t getRowSize() const;
        const std::vector<std::unique_ptr<ValueBase>>& getRowData() const;
        bool isDeleted() const;
        void setAsDeleted();
};
#endif