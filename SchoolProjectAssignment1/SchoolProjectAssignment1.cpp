// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
using namespace std;
#include <vector>
int INT_M = 1000;
int idleTime = 0;
struct p
{
public:
	int totalIOTime = 0;
	int firstBurstTime = 0;
	int lastBurstTime = 0;
	int arrivalTime = 0;
	vector <vector <int>> cpuIO;
	string id;
};

void updateMyP(p &selectedP, int &currentArrivalTime,vector <p> &ready,vector <p> &IO, vector <p>& pValue)
{

	//selectedP.totalIOTime += selectedP.cpuIO[0][1];
	// update my current p values for averages
	int loc = 0;
	for (loc; loc < pValue.size(); loc++)
	{
		if (pValue[loc].id == selectedP.id)
		{
			break;
		}
	}
	pValue[loc].totalIOTime += pValue[loc].cpuIO[0][1];
	pValue[loc].totalIOTime += pValue[loc].cpuIO[0][0];
	//cout << pValue[loc].id << " : " << pValue[loc].arrivalTime << endl;
	if (pValue[loc].arrivalTime == 0)
	{
		//cout << "first burst" << endl;
		//cout << pValue[loc].id << endl;
		pValue[loc].firstBurstTime = currentArrivalTime - pValue[loc].cpuIO[0][0];
	}
	pValue[loc].arrivalTime = currentArrivalTime + pValue[loc].cpuIO[0][1];
	pValue[loc].lastBurstTime = currentArrivalTime;
	//cout << selectedP.arrivalTime << " my arrival time" << endl;
	// remove item from ready
	int it = 0;
	for (it; it < ready.size(); it++)
	{
		if (ready[it].id == selectedP.id)
		{
			break;
		}
	}
	ready.erase(ready.begin() + it);
	/*cout << "Current Ready Queue" << endl << "----------------------" << endl;
	for (int i = 0; i < ready.size(); i++)
	{
		cout << ready[i].id << " Burst " << ready[i].cpuIO[0][0] << " : " << "IO " << ready[i].cpuIO[0][1] << endl;
	}*/
	// add it to the queue.
	IO.push_back(pValue[loc]);
	/*cout << endl << "Current IO Queue" << endl << "----------------------" << endl;
	/for (int i = 0; i < IO.size(); i++)
	{
		cout << IO[i].id << " Burst " << IO[i].cpuIO[0][0] << " : " << "IO " << IO[i].cpuIO[0][1] << endl;
	}
	*/
	// remove the first item in the p queue
	pValue[loc].cpuIO.erase(pValue[loc].cpuIO.begin());
	/*cout << "Selected" << endl << "----------------------" << endl;
	for (int i = 0; i < selectedP.cpuIO.size(); i++)
	{
		for (int j = 0; j < 2; j++)
		{
			cout << selectedP.cpuIO[i][j] << endl;;
		}
	}
	*/
	ready.clear();
}

int nextBurst(vector <p> &ready, int &currentArrivalTime, vector <p> &pValue, vector <p> &IO)
{
	// select item P in ready queue for burst
	if (ready.empty())
	{
		cout << endl << "Next Selected Burst" << endl << "----------------------" << endl;
		cout << "IDLE" << endl << endl;;
		return currentArrivalTime;
	}
	p selectedP = ready[0];
	int lowestBurst = INT_M;
	for (int i = 0; i < ready.size(); i++)
	{
		if (ready[i].cpuIO[0][0] <= lowestBurst)
		{
			if (lowestBurst == ready[i].cpuIO[0][0])
			{

				if (ready[i].arrivalTime < selectedP.arrivalTime)
				{
					selectedP = ready[i];
					lowestBurst = ready[i].cpuIO[0][0];
					continue;
				}
			}
			else
			{
				selectedP = ready[i];
				lowestBurst = ready[i].cpuIO[0][0];
			}
		}

	}

	cout << endl << "Next Selected Burst" << endl << "----------------------" << endl;
	cout << "Current Arrival Time : " << currentArrivalTime << endl;
	cout << selectedP.id << " Burst Length : " << selectedP.cpuIO[0][0] << endl << endl << endl;


	currentArrivalTime += selectedP.cpuIO[0][0];
	//- updating my p values
	updateMyP(selectedP,currentArrivalTime,ready,IO,pValue);

	return currentArrivalTime;
}
void updateMyQueue(vector <p> &ready, int &currentArrivalTime, vector <p> &pValue, vector <p> &IO)
{
	//cout << endl << endl << pValue[8].id << " : " << pValue[8].arrivalTime << " ID/arrivalTime" << endl << endl;
	//cout << endl << endl << currentArrivalTime << " Current Arrival time" << endl << endl;
	// check all p's arrival times - move p's into ready state if ready
	for (int i = 0; i < pValue.size(); i++)
	{
		//cout << pValue[i].arrivalTime << "-Arrival time : Current Arrival Time-" << currentArrivalTime << endl;
		if (pValue[i].arrivalTime <= currentArrivalTime)
		{
			if (!pValue[i].cpuIO.empty())
			{
				ready.push_back(pValue[i]);
			}
			// check if item is in the IO blocking state queue, if it is, remove it.
			for (int j = 0; j < IO.size(); j++)
			{
				if (pValue[i].id == IO[j].id)
				{
					IO.erase(IO.begin() + j);
				}
			}
		}
	}
	if (ready.empty())
	{
		int smallest = INT_M;
		for (int i = 0; i < IO.size(); i++)
		{
			smallest = min(smallest, IO[i].arrivalTime);
		}
		//cout << "The queue is IDLE for " << smallest << " Time units" << endl
		int temp = smallest - currentArrivalTime;
		idleTime += temp;
		cout << idleTime << endl;
		//cout << smallest << endl;
		currentArrivalTime = smallest;
		//cout << currentArrivalTime << " new current arrival time" << endl;
	}



	cout << "Current Ready Queue" << endl << "----------------------" << endl;
	for (int i = 0; i < ready.size(); i++)
	{
		cout << ready[i].id << " Burst " << ready[i].cpuIO[0][0] << " : " << "IO " << ready[i].cpuIO[0][1] << endl;
	}
	cout << endl << "Current IO Queue" << endl << "----------------------" << endl;
	for (int i = 0; i < IO.size(); i++)
	{
		cout << IO[i].id << " : " << "IO " << IO[i].cpuIO[0][1] << endl;
	}
}

