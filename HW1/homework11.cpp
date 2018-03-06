//homework_better3SA
#include<iostream>
#include<cstdio>
#include<vector>
#include<deque>
#include<utility>
#include<ctime>
#include<list>
#include<cstdlib>
#include<cmath>
#include<random>
#define dbg(x)
#define dbg2(x)
#define timer(x)
#define SATIMETHRESH 250

timer(
int tim;
int stim;
int etim;
)
typedef long long ll;
using namespace std;

class point
{
public:
    int r,c;
    point(int a,int b){r=a,c=b;}
    point(){r=0,c=0;}
};

class closeTrees
{
public:
    point co,d1,d2,ro;
    closeTrees()
    {
        co.r=d1.r=d2.r=ro.r=-1;
        co.c=d1.c=d2.c=ro.c=-1;
    }
};

class state
{
public:
    vector<point> liz;
    point startPoint;
    state(){startPoint.r=0,startPoint.c=-1;}
};

vector<point> queens;
vector<list<int> > sar,sac,sad1,sad2;
vector<point*> lc,ld1,ld2;
vector<int> trc;
bool **mat;
int **output;
closeTrees **closet;
state bfs(int,int);
bool dfs(int,int,state&);
state sa(int,int);
bool canBeEaten(point,vector<point>&,int);
bool canBeEaten2(point,int);
bool diag1(point&,point&);
bool diag2(point&,point&);
void buildLists(int);
int diagIndex(point,int,int);
void buildOutput(int);
void prepSimpleDFS(int);
bool simpleDFS(int,int,int,int**,int);
dbg2(int better=0;int worse=0;)

time_t sTime;

int count=0;

void printVector(vector<point> &v)
{
    int i;
    cout<<"Printing vector: ";
    for(i=0;i<v.size();++i)
        cout<<"("<<v[i].r<<","<<v[i].c<<") ";
    cout<<"\n";
}

void printList(list<int> &l)
{
    list<int>::iterator i;
    cout<<"Printing list: ";
    for(i=l.begin();i!=l.end();++i)
        cout<<"("<<queens[*i].r<<","<<queens[*i].c<<") ";
    cout<<"\n";
}

int main()
{
    sTime = time(nullptr);
    timer(stim=etim=clock();)
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    int n,p;
    bool foundTree=false;
    char type[4];
    int i,j;
    state ans;

    scanf("%s%d%d",type,&n,&p);
    char str[n+1];
    mat = new bool*[n];
    closet = new closeTrees*[n];
    for(i=0;i<n;++i)    //take input and construct tr
    {
        scanf("%s",str);
        mat[i] = new bool[n];
        closet[i] = new closeTrees[n];
        trc.push_back(0);
        lc.push_back(NULL);
        for(j=0;j<n;++j){
            if(str[j]=='2'){
                mat[i][j] = 1;
                ++trc[i];
                foundTree=true;
            }
            else
                mat[i][j] = 0;
        }
    }

    if(!foundTree && type[0]=='D')  //simple nQueens only for DFS
    {
        if(p>n)
        {
            printf("FAIL");
            return 0;
        }

        buildOutput(n);
        prepSimpleDFS(n);

        if(simpleDFS(n,0,p,output,0))
        {
            printf("OK\n");
            for(i=0;i<n;++i)
            {
                for(j=0;j<n;++j)
                    printf("%d",output[i][j]);
                printf("\n");
            }
        }
        else
            printf("FAIL");
        return 0;
    }


    trc.push_back(0);   //one extra useless element

    buildLists(n);

    if((trc[0] && p>n*n-trc[0]) || (!trc[0] && p>n))    //if there are trees we cant have more than n^2-t lizards, if there are no trees, we can't have more than n lizards
    {
        printf("FAIL");
        return 0;
    }
    timer(stim = clock()-stim;)
    switch(type[0])
    {
        case 'B':
            ans = bfs(n,p);
            break;
        case 'S':
            ans = sa(n,p);
            break;
        case 'D':
            ans.liz.reserve(n*n);
            dfs(n,p,ans);
            break;
        default:
            printf("FAIL"); //should never reach here
            return 0;
    }
    if(ans.liz.size()==0)
        printf("FAIL");
    else if(type[0]!='S')   //figure out what to print and print it (DFS and BFS)
    {
        printf("OK\n");
        vector<point>::iterator itr=ans.liz.begin();
        dbg(printVector(ans.liz);)
        for(i=0;i<n;++i)
        {
            for(j=0;j<n;++j)
            {
                if(mat[i][j]==1)
                    str[j]='2';
                else
                    str[j]='0';
            }

            while(itr!=ans.liz.end() && (*itr).r==i)
            {
                str[(*itr).c] = '1';
                ++itr;
            }
            printf("%s\n",str);
        }
    }
    else    //print output for SA
    {
        cout<<"OK\n";

        for(i=0;i<n;++i)
        {
            for(j=0;j<n;++j)
                printf("%d",output[i][j]);
            printf("\n");
        }

        dbg2(cout<<"better "<<better<<" worse"<<worse<<endl;)
    }
    timer(cout<<"Time for the shenanigans was "<<stim/(double)CLOCKS_PER_SEC<<endl;)
    timer(cout<<"Time for all shenanigans was "<<(clock()-etim)/(double)CLOCKS_PER_SEC<<endl;)
    dbg(cout<<"Total count = "<<count<<endl;)
    return 0;
}

