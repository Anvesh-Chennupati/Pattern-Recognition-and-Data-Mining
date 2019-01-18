
#include<algorithm>
#include<algorithm>
#include<vector>
#include<random>
#include<iostream>

class isolationForest {
    int m_sampleSize;
	double m_precalculatedC;

    // vector<isolationTree>
    public:

    void clear(){
        m_sampleSize = 0;
        m_precalculatedC = 0;
    }

    // bool Build(int treeCount,)

};

using namespace std;

int main(){
    mt19937 gen(122);

    // cout<<gen;
    uniform_int_distribution<uint32_t> uniform_dis(0, std::numeric_limits<uint32_t>::max());
    // cout<< uniform_dis;
    // cout<<uniform_dis(gen);

    mt19937 wen(uniform_dis(gen));
    // int n =10;
    // while(n--){
    //     mt19937 wen(uniform_dis(gen));

    //     cout<<wen;

    //     cout<<endl;

    // }
    // cout<<wen;

    vector<int> maxs({1,2,3,4,5,6,7,8,9,10});

     for(auto x:maxs){

        cout<<x<< " ";
    }

    uint32_t n = 10;

    while(n--){
    
    shuffle(maxs.begin(), maxs.end(), wen);
    cout<<endl;
    for(auto x:maxs){

        cout<<x<< " ";
    }

    }
    

}