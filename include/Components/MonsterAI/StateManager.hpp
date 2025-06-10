#pragma once

#include <vector>
#include <functional>

struct State
{
    std::function<void()> action;
};

class StateManager
{
private:
    std::vector<std::pair<std::function<bool()>, State*>> stateManager;
    State* currentState;
public:
    StateManager() {}

    ~StateManager() {}

    State* getState(int index)
    {
        return stateManager[index].second;
    }

    State* setCurrentState(int index)
    {
        currentState = stateManager[index].second;
        return currentState;
    }

    State* addState(std::function<bool()> condition)
    {
        auto state = new State();
        stateManager.push_back({condition, state});
        return state;
    }

    void update()
    {   
        for(auto state : stateManager)
        {
            if(state.first()) currentState = state.second;
        }

        currentState->action();
    }
};