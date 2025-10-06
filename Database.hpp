#ifndef DATABASE
#define DATABASE
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include "Table.hpp"

/**
 * @brief класс базы данных, который может оперировать таблицами
 * содержит имя бд - db_name, и указатель на текущую таблицу - table
 */
class Database{
/**
 * creating files(tables)
 * создаеться бд
 * создаються файлы
 * описываеться схема таблицы\
 * нужен контроль за тем, какие строки вводяться в виде типов и нужно изменить сигнатуру чтобы было ясно, какая пара нужна
 * 
 */

    public:
        std::string db_name;
        std::unique_ptr<Table> table; // теперь умный указатель

    public:
        Database(const std::string& db_name) : db_name(db_name), table(nullptr) {}
        ~Database() = default; // умный указатель сам удалит Table

        void createDatabase();
        void createTable(const std::string& table_name, const std::vector<std::pair<std::string, std::string>>& scheme);
        void use_table(const std::string& table_name);
        void defineScheme(const std::string& table_name, const std::vector<std::pair<std::string, std::string>>& columns);
    };

    /**
     * @brief создает папку с именем текущей бд для хранения таблиц
     */
    inline void Database::createDatabase()
    {
        if (!std::filesystem::exists("./DB_test/" + db_name)) {
            std::filesystem::create_directories("./DB_test/" + db_name);
        } else {
            std::cout << "This database already exists" << std::endl;
        }
    }

    /**
     * @brief создает объект текущей таблицы, и вызывает у него создание файла самой таблицы
     * @param table_name - имя таблицы
     * @param scheme - вектор пар, описывающих схему таблицы
     */
    inline void Database::createTable(const std::string& table_name, const std::vector<std::pair<std::string, std::string>>& scheme)
    {
        table = std::make_unique<Table>(db_name, table_name);
        table->createTable();
        table->defineScheme(scheme);
        // удаление не нужно, умный указатель сам освободит память
    }
    
    /**
     * @brief переключаеться на новую таблицу
     * @param table_name - имя новой таблицы
     * @note проверки, есть ли такая таблица итд.
     */
    inline void Database::use_table(const std::string& table_name)
    {
        if (table && table->table_name == table_name)
            return; // уже используется нужная таблица

        table = std::make_unique<Table>(db_name, table_name);
    }

    /**
     * @brief описывает схему таблицы
     * @param table_name - имя таблицы 
     * @param columns - новая схема
     * @note ВРЯД ЛИ НУЖЕН МОЖНО БУДЕТ УБРАТЬ
     */
    inline void Database::defineScheme(const std::string& table_name, const std::vector<std::pair<std::string, std::string>>& columns)
    {
        if (!table || table->table_name != table_name)
            use_table(table_name);

        table->defineScheme(columns);
    }

#endif