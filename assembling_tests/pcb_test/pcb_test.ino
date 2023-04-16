/*
2023-04-15
for test PCB board 
1. relay
*/

// #define 
#define RightmbFree 53 //
#define RightAlarmReset 51 //
#define Rightm0 49 //
#define RightStopMode 47 //
#define RightFWD 33 //
#define RightREV 31 //
// #define RightAlarmOut 40
// #define RightwngN 38
// #define RightSpeedOut 36
// #define RightmmPin 3

#define LeftREV 45 //
#define LeftFWD 43 //
#define LeftmbFree 41 //
#define LeftAlarmReset 39 //
#define Leftm0 37 //
#define LeftStopMode 35 //
// #define LeftSpeedOut 34
// #define LeftwngN 32
// #define LeftAlarmOutN 30
// #define LeftmmPin 2

void relay(){
  char val;
  if (Serial.available() > 0){
    val = Serial.read();
    if (val == 'l'){
      Serial.println("relay HIGH");
      // リレー左上から順に
      digitalWrite(RightREV, HIGH); // 31
      delay(400);
      digitalWrite(RightFWD, HIGH); // 33
      delay(400);
      digitalWrite(LeftStopMode, HIGH); // 35
      delay(400);
      digitalWrite(Leftm0, HIGH); // 37
      delay(400);
      digitalWrite(LeftAlarmReset, HIGH); // 39
      delay(400);
      digitalWrite(LeftmbFree, HIGH); // 41
      delay(800);
    }
    else if(val == 'r'){
      // 右上から順に
      digitalWrite(LeftFWD, HIGH); // 43
      delay(400);
      digitalWrite(LeftREV, HIGH); // 45
      delay(400);
      digitalWrite(RightStopMode, HIGH); // 47
      delay(400);
      digitalWrite(Rightm0, HIGH); // 49
      delay(400);
      digitalWrite(RightAlarmReset, HIGH); // 51
      delay(400);
      digitalWrite(RightmbFree, HIGH); // 53
      delay(800);
    }
    else if (val = 'o'){
      Serial.println("relay LOW");
      // リレー左上から順に
      digitalWrite(RightREV, LOW); // 31
      digitalWrite(RightFWD, LOW); // 33
      digitalWrite(LeftStopMode, LOW); // 35
      digitalWrite(Leftm0, LOW); // 37
      digitalWrite(LeftAlarmReset, LOW); // 39
      digitalWrite(LeftmbFree, LOW); // 41

      // 右上から順に
      digitalWrite(LeftFWD, LOW); // 43
      digitalWrite(LeftREV, LOW); // 45
      digitalWrite(RightStopMode, LOW); // 47
      digitalWrite(Rightm0, LOW); // 49
      digitalWrite(RightAlarmReset, LOW); // 51
      digitalWrite(RightmbFree, LOW); // 53
    }   
  }
}

void setup() {
  Serial.begin(9600);
  // pinMode(relayPin, OUTPUT);
  //モータードライバー速度設定
    // pinMode(LeftmmPin, OUTPUT);
    // pinMode(RightmmPin, OUTPUT);
  
    //右モータードライバーアウトプット
    pinMode(RightFWD, OUTPUT);
    pinMode(RightREV, OUTPUT);
    pinMode(RightStopMode, OUTPUT);
    pinMode(Rightm0, OUTPUT);
    pinMode(RightAlarmReset, OUTPUT);
    pinMode(RightmbFree, OUTPUT);
  
    //右モータードライバーインプット
    // pinMode(RightAlarmOut, INPUT);
    // pinMode(RightwngN, INPUT);
    // pinMode(RightSpeedOut, INPUT);
  
    //左モータードライバーアウトプット
    pinMode(LeftFWD, OUTPUT);
    pinMode(LeftREV, OUTPUT);
    pinMode(LeftStopMode, OUTPUT);
    pinMode(Leftm0, OUTPUT);
    pinMode(LeftAlarmReset, OUTPUT);
    pinMode(LeftmbFree, OUTPUT);
  
    //左モータードライバーインプット
    // pinMode(LeftAlarmOutN, INPUT);
    // pinMode(LeftwngN, INPUT);
    // pinMode(LeftSpeedOut, INPUT);
}

void loop() {
  relay();
}