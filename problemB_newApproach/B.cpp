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
	bool initialRide=true;
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
		auto startAlarmClock = chrono::steady_clock::now();
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

		int costScore_final=INT_MAX;
		vector<string> tripsToOutput_final;
		vector<string> matchingToOutput_final;


		long int maxOneIteration = 0;			
		long int timeAlarm=0;
		srand (time(NULL));

		//||timeAlarm<(r*1000-3*maxOneIteration)
		for(int scheduleIteration=0;scheduleIteration<10000;scheduleIteration++)
		{
			auto startCurrSchedule= chrono::steady_clock::now();



			double prob1,prob2;
			if(scheduleIteration%6==0)
			{			
				prob1=unif(rng)/20;
				prob2=unif(rng)/10+0.2;
			}
			else if(scheduleIteration%6==1)
			{
				prob1=unif(rng)/10+0.2;
				prob2=unif(rng)/20;
			}
			else if(scheduleIteration%6==2)
			{
				prob1=0.1;
				prob2=0.1;
			}
			else if(scheduleIteration%6==3)
			{
				prob1=0.05;
				prob2=0.05;
			}
			else if(scheduleIteration%6==4)
			{
				prob1=0.2;
				prob2=0.2;
			}
			else
			{
				prob1=0.5;
				prob2=0.5;
			}


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
				scheduleVec[i].initialRide=true;
				scheduleVec[i].driverCreate=false;
			}

			//doing a simulation
			for(int simulationTime=0;simulationTime<=ow;simulationTime++)
			{
				
				//Add initial truck trips about to start near simulation time
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
							int idealAdditionTime=currTruckToAddInSystem.reachedCurrAt- 
									travelTime[tripStr(pitstopSequenceVector[indexToAddTruck-1],
										pitstopSequenceVector[indexToAddTruck])];
							if(simulationTime==max(idealAdditionTime,0))
							{
								aboutToReachFromLeft[indexToAddTruck].push_back(currTruckToAddInSystem);
							}
						}
						else
						{
							int idealAdditionTime=currTruckToAddInSystem.reachedCurrAt- 
									travelTime[tripStr(pitstopSequenceVector[indexToAddTruck+1],
										pitstopSequenceVector[indexToAddTruck])];
							if(simulationTime==max(idealAdditionTime,0))
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
						aboutToReachFromRight[pitStopIndex].front().initialRide=false;
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
						aboutToReachFromLeft[pitStopIndex].front().initialRide=false;
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
					


					/////////////////////////////////////////////////////////////////////////////
					/////////////////////////////////////////////////////////////////////////////
					/////////////////////////////////////////////////////////////////////////////
					///////////////////////Truck reaching Done //////////////////////////////////
					/////////////////////////////////////////////////////////////////////////////
					/////////////////////////////////////////////////////////////////////////////
					/////////////////////////////////////////////////////////////////////////////

					//match already waiting driversfromright with trips to go right
					while(!driverFromRight[pitStopIndex].empty() && !waitingToGoRight[pitStopIndex].empty())
					{
						if(pitStopIndex>=(totalPitstops-1))
						{
							continue;
						}
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

					vector<int> indexTruckWithDriver1;
					for(int kt=0;kt<aboutToReachFromRight[pitStopIndex].size();kt++)
					{
						if(aboutToReachFromRight[pitStopIndex][kt].driverCreate)
						{
							indexTruckWithDriver1.push_back(kt);
						}
					}

					int futureMatchIndex1=0;
					//let go of trucks which have no waiting time left or drivers are not there
					vector<int> toRemove;
					
					for(int tj=0;tj<waitingToGoRight[pitStopIndex].size();tj++)
					{
						if(pitStopIndex>=(totalPitstops-1))
						{
							toRemove.push_back(tj);
							continue;
						}
						struct truckNode truckToGoRight=waitingToGoRight[pitStopIndex][tj];
						struct truckNode truckToGoRightCopy=truckToGoRight;


						//make stay
						if(futureMatchIndex1<int(indexTruckWithDriver1.size()) && truckToGoRightCopy.waitTimeLeft>=(aboutToReachFromRight[pitStopIndex][futureMatchIndex1].reachedCurrAt-simulationTime))
						{
							//please don't let it go!
							futureMatchIndex1+=1;
							double currentRandomNumber1 = unif(rng);
							if(currentRandomNumber1>0.97)
							{
								continue;
							}
						}
						
						double currentRandomNumber = unif(rng);
						if(truckToGoRightCopy.waitTimeLeft==0 || currentRandomNumber<prob1)
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
					reverse(toRemove.begin(), toRemove.end());
					//remove from waitingToGoRight[pitStopIndex]
					for(int tj=0;tj<toRemove.size();tj++)
					{
						waitingToGoRight[pitStopIndex].erase( waitingToGoRight[pitStopIndex].begin()+toRemove[tj]);
					}

					/////////////////////////////////////////////////////////////////////////////
					/////////////////////////////////////////////////////////////////////////////
					/////////////////////////////////////////////////////////////////////////////
					///////////////////////Opposite Side Doing Same /////////////////////////////
					/////////////////////////////////////////////////////////////////////////////
					/////////////////////////////////////////////////////////////////////////////
					/////////////////////////////////////////////////////////////////////////////

					//match already waiting driversfromleft with trips to go left
					while(!driverFromLeft[pitStopIndex].empty() && !waitingToGoLeft[pitStopIndex].empty())
					{
						if(pitStopIndex<=(0))
						{
							continue;
						}
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


					vector<int> indexTruckWithDriver;
					for(int kt=0;kt<aboutToReachFromLeft[pitStopIndex].size();kt++)
					{
						if(aboutToReachFromLeft[pitStopIndex][kt].driverCreate)
						{
							indexTruckWithDriver.push_back(kt);
						}
					}

					//Favour Leaving
					//number of drivers(driverFromRight) waiting on (pitStopIndex-1)


					//Favour Waiting 
					//number of trucks coming from aboutToReachFromRight that are coming with createDriver as True


					int futureMatchIndex=0;

					//let go of trucks which have no waiting time left or drivers are not there
					vector<int> toRemove1;
					for(int tj=0;tj<int(waitingToGoLeft[pitStopIndex].size());tj++)	
					{
						if(pitStopIndex<=(0))
						{
							toRemove1.push_back(tj);
							continue;
						}
						struct truckNode truckToGoLeft=waitingToGoLeft[pitStopIndex][tj];
						struct truckNode truckToGoLeftCopy=truckToGoLeft;

						//make stay
						if(futureMatchIndex<int(indexTruckWithDriver.size()) && truckToGoLeftCopy.waitTimeLeft>=(aboutToReachFromLeft[pitStopIndex][futureMatchIndex].reachedCurrAt-simulationTime))
						{
							//please don't let it go!
							futureMatchIndex+=1;
							double currentRandomNumber1 = unif(rng);
							if(currentRandomNumber1>0.97)
							{
								continue;
							}
						}

						double currentRandomNumber = unif(rng);

						if(truckToGoLeftCopy.waitTimeLeft==0 ||	currentRandomNumber<prob2)
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
					reverse(toRemove1.begin(), toRemove1.end());
					//remove from waitingToGoRight[pitStopIndex]
					for(int tj=0;tj<toRemove1.size();tj++)	
					{
						waitingToGoLeft[pitStopIndex].erase(waitingToGoLeft[pitStopIndex].begin()+toRemove1[tj]);
					}


					/////////////////////////////////////////////////////////////////////////////
					/////////////////////////////////////////////////////////////////////////////
					/////////////////////////////////////////////////////////////////////////////
					///////////////////////Updating Wait time of Waiting Trucks//////////////////
					/////////////////////////////////////////////////////////////////////////////
					/////////////////////////////////////////////////////////////////////////////
					/////////////////////////////////////////////////////////////////////////////
					

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
					for(int tak=0;tak<aboutToReachFromRight[maq].size();tak++)
					{
						struct truckNode unmatchTruck=aboutToReachFromRight[maq][tak];
						if(unmatchTruck.driverCreate && !unmatchTruck.initialRide)
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
						if(unmatchTruck.driverCreate && !unmatchTruck.initialRide)
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
				// DEBUG2("########################################");
				// DEBUG2(scheduleIteration);
				// DEBUG2(costScore);
				costScore_final=costScore;
				tripsToOutput_final=tripsToOutput;
				matchingToOutput_final=matchingToOutput;	
			}

			auto endCurrSchedule= chrono::steady_clock::now();
			auto currIterationTime = chrono::duration_cast<chrono::milliseconds>(endCurrSchedule - startCurrSchedule).count();
			maxOneIteration=max(maxOneIteration,currIterationTime);
			auto currAlarmClock = chrono::steady_clock::now();
			timeAlarm = chrono::duration_cast<chrono::milliseconds>(currAlarmClock - startAlarmClock).count();



		}
		// DEBUG2("################################################################################");


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