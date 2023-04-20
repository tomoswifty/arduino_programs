// 左モータードライバーピン割振
#define mRightFWD 33 // CR7
#define mRightREV 31 // CR8
#define stopModeR 47 // CR9
#define m0R 49 // CR10
#define alarmResetR 51 //CR11
#define mbFreeR 53 // CR12
#define alarmOutLN 30
#define wngLN 32
#define speedOutL 34
#define mmLPin 2 // left motor speed PWM --  pin 12

//右モータードライバーピン割振
#define mLeftFWD 43 // CR1
#define mLeftREV 45 // CR2
#define stopModeL 35 // CR3
#define m0L 37 // CR4
#define alarmResetL 39 // CR5
#define mbFreeL 41 // CR6
#define alarmOutRN 40
#define wngRN 38
#define speedOutR 36
#define mmRPin 3 // right motor speed PWM --  pin 12

//シリンダーモータードライバーピン割振
#define dumpFWD 50
#define dumpREV 52
#define liftFWD 46
#define liftREV 48

//シリンダーフィードバックピン割振
#define liftFeedBack A0 // violet cable
#define dumpFeedBack A1 // violet cable

//battery feedback 
int batteryPin = A2; // battery voltage reader
int batteryV = 0;


//動力リレー用リレーピン割振
#define switcherOut 20 //5 // switch output
//外付け非常停止スイッチ用リレー
//#define External_emergency_stop_relay 20

//レシーバーピン割振
const int ch1 = 13; // right horizontal
const int ch3 = 11; // right vertical
const int ch2 = 10; // left horizontal
const int ch4 = 12; // left vertical
const int ch5 = 9; //sw(B) for convert FWD/REV direction
const int ch7 = 7; /// VR channel
const int switcherIn = 8; // ch6 switch input

//モード設定
int gMode = 0; //バケットモード:1, ブレードモード:0

//レシーバー用変数
int VR = 0;
int rightX = 0;
int rightY = 0;
int leftX, leftXa , leftXb;
int leftY = 0;
int switcher = 0;
int gap;

//シリンダーフィードバック用変数
double liftData = 0;
double dumpData = 0;

void encoder(){
  long encoderL =digitalRead(speedOutL);
  long encoderR =digitalRead(speedOutR);
  Serial.print("Encoder L/R = ");
  Serial.print(encoderL);
  Serial.println(encoderR);

  //digitalWrite();
}

void AttachmentModeChange(){
  int val=digitalRead(ch5);
   if (val == 1){
      gMode=1;
  }
   else if(val == 0){
      gMode=0;
   }
  Serial.println(val);
}

void setup() {

  Serial.begin(9600);

  //モータードライバー速度設定
  pinMode(mmLPin, OUTPUT);
  pinMode(mmRPin, OUTPUT);

  //右モータードライバーアウトプット
  pinMode(mRightFWD, OUTPUT);
  pinMode(mRightREV, OUTPUT);
  pinMode(stopModeR, OUTPUT);
  pinMode(m0R, OUTPUT);
  pinMode(alarmResetR, OUTPUT);
  pinMode(mbFreeR, OUTPUT);

  //右モータードライバーインプット
  pinMode(alarmOutRN, INPUT);
  pinMode(wngRN, INPUT);
  pinMode(speedOutR, INPUT);

  //左モータードライバーアウトプット
  pinMode(mLeftFWD, OUTPUT);
  pinMode(mLeftREV, OUTPUT);
  pinMode(stopModeL, OUTPUT);
  pinMode(m0L, OUTPUT);
  pinMode(alarmResetL, OUTPUT);
  pinMode(mbFreeL, OUTPUT);

  //左モータードライバーインプット
  pinMode(alarmOutLN, INPUT);
  pinMode(wngLN, INPUT);
  pinMode(speedOutL, INPUT);

  //シリンダーモータードライバー
  pinMode(liftFWD, OUTPUT);
  pinMode(liftREV, OUTPUT);
  pinMode(dumpFWD, OUTPUT);
  pinMode(dumpREV, OUTPUT);

  //レシーバー
  pinMode(ch1, INPUT);
  pinMode(ch3, INPUT);
  pinMode(ch2, INPUT);
  pinMode(ch4, INPUT);
  pinMode(ch5, INPUT);
  pinMode(ch7, INPUT);
  pinMode(switcherIn, INPUT);

  //動力リレー用リレー
  pinMode(switcherOut, OUTPUT);

  //外付け非常停止スイッチ用リレー
//  pinMode(External_emergency_stop_relay,OUTPUT);

  //アームフィードバック
  pinMode(liftFeedBack, INPUT);
  pinMode(dumpFeedBack, INPUT);

  // HIGH:時間設定 LOW:瞬時停止
  digitalWrite(stopModeR, LOW);
  digitalWrite(stopModeL, LOW);
  
  // HIGH:速度指定を外部入力 LOW:内部速度設定器
  digitalWrite(m0R, HIGH);
  digitalWrite(m0L, HIGH);
  
  //HIGH:ブレーキ解放 LOW:ブレーキ保持
  digitalWrite(mbFreeR, HIGH);
  digitalWrite(mbFreeL, HIGH);

  //Battery Pin
  pinMode(batteryPin, INPUT);
}

