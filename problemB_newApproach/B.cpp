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
	cout<<testcases<<endl;
	for(int abc=0;abc<testcases;abc++)
	{
		string testcaseName;
		cin>>testcaseName;
		int numlinesRedundant;
		cin>>numlinesRedundant;
		unordered_map<string,int> indexPt;
		unordered_map< string, int> travelTime;
		unordered_map< string, bool > aheadMovingMap;
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
		//reading the trucks to Schedule
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
				aheadMovingMap[to_string(currSch.tid)]=false;
			}
			else
			{
				currSch.aheadMoving=true;
				aheadMovingMap[to_string(currSch.tid)]=true;
			}
			//DEBUG2(currSch.aheadMoving);
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
			// DEBUG2(currSch.tid);
			// DEBUG2(waitTimeSchedule);
			currSch.waitTimeAlloted=waitTimeSchedule;
			currSch.waitTimeLeft=waitTimeSchedule;
			scheduleVec.push_back(currSch);
		}
		sort(scheduleVec.begin(), scheduleVec.end(),myfunction);

		int costScore_final=INT_MAX;
		vector<string> tripsToOutput_final;
		vector<string> matchingToOutput_final;

		for(int scheduleIteration=0;scheduleIteration<1;scheduleIteration++)
		{
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

			//making the truck Nodes starting points and waiting time correct
			for(int i=0;i<s;i++)
			{
				scheduleVec[i].currPt=scheduleVec[i].startPt;
				scheduleVec[i].reachedCurrAt=scheduleVec[i].startingTime;
				scheduleVec[i].waitTimeLeft=scheduleVec[i].waitTimeAlloted;
			}

			//doing a simulation
			for(int simulationTime=0;simulationTime<=ow;simulationTime++)
			{
				//DEBUG2("########################");
				//DEBUG2(simulationTime);
				//Add initial truck trips about to start near simulation time
				for(int i=0;i<s;i++)
				{
					struct truckNode currTruckToAddInSystem=scheduleVec[i];
					int indexToAddTruck=indexPt[currTruckToAddInSystem.currPt];
					if(indexToAddTruck==0 || indexToAddTruck==(totalPitstops-1))
					{
						if(simulationTime==0 && indexToAddTruck==0)
						{
							//DEBUG2("Added truck aboutToReachFromLeft");
							//DEBUG2(currTruckToAddInSystem.tid);
							aboutToReachFromLeft[0].push_back(currTruckToAddInSystem);
						}
						else if(simulationTime==0 && indexToAddTruck==(totalPitstops-1))
						{
							//DEBUG2("Added truck aboutToReachFromRight");
							//DEBUG2(currTruckToAddInSystem.tid);
							aboutToReachFromRight[(totalPitstops-1)].push_back(currTruckToAddInSystem);
						}
					}
					else
					{
						if(currTruckToAddInSystem.aheadMoving)
						{
							int idealAdditionTime=currTruckToAddInSystem.reachedCurrAt- 
									travelTime[tripStr(pitstopSequenceVector[indexToAddTruck-1],
										pitstopSequenceVector[indexToAddTruck])];
							//DEBUG2(currTruckToAddInSystem.aheadMoving);
							if(simulationTime==max(idealAdditionTime,0))
							{
								//DEBUG2("Added truck aboutToReachFromLeft");
								//DEBUG2(currTruckToAddInSystem.tid);
								//DEBUG2(indexToAddTruck);
								aboutToReachFromLeft[indexToAddTruck].push_back(currTruckToAddInSystem);
							}
						}
						else
						{
							int idealAdditionTime=currTruckToAddInSystem.reachedCurrAt- 
									travelTime[tripStr(pitstopSequenceVector[indexToAddTruck+1],
										pitstopSequenceVector[indexToAddTruck])];
							//DEBUG2(currTruckToAddInSystem.aheadMoving);
							if(simulationTime==max(idealAdditionTime,0))
							{
								//DEBUG2("Added truck aboutToReachFromRight");
								//DEBUG2(indexToAddTruck);
								//DEBUG2(currTruckToAddInSystem.tid);
								aboutToReachFromRight[indexToAddTruck].push_back(currTruckToAddInSystem);
							}
						}
					}
				}
				for(int pitStopIndex=0;pitStopIndex<totalPitstops;pitStopIndex++)
				{
					// if(pitStopIndex==1 && simulationTime==3)
					// {
					// 	//DEBUG2(aboutToReachFromLeft[pitStopIndex].front().tid);
					// 	//DEBUG2(aboutToReachFromLeft[pitStopIndex].front().reachedCurrAt);
					// }
					
					//truck travelling from right has reached
					while(!aboutToReachFromRight[pitStopIndex].empty() &&
						aboutToReachFromRight[pitStopIndex].front().reachedCurrAt==simulationTime)
					{
						//cerr<<aboutToReachFromRight[pitStopIndex].front().tid<<" truck has reached pitStop "<<pitStopIndex<<" and is waiting to go left"<<endl;
						waitingToGoLeft[pitStopIndex].push_back(aboutToReachFromRight[pitStopIndex].front());
						if(aboutToReachFromRight[pitStopIndex].front().driverCreate)
						{	
							//cerr<<"A drive from right has reached pitStop "<<pitStopIndex<<"from above truck id"<<endl;					
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
						//cerr<<aboutToReachFromLeft[pitStopIndex].front().tid<<" truck has reached pitStop "<<
						//pitstopSequenceVector[pitStopIndex]<<" and is waiting to go right"<<endl;
						waitingToGoRight[pitStopIndex].push_back(aboutToReachFromLeft[pitStopIndex].front());
						if(aboutToReachFromLeft[pitStopIndex].front().driverCreate)
						{					
							//cerr<<"A drive from the left has reached pitStop "<<pitstopSequenceVector[pitStopIndex]<<"from above truck id"<<endl;
							struct driverNode driverCreated;
							driverCreated.reachedAt=simulationTime;
							driverCreated.reachedTripId=aboutToReachFromLeft[pitStopIndex].front().tid;
							driverFromLeft[pitStopIndex].push_back(driverCreated);
						}
						aboutToReachFromLeft[pitStopIndex].pop_front();
					}
					//DEBUG2(waitingToGoRight[pitStopIndex].size());
					
					

					//match already waiting driversfromright with trips to go right
					while(!driverFromRight[pitStopIndex].empty() && !waitingToGoRight[pitStopIndex].empty())
					{
						struct truckNode truckToGoRightCopy=waitingToGoRight[pitStopIndex].front();
						waitingToGoRight[pitStopIndex].pop_front();

						 // cerr<<"there are drivers from right present "<<pitstopSequenceVector[pitStopIndex]<<
						 // "here and there are also trucks waiting to go right, So Match has occured between truck id"<<truckToGoRightCopy.tid<< "which needs to go right"
						 // <<"and "<<driverFromRight[pitStopIndex].front().reachedTripId<<" which came from right"<<endl;
						//update tripsToOutput
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
						costScore+=costVal(simulationTime-driverFromRight[pitStopIndex].front().reachedAt);
						driverFromRight[pitStopIndex].pop_front();
						//update arrival at next stop
						// if((pitStopIndex)!=(totalPitstops-1))
						// {
							truckToGoRightCopy.currPt=pitstopSequenceVector[pitStopIndex+1];
							truckToGoRightCopy.reachedCurrAt=simulationTime+travelTime[tripStr(
								pitstopSequenceVector[pitStopIndex],pitstopSequenceVector[pitStopIndex+1]
								)];
							// cerr<<truckToGoRightCopy.tid<<"is leaving towards right for pitstop"
							// <<pitstopSequenceVector[pitStopIndex+1];
							aboutToReachFromLeft[pitStopIndex+1].push_back(truckToGoRightCopy);
						//}
					}

					//let go of trucks which have no waiting time left or drivers are not there
					vector<int> toRemove;
					
					//DEBUG2(waitingToGoRight[pitStopIndex].size());
					//DEBUG2("traversing trucks wanting to go right");
					for(int tj=0;tj<waitingToGoRight[pitStopIndex].size();tj++)
					{
						if(pitStopIndex>=(totalPitstops-1))
						{
							toRemove.push_back(tj);
							continue;
						}
						struct truckNode truckToGoRight=waitingToGoRight[pitStopIndex][tj];
						struct truckNode truckToGoRightCopy=truckToGoRight;
						//DEBUG2(truckToGoRightCopy.tid);

						double currentRandomNumber = unif(rng);
						if(truckToGoRightCopy.waitTimeLeft==0 || 
							(!aboutToReachFromLeft[pitStopIndex].empty() && (truckToGoRightCopy.waitTimeLeft<=(aboutToReachFromLeft[pitStopIndex].front().reachedCurrAt-simulationTime)))||
							currentRandomNumber<0.2
							)
						{
							//update tripsToOutput
							string outStr=to_string(truckToGoRightCopy.tid)+" "+pitstopSequenceVector[pitStopIndex]
							+" "+to_string(simulationTime);
							
							//driver is created
							truckToGoRightCopy.driverCreate=true;
							//update arrival at next stop
							if((pitStopIndex)!=(totalPitstops-1))
							{
								truckToGoRightCopy.currPt=pitstopSequenceVector[pitStopIndex+1];
								truckToGoRightCopy.reachedCurrAt=simulationTime+travelTime[tripStr(
									pitstopSequenceVector[pitStopIndex],pitstopSequenceVector[pitStopIndex+1]
									)];
							// cerr<<truckToGoRightCopy.tid<<"has left towards right for pitstop"
							// <<pitstopSequenceVector[pitStopIndex+1];
								aboutToReachFromLeft[pitStopIndex+1].push_back(truckToGoRightCopy);
							}
							tripsToOutput.push_back(outStr);
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
						costScore+=costVal(simulationTime-driverFromLeft[pitStopIndex].front().reachedAt);
						driverFromLeft[pitStopIndex].pop_front();
						//update arrival at next stop
						// if((pitStopIndex)!=(0))
						// {
							truckToGoLeftCopy.currPt=pitstopSequenceVector[pitStopIndex-1];
							truckToGoLeftCopy.reachedCurrAt=simulationTime+travelTime[tripStr(
								pitstopSequenceVector[pitStopIndex],pitstopSequenceVector[pitStopIndex-1]
								)];
							aboutToReachFromRight[pitStopIndex-1].push_back(truckToGoLeftCopy);
						// }
					}


					//let go of trucks which have no waiting time left or drivers are not there
					vector<int> toRemove1;
					for(int tj=0;tj<waitingToGoLeft[pitStopIndex].size();tj++)	
					{
						if(pitStopIndex<=(0))
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
							
							//driver is created
							truckToGoLeftCopy.driverCreate=true;
							//update arrival at next stop
							if((pitStopIndex)!=(0))
							{
								truckToGoLeftCopy.currPt=pitstopSequenceVector[pitStopIndex-1];
								truckToGoLeftCopy.reachedCurrAt=simulationTime+travelTime[tripStr(
									pitstopSequenceVector[pitStopIndex],pitstopSequenceVector[pitStopIndex-1]
									)];
							// cerr<<truckToGoLeftCopy.tid<<"has left towards left for pitstop"
							// <<pitstopSequenceVector[pitStopIndex-1];
								aboutToReachFromRight[pitStopIndex-1].push_back(truckToGoLeftCopy);
							}
							tripsToOutput.push_back(outStr);
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
			
			//Adding Cost of the unmatched trucks
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
				for(int maq=0;maq<totalPitstops;maq++)
				{
					//DEBUG2(maq);
					for(int tak=0;tak<aboutToReachFromRight[maq].size();tak++)
					{
						struct truckNode unmatchTruck=aboutToReachFromRight[maq][tak];
						if(unmatchTruck.driverCreate)
						{
							string outStr=pitstopSequenceVector[maq+1]+" "+
							pitstopSequenceVector[maq]+" "+
							to_string(unmatchTruck.tid)+" -1 -1";
							matchingToOutput.push_back(outStr);
						}
					}
					for(int tak=0;tak<aboutToReachFromLeft[maq].size();tak++)
					{
						struct truckNode unmatchTruck=aboutToReachFromLeft[maq][tak];
						if(unmatchTruck.driverCreate)
						{
							string outStr=pitstopSequenceVector[maq-1]+" "+
					pitstopSequenceVector[maq]+" "+
					to_string(unmatchTruck.tid)+" -1 -1";
					matchingToOutput.push_back(outStr);
					
						}

					}

				}
				
			}

			if(costScore<costScore_final)
			{
				costScore_final=costScore;
				tripsToOutput_final=tripsToOutput;
				matchingToOutput_final=matchingToOutput;
			}

		}
		
		// vector< vector<string> > matchingBrokenUp;
		// int numUnmatched=0;
		// for(int i=0;i<matchingToOutput_final.size();i++)
		// {
		// 	vector<string> vhj=(splitString(matchingToOutput_final[i]));
		// 	if(vhj[4]=="-1")
		// 	{
		// 		numUnmatched+=1;
		// 	}
		// }

		// if(int(tripsToOutput_final.size())!=  (int(matchingToOutput_final.size())*2-numUnmatched) )
		// {
		// 	cerr<<"This is in error "<<testcaseName<<endl;
		// 	DEBUG2(numUnmatched);
		// }

		// for(int i=0;i<tripsToOutput_final.size();i++)
		// {
		// 	vector<string> currTripBroken= splitString(tripsToOutput_final[i]);
		// 	bool foundInMatch=false;
		// 	for(int j=0;j<matchingBrokenUp.size();j++)
		// 	{
		// 		if(currTripBroken[0]==matchingBrokenUp[j][2] && currTripBroken[1]==matchingBrokenUp[j][0])
		// 		{
		// 			foundInMatch=true;
		// 			break;
		// 		}
		// 		if(currTripBroken[0]==matchingBrokenUp[j][3] && currTripBroken[1]==matchingBrokenUp[j][1])
		// 		{
		// 			foundInMatch=true;
		// 			break;
		// 		}
		// 	}
		// 	if(!foundInMatch)
		// 	{
		// 		string otherPitStop;
		// 		if(aheadMovingMap[currTripBroken[0]])
		// 		{
		// 			otherPitStop=pitstopSequenceVector[indexPt[currTripBroken[0]]+1];
		// 		}
		// 		else
		// 		{
		// 			otherPitStop=pitstopSequenceVector[indexPt[currTripBroken[0]]+1];
		// 		}

		// 		//add to matchingToOutput_final
		// 		string addThisMatch=currTripBroken[1]+" "+otherPitStop+" "+currTripBroken[0]+" -1 -1";
		// 		matchingToOutput_final.push_back(addThisMatch);
		// 	}
		// }


		set<string> myset( matchingToOutput_final.begin(), matchingToOutput_final.end() );
		matchingToOutput_final.assign( myset.begin(), myset.end() );


		// DEBUG2(costScore);
		// DEBUG2("Below are the trips");
		cout<<testcaseName<<" "<<(tripsToOutput_final.size()+matchingToOutput_final.size()+2)<<endl;
		cout<<int(tripsToOutput_final.size())<<" "<<int(matchingToOutput_final.size())<<endl;
		for(int i=0;i<tripsToOutput_final.size();i++)
		{
			cout<<tripsToOutput_final[i]<<endl;
		}
		//DEBUG2("Below are the matchings");
		for(int i=0;i<matchingToOutput_final.size();i++)
		{
			cout<<matchingToOutput_final[i]<<endl;
		}
		cout<<costScore_final<<endl;

	}


    return 0;
}