vector<int> slr,slc,sld1,sld2;

void prepSimpleDFS(int n)
{
    int i;
    for(i=0;i<n;++i)
    {
        slr.push_back(0);
        slc.push_back(0);
    }
    for(i=0;i<2*n-1;++i)
    {
        sld1.push_back(0);
        sld2.push_back(0);
    }
}

bool simpleDFS(int n,int p,int req, int **otp,int row)
{
    int i;
    if(req==p)
        return true;
    if(row==n+1)
        return false;
    for(i=0;i<n;++i)
    {
        if(slc[i] || sld1[n-1 + i-row] || sld2[row+i]) //found another queen which is conflicting
        {
            continue;
        }
        else
        {
            slc[i]+=1;
            sld1[n-1 + i-row]+=1;
            sld2[row+i]+=1;
            otp[row][i] = 1;
            if(simpleDFS(n,p+1,req,otp,row+1))
                return true;
            otp[row][i] = 0;
            slc[i]-=1;
            sld1[n-1 + i-row]-=1;
            sld2[row+i]-=1;

        }
    }
    return false;
}

void buildLists(int n)
{
    int i,j;
    int row,col;

    for(i=0;i<n;++i)
        trc[n-i-1] = trc[n-i-1] + trc[n-i];

    for(i=0;i<=2*n-2;++i)
    {
        ld1.push_back(NULL);
        ld2.push_back(NULL);
    }

    for(i=0;i<n;++i)    //select column
    {
        col=-1,row=-1;
        for(j=n-1;j>=0;--j)
        {
            if(mat[j][i]==1)
                col=i,row=j;
            closet[j][i].co.r=row,closet[j][i].co.c=col;
        }
    }
    for(i=0;i<n;++i)    //select row
    {
        col=-1,row=-1;
        for(j=n-1;j>=0;--j)
        {
            if(mat[i][j]==1)
                col=j,row=i;
            closet[i][j].ro.r=row,closet[i][j].ro.c=col;
        }
    }
    int siz = 0;
    for(i=0;i<2*n-1;++i)   //select diagonal1
    {
        row=-1,col=-1;
        if(i<=n-1)
        {
            for(j=n-1;j>=n-1-i;--j)
            {
                if(mat[j][i-(n-1)+j]==1)
                    col=i-(n-1)+j,row=j;
                closet[j][i-(n-1)+j].d1.r=row,closet[j][i-(n-1)+j].d1.c=col;
            }
        }
        else
        {
            for(j=n-1;j>=i-n+1;--j)
            {
                if(mat[n-1+j-i][j]==1)
                    col=j,row=n-1+j-i;
                closet[n-1+j-i][j].d1.r=row,closet[n-1+j-i][j].d1.c=col;
            }
        }

    }
    for(i=0;i<2*n-1;++i)   //select diagonal2
    {
        row=-1,col=-1;
        if(i<=n-1)
        {
            for(j=0;j<=i;++j)
            {
                if(mat[i-j][j]==1)
                    col=j,row=i-j;
                closet[i-j][j].d2.r=row,closet[i-j][j].d2.c=col;

            }
        }
        else
        {
            for(j=n-1;j>=i-n+1;--j)
            {
                if(mat[j][i-j]==1)
                    col=i-j,row=j;
                closet[j][i-j].d2.r=row,closet[j][i-j].d2.c=col;

            }
        }

    }
}

