#pragma once

#include <string>
#include <map>
#include "Morse_tree.h"
#include "String_Tokenizer.h"
#include <fstream>
#include <sstream>
using std::map;
using std::string;


class MorseCoder {
public:
	
	//default constructor
	MorseCoder() {}

	//encode function
	std::string encode(const std::string& letters);

	//builds the morse tree
	MorseTree<string> build(const std::string& source_name);


private:

	//morse tree
	MorseTree<string> tree;

	//map used for encoding (letter, code)
	map<string, string> MorseMap;

	//string to hold data from file
	string source_name;
};

MorseTree<string> MorseCoder::build(const string& source_name) {
	tree.root = new BTNode<string>("");
	this->source_name = source_name;
	std::ifstream in(source_name.c_str());
	if (in) {
		string line;
		//read each line of the file
		while (getline(in, line)) {
			//get the letter from position 1
			string letter = line.substr(0, 1);
			//get the code from the rest of the line
			string code = line.substr(1);
			//insert
			MorseMap.insert({ letter, code });
			tree.insert(letter, code);

		}
	}
	//send the buildmap to morse tree build function
	MorseTree<string> local_tree = tree;
	return local_tree;
}

string MorseCoder::encode(const std::string& letters) {
	//split line by space if there more than one word
	string_tokenizer tokens(letters, " ");
	string result;
	//while there are more words
	while (tokens.has_more_tokens()) {
		//store the next word in a temp string
		string temp = tokens.next_token();
		//split the word by letters
		std::istringstream strm(temp);
		char next_char;
		while (strm >> next_char) {
			//reformat the char
			string letter;
			letter += next_char;
			//if the char is a valid letter
			if (MorseMap.find(letter) != MorseMap.end()) {
				//append its code and a space to result
				result += MorseMap[letter];
				result += " ";
			}
			else {
				throw Syntax_Error("Invalid Character Encountered");
			}
		}
	}
	return result;
}

