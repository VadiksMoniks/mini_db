#ifndef ROW
#define ROW
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "Value.hpp"
#include "ValueBase.hpp"
#include <stdexcept>

class Row//это класс отдельной строки записи, он будет состоять из отдельных записей типа Value
{
    private:
        std::vector<std::unique_ptr<ValueBase>> row_data;
        bool is_deleted = false;

    public:
    
        Row() = default;
        
        // Копирование запрещено (из-за unique_ptr)
        Row(const Row&) = delete;
        Row& operator=(const Row&) = delete;

        // Перемещение разрешено
        Row(Row&&) = default;
        Row& operator=(Row&&) = default;

        ~Row() = default;
        
       void add_to_row(const std::string& data_type, const std::string& value) {
          //  std::cout << "Adding value: '" << value << "' with type: '" << data_type << "'\n";

            if (data_type == "int")
                row_data.push_back(std::make_unique<Value<int>>(std::stoi(value)));
            else if (data_type == "double")
                row_data.push_back(std::make_unique<Value<double>>(std::stod(value)));
            else if (data_type == "string")
                row_data.push_back(std::make_unique<Value<std::string>>(value));
            else {
                std::cerr << "ERROR: Unknown data type: '" << data_type << "'\n";
                throw std::runtime_error("Unsupported data type");
            }

        }

        void update_value(int index, std::string data_type, const::std::string& value)
        {
            if (data_type == "int")
                row_data[index] = std::make_unique<Value<int>>(std::stoi(value));
            else if (data_type == "double")
                row_data[index] = std::make_unique<Value<double>>(std::stod(value));
            else if (data_type == "string")
                row_data[index] = std::make_unique<Value<std::string>>(value);
            else {
                std::cerr << "ERROR: Unknown data type: '" << data_type << "'\n";
                throw std::runtime_error("Unsupported data type");
            }
        }

        inline int getRowSize() const
        {
            return static_cast<int>(row_data.size());
        }

        inline const std::vector<std::unique_ptr<ValueBase>>& getRowData() const {
            return row_data;
        }

        inline bool isDeleted() const{
            return is_deleted;
        }

        inline void setAsDeleted(){
            this->is_deleted = true;
        }
};
#endif