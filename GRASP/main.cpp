#include <iostream>

using namespace std;

int greedy_cost(int a, int b);
int isfeasible (int a, int b);
int sum_working (int a);
int solution_feasible();
int sum_hoursofnurse (int a);
int min_gc(int a);
int sort_gc(int a, int b);
int displaysol();
int displaygc();

const int H=8;
const int N=12;
int solution [H][N];
int greedycost[H][N];

int demand_h[H]={2,3,5,1,4,3,1,6};
int minHours = 4;
int maxHours = 6;
int maxPresence= 7;
int maxConsec= 3;
int nurses_h[H];
int sol=0;
int gc_sorted[H][N];

int main()
{
 int nurse,h;
 int feasible,feasible_count;
 int working_nurse=0;
 char ch;
 //initialising gc_sorted with nurses
 for(int m=0;m<H;m++)
 {
     for(int l=0;l<N;l++)
     {
         gc_sorted[m][l]=l;
         solution[m][l]=0;
     }
 }
for(int xx=0;xx<2;xx++)
{

 for(int yy=0;yy<H;yy++)
 {  if(xx==0)
 {
     h=yy;
 }
 else
 {
     h=H-1-yy;
 }
    greedy_cost(h,0);

   // cin>>ch;
    feasible_count=0;
    for(int n=0;n<N;n++)
       { //displaysol();
        greedy_cost(h,0);
        sort_gc(h,0);
        nurse=gc_sorted[h][feasible_count];
        cout<<"\n min nurse"<<nurse;
        if(greedycost[h][nurse]<1000)
        {
         solution[h][nurse]=1;
         feasible= isfeasible(h,nurse);

          if(!feasible)
           {
            solution[h][nurse]=0;
            feasible_count++;
           }
        }
        else
        {
            solution[h][nurse]=0;
        }
        if(sum_working(h)== demand_h[h])
        {   cout<<"\nsum of working nurses in hour:"<<h<<" is "<<sum_working(h);

        }
       }


 }
}
 sol=solution_feasible();
 cout<<"solution is "<< sol;
       if(sol)
       {
        for(int i=0;i<N;i++)
        {
            if(sum_hoursofnurse(i)!=0)
            {
                working_nurse++;
            }
        }
       }
       displaysol();
       cout<<"\n"<<working_nurse<<" out of "<<N<<" nurses are working.";

    return 0;
}

