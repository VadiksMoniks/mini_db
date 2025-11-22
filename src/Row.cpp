#include "DB/Row.hpp"
#include <iostream>

Row::Row() {
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
/**
* @brief метод добавления в стркоу, пушит в вектор объект класса Value
* @param data_type - тип переменной
* @param value - значение переменной
*/           
void Row::add_to_row(const std::string& data_type, const std::string& value) {
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
void Row::update_value(int index, std::string data_type, const::std::string& value)
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
size_t Row::getRowSize() const
{
    return row_data.size();
}

/**
 * @brief получить данные строки
 * @return вернет вектор со всеми данными
 */
const std::vector<std::unique_ptr<ValueBase>>& Row::getRowData() const
{
    return row_data;
}

/**
 * @brief узнать удалена ли строка
 * @return вернет булево значение
 */
bool Row::isDeleted() const
{
    return is_deleted;
}

/**
 * @brief упометить строку как удаленную
 * 
 */
void Row::setAsDeleted()
{
    is_deleted = true;
}