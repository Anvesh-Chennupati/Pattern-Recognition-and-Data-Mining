#include<iostream>
#include<vector>
#include<string>
#include<iterator>
#include<sstream>

using namespace std;

int main(int argc, char const *argv[])
{
    //reading data from dat file
    vector<vector<double> > input;
    int k;
    cin>>k;
    cout<<k;

    while (!cin.eof())
    {

        string line;
        getline(cin, line);        
        istringstream iss( line );
        vector<double> temp;
        vector<string> row;

        while(iss){
            string word;
            if (!getline(iss, line, ','))
                    break;
                try {
                    temp.push_back(stod(line));
                }
                catch(exception e){
                    cout<<"\n some error";
                }
        }
        input.push_back(temp);
        // double number;
        // while ( iss >> number )
        //     temp.push_back( number );
        
        if (cin.fail())
        {
        //error
        break;
        }
    // input.push_back(temp);
    temp.resize(0);
    // cout << line << endl;
    }
    for(auto x:input){
        for(auto y:x){
            cout<<y<<" ";
        }
    cout<<endl;
    }
    return 0;
}
