#include<iostream>
#include<set>
using namespace std;

void removeDuplicates( vector<int>& nums) {
    nums.erase(unique(nums.begin(), nums.end()), nums.end());
}

int main()
{
    vector<int> v = {1,1,2,2,3,4,4};
    removeDuplicates(v);
    for( int i : v ) {
        cout << i << endl;
    }
    return 0;
}