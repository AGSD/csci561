#include<iostream>
#include<cstdio>
#include<vector>
using namespace std;

vector<vector<short> > v;

bool collision(int r,int c)
{
    int i,j;
    int n = v.size();
    for(i=r+1;i<n;++i)    //check column
    {
        if(v[i][c]==2)
            break;
        if(v[i][c]==1)
            return true;
    }
    for(i=c+1;i<n;++i)    //check row
    {
        if(v[r][i]==2)
            break;
        if(v[r][i]==1)
            return true;
    }
    for(i=r+1,j=c+1;i<n && j<n;++i,++j)
    {
        if(v[i][j]==2)
            break;
        if(v[i][j]==1)
            return true;
    }
    for(i=r+1,j=c-1;i<n && j>-1;++i,--j)
    {
        if(v[i][j]==2)
            break;
        if(v[i][j]==1)
            return true;
    }
    return false;
}

int main()
{
    freopen("output.txt","r",stdin);
    char c;
    v.push_back(vector<short>());
    c=getchar();
    if(c=='F')
    {
        cout<<"FAIL CASE";
        return 0;
    }
    if(c=='O')
    {
        getchar(); getchar();
    }
    while((c=getchar())!=EOF)
    {
        if(c=='\n')
            v.push_back(vector<short>());
        else
            v.back().push_back(c-'0');
    }
    v.pop_back();

    int i,j;
    int count=0;
    for(i=0;i<v.size();++i)
    {
        for(j=0;j<v[i].size();++j){
            if(v[i][j]==1){
                    ++count;
                if(collision(i,j))
                {
                    cout<<"COULD NOT PASS";
                    return 0;
                }
            }
        }

    }
    cout<<"PASSED! found queens="<<count;
    return 0;
}
