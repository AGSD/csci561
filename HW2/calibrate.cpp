#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<vector>
#include<deque>
#include<limits>
#include<algorithm>
#include<ctime>
#include<cmath>
#include<fstream>

#define dbg(x)
#define dbg2(x)
#define anl(x)

using namespace std;

int depthMax[5];
long long expectedNodes;
int actualNodes;

class point
{
public:
    int r,c;
    point(){}
    point(int a,int b){r=a,c=b;}
};

int** newBoard(int &n,int** c=NULL);   //returns a new board with nxn size, if provided will copy an old board's values onto the new board
void copyBoard(int** b,int** c,int &n);
void printBoard(int** b,int &n);
void deleteBoard(int** b,int &n);
void minimax(int** b,int &n,int &d);  //figure out which move to make, and return it as a point, also modify the board accordingly
void findMoves(vector<pair<point,int> >&m,int** b,int &n);  //
int minf(int**,int,signed int,signed int,signed int,int&,int);
int maxf(int**,int,signed int,signed int,signed int,int&,int);
int pickFruits(int**b,int &n,pair<point,int> &p);
bool cmpDec(const pair<point,int> &a,const pair<point,int> &b)  //does a come before b in decreasing order
{
    return a.second>b.second;
}

int main()
{
    freopen("calibration.txt","w",stdout);
    int n;
    int** initmat;
    int i,j,k;

    deque<int> d;
    int timer;
    int dlim;
    int denom=0;
    double avg=0;
    srand(time(0));
    for(n=5;n<=26;++n)
    {
        switch(n)
        {
        case 1:
        case 2:
        case 3:
            dlim = 16;
            break;
        case 4:
        case 5:
        case 6:
            dlim = 5;
            break;
        case 7:
        case 8:
        case 9:
        case 10:
            dlim = 4;
            break;
        case 11:
        case 12:
            dlim = 3;
            break;
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:

        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
            dlim = 2;
            break;
        }

            for(k=0;k<10;++k)
            {
                initmat=newBoard(n);

                for(i=0;i<n;++i)
                {
                    d.push_back(i%10);
                }
                for(i=0;i<n;++i)
                {
                    for(j=0;j<n;++j)
                    {
                        initmat[i][j] = d[j];
                    }
                    d.push_front(d.back());
                    d.pop_back();
                    d.push_front(d.back());
                    d.pop_back();
                }
                /*
                for(i=0;i<n;++i)
                {
                    for(j=0;j<n;++j)
                        initmat[i][j] = rand()%5;
                }*/

                timer = clock();
                minimax(initmat,n,dlim);
                deleteBoard(initmat,n);
                d.clear();
                timer = clock()-timer;

                //dbg(cout<<(double)(timer)/CLOCKS_PER_SEC<<", per node = ";)
                dbg(cout<<n<<" "<<k<<" "<<)
                avg+=((double)(timer)/CLOCKS_PER_SEC)/actualNodes/(n*n);
                ++denom;

                //dbg(cout<<" "<<n<<" "<<k<<", dlim= "<<dlim<<", sqrtExpected= "<<sqrt(expectedNodes)<<" actual= "<<actualNodes<<" percent= "<<(double)actualNodes/sqrt(expectedNodes)<<" naturalised="<<((double)(timer)/CLOCKS_PER_SEC)/actualNodes/(n*n)<<endl;)

            }
    }
    cout<<avg/denom;

    return 0;
}

int** newBoard(int &n,int** copy)
{
    int** b;
    int i;
    b = new int*[n];
    for(i=0;i<n;++i)
        b[i] = new int[n];
    if(copy!=NULL)
        copyBoard(copy,b,n);

    return b;
}

void copyBoard(int** b,int** c,int &n)   //copy from b to c
{
    int i,j;
    for(i=0;i<n;++i)
        for(j=0;j<n;++j)
            c[i][j] = b[i][j];
}

void deleteBoard(int** b,int &n)
{
    int i;
    for(i=0;i<n;++i)
        delete b[i];
    delete b;
}

