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
}

bool State::isGoal() const
{
    return mRemaining.size() == 0;
}

int State::getCost() const
{
    return mCost;
}

int State::getHeuristic() const
{
    // each remaining permutation will require at least 1 additional number
    return mRemaining.size();
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

