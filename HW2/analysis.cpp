#include<iostream>
#include<cstdio>
#include<vector>
#include<deque>
#include<cstdlib>
#include<fstream>
#include<limits>
#include<ctime>
#include<cstring>
#include<string>
#include<cmath>
#include<deque>
#include<algorithm>

#define dbg(x) x
#define DEPTHLIMIT 3

dbg(
int depthMax[5];
int expectedNodes;
int actualNodes;
)

using namespace std;

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
void minimax(int** b,int &n);  //figure out which move to make, and return it as a point, also modify the board accordingly
void findMoves(vector<vector<point> >&m,int** b,int &n);  //
int minf(int**,int,signed int,signed int,signed int,int,int);
int maxf(int**,int,signed int,signed int,signed int,int,int);
int pickFruits(int**b,int &n,vector<point> &p);
void sorter(vector<vector<point> >&m,vector<point> &s,int &n,bool);
bool cmpInc(const point &a,const point &b)  //does a come before b in increasing order
{
    return a.r<b.r;
}
bool cmpDec(const point &a,const point &b)  //does a come before b in decreasing order
{
    return a.r>b.r;
}

int main()
{
    ofstream out;
    int n=26,p=9;
    int** initmat;
    int** newmat;
    int timerStart;
    char col;
    int row;
    char c;
    bool match,emptyboard;
    int score1tmp,score2tmp;
    int score1=0,score2=0;
    int i,j;
    bool makeinput=true;
    vector<vector<point> > moves;
    ifstream in;
    double t1=300.00,t2=300.00;
    deque<int> d;
    int timer;
    srand(time(0));
    for(i=0;i<100;++i){
    //make initial board and also save it in save.txt
        //n = rand()%26+1;
        n = 15;
        initmat=newBoard(n);
        /*for(i=0;i<n;++i)
        {
            d.push_back(i%10);
        }
        for(i=0;i<n;++i)
        {
            for(j=0;j<n;++j)
            {
                initmat[i][j] = d[j];
                cout<<initmat[i][j];
            }
            cout<<endl;
            d.push_front(d.back());
            d.pop_back();
            d.push_front(d.back());
            d.pop_back();
        }*/

        for(i=0;i<n;++i)
        {
            for(j=0;j<n;++j)
            {
                initmat[i][j]=rand()%10;
            }
        }

        timer = clock();
        minimax(initmat,n);
        deleteBoard(initmat,n);
        d.clear();
        timer = clock()-timer;
        cout<<(double)(timer)/CLOCKS_PER_SEC<<", per node = "<<((double)(timer)/CLOCKS_PER_SEC)/actualNodes<<endl;
    }

    return 0;
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
void minimax(int** b,int &n) //run minimax and print out the required things
{
    vector<vector<point> >  moves;
    vector<point> srt;
    findMoves(moves,b,n);   //find out what moves we can make
    sorter(moves,srt,n,false);

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

    for(i=0;i<moves.size();++i)
    {
        score = pickFruits(bc,n,moves[srt[i].c]);
        v = minf(bc,n,score,alpha,beta,DEPTHLIMIT,1);
        if(v>alpha)
        {
            alpha = v;
            answer = srt[i].c;
        }
        copyBoard(b,bc,n);
    }

    dbg(cout<<"Expected nodes "<<expectedNodes<<", Actual Nodes "<<actualNodes<<endl;
    cout<<"Percentage = "<<(double)actualNodes/expectedNodes*100<<endl;)
}

int maxf(int**b,int n,signed int score,signed int alpha,signed int beta,int depthLimit,int myDepth)
{
    signed int v,myScore;
    int i;

    int** bc=newBoard(n,b);

    vector<vector<point> >  moves;
    vector<point> srt;
    findMoves(moves,b,n);
    sorter(moves,srt,n,false);

    dbg(
        actualNodes++;
        //if(depthMax[myDepth]<moves.size()){
           // cout<<"depth "<<myDepth<<": "<<moves.size()<<endl;
        //}
    )

    if(moves.size()==0)
    {
        moves.empty();
        deleteBoard(bc,n);
        return score;
    }

    if(depthLimit==myDepth)
    {
        myScore = moves[srt[0].c].size();
        myScore *= myScore;
        moves.empty();
        deleteBoard(bc,n);
        return myScore+score;
    }



    for(i=0;i<moves.size();++i)
    {
        myScore = pickFruits(bc,n,moves[srt[i].c]);
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
    signed int v,myScore;
    int i;

    int** bc=newBoard(n,b);

    vector<vector<point> >  moves;
    vector<point> srt;
    findMoves(moves,b,n);
    sorter(moves,srt,n,true);

    dbg(
        actualNodes++;
       // if(depthMax[myDepth]<moves.size()){
           // cout<<"depth "<<myDepth<<": "<<moves.size()<<endl;
        //}
    )

    if(moves.size()==0)
    {
        moves.empty();
        deleteBoard(bc,n);
        return score;
    }

    if(depthLimit==myDepth)
    {
        myScore = moves[srt[srt.size()-1].c].size();
        myScore *= myScore * -1;
        moves.empty();
        deleteBoard(bc,n);
        return myScore+score;
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

void sorter(vector<vector<point> >&m,vector<point> &s,int &n,bool inc=true)
{
    int i;
    for(i=0;i<m.size();++i)
        s.push_back(point(m[i].size(),i));
    if(inc)
        sort(s.begin(),s.end(),cmpInc);
    else
        sort(s.begin(),s.end(),cmpDec);
}

int pickFruits(int** b,int &n,vector<point> &p)
{
    int i,j;
    int moveHere,moveFrom;
    point pos;
    for(i=0;i<p.size();++i)
    {
        pos=p[i];
        b[pos.r][pos.c] = -1;
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
    return p.size()*p.size();
}

void findMoves(vector<vector<point> > &m,int** b,int &n)
{
    int i,j;
    deque<point> q;
    int name;
    int last;

    int** bc = newBoard(n,b);
    for(j=0;j<n;++j)    //select column
    {
        for(i=n-1;i>=0;--i) //select row
        {
            if(bc[i][j]==-1)  //empty position shows up, we don't need to look in any upper row
                break;
            if(bc[i][j]!=-2)  //is not a position which has already been explored
            {
                m.push_back(vector<point>());
                last=m.size()-1;
                m[last].push_back(point(i,j));
                name = bc[i][j];
                q.push_back(point(i,j));
                bc[i][j]=-2;
                while(!q.empty())
                {
                    point p = q.front();
                    q.pop_front();
                    int r=p.r, c=p.c;
                    if(r+1<n && bc[r+1][c]==name)
                    {
                        q.push_back(point(r+1,c));
                        m[last].push_back(point(r+1,c));
                        bc[r+1][c]=-2;
                    }
                    if(r-1>=0 && bc[r-1][c]==name)
                    {
                        q.push_back(point(r-1,c));
                        m[last].push_back(point(r-1,c));
                        bc[r-1][c]=-2;
                    }
                    if(c+1<n && bc[r][c+1]==name)
                    {
                        q.push_back(point(r,c+1));
                        m[last].push_back(point(r,c+1));
                        bc[r][c+1]=-2;
                    }
                    if(c-1>=0 && bc[r][c-1]==name)
                    {
                        q.push_back(point(r,c-1));
                        m[last].push_back(point(r,c-1));
                        bc[r][c-1]=-2;
                    }
                }
            }
        }
    }

    deleteBoard(bc,n);
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


