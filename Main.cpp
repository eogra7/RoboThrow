#include <iostream>
#include <string.h>
#include <thread>
#include <fstream>


#include "ArmAction.h"

using namespace std;


void acceptInt(int &num, int min, int max) {
    int i;
    cin >> i;
    if (i >= min && i <= max) {
        num = i;
        return;
    } else {
        cout << "Invalid input: " << i << endl;
        acceptInt(num, min, max);
    }
}

void acceptFloat(float &num, float min, float max) {
    float i;
    cin >> i;
    if (i >= min && i <= max) {
        num = i;
        return;
    } else {
        cout << "Invalid input: " << i << endl;
        acceptFloat(num, min, max);
    }
}

int main(int argc, char *argv[]) {

    if (argc == 1) {
        cout << "Usage: \n"
                "./runRoboThrow --interactive\n"
                "./runRoboThrow <file>" << endl;
        return 0;
    }

    RobotArm *arm = new RobotArm();

    if ((strcmp(argv[1], "-i") == 0) || (strcmp(argv[1], "--interactive") == 0)) {
        arm->startInteractive();
        return 0;
    }
    int lines = 0;
    cout << "Reading plan...\n";
    ifstream planFile(argv[1]);
    string line;
    while (getline(planFile, line)) {
        cout << line;
        if (!(arm->planFromString(line))) { break; }
        lines++;
    }
    cout << "Done reading " << lines << "lines from " << argv[1] << endl;

    do {
        cout << '\n' << "Press ENTER to begin program...";
    } while (cin.get() != '\n');

    arm->startPlanned();

    /* int usr_servo;
     float usr_angle;

     bool b1;
     bool b2;
     bool b3;
     bool b4;
     bool b5;

     int basePos;
     int bicepPos;
     int elbowPos;
     int wristPos;
     int gripperPos;
     b1 = false;
     b2 = false;
     b3 = false;
     b4 = false;
     b5 = false;
     while (true) {


         cout << "Servo\tName" << endl;
         cout << "1\tBase\t" << basePos << endl;
         cout << "2\tBicep\t" << bicepPos << endl;
         cout << "3\tElbow\t" << elbowPos << endl;
         cout << "4\tWrist\t" << wristPos << endl;
         cout << "5\tGripper\t" << gripperPos << endl;
         cout << "6\tExecute" << endl
              << endl;

         cout << "Enter action number (1-6): " << endl;
         acceptInt(usr_servo, 1, 6);

         if (usr_servo != 6) {
             cout << "Enter angle (0-180): " << endl;
             acceptFloat(usr_angle, 0, 180);
         }
         if (usr_servo == 1) {
             basePos = usr_angle;
             b1 = true;
         }
         if (usr_servo == 2) {
             bicepPos = usr_angle;
             b2 = true;
         }
         if (usr_servo == 3) {
             elbowPos = usr_angle;
             b3 = true;
         }
         if (usr_servo == 4) {
             wristPos = usr_angle;
             b4 = true;
         }
         if (usr_servo == 5) {
             gripperPos = usr_angle;
             b5 = true;
         }
         if (usr_servo == 6) {
             if (b1) arm->setBasePos(basePos);
             if (b2) arm->setBicepPos(bicepPos);
             if (b3) arm->setElbowPos(elbowPos);
             if (b4) arm->setWristPos(wristPos);
             if (b5) arm->setGripperPos(gripperPos);
             b1 = false;
             b2 = false;
             b3 = false;
             b4 = false;
             b5 = false;
         }


     }


     return 0;*/
}
