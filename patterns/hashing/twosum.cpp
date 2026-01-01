#include<iostream>
#include<unordered_map>
#include <vector>

using namespace std;

void twoSum(vector<int> nums) {
    int target = 9;
    unordered_map<int,int> mp;
    for(int i=0;i<nums.size();i++)
    {
        int need = target - nums[i];

        if(mp.count(need))
            cout<< mp[need]<<"\t"<<i;

        mp[nums[i]] = i;
    }
}

int main()
{
    vector<int> arr = {2,7,11,15};
    twoSum(arr);
    return 0;
}