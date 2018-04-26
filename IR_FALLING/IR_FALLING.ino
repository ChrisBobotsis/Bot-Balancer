/* This code is to test an IR emitter detector pair and produce the timings for it
 *  
 *  Written by: Chris Bobotsis
 
 */


int IR_pin = 2;

volatile bool logic_flag = false;

long duration = 0, lastTime = 0;


bool currentState = HIGH,lastState = HIGH,temp = HIGH;

bool firstPulse = true;



void setup() {
  // put your setup code here, to run once:

//pinMode(IR_pin,INPUT);

Serial.begin(115200);

attachInterrupt(digitalPinToInterrupt(IR_pin), Logic_Change, FALLING);

Serial.print("No Button Pressed, Logic Level:    ");
Serial.println(digitalRead(2));
}

void loop() {
  // put your main code here, to run repeatedly:

if (logic_flag == true) {
  
  cli();
  logic_flag = false;
  sei();

  duration = micros() - lastTime;

  lastTime = micros();

  currentState = !currentState;
  lastState = !currentState;  
  
  Serial.print(currentState);
  Serial.print("    ");
  Serial.println(duration);
  
}




}

void Logic_Change() {

  logic_flag = true;
  
}

