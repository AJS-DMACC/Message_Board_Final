#ifndef COMMENT_H
#define COMMENT_H
#include <string>

class comment
{
private:
    long commentId;
    long postId;
    std::string text;
public:
    comment(long, long, std::string);
    std::string getText(){ return text;}
};

#endif // COMMENT_H
