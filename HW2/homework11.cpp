//AI agent for CSCI 561 HW2 Fall 2017, USC
//Author: Aditya Gaur

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

#define DEPTHLIMIT 4
#define dbg(x)
#define dbg2(x)
#define dbg3(x) x
#define anl(x)

using namespace std;

dbg(
int depthMax[5];
long long expectedNodes;
int actualNodes;
)


anl(
int timeFindMoves;int tfm;
int timeSorter;int ts;
int timeNewBoard;int tnb;
int timeDeleteBoard;int tdb;
int timePickFruits;int tpf;
int timePushes;int tp;
    )



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
void minimax(int** b,int &n,double &t);  //figure out which move to make, and return it as a point, also modify the board accordingly
void findMoves(vector<pair<point,int> >&m,int** b,int &n);  //
int minf(int**,int,signed int,signed int,signed int,int,int);
int maxf(int**,int,signed int,signed int,signed int,int,int);
int pickFruits(int**b,int &n,pair<point,int> &p);
bool cmpDec(const pair<point,int> &a,const pair<point,int> &b)  //does a come before b in decreasing order
{
    return a.second>b.second;
}
int dlim;


int main()
{
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
	int n,p;
	double t;
    int i,j;
    int** b;
    string s;

    scanf("%d%d%lf",&n,&p,&t);
    cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');//getchar();
    b=newBoard(n);
    for(i=0;i<n;++i)    //input initial board
    {
        getline(cin,s);
        for(j=0;j<n;++j)
        {
            if(s[j]=='*')
                b[i][j] = -1;
            else
                b[i][j] = s[j]-'0';
        }
    }
    minimax(b,n,t);
    anl(cout<<(double)timeFindMoves/CLOCKS_PER_SEC<<endl<<(double)timePickFruits/CLOCKS_PER_SEC<<endl;
    cout<<(double)timeNewBoard/CLOCKS_PER_SEC<<endl<<(double)timeDeleteBoard/CLOCKS_PER_SEC<<endl;
    cout<<(double)timeSorter/CLOCKS_PER_SEC<<endl<<(double)timePushes/CLOCKS_PER_SEC<<endl;)
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

int calcDlim(int b,int &n,double &tleft)
{
    //return DEPTHLIMIT;


    tleft = tleft*95/100;   //save a cushion of 5% time
    if(tleft<0.05)
        return 1;
    if(n<=3)
        return 10;  //more than it will ever get, basically go through entire tree
    if(b<=10)
        return n*n;

    ifstream in;

    in.open("calibration.txt");
    double tpn;  //
    in>>tpn;    //input average time per node( normalized by board size)
    in.close();
    dbg(cout<<tpn<<endl;)

    //parameters to play with are b,n,tn
    long long enodes;    //expected number of nodes to expand
    double timeApprox,timeAllotted;
    int d=2;

    //Calculate how much time to allot
    int movesLeft=b/2;
    int lim = log2(b);
    dbg(cout<<"lim = "<<lim<<endl;)
    timeAllotted = tleft*pow(0.8,lim);
    dbg(cout<<"time allotted = "<<timeAllotted<<endl;)

    //Use the allotted time to figure out the depth to go to
    enodes = pow(b,d);
    timeApprox = sqrt(enodes)*10*n*n*tpn;

    for(;;++d)
    {
        enodes = pow(b,d);
        dbg(cout<<"depth d="<<d<<" sqrt enodes="<<sqrt(enodes)<<endl;)
        if(d==3)
            timeApprox = sqrt(enodes)*20*n*n*tpn;
        else
            timeApprox = sqrt(enodes)*10*n*n*tpn;
        dbg(cout<<"depth d="<<d<<" timeApprox="<<timeApprox<<endl;)
        if(timeApprox>timeAllotted)
            break;
    }
    dbg(cout<<"out of for loop with d="<<d<<endl;)
    if(d==2)
        d=1;
    else
        d=d-1;


    dbg(cout<<"Chosen depth = "<<d<<endl;)
    return d;

}

void minimax(int** b,int &n,double &t) //run minimax and print out the required things
{
    vector<pair<point,int> >  moves;moves.reserve(n*n);

    findMoves(moves,b,n);   //find out what moves we can make
    sort(moves.begin(),moves.end(),cmpDec);

    dbg(
        actualNodes=1;
        expectedNodes = pow(moves.size(),DEPTHLIMIT);
        for(int i=0;i<5;++i)
            depthMax[i] = moves.size();
        cout<<"depth 0: "<<moves.size()<<endl;
    )

    signed int alpha=numeric_limits<signed int>::min();
    signed int beta=numeric_limits<signed int>::max();
    int i;
    signed int score;
    int answer;
    signed int v;

    int** bc=newBoard(n,b);
    dlim = calcDlim(moves.size(),n,t);
    dbg3(ofstream out;)
    dbg3(out.open("savedDepths.txt",ios_base::app);
    out<<dlim<<endl;)
    for(i=0;i<moves.size();++i)
    {
        score = pickFruits(bc,n,moves[i]);
        v = minf(bc,n,score,alpha,beta,dlim,1);
        if(v>alpha)
        {
            alpha = v;
            answer = i;
        }
        copyBoard(b,bc,n);
    }
    int r,c;
    r = moves[answer].first.r;
    c = moves[answer].first.c;
    printf("%c%d\n",c+'A',r+1);
    pickFruits(b,n,moves[answer]);
    printBoard(b,n);
    dbg(cout<<"Expected nodes "<<expectedNodes<<endl<<"Actual Nodes "<<actualNodes<<endl;)
    dbg2(cout<<"best value = "<<alpha<<endl;)
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

int maxf(int**b,int n,signed int score,signed int alpha,signed int beta,int depthLimit,int myDepth)
{
    signed int v,myScore=0;
    int i;

    int** bc=newBoard(n,b);

    vector<pair<point,int> >  moves;moves.reserve(n*n);

    findMoves(moves,b,n);
    sort(moves.begin(),moves.end(),cmpDec);

    dbg(
        actualNodes++;
        //if(depthMax[myDepth]<moves.size()){
       //     cout<<"depth "<<myDepth<<": "<<moves.size()<<endl;
       // }
    )

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

int minf(int**b,int n,signed int score,signed int alpha,signed int beta,int depthLimit,int myDepth)
{
    signed int v,myScore=0;
    int i;

    int** bc=newBoard(n,b);

    vector<pair<point,int> >  moves;moves.reserve(n*n);

    findMoves(moves,b,n);
    sort(moves.begin(),moves.end(),cmpDec);

    dbg(
        actualNodes++;
       // if(depthMax[myDepth]<moves.size()){
       //     cout<<"depth "<<myDepth<<": "<<moves.size()<<endl;
       // }
    )

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
