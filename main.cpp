#include <bits/stdc++.h>

using namespace std;

ifstream fin("ctc.in");
ofstream fout("ctc.out");

class Graph
{
private:

    int verticesNo;
    int edgesNo;
    bool oriented;
    vector<vector<int>> adjacencyList;


public:

    Graph();
    Graph(int verticesNo, int edgesNo);                                 //only for transposing a graph
    Graph(int verticesNo, int edgesNo, bool oriented);
    Graph transpose();

    void bfs(int start);                                                //BFS

    void dfs(int vertex, vector<int> &visited);                         //simple dfs

    void dfsScc(int vertex, vector<int> &visited, vector<int> &scc);    //dfs for storing strongly connected components

    void dfsWstack(int vertex, vector<int> &visited, stack<int> &st);   //dfs with a stack for both topological sorting and strongly connected components


    void stronglyConnectedComponents();

    int connectedComponentsNo();

    void topologicalSort();

    void HH();


    virtual ~Graph();

};
Graph::Graph()
{
    verticesNo = edgesNo = oriented = 0;
    this->adjacencyList.clear();
}
Graph::Graph(int verticesNo, int edgesNo)
{
    this->verticesNo = verticesNo;
    this->edgesNo = edgesNo;
    this->adjacencyList.resize(verticesNo+1);
}
Graph::Graph(int verticesNo, int edgesNo, bool oriented)
{
    this->verticesNo = verticesNo;
    this->edgesNo = edgesNo;
    this->oriented = oriented;

    this->adjacencyList.resize(verticesNo + 1);

    int vertex1, vertex2;
    for(int i = 0; i < this->edgesNo; i++)
    {
        fin>>vertex1>>vertex2;
        this->adjacencyList[vertex1].push_back(vertex2);
        if(this->oriented == 0)
            this->adjacencyList[vertex2].push_back(vertex1);
    }

}
Graph::~Graph()
{
        this->adjacencyList.clear();
}
void Graph::bfs(int start)
{
    int n = this->verticesNo;
    vector<int> visited;
    vector<int> distances;
    queue<int> order;
    order.push(start);

    for(int i = 0; i <= n; i++)
    {
        visited.push_back(0);
        distances.push_back(-1);
    }
    distances[start] = 0;
    visited[start] = 1;

    while(!order.empty())
    {
        int current = order.front();
        int sz = adjacencyList[current].size();

        order.pop();

        for(int i = 0; i < sz; i++)
        {
            int temp = adjacencyList[current][i];

            if(visited[temp] == 0)
            {
                visited[temp] = 1;
                order.push(temp);
                distances[temp] = distances[current]+1;
            }
        }

    }

    for(int i = 1; i < distances.size(); i++)
        fout<<distances[i]<<" ";

}
void Graph::dfs(int vertex, vector<int> &visited)
{
    visited[vertex] = 1;

    int sz = adjacencyList[vertex].size();

    for(int i = 0; i < sz; i++)
    {
        if(visited[adjacencyList[vertex][i]] == 0)
            dfs(adjacencyList[vertex][i],visited);
    }
}
void Graph::dfsScc(int vertex, vector<int> &visited, vector<int> &scc)
{
    visited[vertex] = 1;
    scc.push_back(vertex);

    int sz = adjacencyList[vertex].size();

    for(int i = 0; i < sz; i++)
    {
        if(visited[adjacencyList[vertex][i]] == 0)
            dfsScc(adjacencyList[vertex][i],visited, scc);
    }
}
int Graph::connectedComponentsNo()
{

    vector<int> visited;
    int ccn = 0; //no of connected components

    for(int i = 0 ; i <= this->verticesNo; i++)
        visited.push_back(0);

    for(int i = 0; i <= this->verticesNo; i++)
        if(visited[i] == 0)
        {
            dfs(i,visited);
            ccn++;
        }


    return ccn;

}

void Graph::stronglyConnectedComponents()
{
    stack<int> st;

    vector<int> visited;
    vector<vector<int>> sccs;
    vector<int> tempScc;

    int sccn = 0;
    for(int i = 0; i <= verticesNo; i++)
    {
        visited.push_back(0);
    }

    for(int i = 1; i <= verticesNo; i++)
        if(visited[i] == 0)
            dfsWstack(i,visited,st);

    Graph tg = transpose();

    for(int i = 0; i <= verticesNo; i++)
        visited[i] = 0;

    while(!st.empty())
    {
        int temp = st.top();
        st.pop();

        if(visited[temp] == 0)
        {   tempScc.clear();
            tg.dfsScc(temp,visited, tempScc);
            sccn++;
            sccs.push_back(tempScc);
        }
    }
    fout<<sccn<<endl;

    for(int i = 0 ; i < sccs.size(); i++)
    {
        int sz = sccs[i].size();
        for(int j = 0; j < sz; j++)
            fout<<sccs[i][j]<<" ";
            fout<<"\n";

    }


}
void Graph::dfsWstack(int vertex, vector<int> &visited, stack<int> &st)
{
    visited[vertex] = 1;

    int sz = adjacencyList[vertex].size();

    for(int i = 0; i < sz; i++)
    {
        if(visited[adjacencyList[vertex][i]] == 0)
            dfsWstack(adjacencyList[vertex][i],visited,st);
    }

    st.push(vertex);

}
void Graph::topologicalSort()
{
    stack<int> st;

    vector<int> visited;

    for(int i = 0; i <= this->verticesNo; i++)
        visited.push_back(0);

    for(int i = 1 ; i <=this->verticesNo; i++)
        if(visited[i] == 0)
            {dfsWstack(i,visited,st);}

    while(!st.empty())
    {
        fout<<st.top()<<" ";
        st.pop();
    }

}
void Graph::HH()
{

    int n,temp;
    vector<int> seq;
    fin>>n;
    for(int i = 0; i < n; i++)
    {
        fin>>temp;
        seq.push_back(temp);
    }

    while(true)
    {
        sort(seq.begin(),seq.end());
        int sz = seq.size()-1;
        if(seq[sz] == 0)
        {
            cout<<1;
            return;
        }

        int v = seq[sz];
        seq.pop_back();

        if(v > seq.size())
        {
            cout<<0;
            return;
        }

        for(int i = v-1; i >=0; i--)
        {
            seq[i]--;
            if(seq[i] < 0)
            {
                cout<<0;
                return;
            }

        }
    }


}
Graph Graph::transpose()
{
    Graph transposedGraph(verticesNo, edgesNo);

    for(int v = 1; v <= verticesNo; v++)
    {
        int sz = adjacencyList[v].size();

       for(int i = 0; i < sz; i++)
        {
            transposedGraph.adjacencyList[this->adjacencyList[v][i]].push_back(v);
        }

    }


    return transposedGraph;

}

int main()
{
    int N,M;
    fin>>N>>M;

    Graph g(N,M,1);
    g.stronglyConnectedComponents();
    return 0;




    return 0;
}

