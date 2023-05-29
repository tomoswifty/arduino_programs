/*BUCKET TYPE
 *
 */

// Left motor driver pin allocation
#define mRightFWD 33   // CR7
#define mRightREV 31   // CR8
#define stopModeR 47   // CR9
#define m0R 49         // CR10
#define alarmResetR 51 // CR11
#define mbFreeR 53     // CR12
#define alarmOutLN 30
#define wngLN 32
#define speedOutL 34
#define mmLPin 2 // left motor speed PWM -- pin 12

// right motor driver pin allocation
#define mLeftFWD 43    // CR1
#define mLeftREV 45    // CR2
#define stopModeL 35   // CR3
#define m0L 37         // CR4
#define alarmResetL 39 // CR5
#define mbFreeL 41     // CR6
#define alarmOutRN 40
#define wngRN 38
#define speedOutR 36
#define mmRPin 3 // right motor speed PWM -- pin 12

// Cylinder motor driver pin allocation
// #define dumpFWD 50
// #define dumpREV 52
// #define liftFWD 46
// #define liftREV 48

// BackHoe Cylinder Pin Assignment
#define backHoeBoomCylinderFwd 50
#define backHoeBoomCylinderRev 52
#define backHoeArmCylinderFwd 46
#define backHoeArmCylinderRev 48
#define backHoeBucketCylinderFwd A3
#define backHoeBucketCylinderRev A4

// Cylinder feedback pin allocation
#define liftFeedBack A0 // violet cable
#define dumpFeedBack A1 // violet cable

// battery feedback
const int batteryPin = A2; // battery voltage reader
int batteryV = 0;

// Relay pin assignment for power relay
#define switcherOut 20 // 5 // switch output
// relay for external emergency stop switch
// #define External_emergency_stop_relay 20

// DO Harness battery level monitor->notification
#define soc1 22   // DO4
#define soc2 24   // DO5
#define RedLED 21 // pcb extentions D PIN 6

// receiver pin allocation
const int ch1 = 6;  // right horizontal
const int ch2 = 9;  // left vertical
const int ch3 = 8;  // right vertical
const int ch4 = 7;  // left horizontal
const int ch5 = 10; // Toggle Switch B
const int ch6 = 11; // Toggle Switch D
const int ch7 = 12; // VR Knob

// variable for receiver
int vrKnobPulse = 0;
int rightXPulse = 0;
int rightYPulse = 0;
int leftXPulse = 0;
int leftYPulse = 0;
int toggleSwitchBPulse = 0;
int toggleSwitchDPulse = 0;

// variable for cylinder feedback

void setup()
{

  Serial.begin(115200);

  // motor driver speed setting
  pinMode(mmLPin, OUTPUT);
  pinMode(mmRPin, OUTPUT);

  // right motor driver output
  pinMode(mRightFWD, OUTPUT);
  pinMode(mRightREV, OUTPUT);
  pinMode(stopModeR, OUTPUT);
  pinMode(m0R, OUTPUT);
  pinMode(alarmResetR, OUTPUT);
  pinMode(mbFreeR, OUTPUT);

  // right motor driver input
  pinMode(alarmOutRN, INPUT);
  pinMode(wngRN, INPUT);
  pinMode(speedOutR, INPUT);

  // left motor driver output
  pinMode(mLeftFWD, OUTPUT);
  pinMode(mLeftREV, OUTPUT);
  pinMode(stopModeL, OUTPUT);
  pinMode(m0L, OUTPUT);
  pinMode(alarmResetL, OUTPUT);
  pinMode(mbFreeL, OUTPUT);

  // left motor driver input
  pinMode(alarmOutLN, INPUT);
  pinMode(wngLN, INPUT);
  pinMode(speedOutL, INPUT);

  // cylinder motor driver
  pinMode(backHoeBoomCylinderFwd, OUTPUT);
  pinMode(backHoeBoomCylinderRev, OUTPUT);
  pinMode(backHoeArmCylinderFwd, OUTPUT);
  pinMode(backHoeArmCylinderRev, OUTPUT);
  pinMode(backHoeBucketCylinderFwd, OUTPUT);
  pinMode(backHoeBucketCylinderRev, OUTPUT);

  // receiver
  pinMode(ch1, INPUT);
  pinMode(ch3, INPUT);
  pinMode(ch2, INPUT);
  pinMode(ch4, INPUT);
  pinMode(ch6, INPUT);
  pinMode(ch7, INPUT);
  pinMode(ch5, INPUT);

  // relay for power relay
  pinMode(switcherOut, OUTPUT);

  // relay for external emergency stop switch
  //   pinMode(External_emergency_stop_relay,OUTPUT);

  // arm feedback
  pinMode(liftFeedBack, INPUT);
  pinMode(dumpFeedBack, INPUT);

  // HIGH: Time setting LOW: Instantaneous stop
  digitalWrite(stopModeR, LOW);
  digitalWrite(stopModeL, LOW);

  // HIGH: Externally input speed specification LOW: Internal speed potentiometer
  digitalWrite(m0R, HIGH);
  digitalWrite(m0L, HIGH);

  // HIGH: brake released LOW: brake held
  digitalWrite(mbFreeR, HIGH);
  digitalWrite(mbFreeL, HIGH);

  // Battery Pin
  pinMode(batteryPin, INPUT);

  // Do Harness
  pinMode(soc1, INPUT);
  pinMode(soc2, INPUT);
  pinMode(RedLED, OUTPUT);
}

