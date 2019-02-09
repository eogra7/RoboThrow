#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <thread> 
#include <unistd.h>

#include "GPIO.h"


using namespace std;

int servoToPin(int servo);
int degreeToOnDelay(int angle);
void acceptInt(int& num, int min, int max);
void acceptFloat(float& num, float min, float max);

const int PERIOD = 20000;


const int S1_PIN = 13;
const int S2_PIN = 10;
const int S3_PIN = 11;
const int S4_PIN = 12;
const int S5_PIN = 0;

int servoToPin(int servo)
{
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

int degreeToOnDelay(int angle)
{
    return (angle * 10) + 600;
}

void acceptInt(int& num, int min, int max)
{
    int i;
    cin >> i;
    if (i >= min && i <= max) {
        num = i;
        return;
    }
    else {
        cout << "Invalid input: " << i << endl;
        acceptInt(num, min, max);
    }
}

void acceptFloat(float& num, float min, float max)
{
    float i;
    cin >> i;
    if (i >= min && i <= max) {
        num = i;
        return;
    }
    else {
        cout << "Invalid input: " << i << endl;
        acceptFloat(num, min, max);
    }
}

class GPIOExt : public GPIO {
public:
    GPIOExt(int number)
        : GPIO(number){};
    void GenerateVariablePWM(int period, int first_pulse, int last_pulse,
        int num_periods);

    
};

void GPIOExt::GenerateVariablePWM(int period, int first_pulse, int last_pulse, int num_periods)
{
    const double pulse_delta = (last_pulse - first_pulse) / (double)num_periods;
    double curr_pulse = -1;
    for (int i = 0; i < num_periods; i++) {
        // have we sent first pulse yet? no -> send first_pulse : yes -> delta
        curr_pulse = (curr_pulse == -1) ? first_pulse : (curr_pulse + pulse_delta);

        GeneratePWM(period, (int)curr_pulse, 1);
        //cout << curr_pulse << endl;
    }
}

class ArmAction {
    int s_base;
    int s_bicep;
    int s_elbow;
    int s_wrist;
    int s_gripper;
    
    bool concurrent;
    int speed;
    bool fast;
    
    ArmAction(int s_base, int s_elbow, 
}

class RoboCop {
private:
    GPIOExt* base;
    GPIOExt* bicep;
    GPIOExt* eblow;
    GPIOExt* wrist;
    GPIOExt* gripper;
    

    
    int base_pos;
    int bicep_pos;
    int elbow_pos;
    int wrist_pos;
    int gripper_pos;
    
    bool active;
    
    void baseLoop() {
        while(true) {
            base->GeneratePWM(PERIOD, degreeToOnDelay(base_pos), 1);
        }
    }
    
    void bicepLoop() {
        while(true) {
            bicep->GeneratePWM(PERIOD, degreeToOnDelay(bicep_pos), 1);
        }
    }
    
    void eblowLoop() {
        while(true) {
            eblow->GeneratePWM(PERIOD, degreeToOnDelay(elbow_pos), 1);
        }
    }
    
    void gripperLoop() {
        while(true) {
            gripper->GeneratePWM(PERIOD, degreeToOnDelay(gripper_pos), 1);
        }
    }
    void wristLoop() {
        while(true) {
            wrist->GeneratePWM(PERIOD, degreeToOnDelay(wrist_pos), 1);
        }
    }
    
    
    
    thread wrist_thread;
    thread gripper_thread;
    thread elbow_thread;
    thread base_thread;
    thread bicep_thread;
    
public:
    
    RoboCop() {
        active = false;
        base_pos = 90;
        bicep_pos = 90;
        wrist_pos = 90;
        gripper_pos = 90;
        elbow_pos = 90;
        
        base = new GPIOExt(S1_PIN);
        bicep = new GPIOExt(S2_PIN);
        eblow = new GPIOExt(S3_PIN);
        wrist = new GPIOExt(S4_PIN);
        gripper = new GPIOExt(S5_PIN);
        
        base_thread = thread(&RoboCop::baseLoop, this);
        bicep_thread = thread(&RoboCop::bicepLoop, this);
        elbow_thread = thread(&RoboCop::eblowLoop, this);
        wrist_thread = thread(&RoboCop::wristLoop, this);
        gripper_thread = thread(&RoboCop::gripperLoop, this);
    }
    
    ~RoboCop() {
        
    }
    
    void setBasePos(int pos) {
        base_pos = pos;
    }
    
    void setBicepPos(int pos) {
        bicep_pos = pos;
    }
    
    void setElbowPos(int pos) {
        elbow_pos = pos;
    }
    
    void setWristPos(int pos) {
        wrist_pos = pos;
    }
    
    void setGripperPos(int pos) {
        gripper_pos = pos;
    }
    
    void activate() {
        active = true;

    }
    
    void deactivate() {
        active = false;
    }
};

int main(int argc, char* argv[])
{
    
    RoboCop* arm = new RoboCop();
    arm->activate();
    
    int usr_servo;
    float usr_angle;
    
    bool b1;
    bool b2;
    bool b3;
    bool b4;
    bool b5;
    
    int base_pos;
    int bicep_pos;
    int elbow_pos;
    int wrist_pos;
    int gripper_pos;
            b1 = false;
        b2 = false;
        b3 = false;
        b4 = false;
        b5 = false;
    while(true) {

        
        cout << "Servo\tName" << endl;
        cout << "1\tBase\t"<< base_pos << endl;
        cout << "2\tBicep\t"<< bicep_pos << endl;
        cout << "3\tElbow\t" << elbow_pos << endl;
        cout << "4\tWrist\t" << wrist_pos << endl;
        cout << "5\tGripper\t" << gripper_pos << endl;
        cout << "6\tExecute" << endl
             << endl;
    
        cout << "Enter action number (1-6): " << endl;
        acceptInt(usr_servo, 1, 6);
        
        if(usr_servo != 6) {
        cout << "Enter angle (0-180): " << endl;
        acceptFloat(usr_angle, 0, 180);
        }
        if(usr_servo == 1)
        {
        base_pos = usr_angle;
        b1 = true;
        }
        if(usr_servo == 2) 
        {
        bicep_pos = usr_angle;
        b2 = true;
        }
        if(usr_servo == 3) 
        {
        elbow_pos = usr_angle;
        b3 = true;
        }
        if(usr_servo == 4) 
        {
        wrist_pos = usr_angle;
        b4 = true;
        }
        if(usr_servo == 5) 
        {
        gripper_pos = usr_angle;
        b5 = true;
        }
        if(usr_servo == 6) {
                if(b1) arm->setBasePos(base_pos);
                if(b2) arm->setBicepPos(bicep_pos);
                if(b3) arm->setElbowPos(elbow_pos);
                 if(b4) arm->setWristPos(wrist_pos);
                  if(b5) arm->setGripperPos(gripper_pos);
                          b1 = false;
        b2 = false;
        b3 = false;
        b4 = false;
        b5 = false;
        }
        

    }

    
    
    
    
    return 0;
}
