#include "State.hpp"
#include <cstdio>
#include <map>
#include <set>

int State::mNumExpanded = 0;
std::vector<IntSequence> gPerms;

State::State(int n)
{
    // compute all permutations of {1, 2, ..., n}
    for (int i = 0; i < n; ++i)
        mSeq.push_back(i+1);
    gPerms = mSeq.permute();

    // create the remaining permutations list
    for (auto& perm : gPerms) {
        if (perm != mSeq) {
            mRemaining.push_back(&perm);
        }
    }
}

bool State::isGoal() const
{
    return mRemaining.size() == 0;
}

int State::getCost() const
{
    return mSeq.size();
}

int State::getHeuristic() const
{
    // each remaining permutation will require at least 1 additional number
    return mRemaining.size();
}

void State::expand(std::priority_queue<State>& q) const
{
    std::map<IntSequence*, State> toBePushed;
    std::set<IntSequence*> ignore;

    for (auto r : mRemaining) {
        State newState(*this);
        ++mNumExpanded;

        // append sequence r as "efficiently" as possible
        newState.mRemaining.remove(r);
        int ovr = newState.mSeq.overlap(*r);
        for (int i = ovr; i < r->size(); ++i) {
            newState.mSeq.push_back((*r)[i]);
        }

        // we might have included other permutations, get them out of mRemaining
        auto it = newState.mRemaining.begin();
        while (ovr < r->size() - 1 && it != newState.mRemaining.end()) {
            auto prevIt = it++;
            if (newState.mSeq.contains(**prevIt)) {
                // ignore permutations that are embedded in this one
                ignore.insert(*prevIt);
                newState.mRemaining.erase(prevIt);
                ++ovr;
            }
        }
        toBePushed[r] = std::move(newState);
    }

    // push to the queue if not ignored
    for (auto& p : toBePushed) {
        if (ignore.count(p.first) == 0)
            q.push(std::move(p.second));
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