void loop()
{
  toggleSwitchBPulse = pulseIn(ch5, HIGH);
  toggleSwitchDPulse = pulseIn(ch6, HIGH);
  vrKnobPulse = pulseIn(ch7, HIGH); // start ch7 measurement
  rightXPulse = pulseIn(ch1, HIGH); // start ch1 measurement
  rightYPulse = pulseIn(ch3, HIGH); // start ch3 measurement
  leftXPulse = pulseIn(ch2, HIGH);  // start ch2 measurement
  leftYPulse = pulseIn(ch4, HIGH);  // start ch4 measurement

  batteryData(); // battery voltage presentation
  doHarness();

  Serial.print(toggleSwitchBPulse);
  Serial.print(',');
  Serial.print(toggleSwitchDPulse);
  Serial.print(',');
  Serial.print(vrKnobPulse);
  Serial.print(',');
  Serial.print(rightXPulse);
  Serial.print(',');
  Serial.print(rightYPulse);
  Serial.print(',');
  Serial.print(leftXPulse);
  Serial.print(',');
  Serial.print(leftYPulse);
  Serial.print('\n');

  if (toggleSwitchBPulse > 1550)
  {
    digitalWrite(switcherOut, LOW); // power relay ON
    // digitalWrite(External_emergency_stop_relay, LOW);

    if (vrKnobPulse > 1700)
    {

      if (toggleSwitchDPulse < 1550)
      {
        Serial.println("Travel Mode");
        // motor movement
        motor(leftXPulse, leftYPulse);
      }
      else
      {
        Serial.println("Backhoe Mode");
        digitalWrite(mRightFWD, LOW);
        digitalWrite(mRightREV, LOW);
        digitalWrite(mLeftFWD, LOW);
        digitalWrite(mLeftREV, LOW);
        controlBoomCylinder(rightYPulse);
        controlArmCylinder(leftXPulse);
        controlBucketCylinder(rightXPulse);
        backHoeTurn(leftYPulse);
      }
    }
    else
    {
      stopMachine();
    }
  }
  else
  {
    Serial.println("Emergency Stop");
    digitalWrite(switcherOut, HIGH);
    // digitalWrite(External_emergency_stop_relay,HIGH);
    stopMachine();
  }
}

// motor stop function
void stopMachine()
{

  // Stop motors
  analogWrite(mmLPin, 0);
  analogWrite(mmRPin, 0);

  // Stop Cylinders
  digitalWrite(backHoeBoomCylinderFwd, LOW);
  digitalWrite(backHoeBoomCylinderRev, LOW);
  digitalWrite(backHoeArmCylinderFwd, LOW);
  digitalWrite(backHoeArmCylinderRev, LOW);
  digitalWrite(backHoeBucketCylinderFwd, LOW);
  digitalWrite(backHoeBucketCylinderRev, LOW);

  // HIGH: brake released LOW: brake held
  digitalWrite(mbFreeL, LOW);
  digitalWrite(mbFreeR, LOW);

  // display STOP
  // Serial.println("Stop");
}

