#ifndef DATABASE
#define DATABASE

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
 * ИЗМЕНЕНИЕ СХЕМЫ
 * @todo dropDatabase
 */
    void cleanDir(const std::string& path);

    public:
        std::string db_name;
        std::unique_ptr<Table> table; // теперь умный указатель

        Database(const std::string& db_name) : db_name(db_name), table(nullptr) {}
        ~Database() = default; // умный указатель сам удалит Table

        void createDatabase();
        void createTable(const std::string& table_name, const std::vector<std::string>& scheme);
        void useTable(const std::string& table_name);
        void dropDatabase();
        void dropTable(const std::string& table_name);
    };

#endif