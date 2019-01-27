#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>

using namespace std;

int main()
{
    vector<string> s;
    s = {"ACQUIRE 20", "ACQUIRE 22", "RELEASE 22", "RELEASE 20", "max"};
    stack<string> stc;
    unordered_map<string, int> m1;
    for (int i = 0; i < s.size(); i++)
    {
        // cout<<"\n"<<s[i].substr(0, 7)<<endl;

        if ((string)s[i].substr(0, 7) == "ACQUIRE")
        {
            if (m1.find(s[i].substr(9, s[i].length())) != m1.end())
            {
                if (m1[s[i].substr(9, s[i].length())] > 1)
                {
                    cout << "acquire again";
                    exit(0);
                }
                // m1.erase(s[i]);
            }
            else
            {
                m1.insert({s[i].substr(9, s[i].length()), 1});
            }

            stc.push(s[i]);
        }

        else if (s[i].substr(0, 7) == "RELEASE")
        {

            if (m1.find(s[i].substr(9, s[i].length())) != m1.end())
            {
                if (m1[s[i].substr(9, s[i].length())] != 1)
                {
                    cout << "multi release";
                    exit(0);
                }
                else
                {
                    m1.erase(s[i].substr(9, s[i].length()));
                }

                string stemp = stc.top();

                if (s[i].substr(9, s[i].length()) == stemp.substr(9, s[i].length()))
                {
                    stc.pop();
                }
                else
                {
                    cout << "invalid relase order";
                    exit(0);
                }
            }
        }

        else
        {
            cout << "Error invalid input data";
            // exit(0);
        }
    }

    if (stc.size() == 0)
    {
        cout << "\n No error";
    }
    else
    {
        cout << "\n Error";
    }
}