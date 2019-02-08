#include <iostream>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <thread>  

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

class RoboCop {
private:
    GPIOExt base = GPIOExt(S1_PIN);
    GPIOExt bicep = GPIOExt(S2_PIN);
    GPIOExt eblow = GPIOExt(S3_PIN);
    GPIOExt wrist = GPIOExt(S4_PIN);
    GPIOExt gripper = GPIOExt(S5_PIN);
    

    
    int base_pos;
    int bicep_pos;
    int elbow_pos;
    int wrist_pos;
    int gripper_pos;
    
    bool active;
    
    void wristLoop() {
        if(active) {
            wrist.GeneratePWM(PERIOD, wrist_pos, 1);
        }
    }
    
    void gripperLoop() {
        if (active) {
            gripper.GeneratePWM(PERIOD, gripper_pos, 1);
        }
    }
    
    thread wrist_thread;
    thread gripper_thread;
    
public:
    
    RoboCop() {
        active = false;
        base_pos = 90;
        bicep_pos = 90;
        wrist_pos = 90;
        gripper_pos = 90;
        elbow_pos = 90;
        
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
    //bool fast = (argc > 1 && ((strcmp(argv[1], "-f") == 0) || (strcmp(argv[1], "--fast") == 0)));

    // cout << "Servo\tName" << endl;
    // cout << "1\tBase" << endl;
    // cout << "2\tBicep" << endl;
    // cout << "3\tElbow" << endl;
    // cout << "4\tWrist" << endl;
    // cout << "5\tGripper" << endl
    //      << endl;

    // cout << "Enter servo number (1-5): " << endl;
    // int usr_servo;
    // acceptInt(usr_servo, 1, 5);
    // cout << usr_servo << endl;

    // float usr_angle_start;
    // cout << "Enter start angle (0-180): " << endl;
    // acceptFloat(usr_angle_start, 0, 180);

    // float usr_angle_end;
    // cout << "Enter end angle (0-180): " << endl;
    // acceptFloat(usr_angle_end, 0, 180);

    // int usr_speed;
    // cout << "Enter speed in deg/s: " << endl;
    // acceptInt(usr_speed, 1, SHRT_MAX);

    // int total_distance_degrees = abs(usr_angle_end - usr_angle_start);
    // double total_time_ms = total_distance_degrees / (usr_speed / 1000.0);
    // int num_pulses = (int)(total_time_ms / 20.0);

    // GPIOExt gpio(servoToPin(usr_servo));
    // if (!fast)
    //     gpio.GeneratePWM(20000, degreeToOnDelay(usr_angle_start), 50); // 50 pulses @ 20ms = 1000ms (1sec)
    // gpio.GenerateVariablePWM(20000,
    //     degreeToOnDelay(usr_angle_start),
    //     degreeToOnDelay(usr_angle_end),
    //     num_pulses);
    // if (!fast)
    //     gpio.GeneratePWM(20000, degreeToOnDelay(usr_angle_end), 50); // 50 pulses @ 20ms = 1000ms (1sec)
    // // Done
    
    RoboCop* arm = new RoboCop();
    arm->activate();
    
    int usr_servo;
    float usr_angle;
    
    while(true) {
        cout << "Servo\tName" << endl;
        cout << "1\tBase" << endl;
        cout << "2\tBicep" << endl;
        cout << "3\tElbow" << endl;
        cout << "4\tWrist" << endl;
        cout << "5\tGripper" << endl
             << endl;
    
        cout << "Enter servo number (1-5): " << endl;
        acceptInt(usr_servo, 1, 5);
    
        cout << "Enter angle (0-180): " << endl;
        acceptFloat(usr_angle, 0, 180);
        
        if(usr_servo == 4) arm->setWristPos(usr_angle);
        if(usr_servo == 5) arm->setGripperPos(usr_angle);
        
    }

    
    
    
    
    return 0;
}
