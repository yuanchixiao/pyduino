// this script is used for the redmud project for calibrating the sensors
// the project started on 20170409
char delimiter =',';


//---------------------below required by module heat_suction_sensor----------------------------------------------------#
#include <OneWire.h>
OneWire  ds(4);  // on pin 10 (a 4.7K resistor is necessary)
// OneWire DS18S20, DS18B20, DS1822 Temperature Example
//
// http://www.pjrc.com/teensy/td_libs_OneWire.html
//
// The DallasTemperature library can do all this work for you!
// http://milesburton.com/Dallas_Temperature_Control_Library

byte heat_suction_sensor_1_addr[8];
byte heat_suction_sensor_2_addr[8];


int  heat_suction_sensor_heat_sw_1= 6;
int  heat_suction_sensor_heat_sw_2= 5;
int  temp_sampling_number =20;
int  temp_sampling_interval_ms=6000;
//int  temp_sampling_number =20;
//int  temp_sampling_interval_ms=100;

//---------------------above required by module heat_suction_sensor----------------------------------------------------#


//---------------------below required by module salinity_humidity_sensor----------------------------------------------------#

#include <dht.h>
dht DHT;
#define DHT22_PIN_1 2
#define DHT22_PIN_2 3
//---------------------above required by module salinity_humidity_sensor----------------------------------------------------#



//-------------------below required by analog digital for moisture ---------------------

static const uint8_t moist_analog_pins[]  = {A1,A2,A3,A4};
int moist_digital_pins[] = {7,8,9,10};
int moist_number_readings=7;
int moist_dummy_readings=3;
int const moist_number_sensors=sizeof(moist_analog_pins);
float moist_data[moist_number_sensors];

//-------------------above required by analog digital for moisture ---------------------

void setup() {
  Serial.begin(9600); // open serial port, set the baud rate as 9600 bps
  pinMode(3, OUTPUT);



//---------------------below required by module analog moisture sensor----------------------------------------------------#

  for (int i=0; i<moist_number_sensors;i++){
      pinMode(moist_digital_pins[i],OUTPUT);
  }
//---------------------above required by module analog moisture sensor----------------------------------------------------#



//---------------------below required by module heat_suction_sensor----------------------------------------------------#
// define the address
// the 
//the below two sensors are the first generation of suction sensor based on heat principle
//it is been used for calibrating coal tailings during 16Nov--17Apr in area51

//const char  addr='28E5A34A8007F';
heat_suction_sensor_1_addr[0]=0x28;
heat_suction_sensor_1_addr[1]=0xE5;
heat_suction_sensor_1_addr[2]=0xA3;
heat_suction_sensor_1_addr[3]=0x4A;
heat_suction_sensor_1_addr[4]=0x08;
heat_suction_sensor_1_addr[5]=0x00;
heat_suction_sensor_1_addr[6]=0x00;
heat_suction_sensor_1_addr[7]=0x7F;

//addr[0]=0x2847A686800B4;
heat_suction_sensor_2_addr[0]=0x28;
heat_suction_sensor_2_addr[1]=0x47;
heat_suction_sensor_2_addr[2]=0xA6;
heat_suction_sensor_2_addr[3]=0x86;
heat_suction_sensor_2_addr[4]=0x08;
heat_suction_sensor_2_addr[5]=0x00;
heat_suction_sensor_2_addr[6]=0x00;
heat_suction_sensor_2_addr[7]=0xB4;

pinMode(heat_suction_sensor_heat_sw_1, OUTPUT);  // switch for heating sucktion sensor 1
pinMode(heat_suction_sensor_heat_sw_2, OUTPUT);  // switch for heating sucktion sensor 2
//---------------------above required by module heat_suction_sensor----------------------------------------------------#

}


//void loop() {
//Serial.print("Soil1,");
//heat_suction_sensor(heat_suction_sensor_1_addr,heat_suction_sensor_heat_sw_1,temp_sampling_number,temp_sampling_interval_ms); 
//delay(2000);
//heat_suction_sensor(heat_suction_sensor_2_addr,heat_suction_sensor_heat_sw_2,temp_sampling_number,temp_sampling_interval_ms); 
//delay(2000);
//read_salinity_humidity_sensor(DHT22_PIN_2);
//delay(2000);
//read_salinity_humidity_sensor(DHT22_PIN_1);
//delay(2000);
//read_analog_moisture_sensor();
//delay(2000);
//Serial.println();
//delay_min(30);
//}

void loop() {
    String content = "";
    char character;
    while(Serial.available()) {
        character = Serial.read();
        content.concat(character);
        delay (10);
    }
    if (content != ""){
        if (content == "All") {
            Serial.print("All");
            Serial.print(seperator);
            read_analog_moisture_sensor();
            delay(1000);
            read_salinity_humidity_sensor(DHT22_PIN_2);
            delay(1000);
            read_salinity_humidity_sensor(DHT22_PIN_1);
            heat_suction_sensor(heat_suction_sensor_1_addr,heat_suction_sensor_heat_sw_1,temp_sampling_number,temp_sampling_interval_ms); 
            delay(1000);
            heat_suction_sensor(heat_suction_sensor_2_addr,heat_suction_sensor_heat_sw_2,temp_sampling_number,temp_sampling_interval_ms); 
            delay(1000);
            heat_suction_sensor(heat_suction_sensor_3_addr,heat_suction_sensor_heat_sw_2,temp_sampling_number,temp_sampling_interval_ms); 
            Serial.println("AllDone");
        }
        else if (content == "SoilMoisture") {
            Serial.print("SoilMoisture");
            Serial.print(seperator);
            read_analog_moisture_sensor();
            Serial.println("SoilMoistureDone");
        }
        else if (content == "SoilSuction") {
            Serial.print("SoilSuction");
            Serial.print(seperator);
            heat_suction_sensor(heat_suction_sensor_1_addr,heat_suction_sensor_heat_sw_1,temp_sampling_number,temp_sampling_interval_ms); 
            heat_suction_sensor(heat_suction_sensor_2_addr,heat_suction_sensor_heat_sw_2,temp_sampling_number,temp_sampling_interval_ms); 
            heat_suction_sensor(heat_suction_sensor_3_addr,heat_suction_sensor_heat_sw_3,temp_sampling_number,temp_sampling_interval_ms); 
            Serial.println("SoilSuctionDone");
        }
        else if (content == "Salinity") {
            Serial.print("Salinity");
            Serial.print(seperator);
            read_salinity_humidity_sensor(DHT22_PIN_2);
            read_salinity_humidity_sensor(DHT22_PIN_1);
            Serial.println("SalinityDone");
        }
        else {
          Serial.println(content);
        }
    } //content != ""

} //void loop




