# mini_db

## What is this project

It's just a kind of pet-project which I made just to study c++. It represents a similar database which can store some primitive data in .txt files

## Installation

After installing this code you need to create build directory where your program file will be stored

This project doesn't have a cmake file yet so the only way to run it on your device - is to build it manually via this command: 
```bash
g++ src/*.cpp -Iinclude -o build/db_test.exe
```

You can find this command at ```bash src/main.cpp``` at the end of the file if needed

## Usage

You can find your executable file in build directory and run it, after that you'll see a console window where you could print commands to use the program

## Commands list

```db-name``` - Requires database name which will be used to store data
```basah
db-name mydb
```

```create-database``` - Creates database folder inside DB_test directory

```create-table``` - Creates table with table scheme. Required parameters should be passed like this: create-table table_name;;name:string::age:int Dont pass id column it is adding automaticaly NOTE(althou you create table you still dont using. It could be usefull to create multiple tables before start using them)
```bash 
create-table users;;name:string::age:int::gender:char
```

```use-table``` - Switch between tables to use them
```bash
use-table users
```

```insert``` - Inserting data in form like this: name::10::m::true
```bash
insert Alice::26::f
```

```update``` - Updates passed parameter of the row. Requires row id, column name and value for this column
```bash
update 1::age::25
```

```show-table-data``` - Print table scheme and all table data

```delete-row``` - deletes row by it's id
```bash
delete-row 1
```

```help``` - Shows all commands list

```pause``` - Stops the program

```delete-all``` - Delete all rows from table and make _data.txt file empty after finishing work with current table

```drop-db``` - Delete selected database and all tables from it