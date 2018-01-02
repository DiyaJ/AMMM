#include <iostream>
#include <stdlib.h>
#include "GRASP_InputData.h"

using namespace std;

int greedy_cost(int a, int b);
int isfeasible (int a, int b);
int sum_working (int a);
int sum_notfeasible (int a);
int solution_feasible();
int sum_hoursofnurse (int a);
int min_gc(int a);
int sort_gc(int a, int b);
int displaysol();
int displaybestsol();
int displaygc();
int local_search(int a);

int** solution;
int** solution_best;
int** greedycost;
int** notfeasible_array;
int** gc_sorted;

int sol=0;
int bestsol=0;
int remove_nurse=-1;
char ch;

float alpha=0.2;




int main()
{
// allocating 2D arrays
   solution = new int*[H];
   solution_best = new int* [H];
   greedycost = new int*[H];
   notfeasible_array = new int*[H];
   gc_sorted= new int*[H];

   for(int i = 0; i < H; ++i)
      {solution[i] = new int[N];
       solution_best[i] = new int[N];
       greedycost[i]=new int[N];
       notfeasible_array[i] = new int[N];
       gc_sorted[i]= new int[N];
      }


 int nurse,h;
 int feasible,feasible_count;
 int local=0;
 int working_nurse;
 int working_nurse_opt=N;
 int elements_left, minList,maxList,maxListFirst,minRCL,maxRCL,random,alpha_threshold;
 int SS;

 //initialising gc_sorted with nurses
 for(int m=0;m<H;m++)
 {
     for(int l=0;l<N;l++)
     {
         gc_sorted[m][l]=l;
         solution[m][l]=0;
         solution_best[m][l]=0;
         notfeasible_array[m][l]=0;
     }
 }
cout<<"\n Starting GRASP....";
// local search iterations
for(int local_iter=0; local_iter<428; local_iter++)
{ SS=2;

    if(local_iter!=0)
    {SS=1;
     if (working_nurse<working_nurse_opt && sol)
      { working_nurse_opt=working_nurse;
        cout<<"\nWorking nurse optimum:"<<working_nurse_opt;
        bestsol=1;
        for(int c=0;c<H;c++)
        {
         for(int d=0;d<N;d++)
          {
            solution_best[c][d] = solution[c][d];
          }
         }
         displaybestsol();
      }

      else
      {
         for(int c=0;c<H;c++)
           {
           for(int d=0;d<N;d++)
            {
            solution[c][d] = solution_best[c][d];
            }
            }
       }
      local_search(working_nurse);
      local=1;
      cout<<"\n Local Search :"<< local_iter;
    }
working_nurse=0;
for(int xx=0;xx<SS;xx++)
{

 for(int yy=0;yy<H;yy++)
 {  if(xx==0 && !local)
    {
     h=yy;
    }
    else
    {
     h=H-1-yy;
    }

    greedy_cost(h,0);
//    cout<<"\n hour: "<<h;
    maxList=N;
    maxListFirst=N;
    feasible_count=sum_notfeasible(h);
//    cout<<"\n Nurses Working: "<<sum_working(h)<<"/"<<demand_h[h];
    int NWorking_now=0;
    for(int i=0;i<N;i++)
        {
            if(sum_hoursofnurse(i)!=0)
            {
                NWorking_now++;
            }
        }
 /*   cout<<"\n Nurses Working upto now: "<<NWorking_now<<"/"<<N;
       cin>>ch;*/

    for(int n=0;n<maxListFirst;n++)
       {//displaygc();
       //cin>>ch;
       //displaysol();

        greedy_cost(h,0);
        sort_gc(h,0);

        //RCL Logic
        elements_left=sum_working(h);
        minList=0;
        if(elements_left+feasible_count==N)
        {
            maxList=0;
        }
        else
        {
        maxList=N-elements_left-feasible_count-1;  // this is to restrict the list of elements to the elements that have not been chosen yet

        }
        if(n==0)
        {
            maxListFirst=maxList;
        }

        alpha_threshold=greedycost[h][gc_sorted[h][minList]]+ alpha*(greedycost[h][gc_sorted[h][maxList]]-greedycost[h][gc_sorted[h][minList]]);

        for(int g=minList;g<=maxList;g++)
        {
            if(greedycost[h][gc_sorted[h][g]]<=alpha_threshold)
            {   maxRCL=g;

            }
            else
            {
                g=maxList+1;
            }
        }
        minRCL=0;
        random= rand()%(maxRCL+1) + minRCL;
        nurse=gc_sorted[h][random];
    /*    if(h==16 && n>160)
       {
        //   displaygc();
        cout<<"\nminList: "<<minList<<" :gc_sorted "<<gc_sorted[h][minList]<<"  greedycost:"<<greedycost[h][gc_sorted[h][minList]];
        cout<<"\nmaxList: "<<maxList<<" :gc_sorted "<<gc_sorted[h][maxList]<<"  greedycost:"<<greedycost[h][gc_sorted[h][maxList]];
        cout<<"\nNurse: "<< nurse<<"GC: "<<greedycost[h][nurse];
        cout<<"\n Nurses Working: "<<sum_working(h)<<"/"<<demand_h[h];
        cin>>ch;
       }*/
      /*
        cin>>ch;*/
        if(greedycost[h][nurse]<1000)
        { //cout<<"\nsol:"<<solution[h][nurse];
         solution[h][nurse]=1;
         feasible= isfeasible(h,nurse);

          if(!feasible)
           {/*cout<<"\n not feasible";
            cin>>ch;*/
            solution[h][nurse]=0;
            notfeasible_array[h][nurse]=1;
            feasible_count++;
           }
         /* displaysol();
           cout<<"\nsol:"<<solution[h][nurse];
           cin>>ch;*/
        }
        else
        {
            solution[h][nurse]=0;
        }

       }
  //     displaysol();
 //      cout<<"\n Nurses Working: "<<sum_working(h)<<"/"<<demand_h[h];
 //   cin>>ch;

 }

}
 sol=0;
 sol=solution_feasible();
 cout<<"\n solution is "<< sol;
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

       cout<<"\n"<<working_nurse<<" out of "<<N<<" nurses are working.";

}
if(bestsol)
{cout<<"\n The Final solution is:";
cout<<"\n Working Nurses: "<<working_nurse_opt<<" out of "<<N;
displaybestsol();
}
else
{
    cout<<"\n A solution to this problem cannot be found via GRASP";
}
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
     int X6; //if notfeasible
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
       X6=notfeasible_array[hour][i];

       greedycost[hour][i]= 100*(1-X1)+ 1000*X2 - 1000*X3 - 2000*X4 + 3500*X5+ 3500*X6;

     }
     return 0;
 }

 int sum_working (int a)
 { int sum=0;
     for(int i=0;i<N;i++)
     {sum+= solution[a][i];
     }
    return sum;
 }

 int sum_notfeasible (int a)
 { int sum=0;
     for(int i=0;i<N;i++)
     {sum+= notfeasible_array[a][i];
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
     int consec_hours_back=0;
     int consec_hours_forw=0;
     int first_hour,last_hour;

     //maxHours
     if(sum_hoursofnurse(nurse)>maxHours)
     {  return 0;

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
     for( int j=H-1;j>=0;j--)
           {
            if(solution[j][nurse])
              {   flag=1;
                  last_hour=j;
                  j=-1;
              }
           }
     if(flag)
     {
         if((last_hour-first_hour+1)>maxPresence)
         {   return 0;
         }
     }

     //maxconsec
     for(int i=hour;i>=0;i--)
     {
         if(!solution[i][nurse])
         {
             break;
         }
         consec_hours_back++;
     }
     for(int i=hour;i<H;i++)
     {
         if(!solution[i][nurse])
         {
             break;
         }
         consec_hours_forw++;
     }
     if( (consec_hours_back+consec_hours_forw-solution[hour][nurse]) > maxConsec )
     {   return 0;
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
          {   return 0;
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
           {   for(int f=first_hour;f<=last_hour;f++)
               {
                   if(!solution[f][i]&& !solution[f+1][i])
                   { return 0;
                   }
               }
           }

     }

     //demand_h
     for(int w=0;w<H;w++)
     {
         if(sum_working(w)<demand_h[w])
         {   return 0;
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

     return 0;
 }

 int displaybestsol()
 { char ch;
     cout<<"\n The best solution is: \n";
     for(int i=0; i<N; i++)
     {  cout<<"\n";
         for(int j=0;j<H; j++)
         {
             cout<< solution_best[j][i]<<",";
         }

     }

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

int local_search(int a)
{ int gotthenurse=0;
//putting the previous remove nurse back to feasible
  if(remove_nurse>=0)
  {   for(int i=0;i<H;i++)
  {
    notfeasible_array[i][remove_nurse]=0;
  }
  }

   // Updating Remove_nurse
    for(int b= remove_nurse+1;b<N;b++)
    {
        if(sum_hoursofnurse(b)!=0 && gotthenurse==0)
        {
            remove_nurse=b;
            gotthenurse=1;
        }
        else if (sum_hoursofnurse(b)==0)
        {
         for(int i=0; i<H;i++)
          {
            notfeasible_array[i][b]=1;   // so that nurses who don't work don't get picked
          }
        }
    }

    for(int i=0; i<H;i++)
    {
        solution[i][remove_nurse]=0;
        notfeasible_array[i][remove_nurse]=1;   // so that this nurse doesn't get picked again
    }
    cout<<"\n Nurse Removed:"<<remove_nurse;
    return 0;
}
