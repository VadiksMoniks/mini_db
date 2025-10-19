#ifndef TABLE
#define TABLE
#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <optional>
#include "Row.hpp"
/**
 * @see update method update() DONE
 * @see delete methods delete_all() delete_row() DONE
 * @see search by different params search_by_value() DONE I GUES
 * @todo more data types
 * @note НЕТУ ПРОВЕРОК СООТВЕТСТВИЯ ДАННЫХ СХЕМЕ, КОЛИЧЕСТВО ДАННЫХ В СТРОКЕ И ЧТО ДЕЛАТЬ С ПРОПУЩЕНЫМИ ПОЛЯМИ В СТРОКЕ?
 * @see автоинкрементное уникальное id поле
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

    private:
        std::vector<Row> table_data;
        std::map<std::string, int> id_index;
        std::vector<std::pair<std::string, std::string>> scheme;
        bool is_edited = false;
        unsigned int last_id;

    public:
        std::string db_name;
        std::string table_name;

    private:
        std::ifstream open_input_file(const std::string& path);
        int find_column_index(const std::string& column_name);

        void read_data();
        void read_scheme();
        void read_last_id_value();

    public:
        Table(std::string db_name, std::string table_name);
        ~Table();
        static void createTable(const std::string& db_name, const std::string& table_name);
        static void defineScheme(const std::string& db_name, const std::string& table_name, const std::vector<std::pair<std::string, std::string>>& columns);
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


/// @brief при создании объекта таблицы, считать данные из файла, если есть, и считать схему таблицы
Table::Table(std::string db_name, std::string table_name) : db_name(db_name), table_name(table_name)
{
    try{
        this->read_scheme();
        this->read_data();
        this->read_last_id_value();
    }
    catch(const std::exception& e){
        std::cerr << "Error reading table: " << e.what() << "\n";
    }
    //std::cout << "Last ID value is:" << last_id << "\n";
}

/*
 * записать изменения в файл
 * очистить вектор table_data после конца работы
 */
Table::~Table()
{
   // std::cout << "Table destructor called\n";
    try{
        this->insert_into_file();
        this->update_id_value_file();
        table_data.clear();
    }
    catch(const std::exception& e){
        std::cerr << e.what() << "\n";
    }
}

/// @brief создание файлов схемы и данных
void Table::createTable(const std::string& db_name, const std::string& table_name)
{
    if(std::filesystem::exists("./DB_test/" + db_name) && std::filesystem::is_directory("./DB_test/" + db_name)){
        std::ofstream scheme_file ("./DB_test/" + db_name + "/" + table_name + "_scheme.txt");
        std::ofstream data_file ("./DB_test/" + db_name + "/" + table_name + "_data.txt");
        std::ofstream last_id_file ("./DB_test/" + db_name + "/" + table_name + "_last_id_value.txt");

        last_id_file << "1";
    }
    else{
        throw std::runtime_error("Can`t find the database directory. Check if the following dirrectory exists!");
    }
    
}

/// @brief заполняет файл схемы таблицы в формате column_name:data_type
/// поле id автоматически добавляеться
/// @param columns - схема формата column_name | data_type
void Table::defineScheme(const std::string& db_name, const std::string& table_name, const std::vector<std::pair<std::string, std::string>>& columns)// имя:тип
{
    std::ofstream scheme ("./DB_test/" + db_name + "/" + table_name + "_scheme.txt");

    scheme << "id" << ":" << "int" << "\n";

    for(int i=0; i< columns.size(); i++)
    {
        scheme << columns[i].first << ":" << columns[i].second << "\n";
        if(!scheme){
            throw std::runtime_error("Can`t write table scheme to file");
        }
    }
}

/**
 * @brief внутренний метод ищет индекс поля в векторе строки данных, чтобы было ясно, к какой колонке обращаться,
 * то есть, если я хочу обновить name поле, я должен узнать в векторе его индекс, а потом уже обновлять значение
 * @param column_name - имя искомой колонки(id, name, email_address...)
 */
int Table::find_column_index(const std::string& column_name)
{
    for(int i = 0; i< scheme.size(); i++){
        if(scheme[i].first == column_name){
            return static_cast<int>(i);
        }
    }

    throw std::runtime_error("Column '" + column_name + "' not found in table");
}

