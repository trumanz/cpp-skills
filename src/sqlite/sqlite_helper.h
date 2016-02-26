
#ifndef SQLITE_HLPER_H
#define SQLITE_HLPER_H

#include <sqlite3.h>
#include <boost/any.hpp>

void getcolumns(sqlite3_stmt* pstmt, std::vector<boost::any> *values);
void fetch_all_rows(sqlite3_stmt* pstmt, std::vector< std::vector<boost::any> > *rows);

#endif
