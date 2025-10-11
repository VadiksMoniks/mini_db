#include <iostream>
#include <windows.h>
#include "Database.hpp"
#include <windows.h>

int main()
{
    try{ 
    Database db("mydb");
   // db.createDatabase();

    //db.createTable("users", { {"name", "string"}, {"age", "int"} });
    db.use_table("users");
    //db.table->read_scheme();
   // db.table->show_scheme();

    //std::vector<std::string> row1 = {"Alice", "30"};
    //db.table->insert(row1);
                                                    //ЧТОБЫ ЗАПИСАТЬ ЧТО_ТО В ФАЙЛ
    //std::vector<std::string> row2 = {"Bob", "25"};
    //db.table->insert(row2);

    //db.table->read(); // ЧТОБЫ ПРОСТО ГЛЯНУТЬ, ЧТО В ФАЙЛК

    //std::vector<std::string> row4 = {"John", "20", "m", "1"};
    //db.table->insert(row4);
  //  db.table->show_table_data();
    //db.table->update(3, "name", "Insomnia");
    //db.table->delete_row(4);
    auto scheme = db.table->get_scheme();
    auto& table_data = db.table->get_table_data();

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

    std::cout<<"\n";
    
    } catch (const std::exception& e) {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
    }
    system("pause");
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