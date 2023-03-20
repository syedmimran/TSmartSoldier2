#include "IK_Sqlite.h"


int IK_Sqlite::callback(void* parent, int argc, char** argv, char** azColName) {
	IK_Sqlite* sqlLite = (IK_Sqlite*)parent;
	for (int i = 0; i < argc; i++) {
		sqlLite->DbText[sqlLite->DbNumOfRows][i] = argv[i] ? argv[i] : "NULL";
	}
	sqlLite->DbNumOfRows++;
	return 0;
}
//-----------------------------------------------------------------------
bool IK_Sqlite::openDb(const char* dbName) {
	int rc = sqlite3_open(dbName, &db);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n",
			sqlite3_errmsg(db));
		sqlite3_close(db);
		return false;
	}
	return true;
}
//-----------------------------------------------------------------------
bool IK_Sqlite::closeDb() {
	if (db != nullptr) {
		sqlite3_close(db);
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------
int IK_Sqlite::readTableDb(const char* tablename) {
	char sql[100];
	int i = 0;
	sprintf(sql, "Select * From ");
	strcat_s(sql, tablename);
	DbNumOfRows = 0;
	char* err_msg = 0;
	DbText.clear();
	int rc = sqlite3_exec(db, sql, callback, this, &err_msg);

	if (rc != SQLITE_OK) {
		sqlite3_free(err_msg);
		sqlite3_close(db);
		return 0;
	}
	return DbNumOfRows;
}
//-----------------------------------------------------------------------
int IK_Sqlite::getInt(int row, int col) {
	return atoi(DbText[row][col].c_str());
}
//-----------------------------------------------------------------------
float IK_Sqlite::getFloat(int row, int col) {
	return atof(DbText[row][col].c_str());
}
//-----------------------------------------------------------------------
const char* IK_Sqlite::getString(int row, int col) {
	return DbText[row][col].c_str();
}
//-----------------------------------------------------------------------
int IK_Sqlite::getNumOfRows() {
	return DbText.size();
}





