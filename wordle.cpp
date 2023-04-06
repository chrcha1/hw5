// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions her
void helper(int N, int idx, const string& in, const string& floating, const set<string>& dict, set<string>& res);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
		int N = in.length();
    int idx = 0;
    set<string> res;
    helper(N, idx, in, floating, dict, res);

    return res;
}

// Define any helper functions here
void helper(int N, int idx, const string& in, const string& floating, const set<string>& dict, set<string>& res)
{
    int dashCnt = count(in.begin(), in.end(), '-');
    string curr = in;
    string yellows = floating;

    // declaring base cases
    if (idx == N && floating.empty() && dict.find(in) != dict.end())
    {
        res.insert(curr);
        return;
    }
    else if (idx == N && floating.empty())
    {
        return;
    }

    // if dash
    if (curr[idx] == '-')
    {
        int numAlpha = dashCnt - yellows.size();

        // if numdashes > yellows.size()
        if (numAlpha > 0)
        {
            // try fill w whole alphabet
            for (char j = 'a'; j <= 'z'; j++)
            {
                curr[idx] = j;

                // if not floating char
                if (yellows.find(j) == string::npos)
                {
                    helper(N, idx + 1, curr, yellows, dict, res);
                }
                else
                {
                    // removing the floating char & recurse
                    yellows.erase(yellows.find(j), 1);
                    helper(N, idx + 1, curr, yellows, dict, res);

                    // resetting yellows
                    yellows = floating;
                }
            }
        }
        else if (numAlpha == 0)
        {
            // if no alpha char left, try floating
            for (size_t k = 0; k < yellows.size(); k++)
            {
                curr[idx] = yellows[k];
                yellows.erase(k, 1);
                helper(N, idx + 1, curr, yellows, dict, res);

                // resetting yellows
                yellows = floating;
            }
        }
    }
    else
    {
        // move to next char
        helper(N, idx + 1, curr, yellows, dict, res);
    }
}
