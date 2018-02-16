#pragma once

#include <list>
#include <queue>
#include "Sequence.hpp"

class State {
public:
    /**
     * Creates the start state for problem size n
     */
    State(int n);

    State() = default;
    State(const State& s) = default;
    State(State&& s) = default;
    State& operator = (const State&) = default;

public:
    bool isGoal() const;
    int getCost() const;
    int getHeuristic() const;
    void expand(std::priority_queue<State>& q) const;

    /**
     * Returns true if this state is less attractive than rhs; i.e. our
     * current cost + heuristic is greater than that of rhs
     */
    bool operator < (const State& rhs) const;
    
    const IntSequence& getSequence() const { return mSeq; }
    static int getNumExpanded() { return mNumExpanded; }

private:
    IntSequence mSeq;
    std::list<IntSequence*> mRemaining;

    static int mNumExpanded;
};

