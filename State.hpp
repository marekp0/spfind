#pragma once

#include <list>
#include <queue>
#include <memory>
#include "Sequence.hpp"
#include "PermCache.hpp"

class State;
typedef std::shared_ptr<State> StatePtr;
class StatePtrLess;
typedef std::priority_queue<StatePtr, std::vector<StatePtr>, StatePtrLess> StatePtrHeap;

/**
 * Essentially a node in a graph search problem. Represents a subsequence of
 * a superpermutation.
 */
class State : public std::enable_shared_from_this<State> {
public:
    /**
     * Creates the start state for a given set of permutations
     */
    State(PermCache* pc);

    State() = default;
    State(const State& s) = default;
    State(State&& s) = default;
    State& operator = (const State&) = default;

public:
    bool isGoal() const;
    int getCost() const;
    int getHeuristic() const;
    void expand(StatePtrHeap& q);

    /**
     * Returns true if this state is less attractive than rhs; i.e. our
     * current cost + heuristic is greater than that of rhs
     */
    bool operator < (const State& rhs) const;
    
    IntSequence getSequence() const;
    static int getNumExpanded() { return mNumExpanded; }

    void dump() const;

private:
    StatePtr mPrev;
    int mCost;
    PermId mLastAdded = -1;
    std::list<PermId> mRemaining;

    static int mNumExpanded;
};

class StatePtrLess {
public:
    bool operator()(const StatePtr& a, const StatePtr& b) { return *a < *b; }
};

