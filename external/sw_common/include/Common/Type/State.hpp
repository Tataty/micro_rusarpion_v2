#pragma once

enum class DoubleState {
    LOW,
    HIGH,

    OFF = LOW,
    ON  = HIGH,

    DISABLED = LOW,
    ENABLED  = HIGH,
};

enum class TripleState { LOW, MEDIUM, HIGH };