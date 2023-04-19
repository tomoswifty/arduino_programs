/* 
author:Tomoki Sugimoto
created date:20230217
purpose: To control the oriental motor from the proportional controler signal
tested for MSD700 No.1005

updates:
2022-10-26 create file
           change variable name 
2023-02-15 Rewriting cylinders function
           Rewriting motors function
2023-02-16 add proportional controler function
           add IK calcuration
           Rewrite motor drive function
           fix propo to velocity
*/

#define RightmbFree 53
#define RightAlarmReset 51
#define Rightm0 49
#define RightStopMode 47
#define RightAlarmOut 40
#define RightwngN 38
#define RightSpeedOut 36
#define RightFWD 33
#define RightREV 31
#define RightmmPin 3

#define LeftREV 45
#define LeftFWD 43
#define LeftmbFree 41
#define LeftAlarmReset 39
#define Leftm0 37
#define LeftStopMode 35 
#define LeftSpeedOut 34
#define LeftwngN 32
#define LeftAlarmOutN 30
#define LeftmmPin 2

#define c1FWD 46
#define c1REV 48
#define c2FWD 50
#define c2REV 52

#define LiftFB A0
#define DumpFB A1

#define SwitcherOut 20

#define soc1 22    // pcb 4 DO4
#define soc2 24    // pcb 3 DO5
#define RedLED 16  // pcb 6
#define BatteryPin A2

#define M_PI 3.14159265358979323846

const int CH1 = 8;  // right horizontal = X
const int CH2 = 9;  // left vertical = Y
const int CH3 = 6;  // right vertical = Y
const int CH4 = 7;  // left horizontal = X
// const int CH5 = 10; // switch(B)
const int CH6 = 10;  // SwitcherIn 
const int CH7 = 12; // toggle "VR"

int thresholdHigh = 1715, thresholdMid = 1510, thresholdLow = 1308; // プロポ閾値
int rightX = 0; // right stick horizontal
int rightY = 0; // right stick vartical
int leftX  = 0; // left stick horizontal
int leftY  = 0; // left stick vartical
int VR = 0;
int switcher = 0; // variable for ch6 switch

int current_type;
int type = 0;

double batteryVo, batteryVoltage;
int BatteryV = 0;

float motorRpm[2];
float gLinear_x = 0.0;
float gAngular_z = 0.0;

int Extend = 0, Retract = 0;
// double DumpData = 0,LiftData = 0;



void proportionalControler(){
  leftX  = pulseIn(CH2, HIGH);  // CH2の計測を開始
  leftY  = pulseIn(CH4, HIGH);  // CH4の計測を開始
  rightX = pulseIn(CH1, HIGH);  // CH1の計測を開始
  rightY = pulseIn(CH3, HIGH);  // CH3の計測を開始

  // twist value to velosity
  // 1308.0  1715.0
  // for linear
  if(leftY >= thresholdMid){ // plus
    gLinear_x  = map(leftY, 1105, thresholdMid, 0, 462) / 1000; 
  }
  else if(leftY <= thresholdMid){ // minus
    gLinear_x  = - map(leftY, thresholdMid, 1919, 0, 462) / 1000; 
  }
  // for angular
  if(leftX >= thresholdMid){ // plus
    gAngular_z = map(leftX, 1105, 1919, -78, 78) / 100; 
  }
  else if(leftX <= thresholdMid){ // minus
    gAngular_z = - map(leftX, thresholdMid, 1919, 0, 78) / 100; 
  }
  
  // monitoring for check variables
  Serial.print(leftY);
  Serial.print(",");
  Serial.print(leftX);
  Serial.print(",");
  Serial.print(gLinear_x);
  Serial.print(",");
  Serial.println(gAngular_z);
}

void doHarness(){
  if(digitalRead(soc1) == HIGH && digitalRead(soc2) == HIGH){
    digitalWrite(RedLED,HIGH);
    // Serial.println("battery is LOW. 25% cherged");
  }
  else{
    digitalWrite(RedLED,LOW);
    // Serial.println("battery is cherged");
  }
}

void batteryData(){
  BatteryV = analogRead(BatteryPin);
  batteryVo = ((BatteryV + 0.5) * 5.00) / 1024;
  batteryVoltage = batteryVo * 30.00 / 5.00;
  //Serial.print(" Battery Voltage : ");  
  // Serial.print(BatteryV); 
  // Serial.print(","); Serial.print(batteryVo);
  // Serial.print(","); Serial.println(batteryVoltage);
}

