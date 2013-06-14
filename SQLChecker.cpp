#include "SQLChecker.h"

sqlite3     *SQLChecker::db = NULL;
const char  *SQLChecker::filepath = NULL;

void SQLChecker::init(const char *path) {
    SQLChecker::filepath = path;
    open();
    createTable();
    close();
}
void SQLChecker::open() {
    if(sqlite3_open(filepath, &db) != 0)
        perror("Failed to open db");
}
void SQLChecker::close() {
    sqlite3_close(db);
}
void SQLChecker::createTable() {
    open();
    if (sqlite3_exec(db, "CREATE TABLE LOG (date VARCHAR(32), name VARCHAR(32), msg TEXT)", NULL, NULL, NULL))
        perror("Failed to create table...");
    close();
}
void SQLChecker::clearTable() {
    open();
    sqlite3_exec(db, "DELETE FROM LOG", NULL, NULL, NULL);
    close();
}

void SQLChecker::insertLog(const char *date, const char *name, const char *msg) {
    open();
    stringstream sstr;
    sstr << "INSERT INTO LOG (date, name, msg) VALUES ('"<< date <<"','"<< name <<"','"<< msg <<"')";

    if (sqlite3_exec(db, sstr.str().c_str(), NULL, NULL, NULL))
        perror("Failed to insert...");
	close();
}
string SQLChecker::getLastSeen(const char *name) {
    open();
    sqlite3_stmt *stmt;

	stringstream sstr;
	sstr << "SELECT * FROM LOG WHERE name = '" << name << "' ORDER BY date DESC LIMIT 1";
	sqlite3_prepare(db, sstr.str().c_str(), -1, &stmt, NULL);

    struct  tm   *timeinfo;
    time_t  t;

	stringstream sstr2;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
        t = sqlite3_column_int(stmt, 0);
	    timeinfo = localtime(&t);
		sstr2 << asctime(timeinfo) << (char*)sqlite3_column_text(stmt, 1) << " " << endl;
	} sqlite3_finalize(stmt);

    close();
	return sstr2.str();
}
string SQLChecker::getChatLog() {
    open();
    sqlite3_stmt *stmt;
    const char *tail;

    struct  tm   *timeinfo;
    time_t  t;

    if (sqlite3_prepare_v2(db, "SELECT * FROM LOG LIMIT 25", 1000, &stmt, &tail))
        createTable();
	stringstream sstr;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
        t = sqlite3_column_int(stmt, 0);
        timeinfo = localtime(&t);
        string strtime = asctime(timeinfo);
        strtime = strtime.erase(strtime.length()-1);

        sstr << strtime << " "
            << sqlite3_column_text(stmt, 1) << " "
			<< sqlite3_column_text(stmt, 2) << " || ";
	} sqlite3_finalize(stmt);
	close();
	return sstr.str();
}
