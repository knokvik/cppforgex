#include<iostream>
#include<map>
using namespace std;

int main()
{
    map<char,int> mp;
    string value = "aaabbeeecc";
    for( char c : value ) {
        mp[c]++;
    }
    for( const auto& i : mp ) {
        cout << i.first << "\t" << i.second << endl;
    }
    return 0;
}


