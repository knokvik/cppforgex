#include<iostream>
#include <unordered_set>
using namespace std;

bool isDupilcates( int arr[] ) {
    unordered_set<int> seen;
    for( int x : seen ) {
        if(seen.count(x)) return true;
        seen.insert(x);
    }
    return false;
};

int main()
{
    int arr[] = {1,1,3,1};
    cout << isDupilcates(arr);
    return 0;
}