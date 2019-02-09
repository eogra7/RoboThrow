//
// Created by erogra on 2/8/2019.
//

#ifndef ROBOTHROW_ROBOTARM_H
#define ROBOTHROW_ROBOTARM_H

#include <queue>
#include <thread>

#include "GPIO.h"


const int PERIOD = 20000;

class ArmAction {
public:
    int s_base;
    int s_bicep;
    int s_elbow;
    int s_wrist;
    int s_gripper;

    bool concurrent;
    int speed;
    bool fast;

    ArmAction(int s_base, int s_bicep, int s_elbow, int s_wrist, int s_gripper, bool concurrent, int speed, bool fast);
};


class GPIOExt : public GPIO {
public:
    explicit GPIOExt(int number);

    void GenerateVariablePWM(int period, int first_pulse, int last_pulse, int num_periods);

    void PulseOnceAngle(int angle);
};

class RobotArm {
private:
    GPIOExt *base;
    GPIOExt *bicep;
    GPIOExt *elbow;
    GPIOExt *wrist;
    GPIOExt *gripper;

    int basePos;
    int bicepPos;
    int elbowPos;
    int wristPos;
    int gripperPos;

    bool active;
    bool moving;

    std::queue<ArmAction> actions;

    void baseLoop();

    void bicepLoop();

    void elbowLoop();

    void gripperLoop();

    void wristLoop();

    std::thread wrist_thread;
    std::thread gripper_thread;
    std::thread elbow_thread;
    std::thread base_thread;
    std::thread bicep_thread;

public:

    RobotArm();

    ~RobotArm() = default;


    void activate();

    void deactivate();

    void doAction(ArmAction action);

    void startInteractive();

    void startPlanned();

    bool planFromString(std::string &line);
};


#endif //ROBOTHROW_ROBOTARM_H
