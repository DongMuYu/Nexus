// src/particles/Emitters/BaseEmitter.cpp
#include "BaseEmitter.h"

BaseEmitter::BaseEmitter(Vector2 pos, Vector2 dir, float speed, float rate)
    : position(pos), emissionDirection(dir.normalized()),
    emissionSpeed(speed), emissionRate(rate),
    timeSinceLastEmission(0), isActive(true) {
}