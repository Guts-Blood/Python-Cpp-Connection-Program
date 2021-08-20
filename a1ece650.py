#!/usr/bin/env python3
'''
650 Assignment 1 name: Jiawei Qian email:j59qian@uwaterloo.ca
ID: 20887230
'''
import sys;
def main():
    global stmap;
    global interpoint_value;
    global interpoint_parent;

    while True:
        #Detect if there are some var should be deleted
        try:
            begin and end_pos;
        except NameError:
            var_exist = False;
        else:
            var_exist = True;
        if var_exist:
            del begin, end_pos;
        #Get the input
        user_input = sys.stdin.readline();
        user_input=user_input.strip();
        #Take the format input separately
        #Detect eof to end the program
        if not user_input:
            break;
        else:
            command = user_input[0];
            #command judgement
            command_list=['a','c','r','g'];
            if command not in command_list:
                print('Error: Input command dose not exist');
            else:
                if command=='g':
                    g();
                else:
                    try:
                        # get the name of the street
                        begin=None;
                        for i in range(len(user_input)):
                            if begin==None:
                                if user_input[i]=='"':
                                    begin=i;
                            else:
                                if user_input[i]=='"':
                                    end_pos=i;
                                    break;
                        st_name = user_input[begin+1:end_pos].upper();
                    except NameError:
                        print('Error: no street name is specified in input.')
                    else:
                        if end_pos<begin:
                            print('Error: no street name is specified in input.');
                        else:
                            #command judgment
                            if command=='r':
                                try:
                                    
                                    r(st_name);
                                except KeyError:
                                    print('Error: Error: c or r specified for a street that does not exist.')
                            else:
                                try:
                                    # get the datapoints of the street
                                    st_point_path = [];
                                    single_point = [];
                                    for i in range(end_pos + 1, len(user_input)):
                                        if user_input[i]=='(':
                                            begin=i;
                                        if user_input[i]==',':
                                            middle=i;
                                        if user_input[i]==')':
                                            end_pos=i;
                                            single_point.append(int(user_input[begin+1:middle]));
                                            single_point.append(int(user_input[middle+1:end_pos]));
                                            st_point_path.append(single_point);
                                            single_point = [];
                                except ValueError:
                                    print('Error: there is an error in input points format');
                                #If there is no error detected
                                else:
                                    if end_pos<begin:
                                        print('Error: there is an error in input points format');
                                    else:
                                        if command=='a':
                                            del begin,end_pos;
                                            a(st_name,st_point_path);
                                        else:
                                            try:
                                                c(st_name,st_point_path);
                                            except KeyError:
                                                print('Error: c or r specified for a street that does not exist.');

    sys.exit(0);
