#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>

#include <sqlite3.h>

//table  id, name, price , time


TEST(sqlite, table_search) {

   sqlite3 *db;
  
   sqlite3_open(":memory:", &db);

   
   const char* sql_create_table = "CREATE TABLE ware(" \
                          "id     INT   PRIMARY KEY NOT NULL," \
                          "name   TEXT  NOT NULL," \
                          "price  FLOAT NOT NULL," \
                          "time   DATETIME NOT NULL);";

   int rc;
   char *err_msg;
   rc = sqlite3_exec(db, sql_create_table, NULL, NULL, &err_msg); 
   if( rc != SQLITE_OK) {
        printf("SQL error: %s\n", err_msg);
   }


   sqlite3_exec(db, "BEGIN", NULL, NULL, NULL);

   const char *sql_insert = "INSERT INTO ware(id, name, price, time) values(?1, ?2, ?3, ?4);";
   sqlite3_stmt *pstmt;
   sqlite3_prepare_v2(db, sql_insert, -1, &pstmt, NULL); 
   for(int i = 0; i < 1000; i++) {
        int id= i;
        char name[50]; sprintf(name, "%d", i);
        float price = i;
        time_t  time = i;
        sqlite3_bind_int(    pstmt, 1, id );
        sqlite3_bind_text(   pstmt, 2, name, -1,  SQLITE_TRANSIENT); //SQLITE_TRANSIENT, sqlite make its own private copy
        sqlite3_bind_double( pstmt, 3, price );
        sqlite3_bind_int64(  pstmt, 4, time );
    
        rc = sqlite3_step(pstmt);
        ASSERT_EQ(rc, SQLITE_DONE);
        sqlite3_reset(pstmt);
   }

   sqlite3_finalize(pstmt);
   
   sqlite3_exec(db, "INSERT INTO ware(id, name, price, time) values('100000', '1000000', '100000', datetime('1970-01-01 00:00:00'))", NULL, NULL, NULL);
   sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);

   //query all   
   unsigned int count = 0;
   sqlite3_prepare_v2(db, "SELECT * FROM ware where id = '100000'", -1, &pstmt, NULL);
   while((rc=sqlite3_step(pstmt)) != SQLITE_DONE) {
       ASSERT_EQ(rc, SQLITE_ROW);
       count++; 
       int col_count = sqlite3_column_count(pstmt);
       for(int i = 0; i < col_count; i++) {
            int type = sqlite3_column_type(pstmt, i);
            printf("%d, type=%d\n", i, type);
            const char * columnName = sqlite3_column_name(pstmt, i);
            if(type == SQLITE_INTEGER) {
                 int value = sqlite3_column_int(pstmt, i);
                 printf("%s=%d\n", columnName, value);
            }
       }
   }
   sqlite3_finalize(pstmt);

   ASSERT_EQ(1000, count);
}

