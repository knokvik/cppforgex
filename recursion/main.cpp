#include<iostream>
#include <vector>
using namespace std;

void printNto1( const vector<int>& arr ,int i , int s ) {
    if( s>i ) return;
    printNto1( arr , i , s+1);
    cout << arr[s] << " ";
}

void print1toN( const vector<int>& arr ,int i  ) {
    if( i<0 ) return;
    print1toN( arr , i-1);
    cout << arr[i] << " ";
}

int factorial( int n ) {    
    if( n==0 || n ==1 ) return 1;
    return n * factorial(n-1);
}

int power( int num , int power ) {
    if( power == 0 ) return 1;
    return num * num;
}

int main()
{
    vector<int> arr = {0,3,7,2,5,8,4,6,0,1};
    int sum;
    cout << power(3, 3);

    return 0;
}