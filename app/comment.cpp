#include "comment.h"

comment::comment(long commentId, long postId, std::string text){
  this->commentId = commentId;
  this->postId = postId;
   this->text = text;
}