void loop() {
  AttachmentModeChange(); //バケットモードとブレードモードの切り替え
  switcher = pulseIn(switcherIn, HIGH);
  batteryData(); // battery voltage presentation
  
  if (switcher > 1550) {
    digitalWrite(switcherOut, HIGH); //動力リレーON
    //digitalWrite(External_emergency_stop_relay,LOW);
    VR = pulseIn(ch7, HIGH); //ch7の計測を開始

    if (VR > 1700) {
      leftXb = leftXa;
      rightX = pulseIn(ch1, HIGH); // ch1の計測を開始
      rightY = pulseIn(ch3, HIGH); // ch3の計測を開始
      leftX = pulseIn(ch2, HIGH);  // ch2の計測を開始
      leftY = pulseIn(ch4, HIGH);  // ch4の計測を開始
      leftXa = leftX;

      gap = abs(leftXb - leftXa);
      if (leftX < 1000 || leftX > 2000) {
        leftX = leftXb;
        leftXa = leftXb;
      } else {
        leftX = leftX;
        leftXa = leftXa;
      }

      //アーム動作
      lift(rightY);
      dump(rightX);

      // モーター動作
      motor(leftX, leftY);
    } else {
      stopMachine();
    }
  } else {
    digitalWrite(switcherOut, LOW);
    // digitalWrite(External_emergency_stop_relay,HIGH);
    stopMachine();
  }
  encoder();
}

// モーター停止関数
void stopMachine() {

  //モーター速度0
  analogWrite(mmLPin, 0);
  analogWrite(mmRPin, 0);

  //前進後進指示なし
  digitalWrite(liftFWD, LOW);
  digitalWrite(liftREV, LOW);
  digitalWrite(dumpFWD, LOW);
  digitalWrite(dumpREV, LOW);

  //HIGH:ブレーキ解放 LOW:ブレーキ保持
  digitalWrite(mbFreeL, LOW);
  digitalWrite(mbFreeR, LOW);

  // STOP表示
  //Serial.println(" Stop ");
}

