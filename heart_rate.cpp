/*
Calculation of Heart rate using ECG

The strategy used here is to identify the positive peak(usually the R peak) and use the interval between 2 peaks
to calculate the time per beat and consequently heart rate.

Beats per minute = 60/time for a beat in seconds

Note that the details of the calculations are logged to Log.txt

*/



#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main()
{

    ifstream iff;
    iff.open("biological_data3.csv");         //assumes the file to be present in the current directory
    ofstream Log;
    Log.open("Log.txt");

    string s,temp;
    int i = 13;
    char ch = (char)i;                       //each row of the file was delimited by a carriage return
    int flag = 0;                            //flag to toggle between finding maxima and minima
    double preval = 1000;                    //setting previous value to an arbitrary high value

    /* Note that here initially flag is set to 0 and preval to high value because in this data initial values
       are decreasing. Meaning that we will find a minima before we find a maxima. Ideally the first few values
       have to examined to identify the trend to set initial values of flag and preval
    */



    int prepeak;
    int count = 0;
    float sum = 0,diff,avg = 0,curr;

    getline(iff,temp,ch);                         //to ignore the headings of the file


    for(i=1;i<=21082;i++)
    {
        getline(iff,temp,',');
        getline(iff,s,',');
        getline(iff,temp,ch);

        double newval = atof(s.c_str());         //the value of ECG in current

     if(newval != 0)                             //ignoring absent values

        /*
            Ignoring missing values is justifiable here because in the given data set, the missing values occur near the
            negative peaks and I am detecting positive peaks. Moreover even if we miss some peaks due to the missing
            values, the algorithm ensures that we do not miscalculate.
        */

     {
        if(flag == 0)
        {
            if(newval > preval && newval < -0.3) //we are checking for a minima here as flag is 0
            {
                flag = 1;                        //we have hit a minima, we toogle flag to search for maxima now
                Log <<"Found minima at row : "<<i<<endl;
            }
        }
        else
        {
            if(newval < preval && newval>0.3)   // we are checking for a maxima here
            {
                Log<<"Found maxima at row : "<<i<<endl;
                flag = 0;

                if(count == 0)                  //We have hit a maxima for the first time
                {
                    Log <<"This is the first maxima"<<endl;
                    prepeak = i;
                    count++;
                }
                else                            //The general case where we have hit a maxima
                {
                    diff = i - prepeak;
                    curr = (60*1000)/(2*diff);  //calculating heart rate from present and previos peaks
                    Log << "The calculated heart rate is : "<<curr<<endl;

                    if(!(avg != 0 && (curr > 1.75*avg || curr < 0.6*avg)))

                        /*
                            This check is to ensure that calculated value does not vary much from the average.
                            If we missed a peak due to missing values or due to the conditions used above, that value is
                            neglected.
                        */

                    {

                        sum += curr;
                        avg = sum/count;
                        Log << "The current average value is : "<<avg<<endl;
                        count++;
                    }
                    else
                    {
                        Log << "The above value looks incorrect and we ignore it"<<endl;
                    }

                    prepeak = i;

                }



            }
        }

        preval = newval;

     }

    }

    cout<<"The heart rate is  : "<<avg<<endl;
    iff.close();
    Log.close();

    return 0;
}
