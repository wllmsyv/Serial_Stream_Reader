/******************************************************
 * Test Bench: Arduino Mega 2650, XBee-PRO 900
 ******************************************************/
 
#include "Serial_Stream_Reader.h"


Serial_Stream_Reader xbee;
double my_double;
int32_t my_int;
String my_string;
long time1 = 0.0;
long time2 = 0.0;


void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
  Serial.begin(115200);
}

void loop() {

}

void serialEvent1(){
  while (Serial1.available() > 0) {
    xbee.add_byte((char)Serial1.read());
    if(xbee.ready()){
      if(xbee.is_double()) {
        time1= micros();
        my_double = xbee.get_double();
        time2= micros();
        Serial.print("Completed in: "); Serial.print(time2-time1);Serial.print(" us, ");
        Serial.print("Is Double: ");Serial.println(my_double);
      } else if(xbee.is_int()) {
        time1= micros();
        my_int = xbee.get_int();
        time2= micros();
        Serial.print("Completed in: "); Serial.print(time2-time1);Serial.print(" us, ");
        Serial.print("Is Int: ");Serial.println(my_int);
      } else if(xbee.is_string()) {
        time1= micros();
        my_string = xbee.get_string();
        time2= micros();
        Serial.print("Completed in: "); Serial.print(time2-time1);Serial.print(" us, ");
        Serial.print("Is String: ");Serial.println(my_string);
      } else {
        Serial.println("Something Failed");
      } 
    }
  }
}
