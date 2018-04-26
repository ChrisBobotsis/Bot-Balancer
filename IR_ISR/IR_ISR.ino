
uint32_t user_input = 0x0000;   // This will be used to store the value of the user input (based on NEC data)

uint8_t IR_pin = 2;

int interval = 200; // us


// The following volatile variables will be used within the IR interrupt

volatile bool start_pulse = false, input_ready = false, input_ready_copy=false;          // Assign false to begin with
volatile uint32_t data_bit = 0x0000;              // Assign zero to start with
volatile uint32_t curr_input = 0x0000;      // Add bit stream to this from NEC IR data
volatile long fall_now = 0, fall_prev = 0, duration = 0;  // store time-stamp in us
volatile uint8_t counter = 0;

// End of variables for use in ISR






void setup() {
  // put your setup code here, to run once:

attachInterrupt(digitalPinToInterrupt(IR_pin), IR_NEC, FALLING);
Serial.begin(115200);
sei();
}

void loop() {
  // put your main code here, to run repeatedly:
//Serial.println("nothing...");
cli();
input_ready_copy = input_ready;
sei();

  if(input_ready_copy){
    cli();
    user_input = curr_input;
    Serial.println(user_input,HEX);
    input_ready = false;
    curr_input = 0x0000;
    sei();
    
  }

//Serial.print("Start Pulse:   ");
//Serial.println(start_pulse);


}


void IR_NEC (){

fall_now = micros();
duration = fall_now - fall_prev;            // This saves time from having to calculate the duration in every if statement

  if (start_pulse==true){
      

      if( ( duration <= (/*1687.5*/2240+interval) ) && (duration >= (/*1687.5*/ 2240-interval) ) ) {       // giving an interval of 100us around theoretical value... could also create upper and lower bounds variables so that it doesn't waste time with the computation every time...

        data_bit = 0x01;
      }

      else if ( ( duration <= (1125+interval) ) && ( duration >= (1125-interval)) ) {       // giving an interval of 100us around theoretical value
        
        data_bit = 0x00;
      }

      else  {
        start_pulse = false;
      }

                if (start_pulse!=false){
          
                  curr_input |= data_bit;
                  counter++;
          
                                              if(counter<32){
                                                curr_input<<=1;
                                              }
                                      
                                              else{
                                                start_pulse=false;
                                                input_ready=true;
                                              }
                }

      
    }
    

  else if( (duration <= (13500+interval) ) && ( duration >= (13500-interval) ) ) {

    start_pulse = true;
    counter = 0;
  }
      

  fall_prev = fall_now;
}

