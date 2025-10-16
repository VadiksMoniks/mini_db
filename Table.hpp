#ifndef TABLE
#define TABLE
#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
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
        std::unordered_map<std::string, int> id_index;
        std::vector<std::pair<std::string, std::string>> scheme;
        bool is_edited = false;
        unsigned int last_id;

    public:
        std::string db_name;
        std::string table_name;

    public:
        Table(std::string db_name, std::string table_name);
        ~Table();
        void createTable();
        void defineScheme(std::vector<std::pair<std::string, std::string>> columns);
        int find_column_index(const std::string& column_name);//МОЖНО СДЕЛАТЬ ПРИВАТНЫМ
        std::optional<std::reference_wrapper<const Row>>  search_by_value(const std::string& column_name, const std::string& value);

        void insert(const std::vector<std::string>& row);
        void insert_into_file();
        void update(int id, std::string column_name, const std::string&);//пока метод ожидает что я точно укажу индекс в массиве и ожидает строку-значение на вход там уже можно преобразовать в нужный тип.
        void update_id_value_file();
        void delete_row(const int& index);
        void delete_all();

        void read();
        void read_scheme();
        void read_last_id_value();
        const std::vector<Row>& get_table_data() const;
        std::vector<std::pair<std::string, std::string>> get_scheme();
};


/// @brief при создании объекта таблицы, считать данные из файла, если есть, и считать схему таблицы
Table::Table(std::string db_name, std::string table_name) : db_name(db_name), table_name(table_name)
{
    try{
        this->read_scheme();
        this->read();
        this->read_last_id_value();
    }
    catch(const std::runtime_error& e){
        std::cerr << "Error reading table: " << e.what() << std::endl;
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
    catch(const std::runtime_error& e){
        std::cerr << e.what();
    }
}

/// @brief создание файлов схемы и данных
void Table::createTable()
{
    if(std::filesystem::exists("./DB_test/" + db_name) && std::filesystem::is_directory("./DB_test/" + db_name)){
        std::ofstream scheme_file ("./DB_test/" + db_name + "/" + table_name + "_scheme.txt");
        std::ofstream data_file ("./DB_test/" + db_name + "/" + table_name + "_data.txt");
        std::ofstream last_id_file ("./DB_test/" + db_name + "/" + table_name + "_last_id_value.txt");

        last_id = 1;
        last_id_file << last_id;
        last_id_file.close();
    }
    else{
        //ДОПИСАТЬ ЭТОТ БЛОК
    }
    
}

/// @brief заполняет файл схемы таблицы в формате column_name:data_type
/// поле id автоматически добавляеться
/// @param columns - схема формата column_name | data_type
void Table::defineScheme(std::vector<std::pair<std::string, std::string>> columns)// имя:тип
{
    //проверка открылся ли файл
    std::ofstream scheme ("./DB_test/" + db_name + "/" + table_name + "_scheme.txt");

    scheme << "id" << ":" << "int" << "\n";
    this->scheme = columns;
    this->scheme.push_back({"id", "int"});

    for(int i=0; i< columns.size(); i++)
    {
        scheme << columns[i].first << ":" << columns[i].second << "\n";
    }
    scheme.close();
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
                }
            }
        }
        else{
            throw std::runtime_error("Incorrect id value");
        }

    }
    catch(const std::runtime_error& e){
        std::cerr << "Can`t update value: " << e.what(); 
    }
}

/**
 * @brief вставка данных в файл в формате value1:value2:valu3...
 * @note вызываеться автоматически в деструкторе после завершения работы с таблицей
 * @note ЗАПИСЫВАЕТ ПУСТУЮ СТРОКУ В КОНЕЦ ФАЙЛА - ЭТО ПРИВОДИТ К КРАШАМ ПРОГРАММЫ!!!!!!
 * @note ТУТ ВНИМАТЕЛЬНО. ВРОДЕ ПУСТУЮ СТРОКУ ДОПИСЫВАЕТ, ЕСЛИ УДАЛЯТЬ СТРОКИ!!!!!!
 */