// motor function
void motor(int leftXPulse, int leftYPulse)
{
  // radio threshold
  int thresholdHigh = 1715;
  int thresholdMid = 1510;
  int thresholdLow = 1308;

  // movement direction flag
  int Right = 0;
  int Left = 0;
  int Forward = 0;
  int Reverse = 0;

  // Motor rotation direction flag
  bool dirR = false;
  bool dirL = false;
  bool canMove = false;

  // determine movement direction
  if (leftXPulse >= thresholdHigh)
  {
    Left = 1;
  }
  else if (leftXPulse <= thresholdLow)
  {
    Right = 1;
  }
  if (leftYPulse >= thresholdHigh)
  {
    Reverse = 1;
  }
  else if (leftYPulse <= thresholdLow)
  {
    Forward = 1;
  }

  // determine motor operation
  int diffX = abs(leftXPulse - thresholdMid);
  int diffY = abs(leftYPulse - thresholdMid);
  int speedX = map(diffX, 0, 410, 0, 255);
  // Serial.print("speedX => ");
  // Serial.println(speedX);
  int speedY = map(diffY, 0, 410, 0, 255);
  // Serial.print("speedY => ");
  // Serial.println(speedY);

  if (Forward == 1)
  {
    canMove = true;
    dirR = dirL = false;
    if (Right == 1)
    {
      // forward right
      analogWrite(mmLPin, speedY);
      analogWrite(mmRPin, 0);
    }
    else if (Left == 1)
    {
      // forward left
      analogWrite(mmLPin, 0);
      analogWrite(mmRPin, speedY);
    }
    else
    {
      // forward
      analogWrite(mmLPin, speedY);
      analogWrite(mmRPin, speedY);
    }
  }
  else if (Reverse == 1)
  {
    canMove = true;
    dirR = dirL = true;
    if (Right == 1)
    {
      // back right
      analogWrite(mmLPin, speedY);
      analogWrite(mmRPin, 0);
    }
    else if (Left == 1)
    {
      // backward left
      analogWrite(mmLPin, 0);
      analogWrite(mmRPin, speedY);
    }
    else
    {
      // backward
      analogWrite(mmLPin, speedY);
      analogWrite(mmRPin, speedY);
    }
  }
  else
  {
    if (Right == 1)
    {
      // turn right
      canMove = true;
      dirR = true;
      dirL = false;
      analogWrite(mmLPin, speedX);
      analogWrite(mmRPin, speedX);
    }
    else if (Left == 1)
    {
      // turn left
      canMove = true;
      dirR = false;
      dirL = true;
      analogWrite(mmLPin, speedX);
      analogWrite(mmRPin, speedX);
    }
    else
    {
      // stop
      canMove = false;
      analogWrite(mmLPin, 0);
      analogWrite(mmRPin, 0);
    }
  }
  digitalWrite(mRightFWD, dirR & canMove);
  digitalWrite(mRightREV, (!dirR) & canMove);
  digitalWrite(mLeftFWD, dirL & canMove);
  digitalWrite(mLeftREV, (!dirL) & canMove);
}

void batteryData()
{
  batteryV = analogRead(batteryPin);
  double batteryVo = ((batteryV + 0.5) * 5.00) / 1024;
  double batteryVoltage = batteryVo * 30.00 / 5.00;

  // // Serial. print(" Battery Voltage : ");
  // Serial.print(batteryV);
  // Serial.print(" , ");
  // Serial.print(batteryVo);
  // Serial.print(" , ");
  // Serial.println(batteryVoltage);
}

void doHarness()
{
  /*
  Serial.print("soc1:: ");
  Serial.print(digitalRead(soc1));
  Serial.print("---------------------soc2:: ");
  Serial.println(digitalRead(soc2));
  */

  if (digitalRead(soc1) == HIGH && digitalRead(soc2) == HIGH)
  {
    digitalWrite(RedLED, HIGH);
    // Serial.println("battery is LOW. 25% charged");
  }
  else
  {
    digitalWrite(RedLED, LOW);
    // Serial.println("battery is charged");
  }
}

