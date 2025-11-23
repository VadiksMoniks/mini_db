#include <iostream>
#include <windows.h>
#include "DB/Command.hpp"
#include <thread>
#include <chrono>
/**
 * @todo ЕСТЬ ПРОБЛЕМЫ С ПЕРЕДАВАНИЕМ ПАРАМЕТРОВ В НЕКОТОРЫЕ КОМАНДЫ, ЕСЛИ ПЕРЕДАВАТЬ НЕПРАВИЛЬНЫЙ РАЗДЕЛИТЕЛЬ - БУДЕТ КРАШ
 * @todo ЕСТЬ ПРОБЕМЫ С ЛОГИКОЙ НЕКОТОРЫХ ФУНКЦИЙ, ОНИ МОГУТ ПРИВОДИТЬ К КРАШАМ НУЖНО ВСЕ ВНИМАТЕЛЬНО ПРОСМОТРЕТЬ
 */
int main()
{
    Command c;
    std::cout << "help - for commands list \n";
    
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

        //std::cout << "Command: [" << command << "]\n";
        //std::cout << "Params: [" << params << "]\n";

        c.run(command, params);

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    return 0;
}
/** g++ src/*.cpp -Iinclude -o build/db_test.exe
 * сделать два класса Database и Table
 * все методе будут в бд, но те, которые работают с таблицой - тк же и в таблице. бд просто будет передавать вызов
 * данные хранить в текстовых файлах
 * придумать, как создавать колонки
 * использовать хэш таблицу для загрузки всех данные из таблицы и работать с ней. когда переключаюсь между таблицами - удалять хэш таблицу
 * использовать алгоритмы поиска и сортировки
 * таблица создаеться из двух файлов tablename_scheme схема с указанием названий колонок и типов tablename_data упорядоченые данные
 */