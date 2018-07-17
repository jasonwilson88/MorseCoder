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

	//Build tree function called by Coder
	MorseTree<Item_Type> build_tree(map<string, string>& morse_map);

	//Decode function 
	//Returns string
	string decode(const string& morse);
	
	//Builds tree with given root
	MorseTree(BTNode<Item_Type>* new_root) : root(new_root) {}

	// Data Field
	BTNode<Item_Type>* root;

private: 

	MorseTree<Item_Type> build_subtree(map<string, string>& local_map, string& code,
		map<string, string>::iterator current);

	string find_letter(const BTNode<Item_Type>* local_root, const string& code);

	bool has_left_subtree(std::string& code, map<string, string>& local_map) {
		std::string temp = code + ".";
		return local_map.find(temp) != local_map.end();
	}

	bool has_right_subtree(std::string& code, map<string, string>& local_map) {
		std::string temp = code + "_";
		return local_map.find(temp) != local_map.end();
	}

	std::string get_left_code(std::string code) {
		return code += ".";
	}

	std::string get_right_code(std::string code) {
		return code += "_";
	}


};  // End MorseTree

//Public tree building wrapper function
template <typename Item_Type>
MorseTree<Item_Type> MorseTree<Item_Type>::build_tree(map<string, string>& morse_map) {
	auto i = morse_map.begin();
	string code = "";
	MorseTree<Item_Type> tree = build_subtree(morse_map, code, i);
	setRoot(tree.getRoot());
	return tree;
}

//build subtree
template <typename Item_Type>
MorseTree<Item_Type> MorseTree<Item_Type>::build_subtree(map<string, string>& local_map, string& code, 
	map<string, string>::iterator current) {
	//terminates build subtree once all items have been used
	if (current == local_map.end()) {
		string dummy = 0;
		return MorseTree<Item_Type>(dummy, NULL, NULL);
	}
	//build leaf nodes
	if (code.size() == 4 ||
		(!has_left_subtree(code, local_map) && !has_right_subtree(code, local_map))) {
		return MorseTree<Item_Type>(local_map[code], NULL, NULL);
	}
	//build internal nodes
	if (has_left_subtree(code, local_map) && has_right_subtree(code, local_map)) {
		string left_code = get_left_code(code);
		MorseTree<Item_Type> left_tree = build_subtree(local_map, left_code, current++);
		string right_code = get_right_code(code);
		MorseTree<Item_Type> right_tree = build_subtree(local_map, right_code, current++);
		return MorseTree<Item_Type>(local_map[code], left_tree, right_tree);
	}
	//build nodes that only have one child
	if (has_left_subtree(code, local_map) && !has_right_subtree(code, local_map)) {
		string left_code = get_left_code(code);
		MorseTree<Item_Type> left_tree = build_subtree(local_map, left_code, current++);
		MorseTree<Item_Type> right_tree;
		return MorseTree<Item_Type>(local_map[code], left_tree, right_tree);
	}
	//build nodes that only have one child
	if (!has_left_subtree(code, local_map) && has_right_subtree(code, local_map)) {
		MorseTree<Item_Type> left_tree;
		string right_code = get_right_code(code);
		MorseTree<Item_Type> right_tree = build_subtree(local_map, right_code, current++);
		return MorseTree<Item_Type>(local_map[code], left_tree, right_tree);
	}

}
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
		if (next_char == '.') {
			local_root = local_root->left;
		}
		//if the char is a dash go right
		else if (next_char == '_') {
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
#endif


