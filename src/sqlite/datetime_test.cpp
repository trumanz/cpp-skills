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

#include "sqlite_helper.h"

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


//   rc = sqlite3_exec(db, "INSERT INTO ware(id, time) values(1, 1)", NULL, NULL, NULL);
//   ASSERT_EQ(SQLITE_OK, rc);
   rc = sqlite3_exec(db, "INSERT INTO ware(id, time) values(2, datetime('1970-01-01 00:00:02'))", NULL, NULL, NULL);
   ASSERT_EQ(SQLITE_OK, rc);
   rc = sqlite3_exec(db, "INSERT INTO ware(id, time) values(3, datetime('1970-01-01 00:00:03'))", NULL, NULL, NULL);
   ASSERT_EQ(SQLITE_OK, rc);


   std::vector< std::vector<boost::any> > rows;

   sqlite3_prepare_v2(db, "SELECT time FROM ware WHERE datetime(time) > datetime('1970-01-01 00:00:02') ", -1, &pstmt, NULL);
   fetch_all_rows(pstmt, &rows); 
   ASSERT_EQ(1, rows.size());
   ASSERT_EQ(1, rows[0].size());
   ASSERT_EQ("1970-01-01 00:00:03", boost::any_cast<std::string>(rows[0][0]) );
   //ASSERT_EQ(5, boost::any_cast<int>(rows[1][0]) );
   rows.clear();
   sqlite3_finalize(pstmt);


   rc = sqlite3_exec(db, "DROP TABLE ware", NULL, NULL, NULL);
   ASSERT_EQ(SQLITE_OK, rc);
   sqlite3_close(db);   
}

