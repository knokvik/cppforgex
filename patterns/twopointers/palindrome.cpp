#include<iostream>
using namespace std;

bool isPalindrome( string s ) {
    int r,l;
    l=0;r=s.size()-1;
    int limit = s.size()/2;
    for( int i=0;i<limit;i++) {
        if(s[r]==s[l]){
            r--;l++;
            continue;
        }
        return false;
    }
    return true;
}

int main()
{
    string s = "racecar";
    cout << isPalindrome(s);
    return 0;
}