#ifndef POST_H
#define POST_H


#include <string>
using namespace std;
class post
{
private:
   long postId;
   long userId;
   string title;
   string body;
public:
    post(long, long, string, string);
    string getTitle() {return title;}
    long getUserId(){return userId;}
    string getBody() {return  body;}
    long getPostId() {return postId;}

    void setTitle(string);
    void setBody(string);
};

#endif // POST_H
