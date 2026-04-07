#pragma once

/**
 * @brief A class to detect state changes of a digital signal (like a button).
 * @note Each method updates the internal state. Only call ONE method per
 * trigger per logic cycle.
 *
 * @tparam T The type of the state variable (e.g., bool, uint8_t, int).
 */
template < typename T > class EdgeTrigger {
public:
    /**
     * @brief Constructor.
     * @param initial_state The initial state of the signal (defaults to a zero-initialized value of type T).
     */
    explicit EdgeTrigger(T initial_state = T{}) : previousState(initial_state) {}

    /**
     * @brief LOW -> HIGH / Checks for a rising edge event (transition from a "low" or false-like state to a "high" or true-like state).
     * This method updates the internal state.
     * @param currentState The current state of the signal.
     * @return True if a rising edge was detected, otherwise false.
     */
    bool onRisingEdge(T currentState) {
        bool triggered = (previousState == T{}) && (currentState != T{});
        previousState  = currentState;
        return triggered;
    }

    /**
     * @brief HIGH -> LOW / Checks for a falling edge event (transition from a "high" or true-like state to a "low" or false-like state).
     * This method updates the internal state.
     * @param currentState The current state of the signal.
     * @return True if a falling edge was detected, otherwise false.
     */
    bool onFallingEdge(T currentState) {
        bool triggered = (previousState != T{}) && (currentState == T{});
        previousState  = currentState;
        return triggered;
    }

    /**
     * @brief LOW <-> HIGH / Checks for any change in the signal's state.
     * This method updates the internal state.
     * @param currentState The current state of the signal.
     * @return True if the state changed, otherwise false.
     */
    bool onChange(T currentState) {
        bool triggered = previousState != currentState;
        previousState  = currentState;
        return triggered;
    }

private:
    T previousState;
};