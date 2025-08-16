#ifndef VALUEBASE
#define VALUEBASE
#include <string>

class ValueBase {
    public:
        virtual std::string toString() const = 0;
        virtual ~ValueBase() = default;
};
#endif