inline int diagIndex(point p,int t,int n)    //the point for which to work, the type of diagonal wanted(1 or 2)
{

    if(t==1)
    {
        return (p.c-p.r)+(n-1);
    }
    else
    {
        return (p.c+p.r);
    }
}

inline bool diag1(point &a,point &b)
{
    return (a.r-b.r==a.c-b.c);
}

inline bool diag2(point &a,point &b)
{
    return (a.r-b.r==b.c-a.c);
}

bool canBeEaten(point pos,vector<point> &liz,int n)
{
    bool d1,vert,d2;
    d1=vert=d2=false;   //tells us that we haven't worked with the closest lizard on the diagonal1, vertical and diagonal2
    int i;
    point pa;

    for(i=liz.size()-1;i>=0;--i)
    {
        if(vert && d1 && d2)
            break;

        if(liz[i].c==pos.c && !vert)
        {
            pa = closet[liz[i].r][liz[i].c].co;
            if(pa.r==-1)   //could not find a tree
                return true;
            if(pa.r>pos.r)    //the tree that it found comes after the new position
                return true;
            vert=true;
        }
        else if(diag1(pos,liz[i]) && !d1)
        {
            pa = closet[liz[i].r][liz[i].c].d1;
            if(pa.r==-1)   //could not find a tree
                return true;
            if(pa.r>pos.r)    //the tree that it found comes after the new position
                return true;
            d1=true;
        }
        else if(diag2(pos,liz[i]) && !d2)
        {
            pa = closet[liz[i].r][liz[i].c].d2;
            if(pa.r==-1)   //could not find a tree
                return true;
            if(pa.r>pos.r)    //the tree that it found comes after the new position
                return true;
            d2=true;
        }


    }
    return false;
}

inline bool timeUp()
{
    return time(nullptr)-sTime > SATIMETHRESH;
}

state bfs(int n,int p)
{
    int i;
    deque<state> q;
    q.push_back(state());
    point save;
    bool foundChild = false;
    while(!q.empty())
    {
        if(timeUp())
            return state();

        state &st = q.front();
        save = st.startPoint;
        if(st.liz.size()==p)
            return st;
        if(st.startPoint.r==n)
        {
            q.pop_front();
            continue;
        }
        foundChild = false;

        for(i=save.c+1;i<n;++i)
        {
            if(mat[save.r][i]==0)
            {
                if(!canBeEaten(point(save.r,i),st.liz,n))
                {
                    foundChild = true;
                    st.liz.push_back(point(save.r,i));
                    point pa = closet[save.r][i].ro;
                    if(pa.r==-1){
                        st.startPoint.c =-1;
                        st.startPoint.r +=1;
                    }
                    else
                        st.startPoint = pa;
                    q.push_back(st);
                    st.liz.pop_back();
                    st.startPoint = save;
                }
            }
        }
        if(!foundChild && (trc[save.r+1] || p-st.liz.size()<n-save.r))
        {
            st.startPoint.r += 1;    //If nothing worked, try skipping this row entirely
            st.startPoint.c = -1;
            q.push_back(st);
            st.startPoint = save;
        }


        q.pop_front();
    }
    return state();
}

