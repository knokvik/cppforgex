#include<iostream>
#include <unordered_map>
#include <string>
using namespace std;

void freq( string s ) {
    unordered_map<char,int> map;
    for( char c : s ) map[c]++;
    for( auto i : map) {
        cout << i.first << "\t" << i.second << endl;
    }
}

int main()
{
    string str = "abdjsoadaasdcashc";
    freq(str);
    return 0;
}