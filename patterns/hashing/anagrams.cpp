#include<iostream>
#include<string>
#include<vector>
using namespace std;

bool isAnagram( string s , string o ) {
    if(s.size() != o.size()) return false;
    vector<int> v(128,0);
    v.resize(128);
    for(int i=0;i<s.size();i++) {
        v[s[i]]++;
        v[o[i]]--;
    }

    for (int count : v) {
        if (count != 0) {
            return false;
        }
    }

    return true;


}

int main()
{
    string str = "listen";
    string chk = "silent";
    cout << isAnagram(str,chk);
    return 0;
}