bool canBeEaten2(point pt,int n)
{
    if(lc[pt.c]==NULL && ld1[n-1 + pt.c - pt.r]==NULL && ld2[pt.r+pt.c]==NULL)
        return false;
    else    //there is some lizard present
    {
        point pa;
        if(lc[pt.c]!=NULL)
        {
            pa = closet[(*lc[pt.c]).r][(*lc[pt.c]).c].co;
            if(pa.r==-1)
                return true;
            else if(pa.r>pt.r)    //tree comes after lizard
                return true;

        }
        if(ld1[n-1 + pt.c - pt.r]!=NULL)
        {
            pa = closet[(*ld1[n-1 + pt.c - pt.r]).r][(*ld1[n-1 + pt.c - pt.r]).c].d1;
            if(pa.r==-1)
                return true;
            else if(pa.r>pt.r)    //tree comes after lizard
                return true;
        }
        if(ld2[pt.r+pt.c]!=NULL)
        {
            pa = closet[(*ld2[pt.r+pt.c]).r][(*ld2[pt.r+pt.c]).c].d2;
            if(pa.r==-1)
                return true;
            else if(pa.r>pt.r)    //tree comes after lizard
                return true;
        }
    }
    return false;
}

bool dfs(int n,int p,state &s)
{
    if(timeUp()){
            s.liz.clear(); return true;
    }

    point *lic,*lid1,*lid2;
    point pt=s.startPoint,save=s.startPoint;
    if(s.liz.size()==p)
        return true;
    if(pt.r==n) //we went through all the rows
        return false;
    int i;
    bool recursed = false;


    for(i=pt.c+1;i<n;++i)
    {
        if(mat[pt.r][i]==0)
        {
            if(!canBeEaten2(point(pt.r,i),n))
            {
                recursed = true;
                s.liz.push_back(point(pt.r,i));
                point pa = closet[pt.r][i].ro;
                if(pa.r==-1){
                    s.startPoint.c =-1;
                    s.startPoint.r +=1;
                }
                else
                    s.startPoint = pa;

                //s.startPoint = s.liz.back();
                lic = lc[i];
                lid1 = ld1[n-1+i-pt.r];
                lid2 = ld2[i+pt.r];


                lc[i] = ld1[n-1+i-pt.r] = ld2[i+pt.r]= &s.liz.back();
                if(dfs(n,p,s))
                    return true;

                lc[i] = lic;
                ld1[n-1+i-pt.r] = lid1;
                ld2[i+pt.r] = lid2;

                s.liz.pop_back();
                s.startPoint = save;
            }
        }
    }
    if(!recursed && (trc[pt.r+1] || p-s.liz.size()<n-pt.r))
    {
        s.startPoint.r += 1;    //If nothing worked, try skipping this row entirely
        s.startPoint.c = -1;
        if(dfs(n,p,s))
            return true;
        s.startPoint = save;
    }
    return false;

}

inline double schedule(int t)
{
    return 2/(double)log((double)t);
}

void buildOutput(int n)
{
    int i,j;
    output = new int*[n];

    for(i=0;i<n;++i)
        output[i] = new int[n];

    for(i=0;i<n;++i)
    {
        for(j=0;j<n;++j)
        {
            if(mat[i][j]==0)
                output[i][j]=0;
            else
                output[i][j]=2;
        }
    }
}

void makeLists(int n,int p)
{
    int i,j;
    for(i=0;i<=2*n-2;++i)
    {
        sad1.push_back(list<int>());
        sad2.push_back(list<int>());
    }
    for(i=0;i<n;++i)
    {
        sar.push_back(list<int>());
        sac.push_back(list<int>());
    }
    for(i=0;i<p;++i)
        queens.push_back(point());

    buildOutput(n);
}