/**
 * @brief вставка в вектор данных 
 * @param row - вектор данных, он должен идти в таком же порядке, в каком идут данные через схему, то есть name, email_address,...
 * поле id автоматически дописываеться в начало вектора исходя из последнего значения id
 */
void Table::insert(const std::vector<std::string>& row)
{
    if (row.size() + 1 != scheme.size())//+ 1 потому, что id добавляеться автоматически
        throw std::runtime_error("Wrong number of arguments. It should be: " + scheme.size());//("Row size does not match scheme size");

    Row new_row;

    new_row.add_to_row("int", std::to_string(last_id));

    for (size_t i = 0; i < row.size(); ++i)
        new_row.add_to_row(scheme[i + 1].second, row[i]);

    table_data.push_back(std::move(new_row));
    id_index.insert({std::to_string(last_id), table_data.size() - 1});
    is_edited = true;
    last_id++;
}

/**
 * @brief обновление одного значения в строке, индекс которой передаеться как параметр
 * @param row_index - значение поля id
 * @param column_name - имя колонки в которой замениться значение
 * @param value - новое значение для замены
 */
void Table::update(int id, std::string column_name, const std::string& value)
{
    try{

        auto index = id_index.find(std::to_string(id));

        if(index != id_index.end()){
            for(int i = 0; i< scheme.size(); i++){
                if(scheme[i].first == column_name){
                    table_data[index->second].update_value(this->find_column_index(column_name), scheme[i].second, value);
                    std::cout << "Data updated successfuly\n";
                    break;
                }
            }
        }
        else{
            throw std::runtime_error("Incorrect id value");
        }

    }
    catch(const std::runtime_error& e){
        std::cerr << "Can`t update value: " << e.what() << "\n"; 
    }
}

/**
 * @brief вставка данных в файл в формате value1:value2:valu3...
 * @note вызываеться автоматически в деструкторе после завершения работы с таблицей
 * @note ЗАПИСЫВАЕТ ПУСТУЮ СТРОКУ В КОНЕЦ ФАЙЛА - ЭТО ПРИВОДИТ К КРАШАМ ПРОГРАММЫ!!!!!!
 * @note ТУТ ВНИМАТЕЛЬНО. ВРОДЕ ПУСТУЮ СТРОКУ ДОПИСЫВАЕТ, ЕСЛИ УДАЛЯТЬ СТРОКИ!!!!!!
 */
void Table::insert_into_file()
{
    if (!is_edited) return;

    std::ofstream data_file("./DB_test/" + db_name + "/" + table_name + "_data.txt");
    if (!data_file)
        throw std::runtime_error("Can't open file");

    std::cout << "Writing " << id_index.size() << " rows to file" << std::endl;

    for (auto it = id_index.begin(); it != id_index.end(); ++it) {
        const auto& row = table_data[it->second];
        const auto& row_data = row.getRowData();

        std::string row_str;
        for (size_t j = 0; j < row.getRowSize(); j++) {
            row_str += row_data[j]->toString();
            if (j < row.getRowSize() - 1) row_str += ":";
        }

        data_file << row_str;

        // Проверяем, что элемент не последний
        auto next_it = std::next(it);
        if (next_it != id_index.end()) {
            data_file << "\n";
        }

        if (!data_file)
            throw std::runtime_error("Can't write data into file");
    }
}

void Table::update_id_value_file()
{
    std::ofstream id_value_file("./DB_test/" + db_name + "/" + table_name + "_last_id_value.txt");
    id_value_file << last_id;

    if(!id_value_file){
        throw std::runtime_error("Can`t write id value to file");
    }
}

std::ifstream Table::open_input_file(const std::string& path)
{
    std::ifstream file(path);
    if(!file.is_open()){
        throw std::runtime_error("Could not open file: " + path);
    }

    return file;
}

/**
 * @brief считавает из файла данных все в один вектор table_data, который состоит из строк а строки из отдельных значений
 * @note вызываеться в конструкторе при создании оьбъекта таблицы
 */
