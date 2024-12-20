#ifndef DATAMYSQL
#define DATAMYSQL
#include <iostream>
#include <mysqlx/xdevapi.h>
#include <string>
using nasmespace std;

class DataBase{
	public:
		static DataBase& getInstance(){
			static DataBase instance;
			return instacne;
		}
		DataBase(const DataBase &)=delete;
		DataBase &operator=(const DataBase&)=delete;

		
	private:
		DataBase();
		~DataBase();
		 DataBase instance;

}


#endif