// void attachmentModeChange(){
//   /* change direction 
//   blade type : 0 
//   bucket type : 1 */
//   current_type = digitalRead(CH5);
//   switch (current_type){
//   case 0:
//     type = 0;
//     break;
//   case 1:
//     type = 1;
//     break;
//   default:
//     type = 0;
//   }
// }


double velToRpm(double v){
  // convert velocity to rpm 
  const double REDUCTION_RATIO = 100.0;  // 減速比 reduction ratio
  const double WHEEL_RADIUS = 0.1105;  // 車輪半径 radius of gyration [m]
  double n = 30 / M_PI * REDUCTION_RATIO / WHEEL_RADIUS * v;
  
  return n;
}

void ikVelToRpm(double v, double w){
  // calculate linear velocity and angur velocity to left and right velocity
  double D = 0.600;     // 車輪間距離 distance between the wheels [m]
  double v_r = v + D * w;
  double v_l = v - D * w;

  double left_rpm = velToRpm(v_l);
  double right_rpm = velToRpm(v_r);
  
  motorRpm[0] = left_rpm;
  motorRpm[1] = right_rpm;
}


void cylinderMove(int pulse, int FWD, int REV){
  if(pulse >= thresholdHigh){
    digitalWrite(REV, HIGH);
  }
  else if (pulse <= thresholdLow){
    digitalWrite(FWD, HIGH);
  }
  else{
    digitalWrite(FWD, LOW);
    digitalWrite(REV, LOW);
  }
}

void cylinderStop(int c1f, int c1r, int c2f, int c2r){
  digitalWrite(c1f, LOW);
  digitalWrite(c1r, LOW);
  digitalWrite(c2f, LOW);
  digitalWrite(c2r, LOW);
  
  // Sample for additional cylinder
  // ", int c3f, int c3r"
  // digitalWrite(c3f, LOW);
  // digitalWrite(c3r, LOW);
}

void motorStop(){
  analogWrite(LeftmmPin, 0);
  analogWrite(RightmmPin, 0);

  // HIGH:ブレーキ解放 LOW:ブレーキ保持
  digitalWrite(LeftmbFree, LOW);
  digitalWrite(RightmbFree, LOW);

  // STOP表示
  Serial.println("Motor Stop ");
}

void motorDirection(long left_rpm_in, long right_rpm_in){
  long left_rpm;
  long right_rpm;
  
  if(left_rpm_in > 0){
    left_rpm = left_rpm_in;
    digitalWrite(LeftFWD, HIGH);
  }
  else if (left_rpm_in > 0){
    right_rpm = left_rpm_in;
    digitalWrite(RightREV, HIGH);
  }
  else{
    digitalWrite(LeftFWD, LOW);
    digitalWrite(LeftREV, LOW);
  }

  if(right_rpm_in > 0){
    right_rpm = right_rpm_in;
    digitalWrite(RightFWD, HIGH);
  }
  else if (right_rpm_in < 0){
    left_rpm = right_rpm_in;
    digitalWrite(LeftREV, HIGH);
  }
  else{
    digitalWrite(RightFWD, LOW);
    digitalWrite(RightREV, LOW);
  }
}

void motorDrive(int left_rpm, int right_rpm){
  // twist
  int left_byte = map(left_rpm, 0, 4000, 0,255);
  int right_byte = map(right_rpm, 0, 4000, 0,255);

  motorDirection(left_byte, right_byte);
  
  if(left_byte && right_byte != 0.0){
    digitalWrite(LeftmmPin, left_byte);
    digitalWrite(RightmmPin, right_byte);
    Serial.print(left_byte);
    Serial.print(",");
    Serial.println(right_byte);
  }
  else{
    motorStop();
  }
}

