#ifndef VALUE
#define VALUE
#include <string>
#include <type_traits>
#include "ValueBase.hpp"

/**
 * @brief шаблонный класс для хранения значения в строке бд наследуеться от ValueBase
 * @todo добавить перегрузку разных операторов чтобы работать с данными?
 */
template <typename T>
class Value : public ValueBase//это класс одного значения из строки записи. содержит строки: тип и значениеы
{//дописать методы, которые вернут тип типизировано
    private:
    //public:
        T data_type;
        T value;
    public:
        Value(T v) : value(v) {}
        ~Value() override = default;
        std::string toString() const override {
            if constexpr (std::is_same<T, std::string>::value) {
                return value; // без std::to_string
            } else if constexpr (std::is_same_v<T, char>) {
                return std::string(1, value); // строка из одного символа
            } else {
                return std::to_string(value);
            }
        }
};


#endif