int greedy_cost(int a, int b)
 {   int hour=a;
     int nurse_index=b;
     int first_hour, last_hour,flag;

     int X1; //if nurse is working
     int X2; //if demand_h is satisfied
     int X3; //if minhours is not done and nurse is working
     int X4; //if the previous hour of the nurse was free and the nurse is working
     int X5; //if the hour is already assigned

     for(int i=nurse_index;i<N;i++)
     { X1=0;
       X2=0;
       X3=0;
       X4=0;
       X5=0;
       if (sum_hoursofnurse(i) !=0)
          { X1=1;
          }
       if (sum_working(hour)>= demand_h[hour])
          { X2=1;
          }
       if (X1 && (sum_hoursofnurse(i)<minHours) )
       {
           X3=1;
       }
       flag=0;
       for(int k=0;k<H;k++)
           {
            if(solution[k][i])
              {   flag=1;
                  first_hour=k;
                  k=H+1;
              }
           }
           for(int j=H-1;j>=0;j--)
           {
            if(solution[j][i])
              {   flag=1;
                  last_hour=j;
                  j=-1;
              }
           }
       if (X1 && hour>first_hour && solution[hour-1][i]==0 )  //didn't put last_hour constraint for a reason
       {
           X4=1;
       }
       if (solution[hour][i])
       {
           X5=1;
       }
       greedycost[hour][i]= 100*(1-X1)+ 1000*X2 - 1000*X3 - 2000*X4 + 2500*X5;

     }
     displaygc();
     return 0;
 }

 int sum_working (int a)
 { int sum=0;
     for(int i=0;i<N;i++)
     {sum+= solution[a][i];
     }
    return sum;
 }

 int sum_hoursofnurse (int a)
 { int sum=0;
     for(int i=0;i<H;i++)
     {sum+= solution[i][a];
     }
    return sum;
 }

 int isfeasible (int a, int b)
 {
     int hour=a;
     int nurse=b;
     int flag=0;
     int consec_hours=0;
     int first_hour;

     //maxHours
     if(sum_hoursofnurse(nurse)>maxHours)
     {  cout<<"\nMaxhours not feasible hour:"<<hour<<" nurse:"<<nurse;
        return 0;

     }
     //maxpresence
     for(int i=0;i<N;i++)
     {
         if(solution[i][nurse])
         {   flag=1;
             first_hour=i;
             break;
         }
     }
     if(flag)
     {
         if((hour-first_hour+1)>maxPresence)
         {   cout<<"\nMaxPresence not feasible hour:"<<hour<<" nurse:"<<nurse;
             return 0;
         }
     }

     //maxconsec
     for(int i=hour;i>=0;i--)
     {
         if(!solution[i][nurse])
         {
             break;
         }
         consec_hours++;
     }
     if(consec_hours>maxConsec)
     {   cout<<"\nMaxConsec not feasible hour:"<<hour<<" nurse:"<<nurse;
         return 0;
     }
     return 1;
 }

 int solution_feasible()
 { int hoursofnurse;
   int flag=0;
   int j,k,first_hour,last_hour;
     for(int i=0;i<N;i++)
     {   hoursofnurse=sum_hoursofnurse(i);
         //minHours
          if(hoursofnurse!=0 && hoursofnurse<minHours)
          { cout<<"minhours"<<i;
              return 0;
          }
          //consec_resting
          flag=0;
          for( k=0;k<H;k++)
           {
            if(solution[k][i])
              {   flag=1;
                  first_hour=k;
                  k=H+1;
              }
           }
           for( j=H-1;j>=0;j--)
           {
            if(solution[j][i])
              {   flag=1;
                  last_hour=j;
                  j=-1;
              }
           }

           if(flag)
           {
               cout<<"\nfirst_hour:"<<first_hour<<"last_hour:"<<last_hour;
               for(int f=first_hour;f<=last_hour;f++)
               {
                   if(!solution[f][i]&& !solution[f+1][i])
                   { cout<<"consecrest"<<i<<"\tf:"<<f;
                     return 0;
                   }
               }
           }

     }

     //demand_h
     for(int w=0;w<H;w++)
     {
         if(sum_working(w)<demand_h[w])
         {   cout<<"demandh"<<w;
             return 0;
         }
     }

     return 1;
 }

 int min_gc(int a)
 { int hour= a;
   int minimum_gc=greedycost[a][0];
   for(int i=0;i<N;i++)
   { if(greedycost[hour][i]<minimum_gc)
   {
       minimum_gc=greedycost[hour][i];
   }

   }
    return minimum_gc;
 }

 int displaysol()
 { char ch;
     cout<<"\n The solution is: \n";
     for(int i=0; i<N; i++)
     {  cout<<"\n Nurse "<<i<<":";
         for(int j=0;j<H; j++)
         {
             cout<<"\t"<< solution[j][i];
         }

     }
      cin>>ch;
     return 0;
 }

 int displaygc()
 {   cout<<"\nThe greedy cost is: \n";
     for(int i=0; i<N; i++)
     {  cout<<"\n Nurse "<<i<<":";
         for(int j=0;j<H; j++)
         {
             cout<<"\t"<< greedycost[j][i];
         }

     }
     return 0;
 }
int sort_gc(int a, int b)
{
    int hour=a;
    int nurse_index=b;
    int temp;
    for (int i=nurse_index;i<N;i++)
    {
        for(int j=nurse_index;j<N-1;j++)
        {
            if(greedycost[hour][gc_sorted[hour][j]]>greedycost[hour][gc_sorted[hour][j+1]])
            {
                temp=gc_sorted[hour][j];
                gc_sorted[hour][j]=gc_sorted[hour][j+1];
                gc_sorted[hour][j+1]=temp;
            }
        }
    }
    return 0;
}
