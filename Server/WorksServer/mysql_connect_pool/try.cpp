/*
 * =====================================================================================
 *
 *       Filename:  try.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015年07月23日 08时41分15秒
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
#include <mysql_connect.h>
#include <unistd.h>

ConnectionPool *pool = ConnectionPool::getInstance();

int main(int argc, char *argv[])
{
    std::shared_ptr<Connection>con;
    Statement *state;
    ResultSet *result;

    con = pool->getConnect();
    state = con->createStatement();
    state->execute("use XL_db");
    result = state->executeQuery("select * from UserInfo;");
    while(result->next())
    {
        int id = result->getInt("uid");
        std::string name = result->getString("password");
        std::cout << "id:" << id << " name:" << name << std::endl; 
    }
    sleep(10);
    pool->retConnect(con);
    std::cout << pool->getPoolSize() << std::endl;
    sleep(10);

	return EXIT_SUCCESS;
}


