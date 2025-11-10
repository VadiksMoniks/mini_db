#include <iostream>
#include <windows.h>
#include "Database.hpp"
#include "Command.hpp"
#include <windows.h>
#include <thread>  // Для использования sleep
#include <chrono>  // Для единиц времени

int main()
{
    Command c;

    while (true) {
        std::string input;
        std::getline(std::cin, input);

        if (input.empty()) continue;

        size_t space_pos = input.find(' ');
        std::string command = (space_pos == std::string::npos)
                                ? input
                                : input.substr(0, space_pos);
        std::string params;

        if (space_pos != std::string::npos && space_pos + 1 < input.size())
            params = input.substr(space_pos + 1);

        if (command == "pause") {
            system("pause");
            break;
        }

        std::cout << "Command: [" << command << "]\n";
        std::cout << "Params: [" << params << "]\n";

        c.run(command, params);

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    return 0;
}
/** g++ main.cpp DataBase.hpp Table.hpp Row.hpp Value.hpp ValueBase.hpp -o db_test.exe
 * сделать два класса Database и Table
 * все методе будут в бд, но те, которые работают с таблицой - тк же и в таблице. бд просто будет передавать вызов
 * данные хранить в текстовых файлах
 * придумать, как создавать колонки
 * использовать хэш таблицу для загрузки всех данные из таблицы и работать с ней. когда переключаюсь между таблицами - удалять хэш таблицу
 * использовать алгоритмы поиска и сортировки
 * таблица создаеться из двух файлов tablename_scheme схема с указанием названий колонок и типов tablename_data упорядоченые данные
 */