void Table::insert_into_file()//записывает данные в формате data1:data2:data3 \n
{
    if(is_edited){
        std::ofstream data_file("./DB_test/" + db_name + "/" + table_name + "_data.txt");
        std::cout << "Writing " << table_data.size() << " rows to file" << std::endl;

        for(size_t i = 0; i< table_data.size(); i++){
            const auto& row = table_data[i];

            if(!row.isDeleted()){
                std::string row_str;
                const auto& row_data = row.getRowData();

                for(size_t j = 0; j<row.getRowSize(); j++){{
                    row_str += row_data[j]->toString();
                    if (j < row.getRowSize() - 1) row_str += ":";
                }}

                data_file << row_str;
                if (i < table_data.size() - 1) data_file << "\n"; // ЧТОБЫ НЕ БЫЛО ПУСТОЙ СТРОКИ В КОНЦЕ
                //вроде бы здесь ошибка, так как если удалять строку, особенно с конца, то добавление переноса строки будет
                if(!data_file){
                    throw std::runtime_error("Can`t write to to data file");
                }
            }
        }
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

/**
 * @brief считавает из файла данных все в один вектор table_data, который состоит из строк а строки из отдельных значений
 * @note вызываеться в конструкторе при создании оьбъекта таблицы
 * @todo ПРИВЕСТИ К ЕДИНОМУ ВИДУ ВСЕ ПРОВЕРКИ И ЛОГИКУ ФАЙЛОВ read_
 */
void Table::read()
{
    if(std::filesystem::exists("./DB_test/" + db_name + "/" + table_name + "_data.txt") && std::filesystem::exists("./DB_test/" + db_name + "/" + table_name + "_scheme.txt")){
        //std::cout<<"1";
        table_data.clear();

        std::ifstream data_file("./DB_test/" + db_name + "/" + table_name + "_data.txt");
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
            table_data.push_back(std::move(row)); // нужно обернуть в unique_ptr
            //std::cout<<"1 \n";
            const auto& id_value = table_data.back().getRowData();//ТУТ ТИПА Я ИНДЕКСИРУЮ. Я ПОЛУЧАЮ ЗНАЧЕНИЕ ID - ОНО БУДЕТ КЛЮЧЕМ, А ИНДЕКС В МАССИВЕ - ЗНАЧЕНИЕ ДЛЯ БЫСТРОГО ПОИСКА
            id_index.insert({id_value[0]->toString(), table_data.size() - 1});
        }
    }
    else{
       // std::cout<<"0";
    }
}

/**
 * @brief считавает схему таблицы из файла в scheme
 * @note вызываеться в конструкторе при создании оьбъекта таблицы
 * @todo ПРИВЕСТИ К ЕДИНОМУ ВИДУ ВСЕ ПРОВЕРКИ И ЛОГИКУ ФАЙЛОВ read_
 */
void Table::read_scheme() {
    scheme.clear();
    std::ifstream scheme_file("./DB_test/" + db_name + "/" + table_name + "_scheme.txt");
    if (!scheme_file.is_open())
        throw std::runtime_error("Could not open scheme file");

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
 * @todo ПРИВЕСТИ К ЕДИНОМУ ВИДУ ВСЕ ПРОВЕРКИ И ЛОГИКУ ФАЙЛОВ read_
 * @todo ПРОВЕРКИ
 */
void Table::read_last_id_value()
{
    std::ifstream id_value_file("./DB_test/" + db_name + "/" + table_name + "_last_id_value.txt");

    if(!id_value_file.is_open()){
        throw std::runtime_error("Could not open id value file");
    }

    std::string line;
   // if (!std::getline(id_value_file, line) || line.empty()) {
   //     throw std::runtime_error("ID value file is empty or invalid");
   // }
    std::getline(id_value_file, line);
    last_id = std::stoi(line);
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
        std::cerr << e.what();
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
        id_index.erase(std::to_string(index));
        this->table_data[deleted_row_id_value->second].setAsDeleted();
        this->is_edited = true;
    }
    else{
        throw std::runtime_error("There is no rows with such id value");
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
}



#endif