void printOutput(int n)
{
    int i,j;
    for(i=0;i<n;++i)
    {
        for(j=0;j<n;++j)
            cout<<output[i][j];
        cout<<endl;
    }
    cout<<endl;
}

int countConflicts(list<int> &l,int r,int c,char type)
{
    list<int>::iterator itr;
    int total=0;
    int r1,c1;
    dbg(cout<<"Counting conflicts for "<<r<<" "<<c<<" in list ";printList(l);)
    for(itr=l.begin();itr!=l.end();++itr)
    {
        r1 = queens[*itr].r, c1=queens[*itr].c;
        if(r1==r && c1==c)  //skip if you're at the same queen
            continue;
            dbg(cout<<"aao switch karein"<<endl;)
        switch(type)
        {
        case 'r':
            if(c1<c){
                if(closet[r1][c1].ro.c==-1 || c<closet[r1][c1].ro.c)
                    total+=1;
            }else    //c<c1
                if(closet[r][c].ro.c==-1 || c1<closet[r][c].ro.c)
                    total+=1;
            break;
        case 'c':
            if(r1<r){
                if(closet[r1][c1].co.r==-1 || r<closet[r1][c1].co.r)
                    total+=1;
            }else    //r<r1
                if(closet[r][c].co.r==-1 || r1<closet[r][c].co.r)
                    total+=1;
            break;
        case '1':
            if(r1<r){
                if(closet[r1][c1].d1.r==-1 || r<closet[r1][c1].d1.r)
                    total+=1;
            }else    //r<r1
                if(closet[r][c].d1.r==-1 || r1<closet[r][c].d1.r)
                    total+=1;
            break;
        case '2':
            if(r1<r){
                if(closet[r1][c1].d2.r==-1 || r<closet[r1][c1].d2.r)
                    total+=1;
            }else    //r<r1
                if(closet[r][c].d2.r==-1 || r1<closet[r][c].d2.r)
                    total+=1;
            break;
        }
    }
    return total;
}

int place(int qno,int r,int c,int n,bool calcCount=true)
{
    dbg(cout<<"Placing queen at "<<r<<", "<<c<<" from "<<queens[qno].r<<", "<<queens[qno].c<<endl;printOutput(n);)
    queens[qno].r=r, queens[qno].c=c;
    sar[r].push_back(qno);
    sac[c].push_back(qno);
    sad1[diagIndex(point(r,c),1,n)].push_back(qno);
    sad2[diagIndex(point(r,c),2,n)].push_back(qno);
    output[r][c]=1;

    int cnr,cnc,cnd1,cnd2;
    if(calcCount)
    {
        cnr = countConflicts(sar[r],r,c,'r');
        cnc = countConflicts(sac[c],r,c,'c');
        cnd1 = countConflicts(sad1[diagIndex(point(r,c),1,n)],r,c,'1');
        cnd2 = countConflicts(sad2[diagIndex(point(r,c),2,n)],r,c,'2');
        dbg(cout<<"place gave "<<cnr+cnc+cnd1+cnd2<<endl;)
    }dbg(else cout<<"place was not asked count"<<endl;)

    return cnr+cnc+cnd1+cnd2;
}

int pickup(int qno,int r,int c,int n,bool calcCount=true)
{
    dbg(cout<<"Picking up queen from "<<r<<", "<<c<<endl;printOutput(n);)
    int cnr,cnc,cnd1,cnd2;
    if(calcCount)
    {
        cnr = countConflicts(sar[r],r,c,'r');
        cnc = countConflicts(sac[c],r,c,'c');
        cnd1 = countConflicts(sad1[diagIndex(point(r,c),1,n)],r,c,'1');
        cnd2 = countConflicts(sad2[diagIndex(point(r,c),2,n)],r,c,'2');
        dbg(cout<<"pickup gives "<<cnr+cnc+cnd1+cnd2<<endl;)
    }dbg(else cout<<"pickup was not asked count"<<endl;)
    sar[r].remove(qno);
    sac[c].remove(qno);
    sad1[diagIndex(point(r,c),1,n)].remove(qno);
    sad2[diagIndex(point(r,c),2,n)].remove(qno);
    output[r][c]=0;

    return cnr+cnc+cnd1+cnd2;
}

