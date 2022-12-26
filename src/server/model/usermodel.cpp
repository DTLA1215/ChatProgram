#include "usermodel.hpp"
#include "db.h"
#include <iostream>
using namespace std;

bool UserModel::insert(User &user)
{
    char sql[1024] = {0};
    sprintf(sql, "insert into user (name, password, state) values('%s', '%s', '%s')",
            user.GetName().c_str(), user.GetPWD().c_str(), user.GetState().c_str());

    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            user.SetID(mysql_insert_id(mysql.GetConnection()));
            return true;
        }
    }

    return false;
}

User UserModel::query(int id)
{
    char sql[1024] = {0};
    sprintf(sql, "select * from user where id = %d", id);

    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row != nullptr)
            {
                User user;
                user.SetID(atoi(row[0]));
                user.SetName(row[1]);
                user.SetPWD(row[2]);
                user.SetState(row[3]);
                mysql_free_result(res);
                return user;
            }
        }
    }

    return User();
}

bool UserModel::updateState(User user)
{
    char sql[1024] = {0};
    sprintf(sql, "update user set state = '%s' where id = %d",
            user.GetState().c_str(), user.GetID());

    MySQL mysql;
    if (mysql.connect())
    {
        if (mysql.update(sql))
        {
            return true;
        }
    }

    return false;
}

void UserModel::resetState()
{
    char sql[1024] = "update user set state = 'offline' where state = 'online'";

    MySQL mysql;
    if (mysql.connect())
    {
        mysql.update(sql);
    }
}