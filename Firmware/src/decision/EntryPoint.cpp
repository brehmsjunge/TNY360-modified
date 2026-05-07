#include "decision/DecisionLoop.hpp"

// This file contains the two main functions of the Decision Loop : update() and fillIntent().
// One is for updating the internal logic of the decision loop
// The other is for filling the intent object that will be sent to the Reflex core based on the decision state

void DecisionLoop::update(float dt, const IPC::RobotState& state)
{
    // TODO : Implement
}

void DecisionLoop::fillIntent(IPC::ControlIntent& intent, const IPC::RobotState& state)
{
    // TODO : Implement
}