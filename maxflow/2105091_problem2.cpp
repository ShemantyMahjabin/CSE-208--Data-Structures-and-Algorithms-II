#include<iostream>
#include<bits/stdc++.h>
using namespace std;


int m,n;
struct BioData
{
    int height;
    int age;
    int divorced;
};

bool isMatch(const BioData& man,const BioData& woman)
{
    if((abs(man.height-woman.height)<=10) &&
            (abs(man.age-woman.age)<=5) &&
            (man.divorced==woman.divorced))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void makeBipartiteGraph(vector<BioData>&menBiodata,vector<BioData>&womenBiodata,vector<vector<int>>&bGraph)
{
    for(int i=1; i<=m; i++)
    {
        for(int j=1; j<=n; j++)
        {
            if(isMatch(menBiodata[i],womenBiodata[j]))
            {
                bGraph[i][j]=1;
            }
        }
    }
}

bool bpm(vector<vector<int>>&bGraph,int man,vector<int>&match,vector<bool>&seen)
{
    for(int woman=1; woman<=n; woman++)
    {
        if(bGraph[man][woman] && !seen[woman])
        {
            seen[woman]=true;
            if(match[woman]==-1 || bpm(bGraph,match[woman],match,seen))
            {
                match[woman]=man;
                return true;
            }
        }
    }
    return false;
}


int main()
{
    cin>>m>>n;
    vector<BioData>menBiodata(m+1);
    vector<BioData>womenBiodata(n+1);

    for(int i=1; i<=m; i++)
    {
        cin>>menBiodata[i].height>>menBiodata[i].age>>menBiodata[i].divorced;
    }
    for(int j=1; j<=n; j++)
    {
        cin>>womenBiodata[j].height>>womenBiodata[j].age>>womenBiodata[j].divorced;
    }
    vector<vector<int>>bGraph(m+1,vector<int>(n+1,0));
    makeBipartiteGraph(menBiodata,womenBiodata,bGraph);

    vector<int>match(n+1,-1);
    int result=0;

    for(int man=1; man<=m; man++)
    {
        vector<bool>seen(n+1,false);
        if(bpm(bGraph,man,match,seen))
        {
            result++;
        }
    }
    cout<<result<<endl;

    return 0;

}
