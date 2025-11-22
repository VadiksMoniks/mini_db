#include "DB/Command.hpp"
#include <iostream>

Command::Command(){
    commands = {
        {
            "db-name", [this](const std::optional<std::string>& arg){
                if(db != nullptr) delete db;
                
                db = new Database(*arg);
            }
        },
        {
            "create-database", [this](const std::optional<std::string>&arg){
                //Database db(*arg);
                if(db != nullptr){
                    db->createDatabase();
                }
                else{
                    std::cerr << "Database has not been selected. Use db-name command\n";
                }
                
            }
        },
        {
            "create-table", [this](const std::optional<std::string>& arg) {//Throwing error if missing params
                std::vector<std::string> tokens = this->split_args(*arg, ";;");
                std::string table_name = tokens[0];
                this->db->createTable(table_name, this->split_args(tokens[1], "::"));
            }
        },
        {
            "use-table", [this](const std::optional<std::string>& arg){
                if(db != nullptr){
                    db->useTable(*arg);
                } else {
                    std::cerr << "Select database at first\n";
                }
            }
        },
        {
            "insert", [this](const std::optional<std::string>& arg){// ""insert 1:2:name:c:3.14
                if(db != nullptr && db->table != nullptr){
                    db->table->insert(this->split_args(*arg, "::"));
                }
                else{
                    std::cout << "Chose table you want insert into this values via use-table command \n";
                }
                
            }
        },
        {
            "update", [this](const std::optional<std::string>& arg){
                if(db != nullptr && db->table != nullptr){
                    std::vector<std::string> params = this->split_args(*arg, "::");
                    if(params.size() != 3){
                        throw std::runtime_error("Wrong number of params passed");
                    }

                    db->table->update(std::stoi(params[0]), params[1], params[2]);
                }
                else{

                }

            }
        },
        {
            "show-table-data", [this](const std::optional<std::string>& arg){
                if(db != nullptr && db->table != nullptr){
                    const auto& scheme = db->table->get_scheme();
                    const auto& table_data = db->table->get_table_data();

                    for (auto& column_name : scheme) {
                            std::cout << column_name.first << " | ";
                    }
                    
                    std::cout << "\n";

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
                else{
                    std::cerr << "Database or table has not been selected\n";
                }
            }
        },
        {
            "delete-row", [this](const std::optional<std::string>& arg){//проверки
                if(db != nullptr && db->table != nullptr){
                    db->table->delete_row(std::stoi(*arg));
                }
                else{
                    std::cerr << "Database or table has not been selected\n";
                }
                
            }
        },
        {
            "delete-all", [this](const std::optional<std::string>& arg){
                if(db != nullptr && db->table != nullptr){
                    db->table->delete_all();
                }
                else{
                    std::cerr << "Select database and table\n";
                }
            }
        },
        {
            "drop-db", [this](const std::optional<std::string>& arg){
                if(db != nullptr){
                    db->dropDatabase();
                }
                else{
                    std::cerr << "Select database\n";
                }
            }
        },
        {
            "help", [this](const std::optional<std::string>& arg){
                std::cout<<"Commands List: \n";

                for(auto it = commads_description.begin(); it != commads_description.end(); ++it){
                    std::cout<< it->first << " " << it->second << "\n";
                }
            }
        }
    };

    commads_description = {
        {
            "db-name", "Requires database name which will be used to store data"

        },
        {
            "create-database", "Creates database folder"
        },
        {
            "create-table", "Creates table with table scheme. Required parameters should be passed like this: create-table table_name;;name:string::age:int Dont pass id column it is adding automaticaly"
        },
        {
            "use-table", "Switch between tables"
        },
        {
            "insert", "Inserting data of form name::10::m::true"
        },
        {
            "update", "Updates passed parameter of the row. Requires row id, column name and value for this column"
        },
        {
            "show-table-data", "Print table sceheme and all table data"
        },
        {
            "delete-row", "deletes row by it's id"
        },
        {
            "help","Shows all commands list"
        },
        {
            "pause","Stops the programm"
        },
        {
            "delete-all","Delete all rows from table"
        },
        {
            "drop-db", "Delete selected database and all tables from it"
        },
    };
}
Command::~Command()
{
    if(db != nullptr){
        delete db;
    }
}

std::vector<std::string> Command::split_args(std::string const& line, const std::string& delimeter)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = line.find(delimeter);

    while(end != line.npos){
        tokens.push_back(line.substr(start, end - start));
        start = end + delimeter.length();
        end = line.find(delimeter,start);
    }
    if(start < line.size()){
        tokens.push_back(line.substr(start));
    }

    return tokens;
}

void Command::run(std::string command, const std::optional<std::string>& params)
{
    auto it = commands.find(command);

    if(it != commands.end()){
        it->second(params);
    }
    else{
        std::cerr << "Unknown command \n";
    }
}