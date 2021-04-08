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
    
    // frc::SpeedControllerGroup shooterMotors_{shooterMotorOne_, shooterMotorTwo_};
    
    // Might not actually be an error; Don't need this if already burnt to flash using Rev Hardware Client
    rev::CANError followError_ = shooterMotorTwo_.Follow(shooterMotorOne_); 
    bool PIDControl = false; // It'd be a miracle if this worked

    // rev::CANPIDController PIDControllerLeader_ {shooterMotorOne_};

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
                                             .WithPosition(1, 0)
                                             // Adding a Size?
                                             .WithSize(2,1)
                                             // Get Network Table that will be dynamically updated
                                             .GetEntry();
    nt::NetworkTableEntry escalatorSpeed_ = frc::Shuffleboard::GetTab(tabName)
                                                .AddPersistent("Escalator Motor", 0.0)
                                                .WithWidget(frc::BuiltInWidgets::kNumberSlider)
                                                .WithPosition(1, 1)
                                                .WithSize(2,1)
                                                .GetEntry();
    int intakeSignMultiplier_ = 1;
    nt::NetworkTableEntry feederSpeed_ = frc::Shuffleboard::GetTab(tabName)
                                             .AddPersistent("Feeder Motor", 0.0)
                                             .WithWidget(frc::BuiltInWidgets::kNumberSlider)
                                             .WithPosition(1, 2)
                                             .WithSize(2,1)
                                             .GetEntry();
    nt::NetworkTableEntry shooterSpeed_ = frc::Shuffleboard::GetTab(tabName)
                                              .AddPersistent("Shooter Motor", 0.0)
                                              .WithWidget(frc::BuiltInWidgets::kNumberSlider)
                                              .WithPosition(1, 3)
                                              .WithSize(2,1)
                                              .GetEntry();
    nt::NetworkTableEntry shooterSpeedGreen_ = frc::Shuffleboard::GetTab(tabName)
                                              .AddPersistent("Shooter Motor Green Zone", 0.0)
                                              .WithWidget(frc::BuiltInWidgets::kNumberSlider)
                                              .WithPosition(1, 4)
                                              .WithSize(2,1)
                                              .GetEntry();
    bool switchSpeedToGreen_ = true;
    nt::NetworkTableEntry switchSpeedToGreenNTBool_ = frc::Shuffleboard::GetTab(tabName)
                                                        .Add("Green Zone Shooter Speed",true)
                                                        .WithWidget(frc::BuiltInWidgets::kToggleButton)
                                                        .WithPosition(5,0)
                                                        .WithSize(2,2)
                                                        .GetEntry();

    // testing
    nt::NetworkTableEntry actualIntakeSpeed_ = frc::Shuffleboard::GetTab(testingTabName)
                                                .AddPersistent("Escalator Motor", 0.0)
                                                .WithWidget(frc::BuiltInWidgets::kNumberBar)
                                                .WithPosition(1, 1)
                                                .WithSize(5,1)
                                                .GetEntry();
    nt::NetworkTableEntry actualEscalatorSpeed_ = frc::Shuffleboard::GetTab(testingTabName)
                                                .AddPersistent("Escalator Motor", 0.0)
                                                .WithWidget(frc::BuiltInWidgets::kNumberBar)
                                                .WithPosition(1, 1)
                                                .WithSize(5,1)
                                                .GetEntry();
    nt::NetworkTableEntry actualFeederSpeed_ = frc::Shuffleboard::GetTab(testingTabName)
                                             .AddPersistent("Feeder Motor", 0.0)
                                             .WithWidget(frc::BuiltInWidgets::kNumberBar)
                                             .WithPosition(1, 2)
                                             .WithSize(5,1)
                                             .GetEntry();
    nt::NetworkTableEntry actualShooterSpeed1_ = frc::Shuffleboard::GetTab(testingTabName)
                                              .AddPersistent("Shooter Motor", 0.0)
                                              .WithWidget(frc::BuiltInWidgets::kNumberBar)
                                              .WithPosition(1, 3)
                                              .WithSize(5,1)
                                              .GetEntry();
    nt::NetworkTableEntry actualShooterSpeed2_ = frc::Shuffleboard::GetTab(testingTabName)
                                              .AddPersistent("Shooter Motor Green Zone", 0.0)
                                              .WithWidget(frc::BuiltInWidgets::kNumberBar)
                                              .WithPosition(1, 4)
                                              .WithSize(5,1)
                                              .GetEntry();
    
    // nt::NetworkTableInstance defaultNTInst_ = nt::NetworkTableInstance::GetDefault();
    // std::shared_ptr<nt::NetworkTable> cameraPublisher_ = defaultNTInst_.GetTable("CameraPublisher");
    // std::shared_ptr<nt::Value> shooterCam_ = cameraPublisher_->GetValue("Aim Cam Processed");
    // std::shared_ptr<nt::Value> intakeCam_ = cameraPublisher_->GetValue("Aim Cam Processed");
    // frc::ShuffleboardLayout* configContainer_ = &frc::Shuffleboard::GetTab(tabName)
    //                                             .GetLayout("Config", frc::BuiltInLayouts::kList)
    //                                             .WithProperties(wpi::StringMap<std::shared_ptr<nt::Value>>{
    //                                                 std::make_pair("Label position", nt::Value::MakeString("HIDDEN"))
    //                                             })
    //                                             .WithPosition(8,1)
    //                                             .WithSize(1,4);
    // nt::NetworkTableEntry config1_ = configContainer_->AddPersistent("Config 1", true)
    //                                     .WithWidget(frc::BuiltInWidgets::kToggleSwitch)
    //                                     .GetEntry();
    // nt::NetworkTableEntry config2_ = configContainer_->AddPersistent("Config 2", false)
    //                                     .WithWidget(frc::BuiltInWidgets::kToggleSwitch)
    //                                     .GetEntry();
    // nt::NetworkTableEntry config3_ = configContainer_->AddPersistent("Config 3", false)
    //                                     .WithWidget(frc::BuiltInWidgets::kToggleSwitch)
    //                                     .GetEntry();
    // nt::NetworkTableEntry config4_ = configContainer_->AddPersistent("Config 4", false)
    //                                     .WithWidget(frc::BuiltInWidgets::kToggleSwitch)
    //                                     .GetEntry();
    // nt::NetworkTableEntry config5_ = configContainer_->AddPersistent("Config 5", false)
    //                                     .WithWidget(frc::BuiltInWidgets::kToggleSwitch)
    //                                     .GetEntry();
    // nt::NetworkTableEntry config6_ = configContainer_->AddPersistent("Config 6", false)
    //                                     .WithWidget(frc::BuiltInWidgets::kToggleSwitch)
    //                                     .GetEntry();
    
    // ----------------
    // frc::PowerDistributionPanel pdp_{}
};
