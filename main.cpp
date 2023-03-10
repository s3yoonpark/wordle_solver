#include <bits/stdc++.h> 

using namespace std; 

void filter(string input, vector<string>& answer, vector<string>& instruction, set<char>& lst) {
	// work out grey 
	
	vector<int> grey = {1,2,3,4,5}; 

	for (string i : instruction) {
		int n = stoi(i.substr(0,1)); 
		grey.erase(remove(grey.begin(), grey.end(), n), grey.end()); 
	}

	for (int i : grey) {
		lst.insert(input[i-1]); 
	}	

	for (auto it = answer.begin(); it != answer.end(); it++) {
		string word = *it; 
		bool flag = false;
		for (int i : grey) {
			if (word.find(input[i-1]) != string::npos) {
				answer.erase(remove(answer.begin(), answer.end(), word), answer.end()); 
				it -= 1;  
				flag = true; 
				break; 
			}	
		}
		if (flag) continue; 
		for (string i : instruction) {
			int n = stoi(i.substr(0, 1)) - 1; 
			string colour = i.substr(1,1); 
			
			if (colour == "g") {
				if (word[n] != input[n]) {
					flag = true; 
					break; 
				}				
			} else {
				if (! (word.find(input[n]) != string::npos && word[n] != input[n])) {
					flag = true; 
					break;
				}	
			}
		}		

		if (flag) {
			answer.erase(remove(answer.begin(), answer.end(), word), answer.end()); 
			it -= 1; 
			continue;
		}	
	}
}

string recommendation(vector<string>& guess, vector<string>& answer, set<char>& green, set<char>& yellow, set<char>& grey){
	if (answer.size() == 1) return answer[0]; 
	unordered_map <char, int> mp; 
	for (string word : answer) {
		for (char i : word) {
			if (mp.find(i) == mp.end()) {
				mp[i] = 1; 
			} else {
				mp[i] += 1; 
			}	
		}
	}


	vector<int> nb;	
	for (auto& it : mp) {
		nb.push_back(it.second); 
	}
	sort(nb.begin(), nb.end()); 
	for (auto& it : mp) {
		int index = 1 + find(nb.begin(), nb.end(), it.second) - nb.begin(); 
		mp[it.first] = index;
		if (find(green.begin(), green.end(), it.first) != green.end()) {
			mp[it.first] = 0; 
		} else if (find(yellow.begin(), yellow.end(), it.first) != yellow.end()) {
			mp[it.first] = 0; 
		} else if (find(grey.begin(), grey.end(), it.first) != grey.end()){
			mp[it.first] = 0; 
		}
	}

	int maximum = 0; 
	string maximum_word; 	
	for (string word : guess) {
		int sum = 0; 
		set<char> s(word.begin(), word.end()); 
		for (char i : s) {
			sum += mp[i]; 
		}
		
		if (maximum < sum) {
			maximum = sum; 
			maximum_word = word; 
		}	
	}
	guess.erase(remove(guess.begin(), guess.end(), maximum_word), guess.end()); 
	return maximum_word; 
}



int main() {
	// speed optimisation 

	// getting the vectors	
	ifstream g("guessable.txt");	
	ifstream a("acceptables.txt"); 
	string temp; vector<string> guess; vector<string> answer;  
	while (getline(g, temp)) {
		guess.push_back(temp); 
	}
	while (getline(a, temp)) {
		answer.push_back(temp); 
	}
	g.close(); a.close(); 
	
	set<char> green; set<char> yellow; set<char> grey;  	
	for (int i = 1; i < 7; i++) {
		string input; int n;  
		cout << "#" << i << endl; 
		if (answer.size() <= 5) {
			cout << "POSSIBLE ANSWER(S)" << endl; 
			for (string s : answer) cout << "    " << s << endl; 
		}
		string recom = recommendation(guess, answer, green, yellow, grey); 	
		cout << "RECOMMENDED WORD : " << recom << endl; 
		cout << "YOUR GUESS" << endl; 
		cout << "-> ";   
		cin >> input;
		cout << "NUMBER OF COLOUR CHARACTER(S)" << endl; 
		cout << "-> "; 
		cin >> n; vector<string> instruction(n, "");  
		if (n == 5) {
			cout << "Nice! You got it!" << endl;
		   	break;	
		}
		for (int i = 1; i <= n; i++) {
			cout << "    " << i << ")" << endl; 
			cout << "    -> "; cin >> instruction[i-1];
		}
		for (string s : instruction) {
			int n = stoi(s.substr(0,1)) - 1; 
			string colour = s.substr(1,1); 
			if (colour == "g") {
				green.insert(input[n]); 	
			} else {
				yellow.insert(input[n]); 
			}	
		}
		filter(input, answer, instruction, grey); 
		cout << endl << endl; 
	}	
	return 0; 
}
