#define IN1 11 //Uzkurimo rele

bool ch10 = false;
bool powerOn = false;
void setup() {
  Serial.begin(9600);
  
  pinMode(IN1, OUTPUT);
  digitalWrite(IN1, LOW);
  
  pinMode(10, INPUT);
}

void loop() {

  controller();

  if (powerOn == false && ch10 == true) {
    PowerON();
  }
  if (powerOn == true && ch10 == false) {
    PowerOFF();
  }
}

//Energija ON i motinine
void PowerON() {
   powerOn = true;
   Serial.println("Ijungiama elektros grandine ir paleidziama motinine");
   delay(300);
   digitalWrite(IN1, HIGH);
   delay(700);
   digitalWrite(IN1, LOW);
   delay(300);
}

//Energija OFF i motinine
void PowerOFF() {
   powerOn = false;
   Serial.println("Nutraukiama energijos grandine");
   digitalWrite(IN1, HIGH);
   delay(500);
   digitalWrite(IN1, LOW);
   delay(2000);
}

void controller() {
  double channel[4];
  channel[2] = pulseIn(10, HIGH);

  if (channel[2] > 10) {
    if (channel[2] > 1500) {
      ch10 = true;
    }else{
      ch10 = false;
    }
    if (channel[2] < 900) {
      ch10 = false;
    }
  }
}
