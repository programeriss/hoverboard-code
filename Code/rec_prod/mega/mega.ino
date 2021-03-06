#include <SoftwareSerial9.h>

#define IN1 33 //Uzkurimo rele
#define TX_blade 8 //MOSI Blade
#define RX_blade 39 //MISO Blade
#define TX_L 5 //MOSI Left wheel
#define RX_L 37 //MISO Left wheel
#define TX_R 3 //MOSI Right Wheel
#define RX_R 7 //MISO Right Wheel
#define GND1 22
#define GND2 24
#define GND3 26
#define GND4 28
#define GND5 30
#define GND6 32
#define GND7 34
#define GND8 36
#define GND9 38
#define GND10 40
#define VCC1 31
#define VCC6 41
#define LEDPIN 13

SoftwareSerial9 mySerial_wheel_L(RX_L,TX_L);
SoftwareSerial9 mySerial_wheel_R(RX_R,TX_R);
SoftwareSerial9 mySerial_blade(RX_blade,TX_blade);

bool powerOn = false;
int ch9 = 0;
bool ch10 = false;
int ch11 = 0;
int ch12 = 0;
int pav = 0;
int pavmx = 0;

int real_speed_L = 0;
int real_speed_R = 0;

void setup() {
  Serial.begin(9600);
  mySerial_wheel_L.begin(26315);
  mySerial_wheel_R.begin(26315);
  mySerial_blade.begin(26315);

  pinMode(IN1, OUTPUT);
  digitalWrite(IN1, LOW);

  pinMode(GND1, OUTPUT);
  pinMode(GND2, OUTPUT);
  pinMode(GND3, OUTPUT);
  pinMode(GND4, OUTPUT);
  pinMode(GND5, OUTPUT);
  pinMode(GND6, OUTPUT);
  pinMode(GND7, OUTPUT);
  pinMode(GND8, OUTPUT);
  pinMode(GND9, OUTPUT);
  pinMode(GND10, OUTPUT);
  digitalWrite(GND1, LOW);
  digitalWrite(GND2, LOW);
  digitalWrite(GND3, LOW);
  digitalWrite(GND4, LOW);
  digitalWrite(GND5, LOW);
  digitalWrite(GND6, LOW);
  digitalWrite(GND7, LOW);
  digitalWrite(GND8, LOW);
  digitalWrite(GND9, LOW);
  digitalWrite(GND10, LOW);
  pinMode(VCC1, OUTPUT);
  pinMode(VCC6, OUTPUT);
  digitalWrite(VCC1, HIGH);
  digitalWrite(VCC6, HIGH);

  pinMode(12, INPUT);
  pinMode(11, INPUT);
  pinMode(10, INPUT);
  pinMode(9, INPUT);
}

void loop() {

  controller();
  if (ch10) {
    //Go blade
    if (ch11 == 0) {
      goBlade(0, 85);
    }else{
      int blade_speed = ch11 * 12;
      goBlade(blade_speed, 85);
    }
    
    //Go wheel
    updateSpeed();
    goWheel(real_speed_L, real_speed_R, 85);
  }else{
    goBlade(0, 170);
    goWheel(0, 0, 170);
  }
}

//Suktis bleidui komandos siuntimas!
void goBlade(int sp, int onHoverboard) {
      mySerial_blade.write9(256);
      mySerial_blade.write9(sp & 0xFF);
      mySerial_blade.write9((sp >> 8) & 0xFF);
      mySerial_blade.write9(sp & 0xFF);
      mySerial_blade.write9((sp >> 8) & 0xFF);
      mySerial_blade.write9(onHoverboard);
      delayMicroseconds(300);

    Serial.print("BLADE: ");
    Serial.print("sp: ");
    Serial.print(sp);
    Serial.print(" onHoverboard: ");
    Serial.println(onHoverboard);
}

//Suktis ratams komandu siuntimas!
void goWheel(int sp_L, int sp_R, int onHoverboard) {
    mySerial_wheel_L.write9(256);
    mySerial_wheel_L.write9(sp_L & 0xFF);
    mySerial_wheel_L.write9((sp_L >> 8) & 0xFF);
    mySerial_wheel_L.write9(sp_L & 0xFF);
    mySerial_wheel_L.write9((sp_L >> 8) & 0xFF);
    mySerial_wheel_L.write9(onHoverboard);
    mySerial_wheel_L.write9(88);
    mySerial_wheel_L.write9(88);
    delayMicroseconds(300);
    
    mySerial_wheel_R.write9(256);
    mySerial_wheel_R.write9(sp_R & 0xFF);
    mySerial_wheel_R.write9((sp_R >> 8) & 0xFF);
    mySerial_wheel_R.write9(sp_R & 0xFF);
    mySerial_wheel_R.write9((sp_R >> 8) & 0xFF);
    mySerial_wheel_R.write9(onHoverboard);
    mySerial_wheel_R.write9(88);
    mySerial_wheel_R.write9(88);
    delayMicroseconds(300);

    Serial.print("WHEEL: ");
    Serial.print("sp_L: ");
    Serial.print(sp_L);
    Serial.print(" sp_R: ");
    Serial.print(sp_R);
    Serial.print(" onHoverboard: ");
    Serial.println(onHoverboard);
}

