/*BUCKET TYPE
 * 
 */


// Left motor driver pin allocation
# define  mRightFWD  33  // CR7
# define  mRightREV  31  // CR8
# define  stopModeR  47  // CR9
# define  m0R  49  // CR10
# define  alarmResetR  51  // CR11
# define  mbFreeR  53  // CR12
# define  alarmOutLN  30
# define  wngLN  32
# define  speedOutL  34
# define  mmLPin  2  // left motor speed PWM -- pin 12

// right motor driver pin allocation
# define  mLeftFWD  43  // CR1
# define  mLeftREV  45  // CR2
# define  stopModeL  35  // CR3
# define  m0L  37  // CR4
# define  alarmResetL  39  // CR5
# define  mbFreeL  41  // CR6
# define  alarmOutRN  40
# define  wngRN  38
# define  speedOutR  36
# define  mmRPin  3  // right motor speed PWM -- pin 12

// Cylinder motor driver pin allocation
# define  dumpFWD  50
# define  dumpREV  52
# define  liftFWD  46
# define  liftREV  48

// Cylinder feedback pin allocation
# define  liftFeedBack A0 // violet cable
# define  dumpFeedBack A1 // violet cable

// battery feedback
int batteryPin = A2; // battery voltage reader
int batteryV = 0 ;


// Relay pin assignment for power relay
# define  switcherOut  20  // 5 // switch output
// relay for external emergency stop switch
// #define External_emergency_stop_relay 20


// DO Harness battery level monitor->notification
# define  soc1  22  // DO4
# define  soc2  24  // DO5
# define  RedLED  21  // pcb extentions D PIN 6

// receiver pin allocation
const  int ch1 = 6 ; // right horizontal
const  int ch3 = 8 ; // right vertical
const  int ch2 = 9 ; // left vertical
const  int ch4 = 7 ; // left horizontal
const  int ch5 = 11 ; //for convert FWD/REV direction ch6 
const  int ch7 = 12 ; // / VR channel
const  int switcherIn = 10 ; // switch input  sw(B) 

// mode setting
int  gMode = 0 ; // bucket mode: 1, blade mode: 0

// variable for receiver
int VR = 0 ;
int rightX = 0 ;
int rightY = 0 ;
int leftX, leftXa , leftXb;
int leftY = 0 ;
int switcher = 0 ;
int gap;

// variable for cylinder feedback
double liftData = 0 ;
double dumpData = 0 ;


void  AttachmentModeChange (){
  //int val= digitalRead (ch5);
  int val = 1; // bucket val
   if (val == 1 ){
      gMode = 1 ;
  }
   else  if (val == 0 ){
      gMode = 0 ;
   }
  //Serial.println (val) ;
}

void  setup () {

  Serial. begin ( 9600 );

  // motor driver speed setting
  pinMode (mmLPin, OUTPUT);
  pinMode (mmRPin, OUTPUT);

  // right motor driver output
  pinMode ( mRightFWD , OUTPUT);
  pinMode ( mRightREV , OUTPUT);
  pinMode (stopModeR, OUTPUT);
  pinMode (m0R, OUTPUT);
  pinMode (alarmResetR, OUTPUT);
  pinMode (mbFreeR, OUTPUT);

  // right motor driver input
  pinMode (alarmOutRN, INPUT);
  pinMode (wngRN, INPUT);
  pinMode (speedOutR, INPUT);

  // left motor driver output
  pinMode ( mLeftFWD , OUTPUT);
  pinMode ( mLeftREV , OUTPUT);
  pinMode (stopModeL, OUTPUT);
  pinMode (m0L, OUTPUT);
  pinMode (alarmResetL, OUTPUT);
  pinMode (mbFreeL, OUTPUT);

  // left motor driver input
  pinMode (alarmOutLN, INPUT);
  pinMode (wngLN, INPUT);
  pinMode (speedOutL, INPUT);

  // cylinder motor driver
  pinMode (liftFWD, OUTPUT);
  pinMode (liftREV, OUTPUT);
  pinMode (dumpFWD, OUTPUT);
  pinMode (dumpREV, OUTPUT);

  // receiver
  pinMode (ch1, INPUT);
  pinMode (ch3, INPUT);
  pinMode (ch2, INPUT);
  pinMode (ch4, INPUT);
  pinMode (ch5, INPUT);
  pinMode (ch7, INPUT);
  pinMode (switcherIn, INPUT);

  // relay for power relay
  pinMode (switcherOut, OUTPUT);

  // relay for external emergency stop switch
//   pinMode(External_emergency_stop_relay,OUTPUT);

  // arm feedback
  pinMode (liftFeedBack, INPUT);
  pinMode (dumpFeedBack, INPUT);

  // HIGH: Time setting LOW: Instantaneous stop
  digitalWrite (stopModeR, LOW);
  digitalWrite (stopModeL, LOW);
  
  // HIGH: Externally input speed specification LOW: Internal speed potentiometer
  digitalWrite (m0R, HIGH);
  digitalWrite (m0L, HIGH);
  
  // HIGH: brake released LOW: brake held
  digitalWrite (mbFreeR, HIGH);
  digitalWrite (mbFreeL, HIGH);

  // Battery Pin
  pinMode (batteryPin, INPUT);

  // Do Harness
  pinMode(soc1, INPUT);
  pinMode(soc2, INPUT);
  pinMode(RedLED, OUTPUT);
}

