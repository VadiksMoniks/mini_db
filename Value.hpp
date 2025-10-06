#ifndef VALUE
#define VALUE
#include <string>
#include <type_traits>
#include "ValueBase.hpp"
//переделать под ENUM

/**
 * @brief шаблонный класс для хранения значения в строке бд наследуеться от ValueBase
 */
template <typename T>
class Value : public ValueBase//это класс одного значения из строки записи. содержит строки: тип и значениеы
{//дописать методы, которые вернут тип типизировано
    private:
    //public:
        //T data_type;
        T value;
    public:
        Value(T v) : value(v) {}
        ~Value() override = default;
        std::string toString() const override {
            if constexpr (std::is_same<T, std::string>::value) {
                return value; // без std::to_string
            } else {
                return std::to_string(value);
            }
        }
};

/**
 * @brief способ возврата данных, так как само значение должно быть приватным
 */
template <>// потом заменю или уберу !!!
std::string Value<std::string>::toString() const {//ИЗ-ЗА ТОГО, ЧТО Row ХРАНИТ ТОЛЬКО УКАЗАТЕЛИ НА БАЗОВЫЙ КЛАСС, А В НЕМ НЕТУ ПОЛЯ VALUE НУЖЕН ЭТОТ МЕТОД
    return value;
}


#endif