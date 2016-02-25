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


TEST(sqlite, date_time) {

   sqlite3 *db;
  
   sqlite3_open(":memory:", &db);

   
   const char* sql_create_table = "CREATE TABLE ware(" \
                          "id     INT   PRIMARY KEY NOT NULL," \
                          "time   DATETIME NOT NULL);";

   int rc;
   char *err_msg;
   sqlite3_stmt *pstmt;

   rc = sqlite3_exec(db, sql_create_table, NULL, NULL, &err_msg); 
   ASSERT_EQ(SQLITE_OK, rc);


   rc = sqlite3_exec(db, "INSERT INTO ware(id, time) values(1, 1)", NULL, NULL, NULL);
   ASSERT_EQ(SQLITE_OK, rc);
   rc = sqlite3_exec(db, "INSERT INTO ware(id, time) values(2, 2)", NULL, NULL, NULL);
   ASSERT_EQ(SQLITE_OK, rc);

   rc = sqlite3_exec(db, "INSERT INTO ware(id, time) values(3, '1970-01-01 00:00:09')", NULL, NULL, NULL);
   ASSERT_EQ(SQLITE_OK, rc);



   //query all   
   unsigned int count = 0;
   //sqlite3_prepare_v2(db, "SELECT * FROM ware where time > '1970-01-01 00:00:00' ", -1, &pstmt, NULL);
   sqlite3_prepare_v2(db, "SELECT * FROM ware where time > 2 ", -1, &pstmt, NULL);
   while((rc=sqlite3_step(pstmt)) != SQLITE_DONE) {
       ASSERT_EQ(SQLITE_ROW, rc);
       count++; 
   }
   sqlite3_finalize(pstmt);

   ASSERT_EQ(2, count);
}

