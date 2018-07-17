#include "Morse_Coder.h"
#include <iostream>

using namespace std;

int main() {

	MorseTree<string> tree;
	MorseCoder coder;
	tree = coder.build("morse.txt");

	string encoded = coder.encode("string");

	string decoded = tree.decode(encoded);

	cout << decoded << endl;


	return 0;
}