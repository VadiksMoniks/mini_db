#include <filesystem>
#include <iostream>
#include "DB/Database.hpp"

/**
 * @brief создает папку с именем текущей бд для хранения таблиц
 */
void Database::createDatabase()
{
    if (!std::filesystem::exists("../DB_test/" + db_name)) {
        std::filesystem::create_directories("../DB_test/" + db_name);
    } else {
        std::cout << "This database already exists" << std::endl;
    }
}

/**
 * @brief создает объект текущей таблицы, и вызывает у него создание файла самой таблицы
 * @param table_name - имя таблицы
 * @param scheme - вектор пар, описывающих схему таблицы
 */
void Database::createTable(const std::string& table_name, const std::vector<std::string>& scheme) //const std::vector<std::pair<std::string, std::string>>& scheme)
{
    Table::createTable(db_name, table_name);
    Table::defineScheme(db_name, table_name, scheme);
}

/**
 * @brief переключаеться на новую таблицу
 * @param table_name - имя новой таблицы
 * @note проверки, есть ли такая таблица итд.
 */
void Database::useTable(const std::string& table_name)
{
    if (table && table->table_name == table_name)
        return; // уже используется нужная таблица

    table = std::make_unique<Table>(db_name, table_name);
}

void Database::dropDatabase()
{
    try{
        //this->cleanDir("../DB_test/" + db_name); // ЕСЛИ ЗАХОЧУ ИСПОЛЬЗОВАТЬ СВОЙ КОД С РЕКУРСИЕЙ, ТО ПРОСТО РАССКОМЕНТИРОВАТЬ
        //std::filesystem::remove("../DB_test/" + db_name); // ЕСЛИ ЗАХОЧУ ИСПОЛЬЗОВАТЬ СВОЙ КОД С РЕКУРСИЕЙ, ТО ПРОСТО РАССКОМЕНТИРОВАТЬ
        std::filesystem::remove_all("../DB_test/" + db_name);
    }
    catch(std::exception& e){
        std::cerr << e.what() << "\n";
    }

    std::cout << "Database was removed \n";
}

/**
 * @brief для очистки файлов и папок рекурсивно
 */
void Database::cleanDir(const std::string& path)
{
    for(const auto& entry : std::filesystem::directory_iterator(path)){
        if(std::filesystem::is_directory(entry)){
            Database::cleanDir(path + "/" + entry.path().filename().string());
        }
        if(!std::filesystem::remove(entry)){
            throw std::runtime_error("Can`t remove the following file: " + entry.path().string());
        }
    }

}