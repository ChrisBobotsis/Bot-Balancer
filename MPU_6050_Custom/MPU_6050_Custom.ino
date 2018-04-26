#include<Wire.h>


//A4 (SDA), A5 (SCL) Uno
//

class MPU_6050  {

  //Private
      
      int Refresh_Rate_us;       
      
  public:

      double accel_x;
      double accel_y;
      double accel_z;
      double gyro_x;
      double gyro_y;
      double gyro_z;
      double temperature;  

      double accel_x_offset;
      double accel_y_offset;
      double accel_z_offset;
      double gyro_x_offset;
      double gyro_y_offset;
      double gyro_z_offset;

      double x_angle;
      double y_angle;
      double z_angle;

      

      // Member Functions //

      MPU_6050 ();      // This is for the constructor

      setRefresh_Rate_Hz(int);
      getRefresh_Rate_us(){return Refresh_Rate_us;}
      setOffsets();
      setToDegrees();
  
};

MPU_6050:: MPU_6050 (){       // Default constructor

      accel_x = 0;
      accel_y = 0;
      accel_z = 0;
      gyro_x = 0;
      gyro_y = 0;
      gyro_z = 0;
      temperature = 0;

      accel_x_offset = 0;
      accel_y_offset = 0;
      accel_z_offset = 0;
      gyro_x_offset = 0;
      gyro_y_offset = 0;
      gyro_z_offset = 0;

      Refresh_Rate_us = 0;

      x_angle = 0;
      y_angle = 0;
      z_angle = 0;
}

MPU_6050::setRefresh_Rate_Hz(int rate_Hz) {

  Refresh_Rate_us = (1000000/rate_Hz);    // Careful when doing this calculation so that it doesn't round down to zero
  
}

MPU_6050::setOffsets(){

  gyro_x -= gyro_x_offset;
  gyro_y -= gyro_y_offset;
  gyro_z -= gyro_z_offset;
  accel_x -= accel_x_offset;
  accel_y -= accel_y_offset;
  accel_z -= accel_z_offset;
  
}

MPU_6050::setToDegrees(){

  
}

//::: Declaring my Object Type ::::::::
  
  MPU_6050 Gyro_Accel;


byte gyro_config = B00001000;   // setting full scale (FS_SCL) to +/- 500, meaning a value of 65.5 will be given if the axis is moving at 1 degree/second
byte accel_config = B00010000;   // setting scale to +/- 8g    (an output of of 4096 per 1 g??
byte MPU6050_addr = B1101000;   // address of mpu6050 slave device - only 7 bits long

/*
#define gyro_config 0b00001000      // setting full scale (FS_SCL) to +/- 500, meaning a value of 65.5 will be given if the axis is moving at 1 degree/second
#define accel_config 0b00010000     // setting scale to +/- 8g    (an output of of 4096 per 1 g??

#define MPU6050_addr 0b1101000      // address of mpu6050 slave device
*/


long t_now_gyro = 0, t_last_gyro = 0, duration = 0;



void setup() {
  // put your setup code here, to run once:


      //:: Setting up Serial Monitor ::
      Serial.begin(115200);

      //Serial.println("Setup");
      // :: Configuring Registers ::
      writeRegister( MPU6050_addr, 0x6B, 0x00);             // Wake up the MPU6050
      writeRegister( MPU6050_addr, 0x1B, gyro_config);      // Configure Gyroscope
      writeRegister( MPU6050_addr, 0x1C, accel_config);     // COnfigure Accelerometer

      //Serial.println("Config");
      //:: Calibrating Values for Offset ::
    for (int i = 0; i<1000; i++){
    
      updateGyro_Accel_Values( MPU6050_addr, 0x3B ,&Gyro_Accel);
      //Serial.println("Update");
      Gyro_Accel.gyro_x_offset += Gyro_Accel.gyro_x;
      Gyro_Accel.gyro_y_offset += Gyro_Accel.gyro_y;
      Gyro_Accel.gyro_z_offset += Gyro_Accel.gyro_z;
      Gyro_Accel.accel_x_offset += Gyro_Accel.accel_x;
      Gyro_Accel.accel_y_offset += Gyro_Accel.accel_y;
      Gyro_Accel.accel_z_offset += Gyro_Accel.accel_z;
      
      
    }

          Gyro_Accel.gyro_x_offset /= 1000;
          Gyro_Accel.gyro_y_offset /= 1000;
          Gyro_Accel.gyro_z_offset /= 1000;
          Gyro_Accel.accel_x_offset /= 1000;
          Gyro_Accel.accel_y_offset /= 1000;
          Gyro_Accel.accel_z_offset /= 1000;
          
Serial.println("Loop");

      // :: Setting up LED as Visual Aid
      pinMode(13,OUTPUT);       // This is the built-in LED
      digitalWrite(13,HIGH);    // This a visual aid to know that it is now safe to move the sensor

      //
      t_last_gyro = micros();
}

