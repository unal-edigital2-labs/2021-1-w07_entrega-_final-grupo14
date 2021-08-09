#include <Wire.h>
#include <SoftwareSerial.h>

#define OV7670_I2C_ADDRESS 0x21 /*Dirección del bus i2c para ña camara*/

SoftwareSerial miBT(10, 11);  // Bluetooth
//SoftwareSerial Fpga1(8,9);    // Serial para matrices G6(TX) - E6(RX)
//SoftwareSerial Fpga2(6,7);    // Serial para ordenes H1(TX)  - G1(RX)

char dato1=0;
char dato2=0;
char dato3=0; 
int ENA_A = 5;
int ENA_B = 6;


void setup() {

  pinMode(ENA_A,OUTPUT);
  pinMode(ENA_B,OUTPUT);
  
  //Puertos seriales para comunicacion
  miBT.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial.begin(9600);
  delay(10);
  Wire.begin();  
  set_cam_RGB565_QCIF();
  delay(100);
  get_cam_register();
 
  
}

void loop(){

  analogWrite(ENA_A,160);
  analogWrite(ENA_B,160);
  if(miBT.available()){
    dato1 = miBT.read();
    Serial3.write(dato1);
    
    }

  if(Serial3.available()){
    dato3 = Serial3.read();
    miBT.write(dato3);
    }
  
  if(Serial2.available()){
    dato2 = Serial2.read();
    miBT.write(dato2);
    }
  
  
  delay(100);
}

void set_cam_RGB565_QCIF(){
   
  OV7670_write(0x12, 0x80);

delay(100);

 OV7670_write(0xFF, 0xF0); 
 OV7670_write(0x12, 0x0C);  //COM7: Set QCIF and RGB
 //OV7670_write(0x12, 0x04);  //COM7: Set RGB
 OV7670_write(0x11, 0xC0);       //CLKR: Set internal clock to use external clock
 //OV7670_write(0x11, 0x80);    
 OV7670_write(0x0C, 0x08);       //COM3: Enable Scaler
 //OV7670_write(0x0C, 0x00);
 OV7670_write(0x3E, 0x00);
 //OV7670_write(0x3E, 0x1B);   //Enable Scaling for Predefined Formats using COM14
 OV7670_write(0x40,0xD0);      //COM15: Set RGB 565
 OV7670_write(0x8C,0x00);
 
 //Color Bar
 //OV7670_write(0x42, 0x08); 
 //OV7670_write(0x12, 0x0E);

 //Registros Mágicos 
 OV7670_write(0x3A,0x04);
 OV7670_write(0x14,0x18);
 OV7670_write(0x4F,0xb3);//b3 ---- verde
 OV7670_write(0x50,0xb3);//b3 ---- rosado 
 OV7670_write(0x51,0x00);//00  
 OV7670_write(0x52,0x3d);//3d
 OV7670_write(0x53,0xa7);//a7 ---- fucsia  
 OV7670_write(0x54,0xe4);//e4 ---- amarillo  
 OV7670_write(0x58,0x9E);  
 OV7670_write(0x3D,0xC0);  
 OV7670_write(0x17,0x14);  
 OV7670_write(0x18,0x02);  
 OV7670_write(0x32,0x80);
 OV7670_write(0x19,0x03);
 OV7670_write(0x1A,0x7B);
 OV7670_write(0x03,0x0A);
 OV7670_write(0x0F,0x41);
 OV7670_write(0x32,0x80);
 OV7670_write(0x1E,0x00);
 OV7670_write(0x33,0x0B);
 OV7670_write(0x3C,0x78);
 /////////////////////////
 OV7670_write(0x69,0x0B);
 OV7670_write(0x55, 0x00);// Brightness control
 OV7670_write(0x56, 0x7f);// Contrast control 40
 OV7670_write(0x57, 0xFF);// Contrast center 80

 OV7670_write(0x6c, 0x0a);//0a
 OV7670_write(0x6d, 0x55);//55
 OV7670_write(0x6e, 0x11);//11
 OV7670_write(0x6f, 0x9f);//9f
 ////////////////////////
 OV7670_write(0x74,0x00);
 OV7670_write(0xB0,0x84);
 OV7670_write(0xB1,0x0C);
 OV7670_write(0xB2,0x0E);
 OV7670_write(0xB3,0x80);    
         

}



void get_cam_register(){
  Serial.println(get_register_value(0x12), HEX); //COM7
  Serial.println(get_register_value(0x11), HEX); //COM7
  Serial.println(get_register_value(0x40), HEX); //COM15
  Serial.println(get_register_value(0x0C), HEX); //COM3
  Serial.println(get_register_value(0x42), HEX); //
  Serial.println(get_register_value(0x14), HEX); 

  Serial.println(get_register_value(0x17), HEX); 
  Serial.println(get_register_value(0x18), HEX); 

}


int OV7670_write(int reg_address, byte data){
  return I2C_write(reg_address, &data, 1);
 }

 

int I2C_write(int start, const byte *pData, int size){
 //  Serial.println ("I2C init");   
    int n,error;
    Wire.beginTransmission(OV7670_I2C_ADDRESS);
    n = Wire.write(start);
    if(n != 1){
      Serial.println ("I2C ERROR WRITING START ADDRESS");   
      return 1;
    }
    n = Wire.write(pData, size);
    if(n != size){
      Serial.println( "I2C ERROR WRITING DATA");
      return 1;
    }
    error = Wire.endTransmission(true);
    if(error != 0){
      Serial.println( String(error));
      return 1;
    }
    Serial.println ("WRITE OK");
    return 0;
 }

 

byte get_register_value(int register_address){
  byte data = 0;
   Serial.println ("I2C read");   
  Wire.beginTransmission(OV7670_I2C_ADDRESS);
  Wire.write(register_address);
  Wire.endTransmission();
  Wire.requestFrom(OV7670_I2C_ADDRESS,1);
  while(Wire.available()<1);
  data = Wire.read();
  return data;
}
