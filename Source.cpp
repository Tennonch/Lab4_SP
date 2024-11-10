#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <fstream>
#include <sstream>

struct Transition {
    int from;
    char symbol;
    int to;
};

void find_unreachable_states(int num_states, int start_state, const std::vector<Transition>& transitions) {
    std::unordered_set<int> reachable;
    std::queue<int> q;

    reachable.insert(start_state);
    q.push(start_state);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        for (const auto& transition : transitions) {
            if (transition.from == current && reachable.find(transition.to) == reachable.end()) {
                reachable.insert(transition.to);
                q.push(transition.to);
            }
        }
    }

    std::cout << "Unreachable states: ";
    bool found_unreachable = false;
    for (int i = 0; i < num_states; ++i) {
        if (reachable.find(i) == reachable.end()) {
            std::cout << i << " ";
            found_unreachable = true;
        }
    }

    if (!found_unreachable) {
        std::cout << "None";
    }
    std::cout << std::endl;
}

void find_deadlock_states(int num_states, const std::vector<Transition>& transitions) {
    std::vector<bool> has_outgoing(num_states, false);

    for (const auto& transition : transitions) {
        has_outgoing[transition.from] = true;
    }

    std::cout << "Deadlock states: ";
    bool found_deadlock = false;
    for (int i = 0; i < num_states; ++i) {
        if (!has_outgoing[i]) {
            std::cout << i << " ";
            found_deadlock = true;
        }
    }

    if (!found_deadlock) {
        std::cout << "None";
    }
    std::cout << std::endl;
}

int main() {
    std::string filename;
    std::cout << "Enter the filename containing the automaton: ";
    std::cin >> filename;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return 1;
    }

    int num_states, start_state;
    file >> num_states >> start_state;

    int num_final_states;
    file >> num_final_states;

    std::vector<int> final_states(num_final_states);
    for (int i = 0; i < num_final_states; ++i) {
        file >> final_states[i];
    }

    std::vector<Transition> transitions;
    int from, to;
    char symbol;
    while (file >> from >> symbol >> to) {
        transitions.push_back({ from, symbol, to });
    }

    file.close();

    // Analysis
    find_unreachable_states(num_states, start_state, transitions);
    find_deadlock_states(num_states, transitions);

    return 0;
}
