#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "gtest/gtest.h"
#include <boost/utility.hpp>

#include <sqlite3.h>
#include "boost/date_time/posix_time/posix_time.hpp"
using namespace boost::posix_time;
//table  id, name, price , time


class  SqliteWare :  boost::noncopyable{
private:
    sqlite3 *db;
    sqlite3_stmt *istmt;
public:
    SqliteWare() {
       istmt = NULL;
       sqlite3_open(":memory:", &db);
       const char* sql_create_table = "CREATE TABLE ware(" \
                                         "id     INT   PRIMARY KEY NOT NULL," \
                                         "name   TEXT  NOT NULL," \
                                         "price  FLOAT NOT NULL," \
                                         "time   DATETIME NOT NULL);";
       int rc;
       char *err_msg;
       rc = sqlite3_exec(db, sql_create_table, NULL, NULL, &err_msg); 
       assert(rc == SQLITE_OK); 
       rc = sqlite3_exec(db, "CREATE INDEX priceIndex ON ware(price)", NULL, NULL, &err_msg); 
       assert(rc == SQLITE_OK); 
       rc = sqlite3_exec(db, "CREATE INDEX timeIndex ON ware(time)", NULL, NULL, &err_msg); 
       assert(rc == SQLITE_OK); 
   }
   ~SqliteWare() {
        if(istmt)sqlite3_finalize(istmt);
        if(db)sqlite3_close(db);
   }


   void insert(int id, const char *name, float price, time_t timestamp) { 
        sqlite3_exec(db, "BEGIN", NULL, NULL, NULL);
        if(istmt == NULL) { 
             const char *sql_insert = "INSERT INTO ware(id, name, price, time) values(?1, ?2, ?3, ?4);";
             sqlite3_prepare_v2(db, sql_insert, -1, &istmt, NULL); 
        }
        sqlite3_bind_int(    istmt, 1, id );
        sqlite3_bind_text(   istmt, 2, name, -1,  SQLITE_TRANSIENT); //SQLITE_TRANSIENT, sqlite make its own private copy
        sqlite3_bind_double( istmt, 3, price );
        sqlite3_bind_int64(  istmt, 4, timestamp );
        int rc = sqlite3_step(istmt);
        assert(rc == SQLITE_DONE);
        sqlite3_reset(istmt);
        sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);
   }

  
   int query(float min_price, float max_price, time_t min_timestamp, time_t max_timestamp) {
        sqlite3_stmt *stmt;
        int count = 0;
        int rc;
        char sql[200];
        sprintf(sql, "SELECT * FROM ware WHERE (price BETWEEN %f AND %f) AND (time BETWEEN %ld AND %ld)", min_price, max_price, min_timestamp, max_timestamp);
        sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        while((rc=sqlite3_step(stmt)) != SQLITE_DONE) {
            assert(rc == SQLITE_ROW);
            count++;
        }
        sqlite3_finalize(stmt);
        return count;
   }  

};


class  MemoryWare :  boost::noncopyable{
private:
   class Row{
       public:
           Row(int id, const char *name, float price, time_t timestamp) { 
                 this->id = id;
                 this->name = name;
                 this->price = price;
                 this->timestamp = timestamp;
           }
           int id;
           std::string name;
           float price;
           time_t timestamp;
   };
   //std::list<Row*> rows;
   std::multimap<float, Row*> price_row_mulmap;
public:
   MemoryWare() {
   }
   ~MemoryWare() {
        for(std::multimap<float, Row*>::iterator it = price_row_mulmap.begin(); it != price_row_mulmap.end(); it++) delete (it->second);
   }

   void insert(int id, const char *name, float price, time_t timestamp) { 
         Row* r = new Row(id, name, price, timestamp);
         //rows.push_back(r);
         price_row_mulmap.insert(std::pair<float, Row*>(price, r));
   }

  
   int query(float min_price, float max_price, time_t min_timestamp, time_t max_timestamp) {
        std::multimap<float, Row*>::iterator it1 = price_row_mulmap.lower_bound(min_price);
        std::multimap<float, Row*>::iterator it2 = price_row_mulmap.upper_bound(max_price);
        int count = 0;
        for(; it1 != it2; it1++) {
            if(it1->second->timestamp >= min_timestamp && it1->second->timestamp <= max_timestamp) count++;
        }
        return count;
   }  

};


TEST(sqlite, table_search) {
    SqliteWare sqlite_ware;
    MemoryWare mem_ware;
    time_duration duration;
    for(int base_price = 1,  base_time = 200, i = 0; i < 100*1000; i++) {
         float  price = 0.0 + (base_price++)%487;
         time_t time  = (base_time++)%987;
         mem_ware.insert(i, "dummy", price, time);
         //mem_ware.insert(i, "dummy", 0.0 + i, i);
         sqlite_ware.insert(i, "dummy", price, time);
    }
    ptime t1 =  microsec_clock::local_time();
    int rc1 =   mem_ware.query(200.0, 300.0, 100, 600);
    duration = microsec_clock::local_time() - t1;
    printf("memory search used: %s\n", to_simple_string(duration).c_str());

    t1 =  microsec_clock::local_time();
    int rc2 =   sqlite_ware.query(200.0, 300.0, 100, 600);
    duration = microsec_clock::local_time() - t1;
    printf("sqlite search used: %s\n", to_simple_string(duration).c_str());

    ASSERT_EQ(rc1, rc2);
}