void controlBoomCylinder(int pulse)
{
  // radio threshold
  const int thresholdHigh = 1715;
  const int thresholdMid = 1510;
  const int thresholdLow = 1308;

  if (pulse >= thresholdHigh)
  {
    Serial.println("Extend boom");
    digitalWrite(backHoeBoomCylinderRev, LOW);
    digitalWrite(backHoeBoomCylinderFwd, HIGH);
  }
  else if (pulse <= thresholdLow)
  {
    Serial.println("Retract boom");
    digitalWrite(backHoeBoomCylinderFwd, LOW);
    digitalWrite(backHoeBoomCylinderRev, HIGH);
  }
  else
  {
    digitalWrite(backHoeBoomCylinderFwd, LOW);
    digitalWrite(backHoeBoomCylinderRev, LOW);
  }
}

void controlArmCylinder(int pulse)
{
  const int thresholdHigh = 1715;
  const int thresholdMid = 1510;
  const int thresholdLow = 1308;

  if (pulse >= thresholdHigh)
  {
    Serial.println("Extend arm");
    digitalWrite(backHoeArmCylinderFwd, LOW);
    digitalWrite(backHoeArmCylinderRev, HIGH);
  }
  else if (pulse <= thresholdLow)
  {
    Serial.println("Retract arm");
    digitalWrite(backHoeArmCylinderRev, LOW);
    digitalWrite(backHoeArmCylinderFwd, HIGH);
  }
  else
  {
    digitalWrite(backHoeArmCylinderFwd, LOW);
    digitalWrite(backHoeArmCylinderRev, LOW);
  }
}

void controlBucketCylinder(int pulse)
{
  // radio threshold
  const int thresholdHigh = 1715;
  const int thresholdMid = 1510;
  const int thresholdLow = 1308;

  if (pulse >= thresholdHigh)
  {
    Serial.println("Extend bucket");
    digitalWrite(backHoeBucketCylinderFwd, LOW);
    digitalWrite(backHoeBucketCylinderRev, HIGH);
  }
  else if (pulse <= thresholdLow)
  {
    Serial.println("Retract bucket");
    digitalWrite(backHoeBucketCylinderRev, LOW);
    digitalWrite(backHoeBucketCylinderFwd, HIGH);
  }
  else
  {
    digitalWrite(backHoeBucketCylinderFwd, LOW);
    digitalWrite(backHoeBucketCylinderRev, LOW);
  }
}

void backHoeTurn(int leftXPulse)
{
  // radio threshold
  int thresholdHigh = 1715;
  int thresholdMid = 1510;
  int thresholdLow = 1308;

  // movement direction flag
  int Right = 0;
  int Left = 0;

  // Motor rotation direction flag
  bool dirR = false;
  bool dirL = false;
  bool canMove = false;

  // determine movement direction
  if (leftXPulse >= thresholdHigh)
  {
    Right = 1;
  }
  else if (leftXPulse <= thresholdLow)
  {
    Left = 1;
  }

  // determine motor operation
  int diffX = abs(leftXPulse - thresholdMid);
  int speedX = map(diffX, 0, 410, 0, 255);
  // Serial.print("speedX => ");
  // Serial.println(speedX);

  if (Right == 1)
  {
    // turn right
    canMove = true;
    dirR = true;
    dirL = false;
    analogWrite(mmLPin, speedX);
    analogWrite(mmRPin, speedX);
  }
  else if (Left == 1)
  {
    // turn left
    canMove = true;
    dirR = false;
    dirL = true;
    analogWrite(mmLPin, speedX);
    analogWrite(mmRPin, speedX);
  }
  else
  {
    // stop
    canMove = false;
    analogWrite(mmLPin, 0);
    analogWrite(mmRPin, 0);
  }

  digitalWrite(mRightFWD, dirR & canMove);
  digitalWrite(mRightREV, (!dirR) & canMove);
  digitalWrite(mLeftFWD, dirL & canMove);
  digitalWrite(mLeftREV, (!dirL) & canMove);
}