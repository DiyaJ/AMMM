#include <iostream>
#include <cstdlib>
using namespace std;

int main()
{  int Nnurses = 45;
   int H =24;
   float chromosome[2*Nnurses];
   int solution[H][Nnurses];

// Generating random alleles values in chromosome
   for(int total=0; total<2*Nnurses;total++)
   { chromosome[total] = ((double) rand() / (RAND_MAX));
          cout<<"\n"<<chromosome[total];
   }
//decoding the second set of alleles to get the schedules

    for (int k=0; k<Nnurses;k++)
    { //if the first set allele<=0.5 , don't make the nurse work
       if(chromosome[k]<=0.5)
         {for(int j=0;j<H;j++)
           {solution[j][k]=0;
           }
         }
      //if the first set allele>0.5 , make the nurse work and decode the schedule from the second set allele
       else if(chromosome[k]>0.5)
         {float i = 0.5;

           for (int j =0; j <H; j++)
            {if(chromosome[Nnurses+k]>=i)
              {
               solution[j][k]=1;
               chromosome[Nnurses+k] = chromosome[Nnurses+k]-i;
              }
             else
              {
               solution[j][k]=0;
              }
             i=i/2;
            }
        }
    }

    //Display solution matrix
    for(int k=0;k<Nnurses;k++)
    {
        cout<<"\n Nurse "<<k<<":";
        for(int j=0;j<H;j++)
        {
            cout<<solution[j][k];
        }
    }

  // Making a GRASP chromosome from a grasp solution matrix
  int graspsol[H][Nnurses];
  float graspchromosome [2*Nnurses];
  // Generating random GRASP solution matrix
   for(int n=0; n<Nnurses;n++)
    {

    for(int h=0;h<H;h++)
       { graspsol[h][n] = rand() % 2;

       }
    }

  //encoding
  for(int k=0;k<Nnurses;k++)
      { float i=0.5;
        graspchromosome[Nnurses+k]=0;
        // assigning the second set of alleles
          for(int j=0;j<H;j++)
           {
               if(graspsol[j][k]==1)
               {
                graspchromosome[Nnurses+k]+=i;
               }
               i=i/2;
           }
          // assigning the first set of alleles
          if(graspchromosome[Nnurses+k]==0)
          {
              graspchromosome[k]=0.4;     //chosen 0.4 just because it is less than 0.5
          }
          else
          {
              graspchromosome[k]=0.8;     //chosen 0.8 because it is greater than 0.5
          }
      }

 //Display GRASP chromosome
 cout<<"\n GRASP chromosome: \n";
    for(int k=0;k<2*Nnurses;k++)
    {
        cout<<"\n ";

            cout<<graspchromosome[k];

    }


    return 0;
}
