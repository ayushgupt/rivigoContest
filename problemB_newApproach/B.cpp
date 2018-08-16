//
// Created by ayush on 12/7/17.
//

#include <bits/stdc++.h>
using namespace std;
//freopen("input.txt", "r", stdin);
//freopen("output.txt", "w", stdout);
#define DEBUG2(x) do { std::cerr << #x << ": " << x << std::endl; } while (0)
// Useful functions
#define bitcount(x) __builtin_popcount(x) 	// returns the number of 1-bits in x.
#define gcd(a,b) __gcd(a,b)					// gcd of a & b
#define max2(x)  __builtin_ctz(x)  			// returns max power of 2

//priority_queue --> top() returns largest element by default

//string2int= STOI
//int2string= TO_STRING


string tripStr(string cArg,string dArg)
{
	return (cArg+"|"+dArg);
}

int costVal(int val)
{
	if(val<=10)
	{
		return val;
	}
	else
	{
		return 10+(val-10)*2;
	}
}

vector<string> splitString(string input)
{
	std::istringstream ss(input);
	std::string token;

	vector<string> vecReq;
	while(std::getline(ss, token, ' ')) 
	{
		vecReq.push_back(token);
	};
	return vecReq;
}

struct truckNode
{
	int tid;
	string startPt;
	int startingTime;
	string currPt;
	int reachedCurrAt;
	int waitTimeAlloted;
	int waitTimeLeft;
	bool aheadMoving;
	bool matchedAlready=false;
	bool driverCreate=false;
	bool operator<(const truckNode &rhs)const
    {
        return reachedCurrAt>rhs.reachedCurrAt;
    }
};

bool myfunction (truckNode i,truckNode j) 
{ 
	return (i.startingTime<j.startingTime); 
}


struct driverNode
{
	int reachedAt;
	int reachedTripId;
	bool operator<(const driverNode &rhs)const
    {
        return reachedAt>rhs.reachedAt;
    }
};

