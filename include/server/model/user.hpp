#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User
{
    public:
        User(int id = -1, string name = "", string pwd = "", string state = "offline")
        {
            this->id = id;
            this->name = name;
            this->password = pwd;
            this->state = state;
        }

        void SetID(int id) {this->id = id;}
        void SetName(string name) {this->name = name;}
        void SetPWD(string password) {this->password = password;}
        void SetState(string state) {this->state = state;}

        int GetID() { return this->id; }
        string GetName() { return this->name; }
        string GetPWD() { return this->password; }
        string GetState() { return this->state; }

    protected:
        int id;
        string name;
        string password;
        string state;
};

#endif