def calintersect(x1, y1, x2, y2, x3, y3, x4, y4):
    # if the slope is the same
    if (y1 - y2) * (x3 - x4) == (y3 - y4) * (x1 - x2):
        return False;
    # if the line 1 is vertical to x axes
    if x1 == x2:
        # if the line 2 is also vertical to x axes
        if x3 == x4:
            return False;
        else:
            px = x1;
            py = y3 + (y4 - y3) * (px - x3) / (x4 - x3);
            if py >= min(y3, y4) and py <= max(y3, y4) and py >= min(y1, y2) and py <= max(y1, y2):
                return [round(float(px),4), round(float(py),4)];
            else:
                return False;
    else:
        # if line 2 is vertical to x axes
        if x3 == x4:
            px = x3;
            py = y2 + (y2 - y1) * (px - x2) / (x2 - x1);
            if py >= min(y1, y2) and py <= max(y1, y2) and py >= min(y3, y4) and py <= max(y3, y4):
                return [round(float(px),4), round(float(py),4)];
            else:
                return False;
        else:
            # if line 1 and line 2 are both not vertical to x axes
            px = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / (
                        (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
            py = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / (
                        (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
            if px >= min(x1, x2) and px <= max(x1, x2) and px >= min(x3, x4) and px <= max(x3, x4):
                return [round(float(px),4), round(float(py),4)];
            else:
                return False;

#This function perform add function of street
def a(name, st_points):
    global stmap;
    global interpoint_value;
    global interpoint_parent
    if name not in stmap:
        # For every line in new added street
        out_count=0;
        while out_count <(len(st_points) - 1):
            # check all lines already existed.
            for st in stmap:
                count=0;
                while count <len(stmap[st])-1:
                    point1 = stmap[st][count];
                    point2 = stmap[st][count + 1];
                    intersect = calintersect(point1[0], point1[1], point2[0], point2[1], st_points[out_count][0],
                                                  st_points[out_count][1], st_points[out_count + 1][0],
                                                  st_points[out_count + 1][1]);
                    # If there is a intersection point
                    if intersect != False:
                        # if the intersect point is already in the input street
                        if intersect not in st_points:
                            st_points.insert(out_count + 1, intersect);
                        # if the intersect point is already in the exist street
                        if intersect not in stmap[st]:
                            stmap[st].insert(count + 1, intersect)
                        # if the intersect point is in the intersect point lists
                        if intersect not in interpoint_value:
                            interpoint_value.append(intersect);
                            interpoint_parent.append([st, name]);
                        else:
                            position = interpoint_value.index(intersect);
                            if st not in interpoint_parent[position]:
                                interpoint_parent[position].append(st);
                            if name not in interpoint_parent[position]:
                                interpoint_parent[position].append(name);
                    count=count+1;
            out_count=out_count+1;
        stmap[name] = st_points;
    else:
        print('Error: current adding street already exist.');
#This function will perform remove
def r(name):

    stmap.pop(name);
    rm_list = [];
    # find the intersection points which should be removed
    for point in range(len(interpoint_value)):
        if name in interpoint_parent[point]:
            interpoint_parent[point].pop(interpoint_parent[point].index(name));
            if len(interpoint_parent[point]) < 2:
                rm_list.append(point);
                stmap[interpoint_parent[point][0]].pop(
                    stmap[interpoint_parent[point][0]].index(interpoint_value[point]))
    # pop those intersection points out
    rm_list.sort(reverse=True);
    for i in rm_list:
        interpoint_parent.pop(i);
        interpoint_value.pop(i);

#This function will remove a street then readd it with new paths
def c( name, st_points):
    r(name);
    a(name, st_points);

#This function will output the graph
def g():
    V = [];
    E = [];
    #Get the vertice
    for i in stmap:
        for int_p in interpoint_value:
            if int_p in stmap[i]:
                pos=stmap[i].index(int_p)
                pos_list=[pos-1,pos,pos+1];
                for p in pos_list:
                    if p>=0 and p<len(stmap[i]):
                        if stmap[i][p] not in V:
                            V.append(stmap[i][p]);
    #Get the Edge
    for i in stmap:
        for int_p in interpoint_value:
            if int_p in stmap[i]:
                pos=stmap[i].index(int_p)
                pos_list=[pos-1,pos];
                for p in pos_list:
                    if p>=0 and p<len(stmap[i])-1:
                        if [V.index(stmap[i][p]) , V.index(stmap[i][p + 1]) ] not in E:
                            E.append([V.index(stmap[i][p]) , V.index(stmap[i][p + 1]) ]);
    print('V %d' %len(V));
    print('E = {',end='')
    for i in E[0:len(E)-1]:
        print('<%d,%d>,' % (i[0],i[1]),end='');
    if len(E)>0:
        i=E[len(E)-1]
        print('<%d,%d>'% (i[0], i[1]),end='');
    print('}',flush=True);
    
if __name__ == '__main__':
    #create a variable to store the map as a dictionary: name and their points.
    stmap= {};
    #create a list to store the intersection points
    interpoint_value = [];
    #create a list to store the parent street of each intersection points
    interpoint_parent = [];
    main();
    print('end');