void heat_suction_sensor(byte heat_suction_sensor_addr[8],int heat_sw,int sampling_number, int sampling_interval_ms){

  Serial.print("SucHeat");
  Serial.print(delimiter);
  Serial.print(heat_suction_sensor_addr[1],HEX);
  Serial.print(delimiter);
  Serial.print("Heating");
  Serial.print(delimiter);
//  int heat_sw;
//  byte heat_suction_sensor_addr[8];
  digitalWrite(heat_sw, HIGH);
  for (int i=0; i<=sampling_number; i++)
  {
      delay (sampling_interval_ms);
      read_DS18B20_by_addr(heat_suction_sensor_addr) ;
  }

  Serial.print("Dsping");
  Serial.print(delimiter);
  digitalWrite(heat_sw, LOW);
  for (int i=0; i<=sampling_number; i++)
  {
      delay (sampling_interval_ms);
      read_DS18B20_by_addr(heat_suction_sensor_addr) ;
  }
      //Serial.println();
}  // heat_suction_sensor


void read_salinity_humidity_sensor(int digi_pin){

  Serial.print("SaltRH");
  Serial.print(delimiter);
  Serial.print(digi_pin);
  Serial.print(delimiter);
  int chk1=DHT.read22(digi_pin);
  Serial.print(DHT.temperature);
  Serial.print(delimiter);
  Serial.print(DHT.humidity);
  Serial.print(delimiter);
}


void read_DS18B20_by_addr(byte addr[8]) {
  //byte addr[8];
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
//  byte addr[8];
  float celsius;
        
      if (OneWire::crc8(addr, 7) != addr[7]) {
          Serial.println("CRC is not valid!");
          return;
      }
    
      ds.reset();
      ds.select(addr);
      ds.write(0x44, 1);        // start conversion, with parasite power on at the end
      
      delay(1000);     // maybe 750ms is enough, maybe not
      // we might do a ds.depower() here, but the reset will take care of it.
      
      present = ds.reset();
      ds.select(addr);    
      ds.write(0xBE);         // Read Scratchpad
    
      //Serial.print("  Data = ");
      //Serial.print(present, HEX);
      //Serial.print(" ");
      for ( i = 0; i < 9; i++) {           // we need 9 bytes
        data[i] = ds.read();
        //Serial.print(data[i], HEX);
        //Serial.print(" ");
      }
      //Serial.print(" CRC=");
      //Serial.print(OneWire::crc8(data, 8), HEX);
      //Serial.println();
    
      // Convert the data to actual temperature
      // because the result is a 16 bit signed integer, it should
      // be stored to an "int16_t" type, which is always 16 bits
      // even when compiled on a 32 bit processor.
      int16_t raw = (data[1] << 8) | data[0];
      if (type_s) {
        raw = raw << 3; // 9 bit resolution default
        if (data[7] == 0x10) {
          // "count remain" gives full 12 bit resolution
          raw = (raw & 0xFFF0) + 12 - data[6];
        }
      } else {
        byte cfg = (data[4] & 0x60);
        // at lower res, the low bits are undefined, so let's zero them
        if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
        else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
        else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
        //// default is 12 bit resolution, 750 ms conversion time
      }
      celsius = (float)raw / 16.0;
      //Serial.print(delimiter);
      Serial.print(celsius);
      Serial.write(delimiter);
  
  return;
}



void read_analog_moisture_sensor() {
  // read the input on analog pin 0
  for (int i=0; i<moist_number_sensors;i++){
    moist_data[i]=0;
    digitalWrite(moist_digital_pins[i],HIGH);
    delay(1000);

    for (int j=0;j<moist_dummy_readings;j++){
      analogRead(moist_analog_pins[i]);
      //delay(100);
    }

    for (int j=0;j<moist_number_readings;j++){
      moist_data[i]+=analogRead(moist_analog_pins[i]);
      delay(10);
    }

    moist_data[i]=moist_data[i]/moist_number_readings;
    digitalWrite(moist_digital_pins[i],LOW);
  }
    
    for (int i=0; i<moist_number_sensors;i++)
    {
    Serial.print("Mo");
    Serial.print(delimiter);
    //Serial.print((char)moist_analog_pins[i]);   // how to convert this to strings?
    Serial.print(moist_digital_pins[i]);
    Serial.print(delimiter); 
    Serial.print(moist_data[i]);
    Serial.print(delimiter);

    }
    
    //delay_min(30);
    }





void delay_min(int min){
  for (int i=0;i<min;i++)
  {
    for (int j=0;j<12;j++)
    {
      delay(5000);

    }
  }
}