void printBoard(int** b,int &n)
{
    int i,j;
    for(i=0;i<n;++i)
    {
        for(j=0;j<n;++j)
        {
            if(b[i][j]!=-1)
                printf("%d",b[i][j]);
            else
                printf("*");
        }
        printf("\n");
    }
}

void minimax(int** b,int &n,int &dlimit) //run minimax and print out the required things
{
    vector<pair<point,int> >  moves;moves.reserve(n*n);

    findMoves(moves,b,n);   //find out what moves we can make
    sort(moves.begin(),moves.end(),cmpDec);


    actualNodes=1;
    expectedNodes = pow(moves.size(),dlimit);
    for(int i=0;i<5;++i)
        depthMax[i] = moves.size();
    //cout<<"depth 0: "<<moves.size()<<endl;


    signed int alpha=numeric_limits<signed int>::min();
    signed int beta=numeric_limits<signed int>::max();
    int i;
    signed int score;
    int answer;
    signed int v;

    int** bc=newBoard(n,b);

    for(i=0;i<moves.size();++i)
    {
        score = pickFruits(bc,n,moves[i]);
        v = minf(bc,n,score,alpha,beta,dlimit,1);
        if(v>alpha)
        {
            alpha = v;
            answer = i;
        }
        copyBoard(b,bc,n);
    }
}

int evalFn(vector<pair<point,int> > &moves,bool max=true)
{
    bool maxMove=max;
    int myScore = 0;
    double mul=1;
    for(int i=0;i<moves.size();++i)
    {
        double tmpScore = moves[i].second*mul;
        mul*=0.5;
        if(maxMove){
            myScore += tmpScore*tmpScore;
        }
        else
            myScore -= tmpScore*tmpScore;
        if(tmpScore<0.0001)
            break;
        maxMove = !maxMove;
    }
    return myScore;

}

int maxf(int**b,int n,signed int score,signed int alpha,signed int beta,int &depthLimit,int myDepth)
{
    signed int v,myScore=0;
    int i;

    int** bc=newBoard(n,b);

    vector<pair<point,int> >  moves;moves.reserve(n*n);

    findMoves(moves,b,n);
    sort(moves.begin(),moves.end(),cmpDec);

    ++actualNodes;

    if(moves.size()==0)
    {
        moves.empty();
        deleteBoard(bc,n);
        return score;
    }

    if(depthLimit==myDepth)
    {
        moves.empty();
        deleteBoard(bc,n);
        return evalFn(moves,true)+score;
    }



    for(i=0;i<moves.size();++i)
    {
        myScore = pickFruits(bc,n,moves[i]);
        v = minf(bc,n,score+myScore,alpha,beta,depthLimit,myDepth+1);
        if(v>alpha)
            alpha = v;

        if(alpha>=beta)
        {
            deleteBoard(bc,n);
            moves.empty();
            return alpha;

        }
        copyBoard(b,bc,n);
    }
    moves.empty();
    deleteBoard(bc,n);
    return alpha;
}

int minf(int**b,int n,signed int score,signed int alpha,signed int beta,int &depthLimit,int myDepth)
{
    signed int v,myScore=0;
    int i;

    int** bc=newBoard(n,b);

    vector<pair<point,int> >  moves;moves.reserve(n*n);

    findMoves(moves,b,n);
    sort(moves.begin(),moves.end(),cmpDec);

    ++actualNodes;

    if(moves.size()==0)
    {
        moves.empty();
        deleteBoard(bc,n);
        return score;
    }

    if(depthLimit==myDepth)
    {
        moves.empty();
        deleteBoard(bc,n);
        return evalFn(moves,false)+score;
    }



    for(i=0;i<moves.size();++i)
    {
        myScore = -pickFruits(bc,n,moves[i]);
        v = maxf(bc,n,score+myScore,alpha,beta,depthLimit,myDepth+1);
        if(v<beta)
            beta = v;

        if(alpha>=beta)
        {
            moves.empty();
            deleteBoard(bc,n);
            return beta;
        }
        copyBoard(b,bc,n);
    }
    moves.empty();
    deleteBoard(bc,n);
    return beta;
}

