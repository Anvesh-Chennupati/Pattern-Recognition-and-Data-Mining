#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <array>
#include <random>
#include <memory>
#include <queue>

using namespace std;

//class definations

static inline double CalculateH(uint32_t i)
{
    return log(i) + 0.5772156649;
}

// Calculates the C(n) parameter needed for anomaly score calculation
static inline double CalculateC(uint32_t n)
{
    if (n > 2)
    {
        double h = CalculateH(n - 1);
        return double(2.0 * h) - (double(2.0 * (n - 1)) / double(n));
    }
    else if (n == 2)
    {
        return 1.0;
    }
    else
    {
        return 0.0;
    }
}

class IsolationForest;
class isolationTree
{
    friend class IsolationForest;

  public:
    class Node
    {
        uint32_t m_dimId;
        double m_dimSplitValue;
        uint32_t m_size;
        std::unique_ptr<Node> m_left;
        std::unique_ptr<Node> m_right;

      public:
        Node()
        {
            m_dimId = 0;
            m_dimSplitValue = 0;
            m_size = 0;
            m_left = nullptr;
            m_right = nullptr;
        }

        bool Build(mt19937 &rng, vector<vector<double>> &data, int first, int last, int depth, int maxDepth)
        {
            int DimensionCount = data.size();

            if (last < first || last >= data.size())
            {
                return false;
            }

            if (last - first < 1 || depth >= maxDepth)
            {
                m_size = (last - first) + 1;
                return true;
            }

            uniform_int_distribution<uint32_t> dimension_dis(0, DimensionCount - 1);

            double dim = m_dimId = dimension_dis(rng);

            // sort(data.begin() + first, data.begin() + last + 1, [&dim](const auto left, const auto right) { return (*left)[dim] < (*right)[dim]; });

            auto minVal = (data[first])[dim];
            auto maxVal = (data[last])[dim];

            if (minVal == maxVal)
            {
                m_size = (last - first) + 1;
                return true;
            }

            // static int32_t GenerateNext(std::mt19937& rng, int32_t min, int32_t max)
            //     {
            //         std::uniform_int_distribution<int32_t> value_dis(min, max);
            //         return value_dis(rng);
            //     }
            uniform_int_distribution<int32_t> value_dis(minVal, maxVal);
            m_dimSplitValue = value_dis(rng);
            uint32_t middle = first;

            for (middle = first; middle <= last; middle++)
            {
                if ((data[middle])[dim] >= m_dimSplitValue)
                {
                    break;
                }
            }

            if (middle == first)
            {
                m_size = (last - first) + 1;
                return true;
            }

            m_left = std::unique_ptr<Node>(new Node());
            m_right = std::unique_ptr<Node>(new Node());

            if (!m_left->Build(rng, data, first, middle - 1, depth + 1, maxDepth))
            {
                return false;
            }

            if (!m_right->Build(rng, data, middle, last, depth + 1, maxDepth))
            {
                return false;
            }

            return true;
        }

        // bool Serialize(std::ostream& os) const
        // {
        // 	os.write(reinterpret_cast<const char*>(&m_dimId), sizeof(uint32_t));
        // 	os.write(reinterpret_cast<const char*>(&m_dimSplitValue), sizeof(ValueType));
        // 	os.write(reinterpret_cast<const char*>(&m_size), sizeof(uint32_t));

        // 	if (!os.good())
        // 	{
        // 		return false;
        // 	}

        // 	if (!IsLeaf())
        // 	{
        // 		return m_left->Serialize(os) && m_right->Serialize(os);
        // 	}

        // 	return true;
        // }

        // bool Deserialize(std::istream& is)
        // {
        // 	is.read(reinterpret_cast<char*>(&m_dimId), sizeof(uint32_t));
        // 	is.read(reinterpret_cast<char*>(&m_dimSplitValue), sizeof(ValueType));
        // 	is.read(reinterpret_cast<char*>(&m_size), sizeof(uint32_t));

        // 	if (!is.good())
        // 	{
        // 		return false;
        // 	}

        // 	if (!m_size)
        // 	{
        // 		m_left = std::unique_ptr<Node>(new Node());
        // 		m_right = std::unique_ptr<Node>(new Node());

        // 		return m_left->Deserialize(is) && m_right->Deserialize(is);
        // 	}

        // 	return true;
        // }

