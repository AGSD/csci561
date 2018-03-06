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

#define dbg(x)
#define MAKEINPUT true

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
void findMoves(vector<vector<point> >&m,int** b,int &n,int,int);  //
int minf(int**,int,int,int,int,int,int);
int maxf(int**,int,int,int,int,int,int);
int pickFruits(int**b,int &n,vector<point> &p);

int main(int argc, char *argv[])
{
    int move=0;
    ofstream out;
    ofstream save;
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
    double t1save,t2save;

    //read command line arguments
    char agent1[100],agent2[100];
    dbg(for(i=0;i<argc;++i)
    cout<<argv[i]<<endl;)
    if(argc==1)
    {
        strcpy(agent1,"agent1.exe");
        strcpy(agent2,"agent2.exe");
        n=10;
    }
    else
    {
        strcpy(agent1,argv[1]);dbg(cout<<"just copied "<<agent1<<" wow"<<endl;)
        strcpy(agent2,argv[2]);
        if(strcmp(argv[3],"true")==0)
        {
            makeinput = true;
            string s;
            n = atoi(argv[4]);
        }
        else
        {
            makeinput = false;
        }
        dbg(cout<<"just copied "<<agent1<<" wow"<<endl;)
    }
    dbg(cout<<"just copied "<<agent1<<" wow"<<endl;)
    dbg(cout<<agent1<<" "<<agent2<<" "<<makeinput<<" "<<n<<endl;)


    srand(time(0));
    if(makeinput)
    {
        initmat=newBoard(n);
        newmat=newBoard(n);

        //make initial board and also save it in save.txt
        save.open("save.txt",ios_base::app);
        save<<n<<endl<<p<<endl<<t1<<endl;
        out.open("input.txt");
        for(i=0;i<n;++i)
        {
            for(j=0;j<n;++j)
            {
                initmat[i][j]=rand()%10;
                save<<initmat[i][j];
            }
            save<<endl;
        }
        save.close();
        out.close();
    }
    else
    {

        initmat=newBoard(n);
        newmat=newBoard(n);
        in.open("input.txt");
        in>>n>>p>>t1;

        for(i=0;i<n;++i)
        {
            for(j=0;j<n;++j)
            {
                in>>c;
                if(c=='*')
                    initmat[i][j]=-1;
                else
                    initmat[i][j]=c-'0';
            }
        }

        in.close();
    }

    while(t1>0 && t2>0)
    {
        out.open("input.txt");
        out<<n<<endl<<p<<endl<<t1<<endl;
        for(i=0;i<n;++i)
        {
            for(j=0;j<n;++j)
            {
                if(initmat[i][j]!=-1)
                    out<<initmat[i][j];
                else
                    out<<'*';
            }
            out<<endl;
        }
        out.close();

        timerStart = clock();
        system(agent1);
        t1save=t1;
        t1 = t1 - ((clock()-timerStart)/(double)CLOCKS_PER_SEC);

        ++move;
        if(t1<0)
        {
            cout<<"Agent 1 took too much time";
            break;
        }

        in.open("output.txt");
        in>>col>>row;

        for(i=0;i<n;++i)
        {
            for(j=0;j<n;++j)
            {
                in>>c;
                if(c=='*')
                    newmat[i][j]=-1;
                else
                    newmat[i][j]=c-'0';
            }
        }
        in.close();
        moves.clear();
        row-=1;

        if(initmat[row][col-'A']==-1)
        {
            cout<<"Player 1 tried to pick up from an empty spot, he loses.";
            return 0;
        }
        findMoves(moves,initmat,n,row,col-'A');

        dbg(for(i=0;i<moves[0].size();++i)
        cout<<"("<<moves[0][i].r<<","<<moves[0][i].c<<") ";
        cout<<endl;)
/*
        for(i=0;i<moves.size();++i)
            if(moves[i][0].r==row && moves[i][0].c==col-'A')
                break;
*/
        score1 += pickFruits(initmat,n,moves[0]);

        match=true;
        emptyboard=true;
        for(i=0;i<n;++i)
        {
            for(j=0;j<n;++j)
            {
                if(newmat[i][j]!=initmat[i][j])
                {
                    match=false;
                    break;
                }
                if(newmat[i][j]!=-1)
                    emptyboard=false;
            }
            if(!match)
                break;
        }

        if(!match)
        {
            cout<<"Boards did not match, player 1 is faulty"<<endl;
            cout<<"\nExpected board:\n";
            printBoard(initmat,n);
            cout<<endl<<"Got board:\n";
            printBoard(newmat,n);
            dbg(cout<<"ending program"<<endl;)
            return 0;
            dbg(cout<<"uh oh";)
        }

        //output what agent1 did
        cout<<"\nAgent 1:\n"<<col<<row+1<<endl;
        printBoard(initmat,n);
        cout<<"Score1 = "<<score1<<", Score2="<<score2<<endl;
        cout<<"time1 = "<<t1<<", time2 = "<<t2<<endl;
        cout<<"time1diff = "<<t1save-t1<<endl;
        cout<<"move# "<<move<<endl;

        if(emptyboard)
        {
            cout<<"Board is empty, ending program with final scores= "<<score1<<" "<<score2;
            break;
        }

        //print board for agent2
        out.open("input.txt");
        out<<n<<endl<<p<<endl<<t2<<endl;
        for(i=0;i<n;++i)
        {
            for(j=0;j<n;++j)
            {
                if(initmat[i][j]!=-1)
                    out<<initmat[i][j];
                else
                    out<<'*';
            }
            out<<endl;
        }
        out.close();
        timerStart = clock();
        system(agent2);
        t2save=t2;
        t2 = t2 - ((clock()-timerStart)/(double)CLOCKS_PER_SEC);
        if(t2<0)
        {
            cout<<"Agent 2 took too much time";
            break;
        }

        in.open("output.txt");
        in>>col>>row;

        for(i=0;i<n;++i)
        {
            for(j=0;j<n;++j)
            {
                in>>c;
                if(c=='*')
                    newmat[i][j]=-1;
                else
                    newmat[i][j]=c-'0';
            }
        }
        in.close();

        moves.clear();
        row-=1;

        if(initmat[row][col-'A']==-1)
        {
            cout<<"Player 2 tried to pick up from an empty spot, he loses.";
            return 0;
        }

        findMoves(moves,initmat,n,row,col-'A');

        /*for(i=0;i<moves.size();++i)
            if(moves[i][0].r==row && moves[i][0].c==col-'A')
                break;
        */
        score2 += pickFruits(initmat,n,moves[0]);

        match=true;
        emptyboard=true;
        for(i=0;i<n;++i)
        {
            for(j=0;j<n;++j)
            {
                if(newmat[i][j]!=initmat[i][j])
                {
                    match=false;
                    break;
                }
                if(newmat[i][j]!=-1)
                    emptyboard=false;
            }
            if(!match)
                break;
        }

        if(!match)
        {

            cout<<"Boards did not match, player 2 is faulty"<<endl;
            cout<<"\nExpected board:\n";
            printBoard(initmat,n);
            cout<<endl<<"Got board:\n";
            printBoard(newmat,n);

            return 0;
        }

        //output what agent2 did
        cout<<"\nAgent 2:\n"<<col<<row+1<<endl;
        printBoard(initmat,n);
        cout<<"Score1 = "<<score1<<", Score2="<<score2<<endl;
        cout<<"time1 = "<<t1<<", time2 = "<<t2<<endl;
        cout<<"time2diff = "<<t2save-t2<<endl;

        if(emptyboard)
        {
            cout<<"Board is empty, ending program with final scores= "<<score1<<" "<<score2<<endl;
            cout<<"Total moves made= "<<move<<endl;
            break;
        }



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


void copyBoard(int** b,int** c,int &n)   //copy from b to c
{
    int i,j;
    for(i=0;i<n;++i)
        for(j=0;j<n;++j)
            c[i][j] = b[i][j];
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


void deleteBoard(int** b,int &n)
{
    int i;
    for(i=0;i<n;++i)
        delete b[i];
    delete b;
}


void findMoves(vector<vector<point> > &m,int** b,int &n,int row,int col)
{
    int i,j;
    deque<point> q;
    int name;
    int last;

    int** bc = newBoard(n,b);
    i=row,j=col;
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
    deleteBoard(bc,n);
}