int pickFruits(int** b,int &n,pair<point,int> &pa)
{
    int i,j;
    int moveHere,moveFrom;
    int name;
    deque<point> q;

    //put -1 where we need to remove fruits
    i=pa.first.r;
    j=pa.first.c;
    name = b[i][j];
    q.push_back(point(i,j));
    b[i][j]=-1;
    while(!q.empty())
    {
        point p = q.front();
        q.pop_front();
        int r=p.r, c=p.c;
        if(r+1<n && b[r+1][c]==name)
        {
            q.push_back(point(r+1,c));
            b[r+1][c]=-1;
        }
        if(r-1>=0 && b[r-1][c]==name)
        {
            q.push_back(point(r-1,c));
            b[r-1][c]=-1;
        }
        if(c+1<n && b[r][c+1]==name)
        {
            q.push_back(point(r,c+1));
            b[r][c+1]=-1;
        }
        if(c-1>=0 && b[r][c-1]==name)
        {
            q.push_back(point(r,c-1));
            b[r][c-1]=-1;
        }
    }


    //Apply gravity
    for(j=0;j<n;++j)
    {
        for(i=n-1;i>=0;--i)
        {
            if(b[i][j]==-1)
            {
                moveHere=i;
                break;
            }
        }
        if(i==-1)   //entire column was full
            continue;

        for(i=moveHere-1;i>=0;--i)
        {
            if(b[i][j]!=-1)
            {
                moveFrom=i;
                break;
            }
        }
        if(i==-1)   //entire top column was empty
            continue;

        while(moveFrom>-1)  //shifting elements for the entire column
        {
            b[moveHere--][j] = b[moveFrom][j];
            b[moveFrom--][j] = -1;
            while(moveFrom>-1 && b[moveFrom][j]==-1)
                --moveFrom;
        }

    }
    return pa.second*pa.second;
}

void findMoves(vector<pair<point,int> > &m,int** b,int &n)
{
    int i,j;
    deque<point> q;
    int name;
    int last;
    int len;

    int** bc = newBoard(n,b);
    for(j=0;j<n;++j)    //select column
    {
        for(i=n-1;i>=0;--i) //select row
        {
            if(bc[i][j]==-1)  //empty position shows up, we don't need to look in any upper row
                break;
            if(bc[i][j]!=-2)  //is not a position which has already been explored
            {

                len=1;
                m.push_back(pair<point,int>(point(i,j),1));


                name = bc[i][j];

                anl(tfm=clock();)
                q.push_back(point(i,j));
                anl(timeFindMoves+=clock()-tfm;)

                bc[i][j]=-2;
                while(!q.empty())
                {
                    point p = q.front();
                    q.pop_front();
                    int r=p.r, c=p.c;
                    if(r+1<n && bc[r+1][c]==name)
                    {
                        anl(tfm=clock();)
                        q.push_back(point(r+1,c));
                        anl(timeFindMoves+=clock()-tfm;)
                        ++len;

                        bc[r+1][c]=-2;
                    }
                    if(r-1>=0 && bc[r-1][c]==name)
                    {
                        anl(tfm=clock();)
                        q.push_back(point(r-1,c));
                        anl(timeFindMoves+=clock()-tfm;)
                        ++len;

                        bc[r-1][c]=-2;
                    }
                    if(c+1<n && bc[r][c+1]==name)
                    {
                        anl(tfm=clock();)
                        q.push_back(point(r,c+1));
                        anl(timeFindMoves+=clock()-tfm;)
                        ++len;

                        bc[r][c+1]=-2;
                    }
                    if(c-1>=0 && bc[r][c-1]==name)
                    {
                        anl(tfm=clock();)
                        q.push_back(point(r,c-1));
                        anl(timeFindMoves+=clock()-tfm;)
                        ++len;

                        bc[r][c-1]=-2;
                    }
                }
                m.back().second=len;

            }
        }
    }

    deleteBoard(bc,n);
}