// モーター関数
void motor(int leftX, int leftY) {
  //プロポ閾値
  int thresholdHigh = 1715;
  int thresholdMid = 1510;
  int thresholdLow = 1308;

  //移動方向フラグ
  int Right = 0;
  int Left = 0;
  int Forward = 0;
  int Reverse = 0;

  //モーター回転方向フラグ
  bool dirR = false;
  bool dirL = false;
  bool canMove = false;

  // 移動方向決定
  if (leftX >= thresholdHigh) {
    Right = 1;
  } else if (leftX <= thresholdLow) {
    Left = 1;
  }
  if (leftY >= thresholdHigh) {
    Forward = 1;
  } else if (leftY <= thresholdLow) {
    Reverse = 1;
  }

  //前後左右フラグ反転
  if(gMode == 0){
    int temp = Forward;
    Forward = Reverse;
    Reverse = temp;
    temp = Right;
    Right = Left;
    Left = temp;
  }

  //モーター動作決定
  int diffX = abs(leftX-thresholdMid);
  int diffY = abs(leftY-thresholdMid);
  int speedX = map(diffX, 0, 410, 0, 255);
    //Serial.print("speedX => ");
    //Serial.println(speedX);
  int speedY = map(diffY, 0, 410, 0, 255);
    //Serial.print("speedY => ");
    //Serial.println(speedY);
  
  if (Forward == 1) {
    canMove = true;
    dirR = dirL = false;
    if (Right == 1) {
      //右前進
      analogWrite(mmLPin, speedY);
      analogWrite(mmRPin, 0);
    } else if (Left == 1) {
      //左前進
      analogWrite(mmLPin, 0);
      analogWrite(mmRPin, speedY);
    } else {
      //前進
      analogWrite(mmLPin, speedY);
      analogWrite(mmRPin, speedY);
    }
  } else if (Reverse == 1) {
    canMove = true;
    dirR = dirL = true;
    if (Right == 1) {
      //右後進
      analogWrite(mmLPin, speedY);
      analogWrite(mmRPin, 0);
    } else if (Left == 1) {
      //左後進
      analogWrite(mmLPin, 0);
      analogWrite(mmRPin, speedY);
    } else {
      //後進
      analogWrite(mmLPin, speedY);
      analogWrite(mmRPin, speedY);
    }
  } else {
    if (Right == 1) {
      //右旋回
      canMove = true;
      if (gMode == 0) {
        dirR = true;
        dirL = false;
      } else {
        dirR = false;
        dirL = true;  
      }
      analogWrite(mmLPin, speedX);
      analogWrite(mmRPin, speedX);
    } else if (Left == 1) {
      //左旋回
      canMove = true;
      if (gMode == 0) {
        dirR = false;
        dirL = true;
      } else {
        dirR = true;
        dirL = false;  
      }
      analogWrite(mmLPin, speedX);
      analogWrite(mmRPin, speedX);
    } else {
      //停止
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

// アームリフト関数
void lift(int pulse){
  //プロポ閾値
  int thresholdHigh = 1715;
  int thresholdMid = 1510;
  int thresholdLow = 1308;
  
  int Extend = 0;
  int Retract = 0;

  bool dirLift = false;
  bool canLift = false;
  
  if (pulse >= thresholdHigh) {
    Retract = 1;
  } else if (pulse <= thresholdLow) { 
    Extend = 1;
  }

  //Serial.print("Extend => ");
  //Serial.println(Extend);
  //Serial.print("Retract => ");
  //Serial.println(Retract);

  if (Extend == 1) {
    dirLift = true;
    canLift = true;
  } else if (Retract == 1) {
    dirLift = false;
    canLift = true;
  } else {
    canLift = false;
  }
  digitalWrite(liftFWD, dirLift & canLift);
  digitalWrite(liftREV, (!dirLift) & canLift);
}

// アームダンプ関数
void dump(int pulse){
  //プロポ閾値
  int thresholdHigh = 1715;
  int thresholdMid = 1510;
  int thresholdLow = 1308;
  
  int Extend = 0;
  int Retract = 0;

  bool dirDump = false;
  bool canDump = false;
  
  if (pulse >= thresholdHigh) {
    Extend = 1;
  } else if (pulse <= thresholdLow) { 
    Retract = 1;
  }

  //Serial.print("Extend => ");
  //Serial.println(Extend);
  //Serial.print("Retract => ");
  //Serial.println(Retract);

  if (Extend == 1) {
    dirDump = true;
    canDump = true;
  } else if (Retract == 1) {
    dirDump = false;
    canDump = true;
  } else {
    canDump = false;
  }

  if (gMode == 0) {
    digitalWrite(dumpFWD, dirDump & canDump);
    digitalWrite(dumpREV, (!dirDump) & canDump);
  } else {
    digitalWrite(dumpFWD, (!dirDump) & canDump);
    digitalWrite(dumpREV, dirDump & canDump);
  }
}


void batteryData(){
      batteryV = analogRead(batteryPin);
      double batteryVo = ((batteryV+0.5)*5.00)/1024;
      double batteryVoltage = batteryVo*30.00/5.00;
      
      //Serial.print(" Battery Voltage : ");  
      Serial.print(batteryV); 
      Serial.print(","); Serial.print(batteryVo);
      Serial.print(","); Serial.println(batteryVoltage); 
}