void Table::read_data()
{
    std::ifstream data_file = this->open_input_file("./DB_test/" + db_name + "/" + table_name + "_data.txt");

    table_data.clear();//это может быть лишним

    std::string line;

    while (std::getline(data_file, line)) {
        std::stringstream ss(line);
        std::string token;
        Row row;
        int column_index = 0;

        while (std::getline(ss, token, ':')) {
            if (column_index < scheme.size()) {
                std::string type = scheme[column_index].second;
                row.add_to_row(type, token);
            }
            column_index++;
        }
        table_data.push_back(std::move(row));
        //std::cout<<"1 \n";
        const auto& id_value = table_data.back().getRowData();//ТУТ ТИПА Я ИНДЕКСИРУЮ. Я ПОЛУЧАЮ ЗНАЧЕНИЕ ID - ОНО БУДЕТ КЛЮЧЕМ, А ИНДЕКС В МАССИВЕ - ЗНАЧЕНИЕ ДЛЯ БЫСТРОГО ПОИСКА
        id_index.insert({id_value[0]->toString(), table_data.size() - 1});
    }
}

/**
 * @brief считавает схему таблицы из файла в scheme
 * @note вызываеться в конструкторе при создании оьбъекта таблицы
 */
void Table::read_scheme()
{
    std::ifstream scheme_file = this->open_input_file("./DB_test/" + db_name + "/" + table_name + "_scheme.txt");

    scheme.clear();
    std::string line;
    while (std::getline(scheme_file, line)) {
        std::stringstream ss(line);
        std::string column_name, column_type;
        if (std::getline(ss, column_name, ':') && std::getline(ss, column_type)) {
            scheme.push_back({ column_name, column_type });
        }
    }
}

/**
 * @brief считывает last_id value from file
 */
void Table::read_last_id_value()
{
    std::ifstream id_value_file = this->open_input_file("./DB_test/" + db_name + "/" + table_name + "_last_id_value.txt");
    std::string line;

    if (!std::getline(id_value_file, line) || line.empty()) {
        throw std::runtime_error("ID value file is empty or invalid");
    }

    try{
    last_id = std::stoi(line); 
    }
    catch(const std::exception& e){
        throw std::runtime_error("Invalid ID value\n");
    }
}

/**
 * @brief поиск по значению 
 * @param column_name - имя колонки
 * @param value - искомое значение 
 * @note поскольку поле id индексировано, то там поиск через хеш мапу, остальные поля - простой перебор
 * @return вернет вектор данных
 */
std::optional<std::reference_wrapper<const Row>>  Table::search_by_value(const std::string& column_name, const std::string& value)
{
    try{
        if(column_name == "id"){
            auto searched_row_index = id_index.find(value);
            if(searched_row_index != id_index.end()){
                return table_data[searched_row_index->second];
            }
        }
        else{
            int index = this->find_column_index(column_name);
            for(size_t i = 0; i<table_data.size(); i++){
                if(value == table_data[i].getRowData()[index]->toString()){
                    return table_data[i];
                }
            }
        }
    }
    catch(const std::runtime_error& e){
        std::cerr << e.what() << "\n";
        return std::nullopt;
    }

    std::cout<<"No matches found \n";
    return std::nullopt;
}

/**
 * @brief возврат данных
 */
const std::vector<Row>& Table::get_table_data() const
{
   return table_data;
}

/**
 * @brief вывод схемы таблицы в консоль
 */
std::vector<std::pair<std::string, std::string>> Table::get_scheme()
{
    return scheme;
}


/**
 * @brief удаляет строку
 * @param index - индекс строки, ее значение поля id
 * @note каждая строка Row может быть помечена как is_deleted, и при записи в файл она будет проигнорирована
 */
void Table::delete_row(const int& index)//ID ПОКА ЧТО ТУТ ПЕРЕДАЕТЬСЯ ТОЛЬКО ЗНАЧЕНИЕ ID ПОЛЯ
{
    auto deleted_row_id_value = id_index.find(std::to_string(index));
    if(deleted_row_id_value != id_index.end()){
        this->table_data[deleted_row_id_value->second].setAsDeleted();
        id_index.erase(std::to_string(index));
        this->is_edited = true;
    }
    else{
        std::cerr << "There is no rows with such id value";
    }
}

/**
 * @brief удаляет все данные
 * @todo так же можно удалить файл с данными, и нужно проверить, меняеться ли он, если вектор пуст, что тогда пишеться в файл?
 */
void Table::delete_all()
{
    this->is_edited = true;
    table_data.clear();
    id_index.clear();
}



#endif