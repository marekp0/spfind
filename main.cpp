#include <cstdio>
#include <string>
#include <vector>
#include <queue>

#include "State.hpp"


int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("Usage: spfind <n>\n");
        return 1;
    }

    int n = std::stoi(argv[1]);

    // A* search
    std::priority_queue<State> states;
    states.push(State(n));

    State finalState;

    int counter = 0;

    while (true) {
        auto s = states.top();
        states.pop();
        if (s.isGoal()) {
            finalState = s;
            break;
        }
        else {
            s.expand(states);
        }

        if (++counter > 10000) {
            counter = 0;
            auto& s = states.top();
            printf("%d\n", s.getCost() + s.getHeuristic());
        }
    }

    printf("Done!\n");
    printf("States expanded: %d\n", State::getNumExpanded());
    printf("Sequence length: %d\n", finalState.getCost());
    printf("Sequence: ");
    finalState.getSequence().print();
}

