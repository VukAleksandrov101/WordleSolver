#include <bits/stdc++.h>
using namespace std;

#define DICT_LEN (2309)
#define WORD_LEN (5)

#define GUESS_CNT (10)

#define GUESSED true

class Trie {
	Trie* children[26] = { NULL };
	bool isEndOfWord = false;

	friend class WordGenerator;
public:
	void insert(const string& key) {
		Trie* node = this;

		for (int i = 0; i < key.length(); i++) {
			int index = key[i] - 'a';
			if (node->children[index] == NULL)
				node->children[index] = new Trie;

			node = node->children[index];
		}
		node->isEndOfWord = true;
	}

	bool find(const string& key) {
		Trie* node = this;

		for (int i = 0; i < key.length(); i++) {
			int index = key[i] - 'a';
			if (node->children[index] == NULL)
				return false;
			node = node->children[index];
		}

		return node->isEndOfWord;
	}
};

class WordGenerator {
	Trie* dictionary = new Trie;

	string lastGuess = "crane";

	// which letter is on which index, -1 if don't know
	char green[5] = { -1, -1, -1, -1, -1};
	// which letter is not on position but in word
	bool yellow[5][26] = {0};
	// which letter is not in word
	bool none[26] = {0};

	vector<string> possibleWords;

public:
	WordGenerator() {
		// Load dictionary
		ifstream file("dictionary.txt");

		string word;
		while (file >> word) {
			dictionary->insert(word);
		}

		file.close();
	}
	~WordGenerator() {delete dictionary;}

private:

	// saves all possible answers
	void dfs(Trie* node, string s = "") {
		if (s.length() == WORD_LEN) {
			possibleWords.push_back(s);
			return;
		}
		if(!s.length()) possibleWords.clear();

		if ( green[s.length()] != -1) {
			if (node->children[green[s.length()]] == NULL)
				return;

			char letter = 'a' + green[s.length()];
			dfs(node->children[green[s.length()]], s + letter);
			return;
		}

		for (int i = 0; i < 26; i++) {
			if (node->children[i] != NULL && !none[i] && !yellow[s.length()][i]) {
				char letter = 'a' + i;
				dfs(node->children[i], s + letter);
			}
		}
	}
public:
	string generateWord() {
		dfs(dictionary);
		lastGuess=possibleWords[0];
		cout<<"green: ";
		for(int i=0;i<5;i++) {
			if(green[i]!=-1) cout<<(char)(green[i]+'a')<<' ';
		}cout<<endl;
		cout<<"yellow: ";
		for(int i=0;i<5;i++) {
			for(int j=0;j<26;j++) {
				if(yellow[i][j]) cout<<(char)(j+'a')<<' ';
			}
		}cout<<endl;
		cout<<"none: ";
		for(int i=0;i<26;i++) {
			if(none[i]) cout<<(char)(i+'a')<<' ';
		}
		cout<<"Possible words: ";
		for(string s:possibleWords) cout<<s<<", ";
		cout<<endl;
		return lastGuess;
	}

	void addInfo(const string& info) {
		for(int i=0;i<WORD_LEN;i++) {
			if(info[i]=='g')
				green[i]=lastGuess[i]-'a';
			else if(info[i]=='y')
				yellow[i][lastGuess[i]-'a']=true;
			else {
				bool seen=false;
				for(int j=0;j<i;j++){
					if(yellow[j][lastGuess[i]-'a']) {
						seen=true;
						break;
					}
				}
				if(!seen) none[lastGuess[i]-'a']=true;	
			}
		}
	}
};

int main() {
	WordGenerator wordGenerator;

	for (int i = 0; i < GUESS_CNT; i++) {
		cout << wordGenerator.generateWord()<<'\n';
		string info;
		cin >> info;
		wordGenerator.addInfo(info);
	}
}