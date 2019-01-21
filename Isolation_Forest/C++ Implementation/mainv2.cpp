#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<iterator>
#include<algorithm>
#include<queue>
#include "isolation_forest.h"
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


void buildForest(vector<vector<double> > input,int k){
    vector<array<double,2> >data(input.size(),{0,0});

    int j=0;
    for(auto x:input){
        int i =0;
        for(auto y:x){
            data[j][i] = y;
            i++;  
        }
        j++;
    }
    iforest::IsolationForest<double, 2> forest;

	if (!forest.Build(50, 12345, data, 100))
	{
		std::cerr << "Failed to build Isolation Forest.\n";
		exit(1);
	}

	std::vector<double> anomaly_scores;

	if (!forest.GetAnomalyScores(data, anomaly_scores))
	{
		std::cerr << "Failed to calculate anomaly scores.\n";
		exit(2);
	}

	// for (int i = 0; i < anomaly_scores.size(); i++)
	// {
	// 	std::cout << "Anomaly_score[" << i << "] " << anomaly_scores[i] << "\n";
	// }

    //Adding anomaly score to the input vector
    // for(int i =0; i<input.size();i++){
    //     input[i].push_back(anomaly_scores[i]);
    // }
    // cout<<"Before sorting data"<<endl;

    // displayData(input);

    priority_queue<pair<double, int> > pq; 
	for (auto i = 0; i < anomaly_scores.size(); i++)
	 {
	 	pq.push(make_pair(anomaly_scores[i], i)); 
	    // cout << "Anomaly_score[" << i << "] " << anomaly_scores[i] << "\n";
	 }
	while(k--){
		pair<double, int> top = pq.top(); 
		// cout<<top.first<<" "<<top.second<<" ";

		for(int i=0; i<input[top.second].size(); i++){
			cout<<input[top.second][i]<<" ";
		}
        cout<<endl;
		pq.pop();
	}
    // cout<<"After sorting data"<<endl;
    // sort(input.begin(),input.end(),sortcol);
    // sort(input.begin(),input.end());
}

int main(){
    vector<vector<double> > input;
    int k;
    cin>>k;
    // cout<<k<<endl;
    input = readData();
    cout.precision(20);
    // displayData(input);
    validateInput(input);
    buildForest(input,k);
    
	return 0;

}