// This was a brute-force attempt using DFS on a tree. It was obviously too slow.

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <iomanip>

using namespace std;

//solution.h
//-------------------------------------------------------------------------------
struct StackData
{
    int idx;
    int bit0;
    int bit1;
    bool equal;
};

struct aPair
{
    int* a0;
    int* a1;
    bool equal;
};

void intToBinaryArray(int num, int *array);
int binaryArrayToInt(int *array);
string binaryArrayToHex(int size, int *array);
int calcBit(int n, int size, int *a0, int *a1);
vector<aPair> bruteForceFront(int size, int *b);
vector<aPair> bruteForceBack(int size, int *b);
vector<string> mergeCompare(int size, int *b, vector<aPair> fronts, vector<aPair> backs);

//solution.cpp
//-------------------------------------------------------------------------------

void intToBinaryArray(int num, int *array)
{
    for(int i = 0; i < 32; i++)
    {
        array[i] = num & 1;
        num >>= 1;
    }
}

int binaryArrayToInt(int *array)
{
    int result = 0;
    for(int i = 31; i >= 0; i--)
    {
        result ^= array[i];
        result <<= 1;
    }

    return result;
}

string binaryArrayToHex(int size, int *array)
{
    std::stringstream sstream;
    string s = "";
    int temp;

    temp = binaryArrayToInt(array);
    sstream << setfill('0')
            << setw(8)
            << hex << temp;

    for(int i = 32; i < size; i += 32)
    {
        temp = binaryArrayToInt(array + i);
        std::stringstream sstream;
        sstream << ' '
                << setfill('0')
                << setw(8)
                << hex << temp;
    }

    return sstream.str();
}

int calcBit(int n, int size, int *a0, int *a1)
{
    int result = 0;
    if(n < size)
    {
        for(int j = 0; j <= n; j++)
        {
            result ^= a0[j] & a1[n - j];
        }
    }
    else
    {
        for(int j = size - 1; j > n - size; j--)
        {
            result ^= a0[j] & a1[n - j];
        }
    }
    
    return result;
}

vector<aPair> bruteForceFront(int size, int *b)
{
    vector<StackData> stack;
    int i;
    stack.push_back({0, 0, 0, true});
    stack.push_back({0, 0, 1, false});
    stack.push_back({0, 1, 1, true});

    int a0[size];
    int a1[size];
    vector<aPair> results;

    while(!stack.empty())
    {
        StackData d = stack.back();
        stack.pop_back();
        i = d.idx;
        a0[i] = d.bit0;
        a1[i] = d.bit1;

        if(b[i] != calcBit(i, size, a0, a1))
        {
            continue;
        }

        if(i + 1 == size / 2)
        {
            int *a0Copy = new int[size / 2];
            int *a1Copy = new int[size / 2];

            memcpy(a0Copy, a0, (size / 2) * sizeof(int));
            memcpy(a1Copy, a1, (size / 2) * sizeof(int));

            results.push_back({a0Copy, a1Copy, d.equal});
            continue;
        }

        stack.push_back({i + 1, 0, 0, d.equal});
        stack.push_back({i + 1, 0, 1, false});
        if(!d.equal)
        {
            stack.push_back({i + 1, 1, 0, false});
        }
        stack.push_back({i + 1, 1, 1, d.equal});
    }

    return results;
}

vector<aPair> bruteForceBack(int size, int *b)
{
    vector<StackData> stack;
    int i;
    stack.push_back({31, 0, 0, true});
    stack.push_back({31, 0, 1, false});
    stack.push_back({31, 1, 1, true});

    int a0[size];
    int a1[size];
    vector<aPair> results;

    while(!stack.empty())
    {
        StackData d = stack.back();
        stack.pop_back();
        i = d.idx;
        a0[i] = d.bit0;
        a1[i] = d.bit1;

        if(b[i + size - 1] != calcBit(i + size - 1, size, a0, a1))
        {
            continue;
        }

        if(i == size / 2)
        {
            int *a0Copy = new int[size / 2];
            int *a1Copy = new int[size / 2];

            memcpy(a0Copy, a0 + size/2, (size / 2) * sizeof(int));
            memcpy(a1Copy, a1 + size/2, (size / 2) * sizeof(int));

            results.push_back({a0Copy, a1Copy, d.equal});
            continue;
        }

        stack.push_back({i - 1, 0, 0, d.equal});
        stack.push_back({i - 1, 0, 1, false});
        if(!d.equal)
        {
            stack.push_back({i - 1, 1, 0, false});
        }
        stack.push_back({i - 1, 1, 1, d.equal});
    }

    return results;
}

vector<string> mergeCompare(int size, int *b, vector<aPair> fronts, vector<aPair> backs)
{
    vector<string> results;

    int a0[size];
    int a1[size];
    bool equal;
    bool flag;
    string s0;
    string s1;

    for(aPair front: fronts)
    {
        for(aPair back: backs)
        {
            memcpy(a0, front.a0, (size / 2) * sizeof(int));
            memcpy(a1, front.a1, (size / 2) * sizeof(int));
            memcpy(a0 + (size / 2), back.a1, (size / 2) * sizeof(int));
            memcpy(a1 + (size / 2), back.a1, (size / 2) * sizeof(int));
            flag = true;

            for(int i = size / 2; i < (size / 2) + size - 2; i++)
            {
                if(b[i] != calcBit(i, size, a0, a1))
                {
                    flag = false;
                    break;
                }
            }

            if(flag)
            {
                s0 = binaryArrayToHex(size, a0);
                s1 = binaryArrayToHex(size, a1);
                results.push_back(s0 + ' ' + s1);
                if(!(front.equal & back.equal))
                {
                    results.push_back(s1 + ' ' + s0);
                }
            }

            if(front.equal | back.equal)
            {
                continue;
            }

            memcpy(a0, front.a1, (size / 2) * sizeof(int));
            memcpy(a1, front.a0, (size / 2) * sizeof(int));
            flag = true;

            for(int i = size / 2; i < (size / 2) + size - 2; i++)
            {
                if(b[i] != calcBit(i, size, a0, a1))
                {
                    flag = false;
                    break;
                }
            }
            
            if(flag)
            {
                s0 = binaryArrayToHex(size, a0);
                s1 = binaryArrayToHex(size, a1);
                results.push_back(s0 + ' ' + s1);
                results.push_back(s1 + ' ' + s0);
            }
        }
    }

    sort(results.begin(), results.end());
    return results;
}

int main()
{
    int size;
    int temp;
    cin >> size; 
    cin.ignore();

    int *b = new int[2*size]();

    for(int i = 0; i < size/16; i++)
    {
        cin >> hex >> temp;
        cin.ignore();
        intToBinaryArray(temp, b + (i * 32));
    }

    cout << "Front" << endl;
    vector<aPair> frontHalf = bruteForceFront(size, b);
    vector<aPair> backHalf = bruteForceBack(size, b);
    
    vector<string> output = mergeCompare(size, b, frontHalf, backHalf);

    for(string s: output)
    {
        cout << s << endl;
    }
}