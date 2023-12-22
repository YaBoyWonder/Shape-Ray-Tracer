#ifndef _TOKENIZER_HPP
#define _TOKENIZER_HPP

#include <string>
#include <fstream>
#include <vector>

using std::string;
using std::ifstream;
using std::vector;

class Tokenizer {
    /** Given a text file, read it, and break its lines into
     * tokens, using whitespace.  Then, recognize whether each
     * token is a number or not.
     */
 public:
    /** Constructor.
     * @param filename Name of the text file to be tokenized.
     */
    Tokenizer(const string& filename);

    /** Return next token, as a string
     * @return The next token.
     */
    string next_string();

    /** Return next token, as a number.
     * Throws an invalid_argument if the token can't be converted to a float.
     * @return The token's value as a float.
     */
    float next_number();

    /** End of file
     * @return true/false if we have/haven't reached the end of the file.
     */
    bool eof();

    /** Get a string token, die if it doesn't equal the keyword.
     * Throws an invalid_argument if the token doesn't match.
     * @param pattern The keyword to be matched.
     */
    void match(const string& pattern);

    /** Report the current position in the input file.
     * @return The byte # and file name of the stream.
     */
    string file_position();

 private:
    void open(const string& filename);
    ifstream _stream;
    string _filename;
};

#endif