void printall(){
  /*battery output values for IoT sysytem
  IoTシステム用バッテリー出力値*/
    Serial.print(" Battery Voltage : ");  
    Serial.print(BatteryV); 
    Serial.print(","); Serial.print(batteryVo);
    Serial.print(","); Serial.println(batteryVoltage);
     
  /*Value of the controller's switch for type switching
  タイプ切り替えのためのコントローラのスイッチの値*/
    Serial.print(current_type);

  /*Motor speed output from motorDrive function
  モータの速度の表示*/


  /*Value of the attachment arms from Lift function
  バケット/ブレードのシリンダの値*/
    Serial.print("Extend => ");
    Serial.println(Extend);
    Serial.print("Retract => ");
    Serial.println(Retract);
}


void setup() {
  Serial.begin(9600);
  //右モータードライバーアウトプット
  pinMode(RightFWD, OUTPUT);
  pinMode(RightREV, OUTPUT);
  pinMode(RightStopMode, OUTPUT);
  pinMode(Rightm0, OUTPUT);
  pinMode(RightAlarmReset, OUTPUT);
  pinMode(RightmbFree, OUTPUT);
  //右モータードライバーインプット
  pinMode(RightAlarmOut, INPUT);
  pinMode(RightwngN, INPUT);
  pinMode(RightSpeedOut, INPUT);

  //左モータードライバーアウトプット
  pinMode(LeftFWD, OUTPUT);
  pinMode(LeftREV, OUTPUT);
  pinMode(LeftStopMode, OUTPUT);
  pinMode(Leftm0, OUTPUT);
  pinMode(LeftAlarmReset, OUTPUT);
  pinMode(LeftmbFree, OUTPUT);
  //左モータードライバーインプット
  pinMode(LeftAlarmOutN, INPUT);
  pinMode(LeftwngN, INPUT);
  pinMode(LeftSpeedOut, INPUT);

  // HIGH:速度指定を外部入力 LOW:内部速度設定器
  digitalWrite(Rightm0, HIGH);
  digitalWrite(Leftm0, HIGH);
  //モータードライバー速度設定
  pinMode(LeftmmPin, OUTPUT);
  pinMode(RightmmPin, OUTPUT);
  // HIGH:時間設定 LOW:瞬時停止
  digitalWrite(RightStopMode, LOW);
  digitalWrite(LeftStopMode, LOW);
  // HIGH:ブレーキ解放 LOW:ブレーキ保持
  digitalWrite(RightmbFree, HIGH);
  digitalWrite(LeftmbFree, HIGH);


  // シリンダーモータードライバー
  pinMode(c1FWD, OUTPUT);
  pinMode(c1REV, OUTPUT);
  pinMode(c2FWD, OUTPUT);
  pinMode(c2REV, OUTPUT);
  // アームフィードバック
  pinMode(LiftFB, INPUT);
  pinMode(DumpFB, INPUT);

  // レシーバー 
  pinMode(CH1, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH4, INPUT);
  // pinMode(CH5, INPUT);
  pinMode(CH7, INPUT);
  pinMode(CH6, INPUT);
  pinMode(SwitcherOut, OUTPUT); // 動力リレー用リレー

  // Battery Pin
  pinMode(BatteryPin, INPUT);
  pinMode(RedLED, OUTPUT);
  
}

void loop() {
  switcher = pulseIn(CH6, HIGH);
  proportionalControler();
  // attachmentModeChange();
  batteryData();
  doHarness();

  if (switcher > 1550) {
      digitalWrite(SwitcherOut, LOW);
      VR = pulseIn(CH7, HIGH);

    if (VR > 1700) {
      // ----------------test---------------- 
      // gLinear_x, gAngular_z
      // float gLinear_x = 0.011; // 0.4628;   // [m/s]
      // float gAngular_z = 0.0;     // [rad/s]
      // ------------------------------------
      ikVelToRpm(gLinear_x, gAngular_z);

      motorDrive(motorRpm[0],motorRpm[1]);
      
      Serial.print("left motor value: ");
      Serial.print(motorRpm[0]);
      Serial.print(", right motor value: ");
      Serial.println(motorRpm[1]);

      cylinderMove(rightY, c1FWD, c1REV); // cylinder 1
      cylinderMove(rightX, c2REV, c2FWD); // cylinder 2
      }
      else {
        motorStop();
        cylinderStop(c1FWD, c1REV, c2FWD, c2REV);
      }
  }
  else {
    digitalWrite(SwitcherOut, HIGH);
    motorStop();
    cylinderStop(c1FWD, c1REV, c2FWD, c2REV);
  }
  // printall();
}