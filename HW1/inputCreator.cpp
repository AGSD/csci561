#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>

#define tree false
using namespace std;

int main()
{
    freopen("input.txt","w",stdout);
    int n=34;
    int p=34;
    int t=0;
    int i,j;
    //cin>>n>>p>>t;
    srand(time(0));
    cout<<"DFS\n"<<n<<"\n"<<p<<endl;
    for(i=0;i<n;++i)
    {
        for(j=0;j<n;++j)
        {
            if(tree)
            {
                if(rand()%33==0 && rand()%33==0)
                    ++t,cout<<"2";
                else
                    cout<<"0";
            }
            else
                cout<<"0";
        }
        cout<<endl;
    }
    cout<<"\n\n"<<"total trees "<<t;
    return 0;
}
