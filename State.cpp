#include "State.hpp"
#include <cstdio>
#include <map>
#include <set>

int State::mNumExpanded = 0;

// assuming only one perm cache. bad style, but whatever
PermCache* gPermCache = nullptr;

State::State(PermCache* pc)
{
    gPermCache = pc;
    auto& perms = pc->getPerms();

    // arbitrarily set our sequence to the first permutation
    mLastAdded = 0;

    // create the remaining permutations list
    for (int i = 1; i < perms.size(); ++i)
        mRemaining.push_back(i);

    mCost = pc->getN();
    recalcHeuristic();
}

bool State::isGoal() const
{
    return mRemaining.size() == 0;
}

int State::getCost() const
{
    return mCost;
}

void State::expand(StatePtrHeap& q)
{
    // basic idea: think of each permId as a node, where the cost between two
    // nodes is their overlap.

    int n = gPermCache->getN();

    // calculate the maximum possible overlap with our remaining perms
    int maxOvr = 0;
    for (auto r : mRemaining) {
        int ovr = gPermCache->getOverlap(mLastAdded, r);
        maxOvr = ovr > maxOvr ? ovr : maxOvr;

        // bail early if we get the maximum possible overlap
        if (maxOvr == n - 1)
            break;
    }

    // generate new nodes
    for (auto r : mRemaining) {
        int ovr = gPermCache->getOverlap(mLastAdded, r);

        // skip all nodes with less than the max overlap
        // TODO: figure out if this is really right. Seems to work for n <= 5
        if (ovr != maxOvr)
            continue;

        // construct new state
        auto newState = std::make_shared<State>(*this);
        ++mNumExpanded;

        newState->mPrev = shared_from_this();
        newState->mLastAdded = r;
        newState->mRemaining.remove(r);
        newState->mCost += n - ovr;
        newState->recalcHeuristic();

        q.push(newState);
    }
}

bool State::operator < (const State& rhs) const
{
    // want to return true if this state is WORSE than rhs, because
    // std::priority_queue selects the MAX item
    int fl = getCost() + getHeuristic();
    int fr = rhs.getCost() + rhs.getHeuristic();
    if (fl == fr) {
        // to break ties, follow the state with higher cost first, i.e. try to
        // make behave more like depth-first search
        return getCost() < rhs.getCost();
    }
    else
        return fl > fr;
}

IntSequence State::getSequence() const
{
    if (mPrev == nullptr)
        return gPermCache->getPerms()[mLastAdded];
    else {
        auto seq = mPrev->getSequence();
        int ovr = gPermCache->getOverlap(mPrev->mLastAdded, mLastAdded);
        auto& appendSeq = gPermCache->getPerms()[mLastAdded];
        for (int i = ovr; i < appendSeq.size(); ++i)
            seq.push_back(appendSeq[i]);
        return seq;
    }
}

void State::dump() const
{
    printf("g=%3d, h=%3d, g+h=%3d, seq=", getCost(), getHeuristic(), getCost()+getHeuristic());
    getSequence().print();
}

void State::recalcHeuristic()
{
    //mHeuristic = mRemaining.size();
    mHeuristic = calcHeuristicMST();
}

int State::calcHeuristicMST()
{
    int n = gPermCache->getN();

    // for the purposes of calculating the MST, include the last added perm as
    // a node
    mRemaining.push_back(mLastAdded);

    struct Edge {
        int node1, node2;
        int cost;
        // return greater than because std::priority_queue selects greatest first
        bool operator < (const Edge& rhs) const { return cost > rhs.cost; }
    };

    // Kruskal's algorithm
    // Put all edges into a heap
    std::priority_queue<Edge> edges;
    auto ii = ++mRemaining.begin();
    for (int i = 1; ii != mRemaining.end(); ++i, ++ii) {
        auto jj = mRemaining.begin();
        for (int j = 0; jj != ii; ++j, ++jj) {
            Edge e;
            e.node1 = i;
            e.node2 = j;
            int cost1 = n - gPermCache->getOverlap(*ii, *jj);
            int cost2 = n - gPermCache->getOverlap(*jj, *ii);
            e.cost = cost1 < cost2 ? cost1 : cost2;
            edges.push(e);
        }
    }

    // Greedily select the edge with minimum cost, and add it if it does not
    // create a cycle. Use union find to detect cycles
    int totalCost = 0;
    int numEdgesAdded = 0;
    std::vector<int> parent(mRemaining.size(), -1); // for union find; -1 = no parent
    while (numEdgesAdded < mRemaining.size() - 1) {
        Edge e = edges.top();
        edges.pop();

        // use union find to see if e.node1 and e.node2 are in the same tree
        int root1 = e.node1;
        while (parent[root1] != -1)
            root1 = parent[root1];

        int root2 = e.node2;
        while (parent[root2] != -1)
            root2 = parent[root2];

        if (root1 != root2) {
            totalCost += e.cost;
            numEdgesAdded += 1;
            parent[root2] = root1;  // union
        }
    }

    // remove the node we added to mRemaining
    mRemaining.pop_back();

    return totalCost;
}

