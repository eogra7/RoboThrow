//
// Created by erogra on 2/8/2019.
//

#include "ArmAction.h"
#include <sstream>
#include <zconf.h>

const int S1_PIN = 13;
const int S2_PIN = 10;
const int S3_PIN = 11;
const int S4_PIN = 12;
const int S5_PIN = 0;

int servoToPin(int servo);

void RobotArm::baseLoop() {
    while (active) {
        base->PulseOnceAngle(basePos);
    }
}

void RobotArm::bicepLoop() {
    while (active) {
        bicep->PulseOnceAngle(bicepPos);
    }
}

void RobotArm::elbowLoop() {
    while (active) {
        elbow->PulseOnceAngle(elbowPos);
    }
}

void RobotArm::gripperLoop() {
    while (active) {
        gripper->PulseOnceAngle(gripperPos);
    }
}

void RobotArm::wristLoop() {
    while (active) {
        wrist->PulseOnceAngle(wristPos);
    }
}

RobotArm::RobotArm() {
    active = false;
    basePos = 90;
    bicepPos = 90;
    wristPos = 90;
    gripperPos = 90;
    elbowPos = 90;

    base = new GPIOExt(S1_PIN);
    bicep = new GPIOExt(S2_PIN);
    elbow = new GPIOExt(S3_PIN);
    wrist = new GPIOExt(S4_PIN);
    gripper = new GPIOExt(S5_PIN);

    moving = false;

    base_thread = std::thread(&RobotArm::baseLoop, this);
    bicep_thread = std::thread(&RobotArm::bicepLoop, this);
    elbow_thread = std::thread(&RobotArm::elbowLoop, this);
    wrist_thread = std::thread(&RobotArm::wristLoop, this);
    gripper_thread = std::thread(&RobotArm::gripperLoop, this);
}

void RobotArm::activate() {
    active = true;

}

void RobotArm::deactivate() {
    active = false;
}

void RobotArm::startPlanned() {
    while (!actions.empty()) {
        ArmAction action = actions.front();
        actions.pop();
        std::thread actionThread(&RobotArm::doAction, this, action);
        if (!action.concurrent) {
            actionThread.join();
        }
    }
}

bool RobotArm::planFromString(std::string &line) {
    int base, bicep, elbow, wrist, gripper, conc, speed, fast;
    std::istringstream iss(line);
    if (!(iss >> base >> bicep >> elbow >> wrist >> gripper >> conc >> speed >> fast)) {
        return false;
    }

    ArmAction action(base, bicep, elbow, wrist, gripper, static_cast<bool>(conc), speed, static_cast<bool>(fast));
    actions.push(action);

    return true;
}

int max(int a, int b, int c, int d, int e) {
    int m = INT_MIN;
    if (a > m) m = a;
    if (b > m) m = b;
    if (c > m) m = c;
    if (d > m) m = d;
    if (e > m) m = e;
    return m;
}

void RobotArm::doAction(ArmAction action) {
    if (!action.fast) {
        int basePosDelta = action.s_base - basePos;
        int bicepPosDelta = action.s_bicep - bicepPos;
        int elbowPosDelta = action.s_elbow - elbowPos;
        int wristPosDelta = action.s_wrist - wristPos;
        int gripperPosDelta = action.s_gripper - gripperPos;
        // need the biggest change, that will take the longest
        int maxDelta = max(basePosDelta, bicepPosDelta, elbowPosDelta, wristPosDelta, gripperPosDelta);
        double time_ms = maxDelta / (action.speed / 1000.0);
        int num_pulses = (int) (time_ms / 20.0);
        for (int ii = 0; ii < num_pulses; ii++) {
            basePos += (basePosDelta / num_pulses);
            bicepPos += (bicepPosDelta / num_pulses);
            elbowPos += (elbowPosDelta / num_pulses);
            wristPos += (wristPosDelta / num_pulses);
            gripperPos += (gripperPosDelta / num_pulses);
            usleep(20000);
        }
    }
    basePos = action.s_base;
    bicepPos = action.s_bicep;
    elbowPos = action.s_elbow;
    wristPos = action.s_wrist;
    gripperPos = action.s_gripper;
}

void RobotArm::startInteractive() {

}

void GPIOExt::GenerateVariablePWM(int period, int first_pulse, int last_pulse, int num_periods) {
    const double pulse_delta = (last_pulse - first_pulse) / (double) num_periods;
    double curr_pulse = -1;
    for (int i = 0; i < num_periods; i++) {
        // have we sent first pulse yet? no -> send first_pulse : yes -> delta
        curr_pulse = (curr_pulse == -1) ? first_pulse : (curr_pulse + pulse_delta);

        GeneratePWM(period, (int) curr_pulse, 1);
        //cout << curr_pulse << endl;
    }
}

void GPIOExt::PulseOnceAngle(int angle) {
    GeneratePWM(PERIOD, (angle * 10) + 600, 1);
}

GPIOExt::GPIOExt(int number)
        : GPIO(number) {}

int servoToPin(int servo) {
    if (servo == 1)
        return S1_PIN;
    if (servo == 2)
        return S2_PIN;
    if (servo == 3)
        return S3_PIN;
    if (servo == 4)
        return S4_PIN;
    if (servo == 5)
        return S5_PIN;
    return -1;
}

ArmAction::ArmAction(int s_base, int s_bicep, int s_elbow, int s_wrist, int s_gripper, bool concurrent, int speed,
                     bool fast)
        : s_base(s_base), s_bicep(s_bicep), s_elbow(s_elbow), s_wrist(s_wrist), s_gripper(s_gripper),
          concurrent(concurrent), speed(speed), fast(fast) {}