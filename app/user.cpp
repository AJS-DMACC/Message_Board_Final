#include "user.h"
#include <string>
using namespace std;

user::user(long userID, string userName, string Password)
{
    this->username = userName;
    this->password = Password;
    this->userId = userID;
}

void user::setUsername(string userName){
    this->username = userName;
}
void user::setPassword(string Password){
    this->password = Password;
}
