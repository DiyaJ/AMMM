#include <iostream>

using namespace std;

int main()
{
    float n = 0.8;
    float i = 0.5;
    for (int j = 24; j > 0; j --)
        {if(n>=i)
        {cout<<"1";
            n=n-i;
        }
        else
        {cout<<"0";
            
        }
         i=i/2;
        }
    return 0;
}
