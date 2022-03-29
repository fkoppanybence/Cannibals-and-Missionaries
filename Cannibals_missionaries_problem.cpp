#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>

using namespace std;

//this structure stores the states of the left side of the river
struct Tstate
{
    bool side;
    int can, mis;
    struct Tstate *before;
};

//number of cannibals, missionaries and space in the boat
int n = 3;
int m = 3;
int k = 2;

//we store in map states which where already visited
map<Tstate, int> levels;

//using a queue for BFS
queue<Tstate> qStates;

//this operator is for map ordering
bool operator< (const Tstate &a, const Tstate &b)
{
    return a.side < b.side || (a.side==b.side && a.can<b.can) || (a.side==b.side && a.can==b.can && a.mis<b.mis);
}

//print the solution
void printbhbh(Tstate t)
{
    if (t.before != nullptr)
        printbhbh(*t.before);
    cout << t.can << " " << t.mis << " " << t.side << endl;
}

int breadthFirstSearch()
{
    //defining first state
    Tstate start, final;
    start.side = false;
    start.can = n;
    start.mis = m;
    start.before = nullptr;

    //defining final state
    final.can = 0;
    final.mis = 0;
    final.side = true;
    qStates.push(start);

    levels[start] = 0;

    //while we have still undiscovered nodes and we didnt reach the final state
    while (!qStates.empty() && !levels.count(final))
    {
        //in head we store the next node
        Tstate head = qStates.front();
        int aCan, aMis;

        //left side
        if (!head.side)
        {
            aCan = head.can;
            aMis = head.mis;
        }
        else
        {
            //right side
            aCan = n - head.can;
            aMis = m - head.mis;
        }

        //generating all possibilities of number of missionaries
        for (int q = 0; q <= min(k, aMis); ++q)
        {
            //generating all possibilities of cannibals
            for (int w = 0; w <= min(k - q, aCan); w++)
            {
                //the boat cannot be empty
                if ((q + w) == 0)
                    continue;

                //missionaries <= cannibals
                if (q && w > q)
                    break;

                //we generate the new state which will be added to the queue
                start.side = !head.side;
                if (!head.side)
                {
                    start.can = head.can - w;
                    start.mis = head.mis - q;
                }
                else
                {
                    start.can = head.can + w;
                    start.mis = head.mis + q;
                }
                if ((start.can <= start.mis || start.mis == 0) && ((n - start.can) <= (m - start.mis) || start.mis == m) && !levels.count(start))
                {
                    //we create a linked list to search for the nodes of the solution
                    start.before = new Tstate(head);
                    levels[start] = levels[head] + 1;
                    qStates.push(start);
                }
            }
        }
        qStates.pop();
    }
    if (levels.count(final)){
        printbhbh(start);
        return levels[final];
    }
    else
        return -1;
}

int main()
{
    cout << breadthFirstSearch();
}