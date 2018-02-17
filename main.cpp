#include <cstdio>
#include <string>
#include <vector>
#include <queue>

#include "State.hpp"
#include "PermCache.hpp"


int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("Usage: spfind <n>\n");
        return 1;
    }

    int n = std::stoi(argv[1]);
    PermCache pc(n);

    // A* search
    StatePtrHeap states;
    states.push(std::make_shared<State>(&pc));

    StatePtr finalState;

    int counter = 0;

    while (true) {
        StatePtr s = states.top();
        states.pop();

        if (s->isGoal()) {
            finalState = s;
            break;
        }
        else {
            s->expand(states);
        }

        if (++counter > 1000) {
            counter = 0;
            auto& s = states.top();
            printf("Current g+h: %d\n", s->getCost() + s->getHeuristic());
        }
    }

    printf("Done!\n");
    printf("States expanded: %d\n", State::getNumExpanded());
    printf("Sequence length: %d\n", finalState->getCost());
    printf("Sequence: ");
    finalState->getSequence().print();
}

