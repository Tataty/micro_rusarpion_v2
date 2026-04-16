#pragma once

template < typename T > class iFilter {
protected:
    enum State { IGNORE_FIRST, INIT, UPDATE };

    T     value;
    State state;
    bool  ignoreFirstValue;

    virtual void initFilter(T newValue) { value = newValue; }
    virtual void updateFilterValue(T newValue) = 0;

public:
    iFilter(bool ignoreFirstValue) : ignoreFirstValue(ignoreFirstValue) { reset(); }

    void update(T newValue) {
        switch (state) {
        case State::IGNORE_FIRST: {
            state = State::INIT;
        } break;
        case State::INIT: {
            state = State::UPDATE;
            initFilter(newValue);
        } break;
        default:
            updateFilterValue(newValue);
        }
    }
    void reset() {
        if (ignoreFirstValue)
            state = State::IGNORE_FIRST;
        else
            state = State::INIT;
    }

    T    getValue() { return value; }
    bool isReady() { return state == State::UPDATE; }
};