void  loop () {
  AttachmentModeChange (); // Switch between bucket mode and blade mode
  switcher = pulseIn (switcherIn, HIGH);
  batteryData (); // battery voltage presentation
  doHarness ();  
  
  if ( switcher > 1550 ) {
    digitalWrite (switcherOut, LOW); // power relay ON
    // digitalWrite(External_emergency_stop_relay, LOW);
    VR = pulseIn (ch7, HIGH); // start ch7 measurement

    if (VR > 1700 ) {
      leftXb = leftXa;
      rightX = pulseIn (ch1, HIGH); // start ch1 measurement
      rightY = pulseIn (ch3, HIGH); // start ch3 measurement
      leftX = pulseIn (ch2, HIGH);   // start ch2 measurement
      leftY = pulseIn (ch4, HIGH);   // start ch4 measurement
      leftXa = leftX;

      gap = abs (leftXb - leftXa);
      if (leftX < 1000 || leftX > 2000 ) {
        leftX = leftXb;
        leftXa = leftXb;
      } else {
        leftX = leftX;
        leftXa = leftXa;
      }

      // arm movement
      lift (rightY);
      dump (rightX);

      // motor movement
      motor (leftX, leftY);
    } else {
      stopMachine ();
    }
  } else {
    digitalWrite (switcherOut, HIGH);
    // digitalWrite(External_emergency_stop_relay,HIGH);
    stopMachine ();
  }
}

// motor stop function
void  stopMachine () {

  // motor speed 0
  analogWrite (mmLPin, 0 );
  analogWrite (mmRPin, 0 );

  // No forward/backward command
  digitalWrite (liftFWD, LOW);
  digitalWrite (liftREV, LOW);
  digitalWrite (dumpFWD, LOW);
  digitalWrite (dumpREV, LOW);

  // HIGH: brake released LOW: brake held
  digitalWrite (mbFreeL, LOW);
  digitalWrite (mbFreeR, LOW);

  // display STOP
  // Serial.println("Stop");

}

// motor function
void  motor ( int leftX, int leftY) {
  // radio threshold
  int thresholdHigh = 1715 ;
  int thresholdMid = 1510 ;
  int thresholdLow = 1308 ;

  // movement direction flag
  int Right = 0 ;
  int Left = 0 ;
  int Forward = 0 ;
  int Reverse = 0 ;

  // Motor rotation direction flag
  bool dirR = false ;
  bool dirL = false ;
  bool canMove = false ;

  // determine movement direction
  if (leftX >= thresholdHigh) {
    Right = 1 ;
  } else  if (leftX <= thresholdLow) {
    Left = 1 ;
  }
  if (leftY >= thresholdHigh) {
    Forward = 1 ;
  } else  if (leftY <= thresholdLow) {
    Reverse = 1 ;
  }

  // reverse front/rear/left/right flags
  if ( gMode == 0 ){
    int temp = Forward;
    Forward = Reverse;
    Reverse = temp;
    temp = Right;
    Right = Left;
    Left = temp;
  }

  // determine motor operation
  int diffX = abs (leftX-thresholdMid);
  int diffY = abs (leftY-thresholdMid);
  int speedX = map (diffX, 0 , 410 , 0 , 255 );
    // Serial.print("speedX => ");
    // Serial.println(speedX);
  int speedY = map (diffY, 0 , 410 , 0 , 255 );
    // Serial.print("speedY => ");
    // Serial.println(speedY);
  
  if (Forward == 1 ) {
    canMove = true ;
    dirR = dirL = false ;
    if (Right == 1 ) {
      // forward right
      analogWrite (mmLPin, speedY);
      analogWrite (mmRPin, 0 );
    } else  if (Left == 1 ) {
      // forward left
      analogWrite (mmLPin, 0 );
      analogWrite (mmRPin, speedY);
    } else {
      // forward
      analogWrite (mmLPin, speedY);
      analogWrite (mmRPin, speedY);
    }
  } else  if (Reverse == 1 ) {
    canMove = true ;
    dirR = dirL = true ;
    if (Right == 1 ) {
      // back right
      analogWrite (mmLPin, speedY);
      analogWrite (mmRPin, 0 );
    } else  if (Left == 1 ) {
      // backward left
      analogWrite (mmLPin, 0 );
      analogWrite (mmRPin, speedY);
    } else {
      // backward
      analogWrite (mmLPin, speedY);
      analogWrite (mmRPin, speedY);
    }
  } else {
    if (Right == 1 ) {
      // turn right
      canMove = true ;
      if ( gMode == 0 ) {
        dirR = true ;
        dirL = false ;
      } else {
        dirR = false ;
        dirL = true ;  
      }
      analogWrite (mmLPin, speedX);
      analogWrite (mmRPin, speedX);
    } else  if (Left == 1 ) {
      // turn left
      canMove = true ;
      if ( gMode == 0 ) {
        dirR = false ;
        dirL = true ;
      } else {
        dirR = true ;
        dirL = false ;  
      }
      analogWrite (mmLPin, speedX);
      analogWrite (mmRPin, speedX);
    } else {
      // stop
      canMove = false ;
      analogWrite (mmLPin, 0 );
      analogWrite (mmRPin, 0 );
    }
  }
  digitalWrite ( mRightFWD , dirR & canMove);
  digitalWrite ( mRightREV , (!dirR) & canMove);
  digitalWrite ( mLeftFWD , dirL & canMove);
  digitalWrite ( mLeftREV , (!dirL) & canMove);
}