void calculateAverages(vector <p> pValue)
{
	double RT = 0;
	double TT = 0;
	double WT = 0;

	cout << "          RT    " << "TT      " << "WT     " << endl;
	for (int i = 0; i < pValue.size(); i++)
	{
		cout << endl << pValue[i].id << ":   ";
		RT += pValue[i].firstBurstTime;
		cout << "    " << pValue[i].firstBurstTime;
		TT += pValue[i].lastBurstTime;
		cout << "    " << pValue[i].lastBurstTime;
		WT += pValue[i].lastBurstTime - pValue[i].totalIOTime;
		int temp = pValue[i].lastBurstTime - pValue[i].totalIOTime;
		cout << "    " << temp << endl;
	}
	double util = (896 - idleTime);
	util /= 896;
	util *= 100;
	cout << endl;
	cout << "CPU UTILIZATION IS: " << util << " %" << endl;
	cout << "Average RT is : " << RT / 9 << endl;
	cout << "Average WT is : " << WT / 9 << endl;
	cout << "Average TT is : " << TT / 9 << endl;
	return;
}


void sjf(vector <p> &pValue, vector <p> &ready, vector <p> &IO)
{
	// declare initial arrival time - arrival = 0;
	int arrivalTime = 0;

	// ready() - put everything into the ready queue

	updateMyQueue(ready, arrivalTime, pValue, IO);
	arrivalTime = nextBurst(ready, arrivalTime, pValue, IO);
	while (arrivalTime <= 878)
	{
		updateMyQueue(ready, arrivalTime, pValue, IO); //- select item in ready and pass the arrival time
		arrivalTime = nextBurst(ready, arrivalTime, pValue, IO);
	}
	cout << "The Ending Arrival Time Is: " << arrivalTime << endl;
	calculateAverages(pValue);
}



int main()
{
	// declare vectors for ready/IO
	vector <p> ready;
	vector <p> IO;
	// declare p for p1-p8
	p p1, p2, p3, p4, p5, p6, p7, p8, p9;
	vector <vector <int>> val1{ {7,22}, {6,19} ,{12,44} , {8,21} , {10,37}, {5,24}, {6,44}, {7,43}, {8,0} };
	vector <vector <int>> val2{ {14,48}, {15,44} , {17,42}, {22,37}, {19,76}, {14,41}, {16,31}, {17,43}, {18,0}};
	vector <vector <int>> val3{ {8,43}, {7,41} , {6,45}, {8,21}, {9,35}, {14,18}, {5,26}, {3,31}, {6,0} };
	vector <vector <int>> val4{ {13,37}, {4,41} , {5,35}, {12,41}, {8,55}, {15,34}, {6,73}, {5,77}, {3,0} };
	vector <vector <int>> val5{ {6,34}, {7,21} , {5,44}, {6,32}, {7,28}, {3,48}, {11,44}, {6,33}, {3,28}, {4,0} };
	vector <vector <int>> val6{ {9,32}, {4,28} , {5,10}, {6,12}, {7,14}, {9,18}, {12,24}, {15,30}, {8,0} };
	vector <vector <int>> val7{ {14,46}, {17,41} , {11,42}, {15,21}, {4,32}, {7,19}, {16,33}, {10,0}};
	vector <vector <int>> val8{ {4,64}, {5,53} , {6,44}, {4,73}, {6,87}, {5,66}, {8,25}, {6,33}, {9,41}, {7,0} };
	vector <vector <int>> val9{ {13,37}, {8,41} , {7,27}, {12,29}, {5,27}, {6,18}, {3,33}, {4,62}, {6,0} };

	p1.cpuIO = val1;
	p1.id = "P1";
	p2.cpuIO = val2;
	p2.id = "P2";
	p3.cpuIO = val3;
	p3.id = "P3";
	p4.cpuIO = val4;
	p4.id = "P4";
	p5.cpuIO = val5;
	p5.id = "P5";
	p6.cpuIO = val6;
	p6.id = "P6";
	p7.cpuIO = val7;
	p7.id = "P7";
	p8.cpuIO = val8;
	p8.id = "P8";
	p9.cpuIO = val9;
	p9.id = "P9";
	vector <p> pValue{ p1,p2,p3,p4,p5,p6,p7,p8,p9};

	// call sjf function
	sjf(pValue,ready,IO);
}

