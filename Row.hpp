#ifndef ROW
#define ROW
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "Value.hpp"
#include "ValueBase.hpp"
#include <stdexcept>

/**
 * @brief класс строки данных
 * @param row_data вектор строки данных
 * @param is_deleted флаг, указывающий, что строка удалена, и ее не нужно записывать обратно в файл
 */
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

       /**
        * @brief метод добавления в стркоу, пушит в вектор объект класса Value
        * @param data_type - тип переменной
        * @param value - значение переменной
        */           
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

        /**
         * @brief обновляет значение поля в строке
         * @param index - индекс искомого поля в векторе
         * @param data_type - тип переменной
         * @param value - новое значение
         * @note ДОБАВИТЬ ПРОВЕРКУ, МОЖЕТ ЛИ ЭТО ПОЛЕ ХРАНИТЬ ТАКОЙ ТИП ДАННЫХ ,ЕСЛИ ЭТО ПОЛЕ id - ОНО НЕ ДОЛЖНО ХРАНИТЬ СТРОКУ ИТД
         * @note подобная реализация есть в классе Table, но тут нужно добавить тоже
         */
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

        /**
         * @brief узнать размер строки
         * @return вернет целое число
         */
        inline int getRowSize() const
        {
            return static_cast<int>(row_data.size());
        }

        /**
         * @brief получить данные строки
         * @return вернет вектор со всеми данными
         */
        inline const std::vector<std::unique_ptr<ValueBase>>& getRowData() const {
            return row_data;
        }

        /**
         * @brief узнать удалена ли строка
         * @return вернет булево значение
         */
        inline bool isDeleted() const{
            return is_deleted;
        }

        /**
         * @brief упометить строку как удаленную
         * 
         */
        inline void setAsDeleted(){
            this->is_deleted = true;
        }
};
#endif