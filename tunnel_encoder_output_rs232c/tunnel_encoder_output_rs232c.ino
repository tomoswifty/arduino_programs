#include <SoftwareSerial.h>
#define speedOutL 5
#define speedOutR 6
// any digital pin is possible, analog hasn't been tested yet
const byte RX = 7;
const byte TX = 8;
SoftwareSerial RS232C(RX, TX); // RX, TX
//エンコーダ読み取り変数
unsigned long T_Left_us, T_Right_us; //計測したパルス幅 [時間 us]
double N_Left_rpm, N_Right_rpm; //回転数[rpm]
double v_mps; //速度[m/s]
double vl_mps; //速度[m/s]
double vr_mps; //速度[m/s]
double v_kmph; //速度[km/h]

double dl_m = 0 ;
double dr_m = 0 ;


void setup() {
  // initialize serial ports:
  Serial.begin(9600); // RX TX 0
  RS232C.begin(9600);
  Serial.println("Starting ...");
  /*moter encoder*/
  pinMode(speedOutL, INPUT);
  pinMode(speedOutR, INPUT);
  //Interrupt
  pinMode(speedOutL,INPUT_PULLUP);
  pinMode(speedOutR,INPUT_PULLUP);

}
void loop() {
  encoder();
  printer();
  // message send through RS232
  RS232C.write(2);
  
  RS232C.print(vl_mps); // left wheel speed
  Serial.print(vl_mps); 

  RS232C.print(",");
  Serial.print(",");

  RS232C.print(dl_m); // left wheel travel distance
  Serial.print(dl_m);
  
  RS232C.print(",");
  Serial.print(",");

  RS232C.print(vr_mps); // right wheel speed
  Serial.print(vr_mps);
  
  RS232C.print(",");
  Serial.print(",");

  RS232C.print(dr_m); // right wheel travel distance
  Serial.print(dr_m);

  RS232C.print(",");
  Serial.print(",");

  double lidar = 0.00;
  RS232C.print(lidar); // space for lidar sensor 
  Serial.println(lidar);
  
  RS232C.write(3);
 // delay(200);
}
void encoder(){
  /*LOWの0.2ms = 200 usを追加  //100rpm:598000;//4000rpm:300+200; 200rpm:300000; */
  T_Left_us = pulseIn(speedOutL, HIGH,900) + 200 ;
  // T_Left_us = 500 ;
  if(T_Left_us <= 20000 && T_Left_us >= 500){
    T_Left_us = T_Left_us ;
    N_Left_rpm = 2 / (T_Left_us * 1e-6); //回転数 N[rpm]
  }
  else{
    T_Left_us = 0;
    N_Left_rpm = 0;
  }

  T_Right_us = pulseIn(speedOutR, HIGH,900) + 200 ;
  // T_Right_us = 500 ;
  if(T_Right_us <= 20000 && T_Right_us >= 500){
    T_Right_us = T_Right_us ;
    N_Right_rpm = 2 / (T_Right_us * 1e-6); //回転数 N[rpm]
  }
  else{
    T_Right_us = 0;
    N_Right_rpm = 0;
  }

  //速度計算
  v_mps = 0.1105 / 100 * (N_Left_rpm + N_Right_rpm) / 2 * 3.14 / 30 ; //速度 [m/s]
  vl_mps = 0.1105 / 100 * N_Left_rpm * 3.14 / 30; //速度 [m/s]
  vr_mps = 0.1105 / 100 * N_Right_rpm * 3.14 / 30; //速度 [m/s]
  v_kmph = v_mps / 1000 * (60 * 60) ; //速度 [km/h]

  //距離計算
  dl_m = dl_m + vl_mps * (T_Left_us * 1e-3) / 3.14; 
  dr_m = dr_m + vr_mps * (T_Right_us * 1e-3) / 3.14; //3.14は誤差修正
}
void printer(){
  //print encoder result
  Serial.print(T_Left_us);
  Serial.print(" [us]   "); //Left Pulse width
  Serial.print(N_Left_rpm,0);
  Serial.print(" [rpm] ");
  Serial.print(T_Right_us);
  Serial.print(" [us]   "); //Left Pulse width
  Serial.print(N_Right_rpm,0);
  Serial.print(" [rpm]   ");

  // T_Left_us;
  // N_Left_rpm;
  // T_Right_us;
  // N_Right_rpm;

  Serial.print(vl_mps,3); //左速度 [m/s]
  Serial.print(" [m/s]   ");
  Serial.print(",");
  Serial.print(vr_mps,3); //右速度 [m/s]
  Serial.print(" [m/s]   ");
  Serial.print(",");
  // Serial.print(v_kmph,3); //速度 [km/h]
  // Serial.print(" [km/h]   ");
  Serial.print(dl_m); // 左距離
  Serial.print(" [m] ");
  Serial.print(",");
  Serial.print(dr_m); //右距離
  Serial.print(" [m] ");
  Serial.print(",");
  Serial.println("  ");//改行用
}