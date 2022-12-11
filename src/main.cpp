#include <Arduino.h>
#include <Wire1.h>
#include <EEPROM.h>
#include <uStepperS.h>

uStepperS stepper;

int32_t bigNum;
byte a, b, c, d;
byte array4Bytes[4] = {0, 0, 0, 0};
int x = 0;

void requestEvents()
{
  Serial.println(F("---> recieved request"));
}


void receiveEvent(int bytes)
{
  // x = Wire1.read(); // read one character from the I2C
  a = Wire1.read();
  //Serial.println(a);
  b = Wire1.read();
  //Serial.println(b);
  c = Wire1.read();
  //Serial.println(c);
  d = Wire1.read();
  //Serial.println(d);

  bigNum = a;
  bigNum = (bigNum << 8) | b;
  bigNum = (bigNum << 8) | c;
  bigNum = (bigNum << 8) | d;

  Serial.println(bigNum);

  stepper.moveSteps(bigNum);                        // Turn shaft 51200 steps, counterClockWise (equal to one revolution with the TMC native 1/256 microstepping)
  Serial.println(stepper.encoder.getAngleMoved()); // Print out the current angle of the motor shaft.
}

void setup()
{
  // Start the I2C Bus as Slave on address 9
  Serial.begin(115200);
  Wire1.begin(9);
  // Attach a function to trigger when something is received.
  Wire1.onReceive(receiveEvent);
  Wire1.onRequest(requestEvents);
  Serial.println("Slave I2C");

  stepper.setup(CLOSEDLOOP, 400); // Initiate the stepper object to use closed loop control with 200 steps per revolution motor - i.e. 1.8 deg stepper
  // For the closed loop position control the acceleration and velocity parameters define the response of the control:
  stepper.setMaxAcceleration(2000); // use an acceleration of 2000 fullsteps/s^2
  stepper.setMaxVelocity(800);      // Max velocity of 800 fullsteps/s

  stepper.checkOrientation(30.0);  // Check orientation of motor connector with +/- 30 microsteps movement
  stepper.setControlThreshold(15); // Adjust the control threshold - here set to 15 microsteps before making corrective action
}

void loop()
{
}
