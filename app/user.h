#ifndef USER_H
#define USER_H

#include <string>
using namespace std;
class user
{
private:
   long userId;
   string username;
   string password;
public:
    user(long, string, string);
    string getUsername() {return username;}
    string getPassword() {return  password;}
    long getUserId() {return userId;}

    void setUsername(string);
    void setPassword(string);
};

#endif // USER_H
