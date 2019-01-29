# tender_descender
Tender Descender Repository

- `tender_descender_data_logging.ino` : Teensy 3.5 + DFRobot 10DOF IMU Data Logging
  - Outputs data to the built-in SD card as a .txt file in the following format:
    - | height | pressure | temperature | ax | ay | az | roll | pitch | yaw |
 
 

- `tender_descender_final.ino` : Teensy 3.2 + DFRobot 10DOF IMU Final Code
  - Program Description
    - The height is zeroed/tared on power. 
    - Once takeoff is detected the program updates the maximum altitude read.
    - Once altitude is decreasing, the program sets the "apogee" bit to "true"
    - On descent, if the altitude surpasses the threshold height, the program begins to look for sudden changes in the magnitude of the   acceleration (all three axes)
    - If threshold altitude is surpassed on descent, and an acceleration impulse is detected, both e-matches (e1 and e2), as well as the Teensy's on-board LED are set to HIGH.

- `tender_descender_libs.zip` : All relevant/required libraries for the Arduino/Teensy IDE to work with the given hardware/sensors.
