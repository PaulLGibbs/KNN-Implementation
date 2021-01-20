#include <iostream>
#include <fstream>
#include <CMATH>
#include <string> 
#include <algorithm>
#include <vector>
#include <random>
#include <time.h> 
using namespace std;

struct dataEntry
{
	int value;  //binary value, 0 = fail, 1 = pass
	double coord1, coord2; //data value 1 and 2, in this case will be numOfWarts and treatmentArea
	double distance; //distance from point being tested
};

//comparres data entries to allow for them to be sorted based on distance
bool comparison(dataEntry a, dataEntry b) 
{
	return (a.distance < b.distance);
}

//Params: 
//data[]: array of data entries 
//numEntries: number of data entries
//k: Number of elements the test point will be compared against
//testData: data that is to be tested against the training data
//classifies testData using k nearest neighbour algo. This func assumes two groups 
//and returns 0 for failed treatment and 1 for successful treatment 
int classifyADataEntry(vector<dataEntry> data, int k, dataEntry testData, int trainDataSize)
{
		//calc euclidean distance from testData to every point
	for (int i = 0; i < trainDataSize; i++) {
		data[i].distance = sqrt((data[i].coord1 - testData.coord1) * (data[i].coord1 - testData.coord1) +
								(data[i].coord2 - testData.coord2) * (data[i].coord2 - testData.coord2));
	}

	//sort all points low to high based on distance from testData
	sort(data.begin(), data.end(), comparison);

	//Using first k nearest neighbours we can classify the testData
	int frequency1 = 0; // for result 1, treatment failed
	int frequency2 = 0; // for result 2, treatment successful

	for (int i = 0; i < k; i++) {
		if (data[i].value == 0)
			frequency1++;
		else if (data[i].value == 1)
			frequency2++;
	}
	//cout << "Freq1: " << frequency1 << " Freq2: " << frequency2 << endl;
	return(frequency1 > frequency2 ? 0 : 1);
}

int main() 
{
	vector<dataEntry> dataVector;
	vector<dataEntry> testDataVector;
	ifstream readDataFile("data.txt");
	
	string buffer, subStrBuffer;
	double val1, val2;
	int result;
	//go through the file line by line, each line is stored in the buffer
	while (getline(readDataFile, buffer)) {
		//cout << buffer << " " << endl;
		subStrBuffer = buffer.substr(0, buffer.find('\t'));
		val1 = stoi(subStrBuffer);
		//cout << val1 << " " << endl;
		//cout << buffer << endl;
		buffer = buffer.erase(0, subStrBuffer.length()+1);
		//cout << buffer << endl;
		subStrBuffer = buffer.substr(0, buffer.find('\t'));
		val2 = stoi(subStrBuffer);
		//cout << val2 << " " << endl;
		buffer = buffer.erase(0, subStrBuffer.length()+1);
		subStrBuffer = buffer.substr(0, buffer.find('\t'));
		result = stoi(buffer);
		cout << val1 << " " << val2 << " " << result << endl;
		//go through the buffer string and assign the values to 
		dataEntry tempData;
		tempData.coord1 = val1;
		tempData.coord2 = val2;
		tempData.value = result;

		dataVector.push_back(tempData);
	}
	readDataFile.close();



	int trainDataSize = int(dataVector.size() * .9);
	int testDataSize = int(dataVector.size() * .1);
	srand(time(NULL));
	for (int i = 0; i < testDataSize; i++) {
		int randInt = (rand() % (dataVector.size() - 1));
		
		testDataVector.push_back(dataVector[randInt]);
		dataVector.erase(dataVector.begin() + randInt);
		cout << "Test data: " << i << " Coord1: " << testDataVector[i].coord1 << " Coord2: " << testDataVector[i].coord2 << endl;
	}

	//for loop that runs for k = 1 to size of testData vector size
	for (int j = 1; j < 1+testDataVector.size(); j++) {
		int k = j;
		int frequency1 = 0; // for result 1, treatment failed
		int frequency2 = 0; // for result 2, treatment successful
		//for loop to Check all the test data values against their given result
		for (int i = 0; i < testDataVector.size(); i++) {
			int testDataResult = classifyADataEntry(dataVector, k, testDataVector[i], trainDataSize);

			//printf("The value classified to unknown point is %d.\n", testDataResult);
			if (testDataVector[i].value == testDataResult)
				frequency1++;
			else
				frequency2++;

		}
		cout << endl << "K value =" << k << " KNN correct: " << frequency1 << " KNN wrong: " << frequency2;
	}

	return 0;
}