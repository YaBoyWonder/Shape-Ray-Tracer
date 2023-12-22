#include "tokenizer.hpp"
#include <iostream>
#include <sstream>
#include <exception>

using std::invalid_argument;
using std::stringstream;
using std::endl;
using std::cerr;

string downcase(const string &s) {
    string result = "";
    for (int i=0; i<(int)s.length(); i++) {
        char c = s[i];
        if ('A' <= c && c <= 'Z')
            c = c - 'A' + 'a';
        result += c;
    }
    return result;
}

Tokenizer::Tokenizer(const string& filename) {
    open(filename);
}

void Tokenizer::open(const string& filename) {
    _stream.open(filename);
    if (!_stream.is_open()) {
        cerr << "Can't read from " << filename << endl;
        exit(EXIT_FAILURE);
    }
    _filename = filename;
}

// Return next token, as a string
string Tokenizer::next_string() {
    string token;
    _stream >> token;
    return downcase(token);
}

string Tokenizer::file_position() {
    // Where are we?
    int byte_position = (int)_stream.tellg();
    // Open a new stream, and count lines up to where we are.
    ifstream new_stream(_filename);
    int line_num = 0;
    while (new_stream) {
        string line;
        getline(new_stream, line);
        line_num++;
        if (new_stream.tellg() > byte_position)
            break;
    }
    new_stream.close();
    // Report the result.
    stringstream ss;
    ss << "line " << line_num << " of file \"" << _filename + "\"";
    return ss.str();
}

void Tokenizer::match(const string& pattern) {
    string token = next_string();
    if (token != string(pattern)) {
        throw invalid_argument("match expected \"" + pattern + "\", got \""
                               + token + "\""
                               + "\n at" + file_position());
    }
}

// Return next token, as a number
float Tokenizer::next_number() {
    string token;
    _stream >> token;
    // This, from stackoverflow: convert string to float, detect error
    stringstream ss(token);
    float number;
    ss >> number;
    if (ss.fail()) {
        throw invalid_argument("can't convert \"" + token + "\" to float"
                               + "\n at" + file_position());
    }
    return number;
}

bool Tokenizer::eof() {
    return _stream.eof();
}
