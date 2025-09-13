#ifndef TABLE
#define TABLE
#include <filesystem>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "Row.hpp"
/**
 * update DONE
 * delete DONE
 * search by different params
 * more datatypes
 */
class Table{//при создании объекта сразу использовать вектор а при удалении записывать все в файл

    private:
        std::vector<Row> table_data;
        std::vector<std::pair<std::string, std::string>> scheme;
        bool is_edited = false;

    public:
        std::string db_name;
        std::string table_name;

    public:
        Table(std::string db_name, std::string table_name);
        ~Table();
        void createTable();
        void defineScheme(std::vector<std::pair<std::string, std::string>> columns);
        int find_column_index(std::string& column_name);//МОЖНО СДЕЛАТЬ ПРИВАТНЫМ
        /*const Row&*/  void search_by_value(std::string column_name, std::string value);

        void insert(const std::vector<std::string>& row);
        void insert_into_file();
        void update(int index, std::string column_name, const std::string&);//пока метод ожидает что я точно укажу индекс в массиве и ожидает строку-значение на вход там уже можно преобразовать в нужный тип.
        void delete_row(const int& index);
        void delete_all();

        void read();
        void read_scheme();
        void show_table_data();
        void show_scheme();
};

Table::Table(std::string db_name, std::string table_name) : db_name(db_name), table_name(table_name)
{
    this->read_scheme();
    this->read();
}

Table::~Table()
{
   // std::cout << "Table destructor called\n";
    this->insert_into_file();
    table_data.clear();
}

void Table::createTable()
{
    if(std::filesystem::exists("./DB_test/" + db_name) && std::filesystem::is_directory("./DB_test/" + db_name)){
        std::ofstream scheme_file ("./DB_test/" + db_name + "/" + table_name + "_scheme.txt");
        std::ofstream data_file ("./DB_test/" + db_name + "/" + table_name + "_data.txt");
    }
    else{
        //ДОПИСАТЬ ЭТОТ БЛОК
    }
    
}

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

int Table::find_column_index(std::string& column_name)//ДОБАВИТЬ ПРОВЕРКИ И ИСКЛЮЧЕНИЯ, ЕСЛИ НЕ НАЙДЕНо
{
    int index = 0;
    for(int i = 0; i< scheme.size(); i++){
        if(scheme[i].first == column_name){
            index = i;
        }
    }
    return index;
}

void Table::insert(const std::vector<std::string>& row)
{
    if (row.size() + 1 != scheme.size())
        throw std::runtime_error("Too few arguments passed!");//("Row size does not match scheme size");

    Row new_row;

    const Row& last_row = table_data.back();
    const std::vector<std::unique_ptr<ValueBase>>& row_data = last_row.getRowData();
    const std::string& last_id = row_data[0]->toString();//НЕТУ ПОЛЯ VALUE ПОЭТОМУ ИСПОЛЬЗУЕТЬСЧЯ ЭТОТ МЕТОД ТАК КАК НАСЛЕДНИК ЕГО РЕАЛИЗУЕТ

    int id_value = std::stoi(last_id) + 1;
    new_row.add_to_row("int", std::to_string(id_value));

    for (size_t i = 0; i < row.size(); ++i)
        new_row.add_to_row(scheme[i + 1].second, row[i]);

    table_data.push_back(std::move(new_row));
    is_edited = true;
}

void Table::update(int row_index, std::string column_name, const std::string& value)
{
    int index = 0;
    for(int i = 0; i< scheme.size(); i++){
        if(scheme[i].first == column_name){
            index = i;
            table_data[this->find_column_index(column_name)].update_value(index, scheme[i].second, value);
        }
    }
    //если такой колонки нету - выброс исключения или ошибка
}


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
            }
        }
    }
}

void Table::read()//считавает из схемы и данных все в один вектор, который состоит из строк а строки из отдельных значений
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
        }
    }
    else{
       // std::cout<<"0";
    }
}

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

void Table::search_by_value(std::string column_name, std::string value)
{
    int index = this->find_column_index(column_name);

    std::unordered_map<std::string, int> map;

    for(size_t i = 0; i<table_data.size(); i++){
        const auto& row = table_data[i].getRowData();
        map.insert({row[index]->toString(), i});
    }

    auto searched_row_index = map.find(value);
    if(searched_row_index != map.end()){
        auto& searched_row = table_data[searched_row_index->second].getRowData();

        for(size_t i = 0; i<searched_row.size(); i++){
            std::cout<< searched_row[i]->toString();
            if(i < searched_row.size() - 1){
                std::cout<< " : ";
            }
        }
        std::cout<<"\n";
    }
    else{
        std::cout<<"No matches found \n";
    }
}

void Table::show_table_data() {
    for (auto& column_name : scheme) {
        std::cout << column_name.first << " | ";
    }
    std::cout << std::endl;

    for (auto& row : table_data) {
        const auto& row_data = row.getRowData();
        if(!row.isDeleted()){
            for (auto& value : row_data) {
                std::cout << value->toString() << " | ";
            }
        }
        std::cout << std::endl;
    }
}

void Table::show_scheme()
{
    for(auto column_name : scheme){
        std::cout << column_name.first << " | " << column_name.second << " ";
    }
}

void Table::delete_row(const int& index)
{
    this->table_data[index-1].setAsDeleted();
    this->is_edited = true;
}

void Table::delete_all()
{
    this->is_edited = true;
    table_data.clear();
}



#endif