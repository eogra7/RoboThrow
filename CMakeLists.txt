project(RoboThrow)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

set(SOURCE_FILES
        Main.cpp
        GPIO.cpp
        GPIO.h ArmAction.h RobotArm.cpp RobotArm.h)

add_executable(RoboThrow ${SOURCE_FILES})