void updateSpeed() {
    //Serial.print(ch12);
    //Serial.print(" ");
    //Serial.println(ch9);
    
    int turnSpeed = ch9 * 40; //Interval: -1000;1000

    int speed_L = ch12 * 36;
    int speed_R = ch12 * 36;
    
    if (turnSpeed > 0) {
      speed_R += turnSpeed * -1;
    }

    if (speed_L > 1800) {
      speed_L = 1800;
    }
    if (speed_L < -1800) {
      speed_L = -1800;
    }
    
    if (turnSpeed < 0) {
      speed_L += turnSpeed;
    }

    int doo = false;
    if (speed_L > 100 && speed_R == 0) {
      speed_R = 100;
      doo = true;
    }
    if (speed_R > 100 && speed_L == 0) {
      speed_L = 100;
      doo = true;
    }

    if (speed_L < -100 && speed_R == 0) {
      speed_R = -100;
      doo = true;
    }
    if (speed_R < -100 && speed_L == 0) {
      speed_L = -100;
      doo = true;
    }

    if (doo) {
     speed_L *= -1;
     speed_R *= -1;
    }
    
    if (speed_R > 1800) {
      speed_R = 1800;
    }
    if (speed_R < -1800) {
      speed_R = -1800;
    }

    if (speed_R > 0 && speed_L < 0) {
      speed_R = 0;
      speed_L = 0;
    }

    if (speed_R < 0 && speed_L > 0) {
      speed_R = 0;
      speed_L = 0;
    }
    
    //Action
    if (speed_L > real_speed_L) {
      real_speed_L += 200;
    }
    if (speed_R > real_speed_R) {
      real_speed_R += 200;
    }

    if (speed_L < real_speed_L) {
      real_speed_L -= 200;
    }
    if (speed_R < real_speed_R) {
      real_speed_R -= 200;
    }
    ///

    if (real_speed_L > 1800) {
      real_speed_L = 1800;
    }
    if (real_speed_L < -1800) {
      real_speed_L = -1800;
    }
    if (real_speed_R > 1800) {
      real_speed_R = 1800;
    }
    if (real_speed_R < -1800) {
      real_speed_R = -1800;
    }

    if (real_speed_L > 200 && real_speed_R <= 200) {
      real_speed_R = 100;
    }
    if (real_speed_R > 200 && real_speed_L <= 200) {
      real_speed_L = 100;
    }
    if (real_speed_L < -200 && real_speed_R >= -200) {
      real_speed_R = -100;
    }
    if (real_speed_R < -200 && real_speed_L >= -200) {
      real_speed_L = -100;
    }
    
    if (real_speed_R > 0 && real_speed_L < 0) {
      real_speed_R = 0;
      real_speed_L = 0;
    }

    if (real_speed_R < 0 && real_speed_L > 0) {
      real_speed_R = 0;
      real_speed_L = 0;
    }

     if (ch12 < 30 && ch12 > -30) {
      if (turnSpeed > 16) {
          real_speed_R = 500;
          real_speed_L = 100;
        return;
      }
      if (turnSpeed < -16) {
          real_speed_R = 100;
          real_speed_L = 500;
        return;
      }
    }
    
    Serial.print("L:");
    Serial.print(real_speed_L);
    Serial.print(" R:");
    Serial.println(real_speed_R);
}


void controller() {
  double channel[4];
  channel[0] = pulseIn(12, HIGH);
  channel[1] = pulseIn(11, HIGH);
  channel[2] = pulseIn(10, HIGH);
  channel[3] = pulseIn(9, HIGH);

  ch12 = map((int) channel[0], 1065, 1903, -50, 57);
  if (ch12 <= 4 && ch12 >= -4) {
    ch12 = 0;
  }
  if (channel[0] < 900) {
    ch12 = 0;
  }
  
  ch11 = map((int) channel[1], 1067, 1907, 0, 100);
  if (ch11 <= 4) {
    ch11 = 0;
  }
  if (channel[1] < 900) {
    ch11 = 0;
  }

  if (channel[2] > 10) {
    if (channel[2] > 1450) {
      ch10 = true;
    }else{
      ch10 = false;
    }
    if (channel[2] < 900) {
      ch10 = false;
    }
  }
  if (ch12 != 0) {
    ch10 = true;
  }

  
  ch9 = map((int) channel[3], 1065, 1903, -24, 25);
  if (ch9 <= 4 && ch9 >= -4) {
    ch9 = 0;
  }
  if (channel[3] < 900) {
    ch9 = 0;
  }
}
