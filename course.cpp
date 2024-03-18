#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

class Course //course class
{
public:
    string code;
    int credits;
    vector<Course *> prerequisites; //prerequisite courses
    Course(string c, int cr) : code(c), credits(cr) {}
};

class DAG
{
private:
    int V;
    vector<int> *adj;
    vector<int> incomingCount; //to keep track of incoming edges to each vertex

public:
    DAG(int V);
    void addEdge(int u, int v);
    void dfs(int v, vector<bool> &visited, vector<int> &ordering);
    vector<int> topologicalSort();
    vector<string> findFeasibleOrdering(vector<Course> &courses, int max_credits);
};

DAG::DAG(int V) //constructor to initialize the graph with v vertices
{
    this->V = V;
    adj = new vector<int>[V];
    incomingCount.resize(V, 0);
}

void DAG::addEdge(int u, int v) //add an edge between two vertices
{
    adj[u].push_back(v);
    incomingCount[v]++;
}

void DAG::dfs(int v, vector<bool> &visited, vector<int> &ordering) //dfs traversal
{
    visited[v] = true;
    for (int u : adj[v])
    {
        if (!visited[u])
        {
            dfs(u, visited, ordering);
        }
    }
    ordering.push_back(v);
}

vector<int> DAG::topologicalSort() //topological sort to generate an ordering of the courses
{
    vector<bool> visited(V, false);
    vector<int> ordering;
    for (int v = 0; v < V; v++)
    {
        if (!visited[v])
        {
            dfs(v, visited, ordering);
        }
    }
    reverse(ordering.begin(), ordering.end());
    return ordering;
}

vector<string> DAG::findFeasibleOrdering(vector<Course> &courses, int max_credits) //feasible ordering of the courses
{
    vector<int> ordering = topologicalSort();
    vector<string> feasible_ordering;
    vector<int> semester_credits(4, 0);
    queue<int> q;

    for (int v : ordering)
    {
        q.push(v);
    }
    while (!q.empty())
    {
        int u = q.front();
        q.pop();
        feasible_ordering.push_back(courses[u].code);
        for (Course *prereq : courses[u].prerequisites)
        {
            incomingCount[u]--;
            if (incomingCount[u] < 0)
            {
                // There is a cycle, so no feasible ordering can be found
                feasible_ordering.clear();
                return feasible_ordering;
            }
        }
        if (semester_credits[u / 4] + courses[u].credits > max_credits)
        {
            feasible_ordering.pop_back();
            q.push(u);
        }
        else
        {
            semester_credits[u / 4] += courses[u].credits;
        }
    }

    return feasible_ordering;
}

int main()
{
    // Define the courses and their prerequisite relationships
    Course c1("CS101", 4);
    Course c2("CS201", 4);
    Course c3("MATH101", 3);
    Course c4("MATH201", 3);
    Course c5("ELEC101", 3);
    Course c6("ELEC201", 3);
    Course c7("CS301", 4);
    Course c8("CS302", 4);
    Course c9("CS303", 4);
    Course c10("CS304", 4);
    c2.prerequisites.push_back(&c1);
    c3.prerequisites.push_back(&c1);
    c4.prerequisites.push_back(&c3);
    c5.prerequisites.push_back(&c3);
    c6.prerequisites.push_back(&c5);
    c7.prerequisites.push_back(&c2);
    c8.prerequisites.push_back(&c7);
    c9.prerequisites.push_back(&c8);
    c10.prerequisites.push_back(&c8);

    // Create the DAG and add the edges between the courses
    DAG dag(10);
    dag.addEdge(0, 1);
    dag.addEdge(0, 2);
    dag.addEdge(2, 3);
    dag.addEdge(2, 4);
    dag.addEdge(4, 5);
    dag.addEdge(1, 6);
    dag.addEdge(6, 7);
    dag.addEdge(7, 8);
    dag.addEdge(7, 9);

    // Find a feasible ordering of the courses
    vector<Course> courses = {c1, c2, c3, c4, c5, c6, c7, c8, c9, c10};
    vector<string> feasible_ordering = dag.findFeasibleOrdering(courses, 24);

    // Print the feasible ordering of the courses
    if (feasible_ordering.empty())
    {
        cout << "No feasible ordering exists." << endl;
    }
    else
    {
        cout << "Feasible ordering:" << endl;
        for (string course : feasible_ordering)
        {
            cout << course << endl;
        }
    }

    return 0;
}