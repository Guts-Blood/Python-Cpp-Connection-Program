#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <queue>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include<fstream>
#include<cmath>

using namespace std;
int main(int argc, char * argv[])
{
    string input_commands="";
    
    for(int count=1; count<argc;count++)
    {
        string add = argv[count]; 
        input_commands.append(add);
    }

    // open /dev/urandom to read
    std::ifstream urandom("/dev/urandom");

    // check that it did not fail
    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }
    char ch='a';

    //srand((unsigned) time(0));
    //set the initial poisition
    int pos=0;
    //set the default values:
    int num_street=((unsigned int)ch)%9 + 2;
    int k_line_segments=5;
    int wait_time=5;
    int k_points=20;
    while (pos<input_commands.size())
    {
        // cout<<"size of input"<<input_commands.size()<<endl;
        // cout<<input_commands[pos]<<endl;
        if(input_commands[pos]=='-')
        {
            pos=pos+1;
        //s command
            if (input_commands[pos]=='s')
            {
                
                int start=pos+1;
                //cout<<"generate the number of street"<<endl;
                int k;
                //find the next command
                while (pos+1<input_commands.size()&&input_commands[pos+1]!='-')
                {
                    pos++;
                }
                k=atoi(input_commands.substr(start,pos+1).c_str());
                if(k<2){
                    std::cerr<<"Error: number of street must be bigger or equal to 2."<<endl;
                    return -1;
                }

                urandom.read(&ch, 1);
                num_street=(((unsigned int)ch)%(k-1))+2;
                //cout<<num_street<<endl;
            }
        //n command
            else if (input_commands[pos]=='n')
            {
                int start=pos+1;
                //cout<<"get the k of line segements of a street"<<endl;
                
                //find the next command
                while (pos+1<input_commands.size()&&input_commands[pos+1]!='-')
                {
                    pos++;
                }
                k_line_segments=atoi(input_commands.substr(start,pos+1).c_str());
                if(k_line_segments<1){
                    std::cerr<<"Error: number of line-segments must be bigger or equal to 1."<<endl;
                    return -1;
                }
            }
        //l command
            else if (input_commands[pos]=='l')
            {
                
                int start=pos+1;
                //cout<<"generate wait time"<<endl;
                int k;
                //find the next command
                while (pos+1<input_commands.size()&&input_commands[pos+1]!='-')
                {
                    pos++;
                }
                k=atoi(input_commands.substr(start,pos+1).c_str());
                if(k<5){
                    std::cerr<<"Error: wait time must be bigger or equal to 5."<<endl;
                    return -1;
                }
                urandom.read(&ch, 1);
                wait_time=((unsigned int)ch)%(k-4)+5;
            }
        //c command
            else if (input_commands[pos]=='c')
            {
                
                int start=pos+1;
                //cout<<"get k of points parameter"<<endl;
                //find the next command
                while (pos+1<input_commands.size()&&input_commands[pos+1]!='-')
                {
                    pos++;
                }
                k_points=atoi(input_commands.substr(start,pos+1).c_str());
                if(k_line_segments<1){
                    std::cerr<<"Error: coordinate value must be bigger or equal to 1."<<endl;
                    return -1;
                }
            }
        }
        pos++;
    }
        // cout<<num_street<<endl;
        // cout<<k_line_segments<<endl;
        // cout<<wait_time<<endl;
        // cout<<k_points<<endl;
        
        //generate std input
        //point int used to store points corresponding to stree
    // int out_count=0;
    while(true){
        // out_count++;
        int generate_counter=0;
        int valid=0;
        while(generate_counter<25&&valid==0){
            valid=1;
            int **point=new int*[num_street];
            //get how many segments for each line
            vector<int> num_line_segments;
            int num_points;
            for(int counter1=0;counter1<num_street;counter1++)
            {
                urandom.read(&ch, 1);
                num_line_segments.push_back(int(((unsigned int)ch)%k_line_segments+1));
                //cout<<counter1<<" street has "<<num_line_segments[counter1]<<" segments"<<endl;
            }
            for(int counter1=0; counter1<num_street;counter1++)
            {
                //cout<<"size of point "<<counter1<<" is"<<num_line_segments[counter1]<<endl;
                //n line segments corresponding to n+1 points
                num_points=(num_line_segments[counter1]+1);
                point[counter1]=new int[2*num_points];
                //generate points and store them in point var
                for(int counter2=0;counter2<2*(num_line_segments[counter1]+1);counter2++)
                {
                    urandom.read(&ch, 1);
                    point[counter1][counter2]=(((unsigned int)ch)%(2*k_points+1))-k_points;
                    
                }
            }    
            //Judge if we need to regenerate
            vector<float> slope;
            vector<float> b;
            //First calculate slope and b and Judge if there is same adjacent point
            int counter1=0;
            while(counter1<num_street&&valid==1)
            {
                for(int counter2=0;counter2<2*(num_line_segments[counter1]);counter2+=2)
                {
                    int dy;
                    int dx;
                    dy=point[counter1][counter2+1]-point[counter1][counter2+3];
                    dx=point[counter1][counter2]-point[counter1][counter2+2];
                    if(dx!=0){
                        //2 decimal points
                        float temp_k=round(100.0*dy/dx)/100.0;
                        slope.push_back(temp_k);
                        b.push_back(round(100.0*point[counter1][counter2+1]-temp_k*point[counter1][counter2]));
                    }
                    else{
                        //Judge if there is same adjacent point
                        if(dy==0)
                        {
                            valid=0;
                            break;
                        }
                        else{
                            slope.push_back(3.402823466e+38F);
                            b.push_back(float(point[counter1][counter2]));
                        }
                    }
                }
                counter1++;
            }
            //Making judgement on slope and b
            // cout<<"Make judegement"<<endl;
            for(int counter1=0; counter1<slope.size()-1;counter1++)
            {
                // cout<<counter1<<"  "<<slope.size()<<endl;
                
                if(valid==1){
                    for (int counter2=counter1+1;counter2<slope.size();counter2++)
                    {
                        // cout<<"counter2 "<<counter2<<endl;
                        if(slope[counter2]==slope[counter1])
                        {
                            if(b[counter2]==b[counter1]){

                                // //map counter here to the 4 points
                                int num_st_2=0;
                                int st_point_2=counter2;
                                while(st_point_2>=num_line_segments[num_st_2]){
                                    st_point_2=st_point_2-num_line_segments[num_st_2];
                                    num_st_2++;
                                }
                                //For x
                                float point_line2_x1=point[num_st_2][2*(st_point_2)];
                                float point_line2_x2=point[num_st_2][2*st_point_2+2];
                                float line2_x_max;
                                float line2_x_min;
                                if(point_line2_x1>=point_line2_x2){
                                    line2_x_max=point_line2_x1;
                                    line2_x_min=point_line2_x2;
                                }
                                else{
                                    line2_x_max=point_line2_x2;
                                    line2_x_min=point_line2_x1;
                                }
                                //for y
                                float point_line2_y1=point[num_st_2][2*(st_point_2)+1];
                                float point_line2_y2=point[num_st_2][2*st_point_2+2+1];
                                float line2_y_max;
                                float line2_y_min;
                                if(point_line2_y1>=point_line2_y2){
                                    line2_y_max=point_line2_y1;
                                    line2_y_min=point_line2_y2;
                                }
                                else{
                                    line2_y_max=point_line2_y2;
                                    line2_y_min=point_line2_y1;
                                }
                                //Street 1
                                int num_st_1=0;
                                int st_point_1=counter1;
                                while(st_point_1>=num_line_segments[num_st_1]){
                                    st_point_1=st_point_1-num_line_segments[num_st_1];
                                    num_st_1++;
                                }
                                //For x
                                float point_line1_x1=point[num_st_1][2*(st_point_1)];
                                float point_line1_x2=point[num_st_1][2*st_point_1+2];
                                float line1_x_max;
                                float line1_x_min;
                                if(point_line1_x1>=point_line1_x2){
                                    line1_x_max=point_line1_x1;
                                    line1_x_min=point_line1_x2;
                                }
                                else{
                                    line1_x_max=point_line1_x2;
                                    line1_x_min=point_line1_x1;
                                }
                                //for y
                                float point_line1_y1=point[num_st_1][2*(st_point_1)+1];
                                float point_line1_y2=point[num_st_1][2*st_point_1+2+1];
                                float line1_y_max;
                                float line1_y_min;
                                if(point_line1_y1>=point_line1_y2){
                                    line1_y_max=point_line1_y1;
                                    line1_y_min=point_line1_y2;
                                }
                                else{
                                    line1_y_max=point_line1_y2;
                                    line1_y_min=point_line1_y1;
                                }
                                //Make judgement
                                if(line1_x_max>=line2_x_max&&line1_x_min<line2_x_max)
                                {
                                    valid=0;
                                    break;
                                }
                                if(line2_x_max>line1_x_max&&line2_x_min<line1_x_max)
                                {
                                    valid=0;
                                    break;
                                }
                                if(line1_y_max>=line2_y_max&&line1_y_min<line2_y_max)
                                {
                                    valid=0;
                                    break;
                                }                               
                                if(line2_y_max>line1_y_max&&line2_y_min<line1_y_max)
                                {
                                    valid=0;
                                    break;
                                }                                
                                
                            }
                        }
                    }
                }
                else{
                    break;
                }
            }
            slope.clear();
            b.clear();
            //print out the result if it is valid
            if(valid==1){
                //Then we get a 2-d array points which point[i] represent the array of each street
                //and point[i][k] represent the coordinate, when k==even, is x coordinate, k is odd y coordinate
                for(int counter1=0; counter1<num_street;counter1++)
                {
                    //cout<<(num_line_segments[counter1]+1)<<"points"<<endl;
                    cout<<"a \"street"<<counter1<<"\" ";
                    for(int counter2=0;counter2<2*(num_line_segments[counter1]+1);counter2+=2)
                    {
                        cout<<"("<<point[counter1][counter2]<<","<<point[counter1][counter2+1]<<") ";
                    }
                    cout<<endl;
                }
                cout<<"g"<<endl;
                //delete street

                for(int counter1=0; counter1<num_street;counter1++)
                {
                    //cout<<(num_line_segments[counter1]+1)<<"points"<<endl;
                    cout<<"r \"street"<<counter1<<"\" "<<endl;
                }        
                //break the loop after output
            }
            //delete
            for(int i = 0; i < num_street; ++i) {
                delete [] point[i];
                }
            delete [] point;
            generate_counter++;
        }
        //print error if no valid result
        if(valid==0){
            std::cerr<<"Error: failed to generate valid input for 25 simultaneous attempts."<<endl;
            return -1;
        }
        sleep(wait_time);
    }
    cout<<endl;
    return 0;
}
