#include <iostream>
#include <stdint.h>
#include <random>
#include <vector>
#include <string>
#include "isolation_forest.h"

using namespace std;

int main(int argc, char* argv[])
{
	std::mt19937 rng(12345);
	std::vector<std::array<uint32_t, 4>> data;

	// generate some random 4D datapoints
	for (uint32_t i = 0; i < 1000; i++)
	{
		std::array<uint32_t, 4> temp;
		for (uint32_t j = 0; j < 4; j++)
		{
			temp[j] = iforest::UniformRandomNumber<uint32_t>::GenerateNext(rng, 0, 1000);
		}
		data.push_back(temp);
	}

    // cout<<"\n Generated Data: "<<endl;
    // for(auto x:data){
    //     cout<<"\n";
    //     for(auto y:x){
    //         cout<< " "<<y;;
    //     }
    // }

	// add a few anomalies
	for (uint32_t i = 0; i < 5; i++)
	{
		std::array<uint32_t, 4> temp;
		for (uint32_t j = 0; j < 4; j++)
		{
			temp[j] = iforest::UniformRandomNumber<uint32_t>::GenerateNext(rng, 800, 1500);
		}
		data.push_back(temp);
	}

	iforest::IsolationForest<uint32_t, 4> forest;

	if (!forest.Build(50, 12345, data, 100))
	{
		std::cerr << "Failed to build Isolation Forest.\n";
		return 1;
	}

	std::vector<double> anomaly_scores;

	if (!forest.GetAnomalyScores(data, anomaly_scores))
	{
		std::cerr << "Failed to calculate anomaly scores.\n";
		return 2;
	}

	for (uint32_t i = 995; i < 1005; i++)
	{
		std::cout << "Anomaly_score[" << i << "] " << anomaly_scores[i] << "\n";
	}

	return 0;
}