        bool IsLeaf() const
        {
            return m_left == nullptr || m_right == nullptr;
        }

        double GetPathLen(const vector<double> &data, int currentDepth) const
        {
            if (IsLeaf())
            {
                return double(currentDepth) + CalculateC(m_size);
            }

            if (data[m_dimId] < m_dimSplitValue)
            {
                return m_left->GetPathLen(data, currentDepth + 1);
            }
            else
            {
                return m_right->GetPathLen(data, currentDepth + 1);
            }
        }
    };

         int m_sampleSize;
        unique_ptr<Node> m_root;

        void Clear()
        {
            m_sampleSize = 0;
            m_root.reset();
        }

        bool Build(uint32_t seed, vector<vector<double>> &data, int sampleSize)
        {
            Clear();

            if (!data.size() || !sampleSize || sampleSize > static_cast<uint32_t>(data.size()))
            {
                return false;
            }

            m_sampleSize = sampleSize;

            mt19937 gen(seed);

            vector<double> sampleIds;
            sampleIds.reserve(data.size());

            for (uint32_t i = 0; i < static_cast<uint32_t>(data.size()); i++)
            {
                sampleIds.push_back(i);
            }

            shuffle(sampleIds.begin(), sampleIds.end(), gen);

            vector<vector<double>> localData;
            localData.reserve(sampleSize);

            for (uint32_t i = 0; i < sampleSize; i++)
            {
                localData.push_back(data[sampleIds[i]]);
            }

            if (localData.empty())
            {
                return false;
            }

            // The tree height limit maxDepth is automatically set by
            // the subsampling size: maxDepth = ceiling(log2 sampleSize)
            // which is approximately the average tree height
            uint32_t maxDepth = static_cast<uint32_t>(ceil(log2(sampleSize)));

            m_root = std::move(std::unique_ptr<Node>(new Node()));

            return m_root->Build(gen, localData, 0, static_cast<uint32_t>(localData.size()) - 1, 0, maxDepth);
        }

        double GetPathLen(const vector<double>& data) const
		{
			if (!m_root)
			{
				return 0.0;
			}

			return m_root->GetPathLen(data, 0);
		}

		double GetAnomalyScore(const vector<double>& data) const
		{
			if (!m_root)
			{
				return -1.0;
			}

			return pow(2, -GetPathLen(data) / CalculateC(m_sampleSize));
		}

		bool GetAnomalyScores(const std::vector<vector<double> >& data, std::vector<double>& scores) const
		{
			scores.clear();

			if (!data.size())
			{
				return false;
			}

			if (!m_root)
			{
				return false;
			}

			scores.resize(data.size());

			for (auto i = 0; i < data.size(); i++)
			{
				scores[i] = pow(2, -GetPathLen(data[i]) / CalculateC(m_sampleSize));
			}

			return true;
		}

};

class IsolationForest
{
    friend class isolationTree;
    int lsamplesize;
    int lcalculateC;
    vector<isolationTree> mtree;

  public:
    void clearData()
    {
        lsamplesize = 0;
        lcalculateC = 0;
        mtree.clear();
    }

    bool Build(int treeCount, int randomgen, vector<vector<double>> data, int samplesize)
    {

        clearData();
        //checking if data being passed is correct
        
        if (!data.size() || !samplesize || samplesize > data.size())
        {
            return false;
        }
        // cout<<" here 1";
        lsamplesize = samplesize;
        lcalculateC = CalculateC(samplesize);
        
        vector<int> sampleIds;
        sampleIds.reserve(data.size());
        for (int i = 0; i<data.size(); i++)
        {
            sampleIds[i] = i;
            cout<<i<<endl;
        }

        //generating random numbers
        mt19937 gen(randomgen);
        uniform_int_distribution<uint32_t> uniform_dis(0, std::numeric_limits<uint32_t>::max());

        mtree.resize(treeCount);

        for (int i = 0; i < treeCount; i++)
        {
            cout<<i <<endl;
            if (!mtree[i].Build(uniform_dis(gen), data, samplesize))
            {
                return false;
            }

            return true;
        }
    }

    	double GetAnomalyScore(const vector<double>& data) const
		{
			double totalPathLen = 0;

			if (mtree.empty())
			{
				return -1.0;
			}

			for (const auto& t : mtree)
			{
				totalPathLen += t.GetPathLen(data);
			}

			double avgPathLen = totalPathLen / double(mtree.size());

			return pow(2, -avgPathLen / lcalculateC);
		}

