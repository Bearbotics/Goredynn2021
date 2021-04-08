#pragma once

//define constants for id's of sensors and motors
constexpr int pigeonId = 0;
constexpr int frMotorId = 1;
constexpr int flMotorId = 2;
constexpr int rrMotorId = 3;
constexpr int rlMotorId = 4;
constexpr int intakeMotorId = 5;
constexpr int escalatorMotorId = 6;
constexpr int feederMotorId = 7;
constexpr int shooterMotorOneId = 8;
constexpr int shooterMotorTwoId = 9;

// first joystick is the first joystick
// that's plugged into the computer
// which should be the 
// logitech extreme 3d pro joystick
constexpr int firstJoystickId = 0;
constexpr int secondJoystickId = 1;

constexpr int triggerButton = 1;
// I believe sideButton only exists on the
// logitech extreme 3d pro joystick
// it's mapped somewhere different on the
// logitech attack 3
constexpr int sideButton = 2;

constexpr char tabName[] = "Dashboard";
constexpr char visionTabName[] = "Vision";
constexpr char testingTabName[] = "Testing - AHHHHHHHHHHHHH";

// for Camera stuff? Applies to both cameras
// REMEMBER TO ADJUST IF ADJUSTING CAM RESOLUTION
constexpr int camWidth = 320;
constexpr int camHeight = 240;

// useless ig?
// constexpr int pdpId = 20;