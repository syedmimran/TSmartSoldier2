#pragma once


#include "C:/Sqlite/sqlite3.h"
#include <map>
#include <string>


class IK_Sqlite
	{
	public:

		IK_Sqlite(const char* dbName) {
			openDb(dbName);
		}
		~IK_Sqlite() {
			closeDb();
		}

		int readTableDb(const char* tablename);
		int getInt(int row, int col);
		const char* getString(int row, int col);
		float getFloat(int row, int col);
		int getNumOfRows();
		std::map<int, std::map<int, std::string>> DbText;
		static int callback(void* parent, int argc, char** argv, char** azColName);
		int DbNumOfRows;

	private:
		sqlite3* db;
		bool openDb(const char* dbName);
		bool closeDb();


};