state sa(int n,int p)
{
    int i,j,t;
    int newPosr,newPosc,qMove;
    int diff;
    int curPosr,curPosc;
    double T,P,R;
    int qno=0;
    int totalConflicts=0,newConflicts;
    bool donePlacing=false;

 	random_device rd;
    default_random_engine generator(rd());
    uniform_real_distribution<double> distribution(0.0,1.0);
    uniform_int_distribution<int> dist_r(0,n-1),dist_c(0,n-1),dist_q(0,p-1);

    makeLists(n,p);
    //Lets make our initial state

    for(i=0;i<n;++i)
    {
        for(j=0;j<n;++j)
        {
            if(output[i][j]==0)
            {
                totalConflicts += place(qno,i,j,n);
                dbg(cout<<"Current total conflicts = "<<totalConflicts<<endl;)
                qno += 1;
                if(qno==p)
                {
                    donePlacing = true;
                    break;
                }

                //now we find the next viable position
                if(closet[i][j].ro.c==-1) //no tree now, so must go to next
                    break;
                else
                    j=closet[i][j].ro.c; //we put j at the next tree, it will automatically do a plus 1 too
            }
        }
        if(donePlacing)
            break;
    }


    dbg(printVector(queens);)


    if(qno<p)   //in case we werent able to place all the queens
        return state();

    if(totalConflicts==0)   //in case we already have a perfect state!
    {
        state s;
        s.liz.push_back(point(0,0));    //just to avoid the FAIL condition
        return s;
    }


    if(n*n-trc[0]-p==0) //if there is no space left to move any lizards (We've already checked if total conflicts was 0, and its not)
        return state();

    //srand(time(0));
    //OK now we're done making out initial state, and it has a value of totalConflicts

    for(t=1;;++t)
    {
        dbg(++count;)
        if(timeUp())
            return state();
        T = schedule(t);
        newConflicts = totalConflicts;
        qMove = dist_q(generator);   //choose a random queen
        curPosr = queens[qMove].r;
        curPosc = queens[qMove].c;
        newPosr=curPosr, newPosc=curPosc;
        while(output[newPosr][newPosc]!=0) //choose a legal random position
        {
            newPosr = dist_r(generator);
            newPosc = dist_c(generator);
        }
        int tmp = pickup(qMove,curPosr,curPosc,n);
        newConflicts -= tmp;
        dbg(if(newConflicts<0){cout<<"lul "<<tmp<<endl;exit(1);})    //pickup the queen, thus reducing some conflicts;
        newConflicts += place(qMove,newPosr,newPosc,n);

        diff = totalConflicts-newConflicts;
        if(diff>0)  //better state
        {
            dbg2(++better;)
            totalConflicts = newConflicts;  //we commit to the change
            if(totalConflicts==0)   //whippee we're done
            {
                state s;
                s.liz.push_back(point(0,0));    //just to avoid the FAIL condition
                return s;
            }
            continue;
        }
        else    //worse state
        {

            P = exp(diff/T);
            R = distribution(generator);    //gives a number in [0,1)
            if(R<=P)    //we will accept the bad state
            {
                dbg2(++worse;)
                totalConflicts = newConflicts;
                continue;
            }
            else
            {
                //revert the queen back to the original position, and then we go on
                pickup(qMove,newPosr,newPosc,n,false);
                place(qMove,curPosr,curPosc,n,false);
                continue;
            }
        }
    }


}
