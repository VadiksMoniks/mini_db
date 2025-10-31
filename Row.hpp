#ifndef ROW
#define ROW
#include <vector>
#include <string>
#include <iostream>
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
    private:
        std::vector<std::unique_ptr<ValueBase>> row_data;
        using FactoryFunc = std::function<std::unique_ptr<ValueBase>(const std::string&)>;
        std::unordered_map<std::string, FactoryFunc> factories;
        bool is_deleted = false;

    public:
    
        Row() {
            factories["int"] = [](const std::string& v) {
                return std::make_unique<Value<int>>(std::stoi(v));
            };
            factories["double"] = [](const std::string& v) {
                return std::make_unique<Value<double>>(std::stod(v));
            };
            factories["string"] = [](const std::string& v) {
                return std::make_unique<Value<std::string>>(v);
            };
            factories["char"] = [](const std::string& v) {
                if (v.empty())
                    throw std::runtime_error("Empty char string");
                return std::make_unique<Value<char>>(v[0]);
            };
            factories["bool"] = [](const std::string &v) {
                return std::make_unique<Value<bool>>(v == "true" || v == "1");
            };
        }
        
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
            if (!Validator::is_valid(value, data_type)) {
                throw std::runtime_error("Passed value(" + value + ") doesn`t match a following datatype: " + data_type);
            }

            auto it = factories.find(data_type);
            if (it != factories.end()) {
                row_data.push_back(it->second(value));
            } else {
                throw std::runtime_error("Unsupported data type: " + data_type);
            }
        }

        /**
         * @brief обновляет значение поля в строке
         * @param index - индекс искомого поля в векторе
         * @param data_type - тип переменной
         * @param value - новое значение
         */
        void update_value(int index, std::string data_type, const::std::string& value)
        {
            auto it = factories.find(data_type);
            if (it != factories.end()){
                row_data[index] = it->second(value);
            }
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