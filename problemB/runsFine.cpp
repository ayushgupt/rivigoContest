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

struct tripNode
{
	int tid;
	int depTime;
	bool operator<(const tripNode &rhs)const
    {
        return depTime<rhs.depTime;
    }
};

struct ScheduleNode
{
	int tid;
	string nextPt;
	int startsAfter;
	string origin;
	string destination;
	int arriveBy;
};


int testCases;
string currTestCaseName;


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


int main()
{
	cin>>testCases;
	cout<<testCases<<endl;
	for(int overallCount=0;overallCount<testCases;overallCount++)
	{
			unordered_map< string, int> travelTime;
			unordered_map< string, vector<tripNode> > tripsVec;
			int t,s,ow,r;
			int redundantLines; 
			int out_d = 0;
			cin>>currTestCaseName>>redundantLines;

			string garbage;
			getline(cin,garbage);
			string seqLine;
			getline(cin,seqLine);
			//DEBUG2(seqLine);
			unordered_map<string,int> indexPt;
			vector<string> pitstopSequenceVector=splitString(seqLine);
			//DEBUG2(pitstopSequenceVector[0]);
			for(int xq=0;xq<pitstopSequenceVector.size();xq++)
			{
				indexPt[pitstopSequenceVector[xq]]=xq;
			}

			cin>>t>>s>>ow>>r;
			vector<string> linesOut;
			int finalTotalScore=0;


			for(int i=0;i<t;i++)
			{
				string a,b;
				int time;
				cin>>a>>b>>time;
				travelTime[tripStr(a,b)]=time;
			}
			vector<ScheduleNode> scheduleVec;

			//DEBUG2("h1");

			for(int i=0;i<s;i++)
			{
				struct ScheduleNode currSch;
				cin>>currSch.tid>>currSch.nextPt>>currSch.startsAfter>>currSch.origin
				>>currSch.destination>>currSch.arriveBy;
				int schDepTime=currSch.startsAfter;
				string currPt=currSch.nextPt;
				int schDepOld=schDepTime;
				//DEBUG2(i);
				while(schDepTime<ow && currPt!=currSch.destination)
				{
					out_d+=1;
					string currStr=to_string(currSch.tid)+" "+currPt+" "+to_string(schDepTime);
					string currPtOld=currPt;
					schDepOld=schDepTime;
					//DEBUG2("he2");
					if(currSch.destination==pitstopSequenceVector[0])
					{
						//move back word
						//DEBUG2("he5");
						currPt=pitstopSequenceVector[indexPt[currPt]-1];
						//DEBUG2("he3");
						schDepTime+=travelTime[tripStr(currPtOld,currPt)];
					}
					else
					{
						//move forward
						//DEBUG2("he6");
						//DEBUG2(indexPt[currPt]);
						currPt=pitstopSequenceVector[indexPt[currPt]+1];
						//DEBUG2("he4");
						schDepTime+=travelTime[tripStr(currPtOld,currPt)];
					}

					//DEBUG2("he1");
					struct tripNode dum;
					dum.tid=currSch.tid;
					dum.depTime=schDepOld;
					if(tripsVec.find(tripStr(currPtOld,currPt))==tripsVec.end())
					{
						vector<tripNode> init;
						init.push_back(dum);
						tripsVec[tripStr(currPtOld,currPt)]=init;
					}
					else
					{
						tripsVec.at( tripStr(currPtOld,currPt) ).push_back(dum);
					}



					linesOut.push_back(currStr);
				}
			}

			int out_m=0;
			//DEBUG2("h2");
			for(int startNode=0;startNode<(pitstopSequenceVector.size()-1);startNode++)
			{
				vector<int> finalMatching1;
				vector<int> finalMatching2;
				int finalScore=INT_MAX;
				srand (time(NULL));
				int endNode=startNode+1;

				
					//DEBUG2("startedForASegment");
					//DEBUG2(startNode);
					//DEBUG2(endNode);
					string vec1Str = tripStr(pitstopSequenceVector[startNode],pitstopSequenceVector[endNode]);
					string vec2Str = tripStr(pitstopSequenceVector[endNode],pitstopSequenceVector[startNode]);
					//DEBUG2("hey6");
					//DEBUG2(vec1Str);
					int ttime1=travelTime.at(vec1Str);
					//DEBUG2("hey5");
					if(tripsVec.find(vec1Str)==tripsVec.end() && tripsVec.find(vec2Str)!=tripsVec.end() )
					{

						for(auto tripsAlone:tripsVec.at(vec2Str))
						{
							string curr="";
							out_m+=1;
							curr=pitstopSequenceVector[endNode]+" "+pitstopSequenceVector[startNode]+" "+to_string(tripsAlone.tid)+" -1 -1";
							linesOut.push_back(curr);

						}
						continue;
					}
					if(tripsVec.find(vec1Str)!=tripsVec.end() && tripsVec.find(vec2Str)==tripsVec.end() )
					{
						for(auto tripsAlone:tripsVec.at(vec1Str))
						{
							string curr="";
							out_m+=1;
							curr=pitstopSequenceVector[startNode]+" "+pitstopSequenceVector[endNode]+" "+to_string(tripsAlone.tid)+" -1 -1";
							linesOut.push_back(curr);

						}
						continue;
					}
					if(tripsVec.find(vec1Str)==tripsVec.end() && tripsVec.find(vec2Str)==tripsVec.end() )
					{
						continue;
					}
					vector<tripNode> vec1=tripsVec.at(vec1Str);
					
					//DEBUG2("hey4");
					int ttime2=travelTime.at(vec2Str);
					vector<tripNode> vec2=tripsVec.at(vec2Str);
					sort(vec1.begin(), vec1.end());
					sort(vec2.begin(), vec2.end());
					int v1s=int(vec1.size());
					int v2s=int(vec2.size());
					
					//DEBUG2("hey3");
					vector< vector<int> > weightVec((v1s+1),vector<int>(v2s+1,INT_MAX));
					for(int i=0;i<v1s;i++)
					{
						weightVec[i][v2s]=costVal(max(ow-(vec1[i].depTime+ttime1),0));
					}
					for(int i=0;i<v2s;i++)
					{
						weightVec[v1s][i]=costVal(max(ow-(vec2[i].depTime+ttime2),0));
					}
					//DEBUG2("hey2");
					for(int i=0;i<v1s;i++)
					{
						for(int j=0;j<v2s;j++)
						{
							if(vec2[j].depTime >= (vec1[i].depTime+ttime1))
							{
								weightVec[i][j]= costVal(vec2[j].depTime-(vec1[i].depTime+ttime1));
							}
							if(vec1[i].depTime>=(vec2[j].depTime+ttime2))
							{
								weightVec[i][j]=costVal(vec1[i].depTime-(vec2[j].depTime+ttime2));
							}
						}
						//cout<<endl;
					}

					if(v1s==0 || v2s==0)
					{
						continue;
					}

					int topHowMany=min(5,(v1s));
					
					//DEBUG2("hey1");
					vector< vector<int> > smallIndexes(v1s,vector<int>(topHowMany,v2s));
					for(int i=0;i<v1s;i++)
					{
						vector<int> currW=weightVec[i];
						sort(currW.begin(), currW.end());
						int cutOff=currW[topHowMany-1];
						int insert_count=0;
						for(int j=0;j<=v2s && insert_count<topHowMany;j++)
						{
							if(weightVec[i][j]<=cutOff)
							{
								smallIndexes[i][insert_count]=j;
								insert_count+=1;
							}
						}
					}

					//DEBUG2("hey0");



					for(int testrun=0;testrun<10000;testrun++)
					{
						//DEBUG2("final");
							vector<int> matching1(v1s,v2s);
							vector<int> matching2(v2s,v1s);
							set<int> unmatchedFor1;
							int scoring=0;
							for(int i=0;i<v2s;i++)
							{
								unmatchedFor1.insert(i);
							}
							//DEBUG2("final2");
							//DEBUG2(v1s);
							//DEBUG2(topHowMany);
							//generate initial matching and score
							for(int i=0;i<v1s;i++)
							{
								//DEBUG2(i);
								int count=0;
								while(count<100)
								{
									int iSec;
									if(  ((double) rand() / (RAND_MAX)) <0.6)
									{
										iSec= smallIndexes[i][(rand()%topHowMany)];
										//iSec==min_element( weightVec[i].begin(), weightVec[i].end() ) - weightVec[i].begin();
									}
									else
									{
										iSec = rand() % v2s;
									}
									//DEBUG2(iSec);
									if( (iSec==v2s||unmatchedFor1.find(iSec)!=unmatchedFor1.end()) && weightVec[i][iSec]!=INT_MAX)
									{
										//can match
										matching1[i]=iSec;
										scoring+=weightVec[i][iSec];
										if(iSec!=v2s)
										{									
											matching2[iSec]=i;
											unmatchedFor1.erase(iSec);
										}
										break;
									}
									count+=1;
								}
								if(count==100)
								{
									matching1[i]=v2s;
									scoring+=weightVec[i][v2s];
								}
								//DEBUG2(matching1[i]);
							}
							//DEBUG2("final3");
							for (set<int>::iterator it=unmatchedFor1.begin(); it!=unmatchedFor1.end(); ++it)
							{
								scoring+=weightVec[v1s][*it];
							}
							//DEBUG2("final1");
							int initialScore=scoring;
							//DEBUG2(scoring);

							int last_opti=INT_MAX;
							int count_opti=0;
							for(int opti=0;opti<1000 && (opti-last_opti)<100;opti++)
							{
								//DEBUG2("entered");
								int v1change= rand() % v1s;
								int v2match= matching1[v1change];
								//DEBUG2("entered2");
								int z= smallIndexes[v1change][(rand()%topHowMany)];
								//int z=min_element( weightVec[v1change].begin(), weightVec[v1change].end() ) - weightVec[v1change].begin();
								//DEBUG2(z);
								//DEBUG2(v2match);
								//DEBUG2("entered1");
								if(z!=v2s)//&& weightVec[v1change][v2match]>weightVec[v1change][z])
								{
									//DEBUG2("opti");
									int v1change_1 = matching2[z];
									if(weightVec[v1change][z]==INT_MAX || weightVec[v1change_1][v2match]==INT_MAX ||
									weightVec[v1change_1][z]==INT_MAX || weightVec[v1change][v2match]==INT_MAX )
									{
										//DEBUG2("not_match");
										continue;
									}
									int addn=weightVec[v1change][z]+weightVec[v1change_1][v2match];
									int subn=weightVec[v1change][v2match]+weightVec[v1change_1][z];
									if(addn>subn)
									{
										//DEBUG2("not_improve");
										continue;
									}
									last_opti=opti;
									count_opti+=1;
									scoring=(scoring-subn+addn);
									matching1[v1change]=z;
									if(v1change_1!=v1s)
									{
										matching1[v1change_1]=v2match;
									}
									else
									{
										if(v2match!=v2s)
										{
											unmatchedFor1.insert(v2match);
										}
									}
									if(v2match!=v2s)
									{
										matching2[v2match]=v1change_1;
									}
									else
									{
										unmatchedFor1.erase(z);
									}
									matching2[z]=v1change;
								}
							}

							//DEBUG2(initialScore-scoring);
							//DEBUG2(count_opti);

							if(scoring<finalScore)
							{
								 // DEBUG2("######################################################");
								 // DEBUG2(last_opti);
								 // DEBUG2(scoring);
								 // DEBUG2(finalScore);
								 // DEBUG2(initialScore-scoring);
								 // DEBUG2(testrun);

								// DEBUG2("######################################################");
								finalMatching1=matching1;
								finalMatching2=matching2;
								finalScore=scoring;
							}
					}

					//DEBUG2("hey08");

					
					for(int i=0;i<v1s;i++)
					{

						string curr=pitstopSequenceVector[startNode]+" "+pitstopSequenceVector[endNode]+" "+to_string(vec1[i].tid)+" ";
						if(finalMatching1[i]==v2s)
						{
							curr+="-1 -1";
						}
						else
						{
							if((vec1[i].depTime+ttime1)<=vec2[finalMatching1[i]].depTime)
							{
								curr+=to_string(vec2[finalMatching1[i]].tid)+" "+pitstopSequenceVector[endNode];
							}
							else
							{
								curr+=to_string(vec2[finalMatching1[i]].tid)+" "+pitstopSequenceVector[startNode];
							}
						}
						out_m+=1;
						linesOut.push_back(curr);
					}
					for(int i=0;i<v2s;i++)
					{
						string curr="";
						if(finalMatching2[i]==v1s)
						{
							curr=pitstopSequenceVector[endNode]+" "+pitstopSequenceVector[startNode]+" "+to_string(vec2[i].tid)+" -1 -1";
						}
						if(!curr.empty())
						{
							out_m+=1;
							linesOut.push_back(curr);					
						}				
					}
					//DEBUG2("scoring");
					// DEBUG2("below are v1 deptimes");
					// for(int i=0;i<v1s;i++)
					// {
					// 	cout<<vec1[i].depTime<<" ";
					// }
					// cout<<endl;
					// DEBUG2("below are v2 deptimes");
					// for(int i=0;i<v2s;i++)
					// {
					// 	cout<<vec2[i].depTime<<" ";
					// }
					// cout<<endl;
					// for(int i=0;i<=v1s;i++)
					// {
					// 	for(int j=0;j<=v2s;j++)
					// 	{
					// 		cout<<weightVec[i][j]<<" ";
					// 	}
					// 	cout<<endl;
					// }
				
				if(finalScore!=INT_MAX)
				{
					finalTotalScore+=finalScore;
				}
			}

			//DEBUG2("h3");

			cout<<currTestCaseName<<" "<<(out_m+out_d+2)<<endl;
			cout<<out_d<<" "<<out_m<<endl;
			for(int i=0;i<linesOut.size();i++)
			{
				cout<<linesOut[i]<<endl;
			}
			cout<<finalTotalScore<<endl;

	}


    return 0;
}