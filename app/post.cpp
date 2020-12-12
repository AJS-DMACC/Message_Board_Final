#include "post.h"
#include <string>
using namespace std;

post::post(long postID, long userId, string title, string body)
{
    this->body = body;
    this->postId = postID;
    this->userId = userId;
    this->title = title;
}

void post::setBody(string body){
    this->body = body;
}
void post::setTitle(string title){
    this->title = title;
}
