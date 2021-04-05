// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#pragma once

#include <frc/TimedRobot.h>
#include "Constants.h"
#include <ctre/Phoenix.h>
#include <rev/CANSparkMax.h>
#include <frc/Joystick.h>
#include <frc/drive/MecanumDrive.h>
#include <frc/shuffleboard/Shuffleboard.h>
#include <frc/PowerDistributionPanel.h>
#include <frc/SpeedControllerGroup.h>
#include <networktables/NetworkTable.h>

//create class of robot with parent
class Robot : public frc::TimedRobot
{

public:
    void RobotInit() override;
    void RobotPeriodic() override;

    void AutonomousInit() override;
    void AutonomousPeriodic() override;

    void TeleopInit() override;
    void TeleopPeriodic() override;

    void DisabledInit() override;
    void DisabledPeriodic() override;

    void TestInit() override;
    void TestPeriodic() override;

private:
    // Initialize pigeonIMU from device ID pigeon ID
    ctre::phoenix::sensors::PigeonIMU pigeon_{pigeonId};

    // Motor Intializers
    rev::CANSparkMax frMotor_{frMotorId, rev::CANSparkMax::MotorType::kBrushless};
    rev::CANSparkMax flMotor_{flMotorId, rev::CANSparkMax::MotorType::kBrushless};
    rev::CANSparkMax rrMotor_{rrMotorId, rev::CANSparkMax::MotorType::kBrushless};
    rev::CANSparkMax rlMotor_{rlMotorId, rev::CANSparkMax::MotorType::kBrushless};
    rev::CANSparkMax intakeMotor_{intakeMotorId, rev::CANSparkMax::MotorType::kBrushed};
    rev::CANSparkMax escalatorMotor_{escalatorMotorId, rev::CANSparkMax::MotorType::kBrushed};
    rev::CANSparkMax feederMotor_{feederMotorId, rev::CANSparkMax::MotorType::kBrushed};
    // shooterMotorOne_ will be the leader
    rev::CANSparkMax shooterMotorOne_{shooterMotorOneId, rev::CANSparkMax::MotorType::kBrushed};
    // shooterMotorTwo_ will follow shooterMotorOne_
    rev::CANSparkMax shooterMotorTwo_{shooterMotorTwoId, rev::CANSparkMax::MotorType::kBrushed};
    frc::SpeedControllerGroup shooterMotors_{shooterMotorOne_, shooterMotorTwo_};
    
    // Might not actually be an error; Don't need this if burnt to flash using Rev Hardware Client
    // rev::CANError followError_ = shooterMotorTwo_.Follow(shooterMotorOne_); 
    bool PIDControl = false; // It'd be a miracle if this worked

    rev::CANPIDController PIDControllerLeader_ {shooterMotorOne_};

    // Initialize Joysticks
    frc::Joystick firstJoystick_{firstJoystickId};
    frc::Joystick secondJoystick_{secondJoystickId};

    // Initialize MecanumDrive object called mecanumDrive_
    frc::MecanumDrive mecanumDrive_{frMotor_, flMotor_, rrMotor_, rlMotor_};

    // ------------------------
    // Add mecanum display to shuffleboard
    frc::ComplexWidget *mecanumShuffleboard = &frc::Shuffleboard::GetTab(tabName)
                                                   .Add("Mecanum Drive", mecanumDrive_)
                                                   .WithWidget(frc::BuiltInWidgets::kMecanumDrive)
                                                   .WithPosition(4,2)
                                                   .WithSize(4,2);

    // Initialize a bunch of shuffleboard network tables
    // for the speeds that everything should be set at
    nt::NetworkTableEntry intakeSpeed_ = frc::Shuffleboard::GetTab(tabName)
                                             // AddPersistent makes sure that if the controller is set,
                                             // next time shuffleboard loads up, it'll use that as the
                                             // default value
                                             .AddPersistent("Intake Motor", 0.0)
                                             // Builtin Widget made to set the speed of speed controllers
                                             .WithWidget(frc::BuiltInWidgets::kNumberSlider)
                                             // Set Position of widget
                                             .WithPosition(1, 1)
                                             // Adding a Size?
                                             .WithSize(2,1)
                                             // Get Network Table that will be dynamically updated
                                             .GetEntry();
    nt::NetworkTableEntry escalatorSpeed_ = frc::Shuffleboard::GetTab(tabName)
                                                .AddPersistent("Escalator Motor", 0.0)
                                                .WithWidget(frc::BuiltInWidgets::kNumberSlider)
                                                .WithPosition(1, 2)
                                                .WithSize(2,1)
                                                .GetEntry();
    nt::NetworkTableEntry feederSpeed_ = frc::Shuffleboard::GetTab(tabName)
                                             .AddPersistent("Feeder Motor", 0.0)
                                             .WithWidget(frc::BuiltInWidgets::kNumberSlider)
                                             .WithPosition(1, 3)
                                             .WithSize(2,1)
                                             .GetEntry();
    nt::NetworkTableEntry shooterSpeed_ = frc::Shuffleboard::GetTab(tabName)
                                              .AddPersistent("Shooter Motor", 0.0)
                                              .WithWidget(frc::BuiltInWidgets::kNumberSlider)
                                              .WithPosition(1, 4)
                                              .WithSize(2,1)
                                              .GetEntry();
    // ----------------
    // frc::PowerDistributionPanel pdp_{}
};