void loop() {
  // put your main code here, to run repeatedly:


      //:: Updating Sensor Values in Object & Setting Offsets Based on Calibration
      updateGyro_Accel_Values( MPU6050_addr, 0x3B, &Gyro_Accel);
      Gyro_Accel.setOffsets();




      t_now_gyro = micros();

      duration = t_now_gyro-t_last_gyro;

      Gyro_Accel.x_angle += (double)(Gyro_Accel.gyro_x*duration)/(1000000*65.5);
      Gyro_Accel.y_angle += (double)(Gyro_Accel.gyro_y*duration)/(1000000*65.5);
      Gyro_Accel.z_angle += (double)(Gyro_Accel.gyro_z*duration)/(1000000*65.5);

      t_last_gyro = t_now_gyro;
      
      //*
      Serial.print("X Angle:   ");
      Serial.print(Gyro_Accel.x_angle);
      Serial.print("   Y Angle:   ");
      Serial.print(Gyro_Accel.y_angle);
      Serial.print("   Z Angle:   ");
      Serial.println(Gyro_Accel.z_angle);
      //*/

      delayMicroseconds(1000);

}





inline void writeRegister( uint8_t SlaveAddr, uint8_t RegisterAddr, uint8_t RegisterVal) {

  Wire.beginTransmission(MPU6050_addr);    // this allows us to communicate with the device
  Wire.write(RegisterAddr);                 // point to the register we wish to write to
  Wire.write(RegisterVal);                  // write to the register
  Wire.endTransmission();                   // end transmission
  
}

inline void updateGyro_Accel_Values( byte SlaveAddr, byte StartRegisterAddr, MPU_6050 * Sensor) {
  
  Wire.beginTransmission(SlaveAddr);          // begin transmission with MPU6050
  Wire.write(StartRegisterAddr);              // set starting address to receive data from
  Wire.endTransmission();
  Wire.requestFrom(SlaveAddr,14);

  //Serial.println("request");
  //while(true){Serial.println(Wire.available());};  
  while(Wire.available()<14);                   // wait for all 14 bytes to be recieved

  //Serial.println("available");
  Sensor->accel_x = (Wire.read()<<8)|Wire.read();    // first wire.read() fills the first 8 bits and the we bit shift right and add the next 8 bits
  Sensor->accel_y = (Wire.read()<<8)|Wire.read();
  Sensor->accel_z = (Wire.read()<<8)|Wire.read();

  Sensor->temperature = (Wire.read()<<8)|Wire.read();

  Sensor->gyro_x = (Wire.read()<<8)|Wire.read();
  //Serial.print("Gx:   "); Serial.println(Sensor->gyro_x);
  Sensor->gyro_y = (Wire.read()<<8)|Wire.read();
  //Serial.print("Gy:   "); Serial.println(Sensor->gyro_y);
  Sensor->gyro_z = (Wire.read()<<8)|Wire.read();
  //Serial.print("Gz:   "); Serial.println(Sensor->gyro_z);

  //Wire.endTransmission();
  
}

