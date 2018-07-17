#include "String_Tokenizer.h"
using std::string;

/** Position start and end so that start is the index of the start
    of the next token and end is the end.
*/
void string_tokenizer::find_next() {
  // Find the first character that is not a delimeter
  /*<snippet id="1" omit="false">*/
  start = the_source.find_first_not_of(the_delim, end);
  /*</snippet>*/
  // Find the next delimeter
  /*<snippet id="2" omit="false">*/
  end = the_source.find_first_of(the_delim, start);
  /*</snippet>*/
}

/** Determine if there are more tokens
    @return true if there are more tokens
*/
bool string_tokenizer::has_more_tokens() {
  return start != string::npos;
}

/** Retrieve the next token 
    @return the next token. If there are no more
    tokens, an empty string is returned
*/
string string_tokenizer::next_token() {
  // Make sure there is a next token
  if (!has_more_tokens())
    return "";
  // Save the next token in return_value
  /*<snippet id="3" omit="false">*/
  string token = the_source.substr(start, end - start);
  /*</snippet>*/
  // Find the following token
  find_next();
  // Return the next token
  return token;
}

