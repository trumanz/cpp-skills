#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <boost/any.hpp>
#include <boost/static_assert.hpp>

#include <vector>
#include <assert.h>
#include "sqlite_helper.h"

BOOST_STATIC_ASSERT_MSG(sizeof(boost::any) < 16, "If sizeof boost::any too larger, consider performance issue ");


void getcolumns(sqlite3_stmt* pstmt, std::vector<boost::any> *values) {

       int col_count = sqlite3_column_count(pstmt);
       values->resize(col_count, boost::any());
       for(int i = 0; i < col_count; i++) {
            int type = sqlite3_column_type(pstmt, i);
            //const char * columnName = sqlite3_column_name(pstmt, i);
//            printf("%s, %d\n", columnName, type);
            switch(type) {
                case SQLITE_INTEGER:
                    (*values)[i] = sqlite3_column_int64(pstmt, i);
                    break;
                case SQLITE_FLOAT:
                    (*values)[i]= sqlite3_column_double(pstmt, i);
                    break;
                case SQLITE_TEXT:
                    {
                      int len = sqlite3_column_bytes(pstmt, i);
                      const unsigned char *str = sqlite3_column_text(pstmt, i);
                      (*values)[i] = std::string(str, str+len);
                    }
                    break;
                case SQLITE_BLOB:
                    {
                        int len = sqlite3_column_bytes(pstmt, i);
                        const char *str = (const char *)sqlite3_column_blob(pstmt, i);
                        (*values)[i] = std::vector<char>(str, str+len);
                     }
                    break;
                case SQLITE_NULL:
                    break;
                default:
                    assert("handle type" == NULL);
            }
       }
}


void fetch_all_rows(sqlite3_stmt* pstmt, std::vector< std::vector<boost::any> > *rows) {
   int rc;
   while((rc=sqlite3_step(pstmt)) != SQLITE_DONE) {
       assert(SQLITE_ROW == rc);
       std::vector<boost::any> cells;
       getcolumns(pstmt, &cells);
       rows->push_back(cells);
   }

}


