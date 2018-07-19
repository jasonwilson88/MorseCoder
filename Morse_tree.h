#ifndef MORSE_TREE_H_
#define MORSE_TREE_H_

#include "Binary_Tree.h"
#include "String_Tokenizer.h"
#include "Morse_Coder.h"
#include "Syntax_Error.h"
#include <map>
#include <sstream>
#include <string>
using std::map;
using std::string;

template<typename Item_Type>
class MorseTree : public Binary_Tree<Item_Type> {
public:

	//Default Constructor
	MorseTree() : root(NULL) {}

	//Constructs a morse tree with given info
	MorseTree(const Item_Type& the_data,
		const MorseTree<Item_Type>& left_child
		= MorseTree(),
		const MorseTree<Item_Type>& right_child
		= MorseTree()) :
		root(new BTNode<Item_Type>(the_data, left_child.root,
			right_child.root)) {}


	bool insert(const Item_Type& item, const Item_Type& code);


	//Decode function 
	//Returns string
	string decode(const string& morse);
	
	//Builds tree with given root
	MorseTree(BTNode<Item_Type>* new_root) : root(new_root) {}

	// Data Field
	BTNode<Item_Type>* root;

private: 

	bool insert(BTNode<Item_Type>*& local_root, const Item_Type& item, const Item_Type& code, int current);

	string find_letter(const BTNode<Item_Type>* local_root, const string& code);

	bool is_dot(const char item) {
		if (item == '.')
			return true;
		else
			return false;
	}

	bool is_dash(const char item) {
		if (item == '_')
			return true;
		else
			return false;
	}


};  // End MorseTree

//decode wrapper function
template <typename Item_Type>
string MorseTree<Item_Type>::decode(const string& morse) {
	string result;
	//split the string to be decoded by letter codes
	string_tokenizer tokens(morse, " ");
	while (tokens.has_more_tokens()) {
		string temp = tokens.next_token();
		//determine each letter from code
		//and append it to result string
		result += find_letter(root, temp);
	}

	return result;
}

//function to find each individual letter
template <typename Item_Type>
string MorseTree<Item_Type>::find_letter(const BTNode<Item_Type>* local_root, const string& code) {
	//split the letter code by char
	std::istringstream strm(code);
	char next_char;
	while (strm >> next_char) {
		//if the char is a dot go left
		if (is_dot(next_char)) {
			local_root = local_root->left;
		}
		//if the char is a dash go right
		else if (is_dash(next_char)) {
			local_root = local_root->right;
		}
		else {
			throw Syntax_Error("Invalid character encountered");
		}
	}
	//once all char are processed convert the local root to string
	//and append it to the result
	string result = local_root->to_string();
	return result;
}
template <typename Item_Type>
bool MorseTree<Item_Type>::insert(const Item_Type& item, const Item_Type& code) {
	int current = 0;
	return insert(this->root, item, code, current);
}

template <typename Item_Type>
bool MorseTree<Item_Type>::insert(BTNode<Item_Type>*& local_root, const Item_Type& item,
	const Item_Type& code, int current) {
	//create dummy roots for letters that haven't been inserted yet
	if (local_root == NULL) {
		Item_Type dummy;
		local_root = new BTNode<Item_Type>(dummy);
	}
	//once you have reached destination insert data
	if (current == code.size()) {
			local_root->data = item;
			return true;
	}
	//if you're not at the final location recursively traverse tree
	else {
		if (is_dot(code[current])) {
			return insert(local_root->left, item, code, ++current);
		}
		else if (is_dash(code[current])) {
			return insert(local_root->right, item, code, ++current);
		}
		else
			return false;
	}
}
#endif


