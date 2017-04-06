#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main()
{

    ifstream iff;
    iff.open("biological_data3.csv");

    string s,temp;
    int i = 13;
    char ch = (char)i;
    int flag = 0;
    double preval = 1000;
    int prepeak;
    int count = 0;
    float sum = 0,diff;

    getline(iff,temp,ch);


    for(i=1;i<=21082;i++)
    {
        getline(iff,temp,',');
        getline(iff,temp,',');
        getline(iff,s,ch);

        double newval = atof(s.c_str());

        if(flag == 0)
        {
            if(newval > preval && newval < 60)
            {
                flag = 1;
                cout<<"found minima at "<<i<<endl;
            }
        }
        else
        {
            if(newval < preval && newval>80)
            {
                cout<<"found maxima at "<<i<<endl;
                flag = 0;

                if(count == 0)
                {
                    prepeak = i;
                    count++;
                }
                else
                {
                    diff = i - prepeak;
                    sum += 500/(2*diff) * 60;
                    prepeak = i;
                    count++;

                }

            }
        }

        preval = newval;



    }

    count--;
    sum = sum/count;




    cout<<sum;
    iff.close();

    return 0;
}
