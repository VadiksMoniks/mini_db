#ifndef VALUEBASE
#define VALUEBASE
#include <string>

/**
 * @brief интерфейс значения сроки, нужен, чтобы вектор мог хранить разные типы данных в себе
 * @note поскольку вектор хранит только один типа данных, а сам класс Value содержит разные - мы говорим вектору, что он7 будет хранить тип ValueBase
 */
class ValueBase {
    public:
        virtual std::string toString() const = 0;
        virtual ~ValueBase() = default;
};
#endif