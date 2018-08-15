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

int t,s,ow;
vector<string> uniqStops;
unordered_map< string, int> travelTime;
unordered_map< string, vector<tripNode> > tripsVec;

void uniquing()
{
	set<string> s;
	unsigned size = uniqStops.size();
	for( unsigned i = 0; i < size; ++i ) s.insert( uniqStops[i] );
	uniqStops.assign( s.begin(), s.end() );
}

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


int main()
{
	cin>>t>>s>>ow;
	vector<string> linesOut;
	int finalTotalScore=0;

	for(int i=0;i<t;i++)
	{
		string a,b;
		int time;
		cin>>a>>b>>time;
		uniqStops.push_back(a);
		uniqStops.push_back(b);
		travelTime[tripStr(a,b)]=time;
	}
	for(int i=0;i<s;i++)
	{
		int id;
		string a,b;
		int dtime;
		cin>>id>>a>>dtime>>b;
		struct tripNode dum;
		dum.tid=id;
		dum.depTime=dtime;
		if(tripsVec.find(tripStr(a,b))==tripsVec.end())
		{
			vector<tripNode> init;
			init.push_back(dum);
			tripsVec[tripStr(a,b)]=init;
		}
		else
		{
			tripsVec.at( tripStr(a,b) ).push_back(dum);
		}
	}
	uniquing();
	for(int startNode=0;startNode<uniqStops.size();startNode++)
	{
		vector<int> finalMatching1;
		vector<int> finalMatching2;
		int finalScore=INT_MAX;
		srand (time(NULL));
		for(int endNode=(startNode+1);endNode<uniqStops.size();endNode++)
		{
			string vec1Str = tripStr(uniqStops[startNode],uniqStops[endNode]);
			int ttime1=travelTime.at(vec1Str);
			vector<tripNode> vec1=tripsVec.at(vec1Str);
			string vec2Str = tripStr(uniqStops[endNode],uniqStops[startNode]);
			int ttime2=travelTime.at(vec2Str);
			vector<tripNode> vec2=tripsVec.at(vec2Str);
			sort(vec1.begin(), vec1.end());
			sort(vec2.begin(), vec2.end());
			int v1s=int(vec1.size());
			int v2s=int(vec2.size());
			vector< vector<int> > weightVec((v1s+1),vector<int>(v2s+1,INT_MAX));
			for(int i=0;i<v1s;i++)
			{
				weightVec[i][v2s]=costVal(max(ow-(vec1[i].depTime+ttime1),0));
			}
			for(int i=0;i<v2s;i++)
			{
				weightVec[v1s][i]=costVal(max(ow-(vec2[i].depTime+ttime2),0));
			}
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
			for(int testrun=0;testrun<100;testrun++)
			{
					vector<int> matching1(v1s,v2s);
					vector<int> matching2(v2s,v1s);
					set<int> unmatchedFor1;
					int scoring=0;
					for(int i=0;i<v2s;i++)
					{
						unmatchedFor1.insert(i);
					}

					//generate initial matching and score
					for(int i=0;i<v1s;i++)
					{
						//DEBUG2(i);
						int count=0;
						while(count<100)
						{
							int iSec = rand() % v2s;
							//DEBUG2(iSec);
							if(unmatchedFor1.find(iSec)!=unmatchedFor1.end() && weightVec[i][iSec]!=INT_MAX)
							{
								//can match
								matching1[i]=iSec;
								matching2[iSec]=i;
								scoring+=weightVec[i][iSec];
								unmatchedFor1.erase(iSec);
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
					for (set<int>::iterator it=unmatchedFor1.begin(); it!=unmatchedFor1.end(); ++it)
					{
						scoring+=weightVec[v1s][*it];
					}
					int initialScore=scoring;
					//DEBUG2(scoring);

					for(int opti=0;opti<100000;opti++)
					{
						//DEBUG2("entered");
						int v1change= rand() % v1s;
						int v2match= matching1[v1change];
						int z=min_element( weightVec[v1change].begin(), weightVec[v1change].end() ) - weightVec[v1change].begin();
						//DEBUG2(z);
						//DEBUG2(v2match);
						if(z!=v2s && weightVec[v1change][v2match]>weightVec[v1change][z])
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
							break;
						}
					}

					DEBUG2(initialScore-scoring);

					if(scoring<finalScore)
					{
						finalMatching1=matching1;
						finalMatching2=matching2;
						finalScore=scoring;
					}
			}


			for(int i=0;i<v1s;i++)
			{

				string curr=uniqStops[startNode]+" "+uniqStops[endNode]+" "+to_string(vec1[i].tid)+" ";
				if(finalMatching1[i]==v2s)
				{
					curr+="-1 -1";
				}
				else
				{
					if((vec1[i].depTime+ttime1)<=vec2[finalMatching1[i]].depTime)
					{
						curr+=to_string(vec2[finalMatching1[i]].tid)+" "+uniqStops[endNode];
					}
					else
					{
						curr+=to_string(vec2[finalMatching1[i]].tid)+" "+uniqStops[startNode];
					}
				}
				linesOut.push_back(curr);
			}
			for(int i=0;i<v2s;i++)
			{
				string curr="";
				if(finalMatching2[i]==v1s)
				{
					curr=uniqStops[endNode]+" "+uniqStops[startNode]+" "+to_string(vec2[i].tid)+" -1 -1";
				}
				if(!curr.empty())
				{
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
		}
		if(finalScore!=INT_MAX)
		{
			finalTotalScore+=finalScore;
		}
	}
	cout<<linesOut.size()<<endl;
	for(int i=0;i<linesOut.size();i++)
	{
		cout<<linesOut[i]<<endl;
	}
	cout<<finalTotalScore<<endl;

    return 0;
}