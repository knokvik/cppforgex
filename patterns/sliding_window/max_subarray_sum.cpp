#include<iostream>
using namespace std;

int main()
{
    vector<int> nums = {1,2,3,4,5};
    int n = nums.size();
    int k = 3;
   int sum = 0;

    for(int i = 0; i < k; i++)
    {
        sum += nums[i];
    }

    int ans = sum;

    for(int i = k; i < n; i++)
    {
        sum += nums[i];
        sum -= nums[i-k];

        ans = max(ans,sum);
    }
    cout << ans;
        return 0;
}