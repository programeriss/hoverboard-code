int ch9 = 0;
bool ch10 = false;
int ch11 = 0;
int ch12 = 0;

void setup() {
  pinMode(12, INPUT);
  pinMode(11, INPUT);
  pinMode(10, INPUT);
  pinMode(9, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  controller();
}

void controller() {
  double channel[4];
  channel[0] = pulseIn(12, HIGH, 15000);
  channel[1] = pulseIn(11, HIGH, 15000);
  channel[2] = pulseIn(10, HIGH, 15000);
  channel[3] = pulseIn(9, HIGH, 15000);

  ch12 = map((int) channel[0], 1065, 1903, -50, 50);
  if (ch12 >= 25) {
    ch12 = 50;
  }else if (ch12 <= -25) {
    ch12 = -50;
  }else{
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
  
  if (channel[2] > 1500) {
    ch10 = true;
  }else{
    ch10 = false;
  }
  if (channel[2] < 900) {
    ch10 = false;
  }

  
  ch9 = map((int) channel[3], 1065, 1903, -25, 25);
  if (ch9 >= 15) {
    ch9 = 25;
  }else if (ch9 <= -15) {
    ch9 = -25;
  }else{
    ch9 = 0;
  }
  if (channel[3] < 900) {
    ch9 = 0;
  }
  
  
  Serial.print(ch10);
  Serial.print(' ');
  Serial.print(ch11);
  Serial.print(' ');
  Serial.print(ch12);
  Serial.print(' ');
  Serial.println(ch9);
}