int main()
{
	std::mt19937_64 rng;
    // initialize the random number generator with time-dependent seed
    uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
    rng.seed(ss);
    // initialize a uniform distribution between 0 and 1
    std::uniform_real_distribution<double> unif(0, 1);
	int testcases;
	cin>>testcases;
	cout<<testcases;
	for(int abc=0;abc<testcases;abc++)
	{
		string testcaseName;
		cin>>testcaseName;
		int numlinesRedundant;
		cin>>numlinesRedundant;
		unordered_map<string,int> indexPt;
		unordered_map< string, int> travelTime;
		string garbage;
		getline(cin,garbage);
		string seqLine;
		getline(cin,seqLine);
		vector<string> pitstopSequenceVector=splitString(seqLine);
		int totalPitstops=int(pitstopSequenceVector.size());
		for(int xq=0;xq<pitstopSequenceVector.size();xq++)
		{
			indexPt[pitstopSequenceVector[xq]]=xq;
		}
		int t,s,ow,r;
		cin>>t>>s>>ow>>r;
		vector<truckNode> scheduleVec;

		//reading up the timings
		for(int i=0;i<t;i++)
		{
			string a,b;
			int time;
			cin>>a>>b>>time;
			travelTime[tripStr(a,b)]=time;
		}
		for(int i=0;i<s;i++)
		{
			struct truckNode currSch;
			string origin;
			string destination;
			int arriveBy;
			cin>>currSch.tid>>currSch.startPt>>currSch.startingTime>>origin>>destination>>arriveBy;
			currSch.currPt=currSch.startPt;
			currSch.reachedCurrAt=currSch.startingTime;
			if(destination==pitstopSequenceVector[0])
			{
				currSch.aheadMoving=false;
			}
			else
			{
				currSch.aheadMoving=true;
			}
			int startIndexPt=indexPt[currSch.currPt];
			int destIndexPt=indexPt[destination];
			int waitTimeSchedule=0;
			int timeTravelSchedule=0;
			if(destIndexPt>startIndexPt)
			{				
				for(int kbc=startIndexPt+1;kbc<=destIndexPt;kbc++)
				{
					timeTravelSchedule+=travelTime[tripStr(pitstopSequenceVector[kbc-1],pitstopSequenceVector[kbc])];
				}
			}
			else if(destIndexPt<startIndexPt)
			{			
				for(int kbc=startIndexPt-1;kbc>=destIndexPt;kbc--)
				{
					timeTravelSchedule+=travelTime[tripStr(pitstopSequenceVector[kbc+1],pitstopSequenceVector[kbc])];
				}
			}
			waitTimeSchedule=(arriveBy- currSch.startingTime)-timeTravelSchedule;
			currSch.waitTimeAlloted=waitTimeSchedule;
			currSch.waitTimeLeft=waitTimeSchedule;
			scheduleVec.push_back(currSch);
		}
		sort(scheduleVec.begin(), scheduleVec.end(),myfunction);


		//Start iterations here
		int costScore=0;
		vector<string> tripsToOutput;
		vector<string> matchingToOutput;
		
		vector< deque<truckNode> > waitingToGoLeft(totalPitstops);
		vector< deque<truckNode> > aboutToReachFromRight(totalPitstops);
		
		vector< deque<truckNode> > waitingToGoRight(totalPitstops);
		vector< deque<truckNode> > aboutToReachFromLeft(totalPitstops);
		
		vector< deque<driverNode> > driverFromLeft(totalPitstops);
		vector< deque<driverNode> > driverFromRight(totalPitstops);


		for(int i=0;i<s;i++)
		{
			scheduleVec[i].currPt=scheduleVec[i].startPt;
			scheduleVec[i].reachedCurrAt=scheduleVec[i].startingTime;
			scheduleVec[i].waitTimeLeft=scheduleVec[i].waitTimeAlloted;
			// if(scheduleVec[i].aheadMoving)
			// {
			// 	aboutToReachFromLeft[indexPt[scheduleVec[i].currPt]].push(scheduleVec[i]);
			// }
			// else
			// {
			// 	aboutToReachFromRight[indexPt[scheduleVec[i].currPt]].push(scheduleVec[i]);
			// }
		}


		for(int simulationTime=0;simulationTime<=ow;simulationTime++)
		{
			DEBUG2(simulationTime);
			for(int i=0;i<s;i++)
			{
				struct truckNode currTruckToAddInSystem=scheduleVec[i];
				int indexToAddTruck=indexPt[currTruckToAddInSystem.currPt];
				if(indexToAddTruck==0 || indexToAddTruck==(totalPitstops-1))
				{
					if(simulationTime==0 && indexToAddTruck==0)
					{
						aboutToReachFromLeft[0].push_back(currTruckToAddInSystem);
					}
					else if(simulationTime==0 && indexToAddTruck==(totalPitstops-1))
					{
						aboutToReachFromRight[(totalPitstops-1)].push_back(currTruckToAddInSystem);
					}
				}
				else
				{
					if(currTruckToAddInSystem.aheadMoving)
					{
						if(simulationTime==
							(currTruckToAddInSystem.reachedCurrAt- 
								travelTime[tripStr(pitstopSequenceVector[indexToAddTruck-1],
									pitstopSequenceVector[indexToAddTruck])]
								)
							)
						{
							aboutToReachFromLeft[indexToAddTruck].push_back(currTruckToAddInSystem);
						}
					}
					else
					{
						if(simulationTime==
							(currTruckToAddInSystem.reachedCurrAt- 
								travelTime[tripStr(pitstopSequenceVector[indexToAddTruck+1],
									pitstopSequenceVector[indexToAddTruck])]
								)
							)
						{
							aboutToReachFromRight[indexToAddTruck].push_back(currTruckToAddInSystem);
						}
					}
				}
			}
			for(int pitStopIndex=0;pitStopIndex<totalPitstops;pitStopIndex++)
			{
				
				//truck travelling from right has reached
				while(!aboutToReachFromRight[pitStopIndex].empty() &&
					aboutToReachFromRight[pitStopIndex].front().reachedCurrAt==simulationTime)
				{
					waitingToGoLeft[pitStopIndex].push_back(aboutToReachFromRight[pitStopIndex].front());
					if(aboutToReachFromRight[pitStopIndex].front().driverCreate)
					{						
						struct driverNode driverCreated;
						driverCreated.reachedAt=simulationTime;
						driverCreated.reachedTripId=aboutToReachFromRight[pitStopIndex].front().tid;
						driverFromRight[pitStopIndex].push_back(driverCreated);
					}
					aboutToReachFromRight[pitStopIndex].pop_front();
				}
				
				//truck travelling from left has reached
				while( !aboutToReachFromLeft[pitStopIndex].empty() &&
					aboutToReachFromLeft[pitStopIndex].front().reachedCurrAt==simulationTime)
				{
					waitingToGoRight[pitStopIndex].push_back(aboutToReachFromLeft[pitStopIndex].front());
					if(aboutToReachFromLeft[pitStopIndex].front().driverCreate)
					{					
						struct driverNode driverCreated;
						driverCreated.reachedAt=simulationTime;
						driverCreated.reachedTripId=aboutToReachFromLeft[pitStopIndex].front().tid;
						driverFromLeft[pitStopIndex].push_back(driverCreated);
					}
					aboutToReachFromLeft[pitStopIndex].pop_front();
				}
				
				

				//match already waiting driversfromright with trips to go right
				while(!driverFromRight[pitStopIndex].empty() && !waitingToGoRight[pitStopIndex].empty())
				{

					//update tripsToOutput
					struct truckNode truckToGoRightCopy=waitingToGoRight[pitStopIndex].front();
					waitingToGoRight[pitStopIndex].pop_front();
					truckToGoRightCopy.driverCreate=false;
					string outStr=to_string(truckToGoRightCopy.tid)+" "+pitstopSequenceVector[pitStopIndex]
					+" "+to_string(simulationTime);
					tripsToOutput.push_back(outStr);
					//update matchingToOutput
					string outStr1=pitstopSequenceVector[pitStopIndex+1]+" "+
						pitstopSequenceVector[pitStopIndex]+" "+
						to_string(driverFromRight[pitStopIndex].front().reachedTripId)+" "+
						to_string(truckToGoRightCopy.tid)+" "+
						pitstopSequenceVector[pitStopIndex];
					matchingToOutput.push_back(outStr1);
					costScore+=costVal(driverFromRight[pitStopIndex].front().reachedAt-simulationTime);
					driverFromRight[pitStopIndex].pop_front();
					//update arrival at next stop
					if((pitStopIndex)!=(totalPitstops-1))
					{
						truckToGoRightCopy.currPt=pitstopSequenceVector[pitStopIndex+1];
						truckToGoRightCopy.reachedCurrAt=simulationTime+travelTime[tripStr(
							pitstopSequenceVector[pitStopIndex],pitstopSequenceVector[pitStopIndex+1]
							)];
						aboutToReachFromLeft[pitStopIndex+1].push_back(truckToGoRightCopy);
					}
				}

				//let go of trucks which have no waiting time left or drivers are not there
				vector<int> toRemove;
				
				for(int tj=0;tj<waitingToGoRight[pitStopIndex].size();tj++)
				{
					if(pitStopIndex==(pitStopIndex-1))
					{
						toRemove.push_back(tj);
						continue;
					}
					struct truckNode truckToGoRight=waitingToGoRight[pitStopIndex][tj];
					struct truckNode truckToGoRightCopy=truckToGoRight;

					double currentRandomNumber = unif(rng);
					if(truckToGoRightCopy.waitTimeLeft==0 || 
						(!aboutToReachFromLeft[pitStopIndex].empty() && (truckToGoRightCopy.waitTimeLeft<=(aboutToReachFromLeft[pitStopIndex].front().reachedCurrAt-simulationTime)))||
						currentRandomNumber<0.2
						)
					{
						//update tripsToOutput
						string outStr=to_string(truckToGoRightCopy.tid)+" "+pitstopSequenceVector[pitStopIndex]
						+" "+to_string(simulationTime);
						tripsToOutput.push_back(outStr);
						//driver is created
						truckToGoRightCopy.driverCreate=true;
						//update arrival at next stop
						if((pitStopIndex)!=(totalPitstops-1))
						{
							truckToGoRightCopy.currPt=pitstopSequenceVector[pitStopIndex+1];
							truckToGoRightCopy.reachedCurrAt=simulationTime+travelTime[tripStr(
								pitstopSequenceVector[pitStopIndex],pitstopSequenceVector[pitStopIndex+1]
								)];
							aboutToReachFromLeft[pitStopIndex+1].push_back(truckToGoRightCopy);
						}
						toRemove.push_back(tj);
					}
				}
				//remove from waitingToGoRight[pitStopIndex]
				for(int tj=0;tj<toRemove.size();tj++)
				{
					waitingToGoRight[pitStopIndex].erase( waitingToGoRight[pitStopIndex].begin()+toRemove[tj]);
				}

				////////////////////doing the same thing in opposite sense

				//match already waiting driversfromleft with trips to go left
				while(!driverFromLeft[pitStopIndex].empty() && !waitingToGoLeft[pitStopIndex].empty())
				{
					//update tripsToOutput
					struct truckNode truckToGoLeftCopy=waitingToGoLeft[pitStopIndex].front();
					waitingToGoLeft[pitStopIndex].pop_front();
					truckToGoLeftCopy.driverCreate=false;
					string outStr=to_string(truckToGoLeftCopy.tid)+" "+pitstopSequenceVector[pitStopIndex]
					+" "+to_string(simulationTime);
					tripsToOutput.push_back(outStr);
					//update matchingToOutput
					string outStr1=pitstopSequenceVector[pitStopIndex-1]+" "+
						pitstopSequenceVector[pitStopIndex]+" "+
						to_string(driverFromLeft[pitStopIndex].front().reachedTripId)+" "+
						to_string(truckToGoLeftCopy.tid)+" "+
						pitstopSequenceVector[pitStopIndex];
					matchingToOutput.push_back(outStr1);
					costScore+=costVal(driverFromLeft[pitStopIndex].front().reachedAt-simulationTime);
					driverFromLeft[pitStopIndex].pop_front();
					//update arrival at next stop
					if((pitStopIndex)!=(0))
					{
						truckToGoLeftCopy.currPt=pitstopSequenceVector[pitStopIndex-1];
						truckToGoLeftCopy.reachedCurrAt=simulationTime+travelTime[tripStr(
							pitstopSequenceVector[pitStopIndex],pitstopSequenceVector[pitStopIndex-1]
							)];
						aboutToReachFromRight[pitStopIndex-1].push_back(truckToGoLeftCopy);
					}
				}


				//let go of trucks which have no waiting time left or drivers are not there
				vector<int> toRemove1;
				for(int tj=0;tj<waitingToGoLeft[pitStopIndex].size();tj++)	
				{
					if(pitStopIndex==(0))
					{
						toRemove1.push_back(tj);
						continue;
					}
					struct truckNode truckToGoLeft=waitingToGoLeft[pitStopIndex][tj];
					struct truckNode truckToGoLeftCopy=truckToGoLeft;

					double currentRandomNumber = unif(rng);

					if(truckToGoLeftCopy.waitTimeLeft==0 || 
						((!aboutToReachFromRight[pitStopIndex].empty()) &&
						 (truckToGoLeftCopy.waitTimeLeft<=
						 	(aboutToReachFromRight[pitStopIndex].front().reachedCurrAt-simulationTime)))||
						currentRandomNumber<0.2
						)
					{
						//update tripsToOutput
						string outStr=to_string(truckToGoLeftCopy.tid)+" "+pitstopSequenceVector[pitStopIndex]
						+" "+to_string(simulationTime);
						tripsToOutput.push_back(outStr);
						//driver is created
						truckToGoLeftCopy.driverCreate=true;
						//update arrival at next stop
						if((pitStopIndex)!=(totalPitstops-1))
						{
							truckToGoLeftCopy.currPt=pitstopSequenceVector[pitStopIndex+1];
							truckToGoLeftCopy.reachedCurrAt=simulationTime+travelTime[tripStr(
								pitstopSequenceVector[pitStopIndex],pitstopSequenceVector[pitStopIndex-1]
								)];
							aboutToReachFromLeft[pitStopIndex+1].push_back(truckToGoLeftCopy);
						}
						toRemove1.push_back(tj);
					}
				}
				//remove from waitingToGoRight[pitStopIndex]
				for(int tj=0;tj<toRemove1.size();tj++)	
				{
					waitingToGoLeft[pitStopIndex].erase(waitingToGoLeft[pitStopIndex].begin()+toRemove1[tj]);
				}

				

				for(int tj=0;tj<waitingToGoLeft[pitStopIndex].size();tj++)	
				{
					waitingToGoLeft[pitStopIndex][tj].waitTimeLeft-=1;	
				}

				for(int tj=0;tj<waitingToGoRight[pitStopIndex].size();tj++)	
				{
					waitingToGoRight[pitStopIndex][tj].waitTimeLeft-=1;	
				}
				//updateWaitingTimeLeft
			}
		}

		for(int pitStopIndex=0;pitStopIndex<totalPitstops;pitStopIndex++)
		{
			// update
			// int reachedAt;
			// int reachedTripId;
			// matchingToOutput
			for(int zez=0;zez< driverFromLeft[pitStopIndex].size();zez++)
			{
				string outStr=pitstopSequenceVector[pitStopIndex-1]+" "+
				pitstopSequenceVector[pitStopIndex]+" "+
				to_string(driverFromLeft[pitStopIndex][zez].reachedTripId)+" -1 -1";
				matchingToOutput.push_back(outStr);
				costScore+=costVal(ow - driverFromLeft[pitStopIndex][zez].reachedAt);
			}
			for(int zez=0;zez<driverFromRight[pitStopIndex].size();zez++)
			{
				string outStr=pitstopSequenceVector[pitStopIndex+1]+" "+
				pitstopSequenceVector[pitStopIndex]+" "+
				to_string(driverFromRight[pitStopIndex][zez].reachedTripId)+" -1 -1";
				matchingToOutput.push_back(outStr);
				costScore+=costVal(ow - driverFromRight[pitStopIndex][zez].reachedAt);
			}

			
		}


		DEBUG2(costScore);



	}


    return 0;
}