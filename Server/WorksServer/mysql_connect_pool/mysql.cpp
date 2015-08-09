/*
 * =====================================================================================
 *
 *       Filename:  mysql_connect.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年07月22日 23时47分12秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (wangweihao), 578867817@qq.com
 *        Company:  xiyoulinuxgroup
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>
#include <sstream>
//mysql driver
#include <mysql_connection.h>
#include <mysql_driver.h>
//mysql execute
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <exception>

using namespace std;
using namespace sql;

#define DBHOST "tcp://127.0.0.1:3306"
#define USER "root"
#define PASSWORD "w13659218813"

int main(int argc, char *argv[])
{
	Driver *driver;
    Connection *conn;
    ResultSet *result;
    Statement *state;

    driver = get_driver_instance();
    try{
        conn = driver->connect(DBHOST, USER, PASSWORD);
    } catch(std::runtime_error &e){
        perror("运行出错");
        exit(1);
    }
    
    state = conn->createStatement();
    state->execute("use XL_db;");
    result = state->executeQuery("select * from UserInfo;");
    while(result->next())
    {
        int id = result->getInt("uid");
        string name = result->getString("password");
        std::cout << "id:" << id << " name:" << name << std::endl;
    }
    delete conn;
    driver = NULL;
    conn = NULL;
    

	return EXIT_SUCCESS;
}


