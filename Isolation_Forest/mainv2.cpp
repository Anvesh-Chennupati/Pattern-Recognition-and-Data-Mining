#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<iterator>

using namespace std;

vector<vector<double> > readData(){
    vector<vector<double> > input;

    while(!cin.eof()){
        string line;
        vector<double> instance;
        getline(cin,line);

        if(!line.length()<1){
                    istringstream ss(line);

        while(ss){
            string word;

            if(!getline(ss, word, ','))
                break;
            
            try{
                instance.push_back(stod(word));
            }
            catch(exception e){
                cout<<"Invalid data: "<<e.what();
                exit(1);
            }
        }

        input.push_back(instance);
        instance.clear();
        }

    }
    // input.erase(input.begin());
    return input;
}

void displayData(vector<vector<double> > input){
    for(auto x:input){
        for(auto y:x){
            cout<<y<<" ";
        }
    cout<<endl;
    }
}

void validateInput(vector<vector<double> > input){
    //checking for invalid instances with faulty dimensions
    vector<int> instLengths;
    for(auto x: input){
        instLengths.push_back(x.size());
    }
    for(int i =1;i<instLengths.size();i++){
        if(instLengths[i-1]!=instLengths[i]){
            // cout<<instLengths[i-1] <<" "<<instLengths[i];
            cout<<"\nInvalid dimensions in input data";
            exit(1);
        }
    }
}
int main(){
    vector<vector<double> > input;
    int k;
    cin>>k;
    // cout<<k<<endl;
    input = readData();
    // displayData(input);
    validateInput(input);
}