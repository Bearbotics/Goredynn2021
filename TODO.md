### Stuff that I need to implement
- Write something that interfaces with the analog sensor and shows it in Shuffleboard
- Implement Follower Mode
  - Primarily done, but I think it'd still be best implemented through Rev Hardware Client
- Add support for PID control through the dashboard using the velocity preset
  - Use kPIDController in Shuffleboard
  - Maybe add toggle between PID and SetSpeed()?
- *Maybe implement export networktable values as json or something?*

### Stuff that I'll ask other people to do
- Need to get William to help me test things out?
- 3d Print Raspberry Pi Case
- Add encoder to the other shooter motor and connect them to their Spark Maxes
- Get person to add shaft encoder?, instead of using motor outputs
  - Simplifies things vassssstly
    - No need to worry about gearing
    - No need to worry about two SEPERATE PID control loops
    - REF: chiefdelphi.com/t/pid-on-2-motor-shooter-wheel/159528/5

### Stuff todo when I'm at Robotics
1. Upload currently built code to robot
2. Reimage Raspberry Pi using etcher and the pi image that's already been downloaded
3. Make the front shooter motor a Follower to the other motor
4. While waiting, run frc-characterization suite on the shooter motors?
5. Upload visual.py after getting the raspbian image up and running to the web interface
6. Hope code works!!!!
7. Inevitably Debug....