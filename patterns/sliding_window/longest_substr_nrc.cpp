#include<iostream>
#include<unordered_set>
using namespace std;

int main()
{
    string s = "abcabcbb";
    int left=0;
    int ans=0;
    unordered_set<int> set;
    for(int right=0;right<s.size();right++){
        while(set.count(s[right])) {
            set.erase(s[left]);
            left++;
        }

        set.emplace(s[right]);
        ans = max(ans,right-left+1);
    }
    cout << "Longest Substr:" << ans << endl;
    return 0;
}