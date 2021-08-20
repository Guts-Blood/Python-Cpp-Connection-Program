// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <queue>

using namespace std;
//Define the Vertice type as GraphNode
class GraphNode
{
    public:
    //initiali some attribute
        int color;
        int distance;
        int parent;
        vector<int> adj_list;
};
//Define the Graph class
class Graph
{
    public:
        void setVertice(int);
        void addEdge(int,int);
        void Shortestpath(int,int);
        void initialize();
        vector<GraphNode> Vertice;
};
//reset the Graph
void Graph::initialize()
{
    Vertice.clear();
}

void Graph::addEdge(int a,int b)
{
    Vertice[a].adj_list.push_back(b);
    Vertice[b].adj_list.push_back(a);
}

//setVertice
void Graph::setVertice(int v)
{
    for(int count=0;count<v;count++)
    {
        GraphNode temp;
        Vertice.push_back(temp);
    }
}
//use BFS to find the shortest path
void Graph::Shortestpath(int s,int v)
{
    //Initialize vertex
    for(int counter=0;counter<Vertice.size();counter++)
    {
        Vertice[counter].color=0;
        Vertice[counter].distance=2147483647;
        Vertice[counter].parent=-1;
    }
    //first we process the source vertice
    Vertice[s].color++;
    Vertice[s].distance=0;
    //Use a queue to perform the BFS
    queue<GraphNode> Q;
    Q.push(Vertice[s]);
    //Use a int queue to track the parent
    queue<int> Q_position;
    Q_position.push(s);
    //BFS start here
    while(!Q.empty())
    {
        //Get the Vertice pop out
        GraphNode u;
        u=Q.front();
        Q.pop();
        int position;
        position=Q_position.front();
        Q_position.pop();
        for(int counter=0;counter<u.adj_list.size();counter++)
        {
            int temp=u.adj_list[counter];
            //If the adj node's color is white(==0)
            if (Vertice[temp].color==0)
            {
                Vertice[temp].color++;
                Vertice[temp].distance=u.distance+1;
                Vertice[temp].parent=position;
                Q.push(Vertice[temp]);
                Q_position.push(temp);
            }
        }
        u.distance++;
    }
    //Then we print the shortest path from s to v
    vector<int> path;
    GraphNode cur=Vertice[v];
    //If the path dose not exist
    if (cur.parent==-1)
    {
        cout<<"Error: there is no path from "<<s<<" to "<<v<<endl;
    }
    //if it exist
    else{
        path.push_back(v);
        do
        {
            path.push_back(cur.parent);
            cur=Vertice[cur.parent];
        }while (cur.distance>0);
        //Reverse order and output them
        //reverse(path.begin(),path.end());
        for(int count=path.size()-1;count>0;count--)
        {
            cout<<path[count]<<"-";
        }
        cout<<path[0]<<endl;
    }

}
//main function begins here
int main()
{
    string input_commands;
    //get the input commands
    //cin.ignore();
    //initialize the class var newGraph
    Graph newGraph;
    //cout<<"in a2ece650 now!"<<endl;
    do
    {
    
    getline(cin,input_commands);
    //V command
    // cout<<"get an input"<<endl;
    // cout<<input_commands<<endl;
    if (input_commands[0]=='V')
    {
        
        cout<<input_commands<<endl;
        //Initialize the graph and vertice whenever V commands detected
        newGraph.initialize();
        int v;
        v=atoi(input_commands.substr(1).c_str());
        newGraph.setVertice(v);
    }
    //E command
    else if(input_commands[0]=='E')
    {
        cout<<input_commands<<endl;
        //cout<<"specify the edges"<<endl;
        int begin;
        int middle;
        int end;
        for(int count=0;count<input_commands.length();count++)
        {
            if (input_commands[count]=='<')
            {
                begin=count+1;
            }
            if (input_commands[count]==',')
            {
                middle=count;
            }
            //A point is generated
            if (input_commands[count]=='>')
            {
                end=count;
                int vertice1;
                int vertice2;
                vertice1=atoi(input_commands.substr(begin,middle).c_str());
                vertice2=atoi(input_commands.substr(middle+1,end).c_str());
                //detect if vertice is exist in our graph, if not, print out error
                if (vertice1<newGraph.Vertice.size()&&vertice2<newGraph.Vertice.size())
                {
                    newGraph.addEdge(vertice1,vertice2);
                    //cout<<"add edge successful"<<endl;
                }
                else
                {
                    cout<<"Error: Input edge excess the graph vertice size"<<endl;
                    //cout<<newGraph.Vertice.size()<<endl;
                    break;
                }
            }
        }
    }
    //s command
    else if(input_commands[0]=='s')
    {
        //cout<<"print the shortest path"<<endl;
        //First we get the std input by detecting space
        int begin=0;
        int end=input_commands.length();
        for(int count=1;count<input_commands.length();count++)
        {
            if(isspace(input_commands[count])&&begin!=0)
            {
                end=count;
                break;
            }
            if(isspace(input_commands[count]))
            {
                begin=count;
            }
        }
        int source;
        int vertice;
        //convert the string input into int input
        source=atoi(input_commands.substr(begin,end).c_str());
        vertice=atoi(input_commands.substr(end+1).c_str());
        //Check if the std input is valid, if not, print out error
        if (source<newGraph.Vertice.size()&&vertice<newGraph.Vertice.size())
        {
            if(source==vertice)
            {
                cout<<"Error: There is no meaning check the shortest path from a vertice to itself"<<endl;
            }
            else
            {
                newGraph.Shortestpath(source,vertice);
            }
        }
        else
        {
            cout<<"Error: Input Vertice does not exists in our graph"<<endl;
        }
        
    }
    //check if the input is eof, if not then print out error
    else
    {
        if (!cin.eof())
        {
            cout<<"Error: no such command exisit"<<endl;
        }
        // else{
        //     eofcount=eofcount+1;
        // }
    }
    }while(!cin.eof());
    //cout<<"end of file"<<endl;
    // fflush(stdin);

    
    return 0;
}
