#ifndef TABLE
#define TABLE

#include <map>
#include <optional>
#include "Row.hpp"
/**
 * @todo more data types
 * @note ЧТО ДЕЛАТЬ С ПРОПУЩЕНЫМИ ПОЛЯМИ В СТРОКЕ? nullable
 */

 /**
/// @brief класс таблицы данных
  * вектор trable_data - набор отдельных записей - строк формата Row
  * мапа id_index для быстрого поиска по индексу записи, это нужно так, как бедет использоваться автоинкремент уникальные id, то есть может быть ситуация, что идут id: 1,2,5
  * scheme - схема таблицы. нужна для заполнения таблицы и для того, чтобы правильно вставлять значения - вектор пар имя колонки | тип данных
  * is_edited - для того, чтобы после конца работы с таблицей, если она менялась - обновить файл с данными
  * db_name - имя базы данных - папка с файлами
  * table_name - имя файа, откуда считываються, и куда пишуться данные
  * НУЖНО ЧТО-ТО РЕШАТЬ С РАБОТОЙ ПРОГРАММЫ, ТАК КАК ЛОГИКА РАБОТЫ КЛАССА БД ПРОТИВОРЕЧИТ ЛОГИКЕ РАБОТЫ ЭТОГО КЛАССА
  */
class Table{

        std::vector<Row> table_data;
        std::map<std::string, int> id_index;
        std::vector<std::pair<std::string, std::string>> scheme;
        bool is_edited = false;
        unsigned int last_id;

        std::ifstream open_input_file(const std::string& path);
        int find_column_index(const std::string& column_name);

        void read_data();
        void read_scheme();
        void read_last_id_value();

    public:
        std::string db_name;
        std::string table_name;

        Table(std::string db_name, std::string table_name);
        ~Table();
        static void createTable(const std::string& db_name, const std::string& table_name);
        static void defineScheme(const std::string& db_name, const std::string& table_name, const std::vector<std::string>& columns);//const std::vector<std::pair<std::string, std::string>>& columns);
        std::optional<std::reference_wrapper<const Row>>  search_by_value(const std::string& column_name, const std::string& value);

        void insert(const std::vector<std::string>& row);
        void insert_into_file();
        void update(int id, std::string column_name, const std::string&);
        void update_id_value_file();
        void delete_row(const int& index);
        void delete_all();

        const std::vector<Row>& get_table_data() const;
        std::vector<std::pair<std::string, std::string>> get_scheme();
};

#endif