// arm lift function
void  lift ( int pulse) {
  // radio threshold
  int thresholdHigh = 1715 ;
  int thresholdMid = 1510 ;
  int thresholdLow = 1308 ;
  
  int Extend = 0 ;
  int Retract = 0 ;

  bool dirLift = false ;
  bool canLift = false ;
  
  if (pulse >= thresholdHigh) {
    Retract = 1 ;
  } else  if (pulse <= thresholdLow) {
    Extend = 1 ;
  }

  // Serial.print("Extend => ");
  // Serial.println(Extend);
  // Serial.print("Retract => ");
  // Serial.println(Retract);

  if (Extend == 1 ) {
    dirLift = true ;
    canLift = true ;
  } else  if (Retract == 1 ) {
    dirLift = false ;
    canLift = true ;
  } else {
    canLift = false ;
  }
  digitalWrite (liftFWD, (!dirLift) & canLift); // dirLift->(!dirLift) // Swap 20220909
  digitalWrite (liftREV, dirLift & canLift);
}

// arm dump function
void  dump ( int pulse) {
  // radio threshold
  int thresholdHigh = 1715 ;
  int thresholdMid = 1510 ;
  int thresholdLow = 1308 ;
  
  int Extend = 0 ;
  int Retract = 0 ;

  bool dirDump = false ;
  bool canDump = false ;
  
  if (pulse >= thresholdHigh) {
    Extend = 1 ;
  } else  if (pulse <= thresholdLow) {
    Retract = 1 ;
  }

  // Serial.print("Extend => ");
  // Serial.println(Extend);
  // Serial.print("Retract => ");
  // Serial.println(Retract);

  if (Extend == 1 ) {
    dirDump = true ;
    canDump = true ;
  } else  if (Retract == 1 ) {
    dirDump = false ;
    canDump = true ;
  } else {
    canDump = false ;
  }

  // if (gMode == 0) {
    digitalWrite (dumpFWD, dirDump & canDump);
    digitalWrite (dumpREV, (!dirDump) & canDump);
  // } else {
    // digitalWrite(dumpFWD, (!dirDump) & canDump);
    // digitalWrite(dumpREV, dirDump & canDump);
  // }
}


void  batteryData (){
      batteryV = analogRead (batteryPin);
      double batteryVo = ((batteryV+ 0.5 )* 5.00 )/ 1024 ;
      double batteryVoltage = batteryVo* 30.00 / 5.00 ;
      
      // Serial. print(" Battery Voltage : ");  
      Serial. print (batteryV);
      Serial. print ( " , " ); Serial. print (batteryVo);
      Serial. print ( " , " ); Serial. println (batteryVoltage);
}

void  doHarness (){
  /*
  Serial.print("soc1:: ");
  Serial.print(digitalRead(soc1));
  Serial.print("---------------------soc2:: ");
  Serial.println(digitalRead(soc2));
  */

   if ( digitalRead(soc1) == HIGH && digitalRead(soc2) == HIGH){
        digitalWrite (RedLED, HIGH);
        //Serial.println("battery is LOW. 25% charged");
    }
    else {
        digitalWrite (RedLED, LOW);
        //Serial.println("battery is charged");
    }
}
