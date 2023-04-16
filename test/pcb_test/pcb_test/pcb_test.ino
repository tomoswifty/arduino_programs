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

void relayOff(){
  Serial.println("all relays LOW");
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

void relay(){
  int d=1000; // リレーHIGH時間
  char val;
  int num;
  if (Serial.available() > 0){
    val = Serial.read();
    num = Serial.read();
    
    if (val == 'o'){
      relayOff();
    }

    //左側リレー個別確認
    else if (val == '1'){
      Serial.println("relay HIGH : CR1, Arduino pin No.31, FWD_L");
      digitalWrite(RightREV, HIGH); // 31
      delay(d);
      digitalWrite(RightREV, LOW); // 31
      Serial.println("relay LOW : CR1");
    }
    else if (val == '2'){
      Serial.println("relay HIGH : CR2, Arduino pin No.33");  
      digitalWrite(RightFWD, HIGH); // 33
      delay(d);
      digitalWrite(RightFWD, LOW); // 33
      Serial.println("relay LOW : CR2");
    }
    else if (val == '3'){
    Serial.println("relay HIGH : CR3, Arduino pin No.35");  
      digitalWrite(LeftStopMode, HIGH); // 35
      delay(d);
      Serial.println("relay LOW : CR3");
      digitalWrite(LeftStopMode, LOW); // 35
    }
    else if (val == '4'){
    Serial.println("relay HIGH : CR4, Arduino pin No.37");  
      digitalWrite(Leftm0, HIGH); // 37
      delay(d);
      Serial.println("relay LOW : CR4");
      digitalWrite(Leftm0, LOW); // 37
    }
    else if (val == '5'){
    Serial.println("relay HIGH : CR5, Arduino pin No39.");  
      digitalWrite(LeftAlarmReset, HIGH); // 39
      delay(d);
      Serial.println("relay LOW : CR5");
      digitalWrite(LeftAlarmReset, LOW); // 39
    }
    else if (val == '6'){
    Serial.println("relay HIGH : CR6, Arduino pin No.41");  
      digitalWrite(LeftmbFree, HIGH); // 41
      delay(d);
      Serial.println("relay LOW : CR6");
      digitalWrite(LeftmbFree, LOW); // 41
    }

    // 右側リレー個別確認
    else if (val == '7'){
      Serial.println("relay HIGH : CR7, Arduino pin No.43");  
      digitalWrite(LeftFWD, HIGH); // 43
      delay(d);
      Serial.println("relay LOW : CR7");
      digitalWrite(LeftFWD, LOW); // 43
    }
    else if (val == '8'){
      Serial.println("relay HIGH : CR8, Arduino pin No.45");  
      digitalWrite(LeftREV, HIGH); // 45
      delay(d);
      Serial.println("relay LOW : CR8");
      digitalWrite(LeftREV, LOW); // 45
    }
    else if (val == '9'){
      Serial.println("relay HIGH : CR9, Arduino pin No.47");  
      digitalWrite(RightStopMode, HIGH); // 47
      delay(d);
      Serial.println("relay LOW : CR9");
      digitalWrite(RightStopMode, LOW); // 47
    }
    else if (val == 'q'){
      Serial.println("relay HIGH : CR10, Arduino pin No.49");  
      digitalWrite(Rightm0, HIGH); // 49
      delay(d);
      Serial.println("relay LOW : CR10");
      digitalWrite(Rightm0, LOW); // 49
    }
    else if (val == 'w'){
      Serial.println("relay HIGH : CR11, Arduino pin No.51");  
      digitalWrite(RightAlarmReset, HIGH); // 51
      delay(d);
      Serial.println("relay LOW : CR11");
      digitalWrite(RightAlarmReset, LOW); // 51
    }
    else if (val == 'e'){
      Serial.println("relay HIGH : CR12, Arduino pin No.53");  
      digitalWrite(RightmbFree, HIGH); // 53
      delay(d);
      Serial.println("relay LOW : CR12");
      digitalWrite(RightmbFree, LOW); // 53
    }

    else if (val == 'l'){
    Serial.println("Left side relays HIGH");
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
    delay(400);
    Serial.println("Left side relays LOW");  
    }

    else if(val == 'r'){
    Serial.println("Left side relays HIGH");  
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
    delay(400);
    Serial.println("Left side relays LOW");  
    }

    else if (val == 'h'){
    Serial.print("all relays HIGH  ");  
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
    Serial.print("left side Done  ");
    delay(800);

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
    Serial.println("Right side Done");  
    delay(800);
    relayOff();
    }
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(RightREV, OUTPUT);
  pinMode(RightFWD, OUTPUT);
  pinMode(LeftStopMode, OUTPUT);
  pinMode(Leftm0, OUTPUT);
  pinMode(LeftAlarmReset, OUTPUT);
  pinMode(LeftmbFree, OUTPUT);
  
  pinMode(LeftREV, OUTPUT);
  pinMode(LeftFWD, OUTPUT);
  pinMode(RightStopMode, OUTPUT);
  pinMode(Rightm0, OUTPUT);
  pinMode(RightAlarmReset, OUTPUT);
  pinMode(RightmbFree, OUTPUT);
}

void loop() {
  relay();
}