		bool GetAnomalyScores(const std::vector<vector<double> >& data, std::vector<double>& scores) const
		{
            cout<<"herer";
			scores.clear();

			if (!data.size())
			{
				return false;
			}

			if (mtree.empty())
			{
				return false;
			}

			scores.resize(data.size());

			for (auto i = 0; i < data.size(); i++)
			{
				double totalPathLen = 0;

				for (const auto& t : mtree)
				{
					totalPathLen += t.GetPathLen(data[i]);
				}

				double avgPathLen = totalPathLen / double(mtree.size());

				scores[i] = pow(2, -avgPathLen / lcalculateC);
			}

			return true;
		}
};

//method definations

//read data from file

vector<vector<double>> readData()
{
    vector<vector<double>> input;

    while (!cin.eof())
    {
        string line;
        vector<double> instance;
        getline(cin, line);

        if (!line.length() < 1)
        {
            istringstream ss(line);

            while (ss)
            {
                string word;

                if (!getline(ss, word, ','))
                    break;

                try
                {
                    instance.push_back(stod(word));
                }
                catch (exception e)
                {
                    cout << "Invalid data: " << e.what();
                    exit(1);
                }
            }

            input.push_back(instance);
            instance.clear();
        }
    }

    return input;
}

// validate input data
void validateInput(vector<vector<double>> input)
{
    //checking for invalid instances with faulty dimensions
    vector<int> instLengths;
    for (auto x : input)
    {
        instLengths.push_back(x.size());
    }
    for (int i = 1; i < instLengths.size(); i++)
    {
        if (instLengths[i - 1] != instLengths[i])
        {
            // cout<<instLengths[i-1] <<" "<<instLengths[i];
            cout << "\nInvalid dimensions in input data";
            exit(1);
        }
    }
}

// building the forest and performing computation

void buildForest(vector<vector<double>> input, int k)
{
    // vector<array<double,2> >data(input.size(),{0,0});

    // int j=0;
    // for(auto x:input){
    //     int i =0;
    //     for(auto y:x){
    //         data[j][i] = y;
    //         i++;
    //     }
    //     j++;
    // }
    IsolationForest forest;
    
    if (!forest.Build(50, 12345, input, 100))
    {
        std::cerr << "Failed to build Isolation Forest.\n";
        exit(1);
    }

    std::vector<double> anomaly_scores;

    if (!forest.GetAnomalyScores(input, anomaly_scores))
    {
        std::cerr << "Failed to calculate anomaly scores.\n";
        exit(2);
    }

    for (int i = 0; i < anomaly_scores.size(); i++)
    {
        cout<<"\nhere";
    	std::cout << "Anomaly_score[" << i << "] " << anomaly_scores[i] << "\n";
    }

    //Adding anomaly score to the input vector
    // for(int i =0; i<input.size();i++){
    //     input[i].push_back(anomaly_scores[i]);
    // }
    // cout<<"Before sorting data"<<endl;

    // displayData(input);

    priority_queue<pair<double, int>> pq;
    for (auto i = 0; i < anomaly_scores.size(); i++)
    {
        pq.push(make_pair(anomaly_scores[i], i));
        // cout << "Anomaly_score[" << i << "] " << anomaly_scores[i] << "\n";
    }
    while (k--)
    {
        pair<double, int> top = pq.top();
        // cout<<top.first<<" "<<top.second<<" ";

        for (int i = 0; i < input[top.second].size(); i++)
        {
            cout << input[top.second][i] << " ";
        }
        cout << endl;
        pq.pop();
    }
    // cout<<"After sorting data"<<endl;
    // sort(input.begin(),input.end(),sortcol);
    // sort(input.begin(),input.end());
}

void displayData(vector<vector<double>> input)
{
    for (auto x : input)
    {
        for (auto y : x)
        {
            cout << y << " ";
        }
        cout << endl;
    }
}

int main()
{

    vector<vector<double>> input;
    int k;
    cin >> k;

    input = readData();

    //to display out with high precision
    cout.precision(20);

    //display input data
    displayData(input);

    //validate input data
    validateInput(input);

    //building forest
    buildForest(input, k);

    return 0;
}
