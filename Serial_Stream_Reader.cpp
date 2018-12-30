#include "Serial_Stream_Reader.h"
#include "Arduino.h"


/*******************************************************
 * Constructor:
 *  Initializes local varialbes
 ******************************************************/

Serial_Stream_Reader::Serial_Stream_Reader(){
  decimal_count   = 0;
  index           = 0;
  buffer_string   = "";
  buffer_int      = 0;
  buffer_double   = 0.0;
  rdy             = false;
  _is_int         = true;
  _is_double      = true;
  _is_string      = true;
}




void Serial_Stream_Reader::add_byte(char new_byte) {
  // If more bytes are beeing added after the data is ready, 
  // then clear the buffer before adding more. 
  if(rdy){
    clear_buffer();
  }
  
  if((index < BUFFER_SIZE) && ((new_byte > 0x2F && new_byte < 0x3A) || (index ==0 && new_byte == 0x2D) || (new_byte == 0x2E && decimal_count == 0))){
    buffer_char[index] = new_byte;
    index++;
    if(new_byte == 0x2E){
      decimal_count++;
      _is_int = false;
    }
  } else if(new_byte > 20 && new_byte < 0x7F) {
    _is_int         = false;
    _is_double      = false;
  }
  
  if(new_byte > 20 && new_byte < 0x7F){
    buffer_string = buffer_string + new_byte;
  } else {
    resolve();
    rdy = true;
  }
  
}


/*******************************************************
 * Boolean Functions:
 *  Used for checking the status of the incoming
 *  information.
 ******************************************************/
bool Serial_Stream_Reader::ready() {
  return rdy;
}

bool Serial_Stream_Reader::is_string(){
  if(rdy) {
    return _is_string;
  } else {
    return false;
  }
}

bool Serial_Stream_Reader::is_double(){
  if(rdy) {
    return _is_double;
  } else {
    return false;
  }
}

bool Serial_Stream_Reader::is_int(){
  if(rdy) {
    return _is_int;
  } else {
    return false;
  }
}


/*******************************************************
 * Getters:
 *   Reading information returns the info and resets the 
 *   buffer.
 ******************************************************/
String Serial_Stream_Reader::get_string(){
  String temp = buffer_string;
  rdy = false;
  clear_buffer();
  return temp;
}

double Serial_Stream_Reader::get_double(){
  double temp = buffer_double;
  rdy = false;
  clear_buffer();
  return temp;
}

int32_t Serial_Stream_Reader::get_int(){
  int32_t temp = buffer_int;
  rdy = false;
  clear_buffer();
  return temp;
}

/*******************************************************
 * Private Helper Functions:
 ******************************************************/

void Serial_Stream_Reader::resolve() {
  if(_is_int && index > 0){
    _is_double = false;
    _is_string = false;
    parse_int(); 
  } else if(_is_double && index > 0) {
    _is_string = false;
    parse_double();
  }
}


void Serial_Stream_Reader::parse_int(){
  int32_t sum = 0;
  int8_t num_digits = 0;
  index--;
  num_digits = index;
  while(index > -1){
    if(buffer_char[index] == 0x2D){
      // if a "-" is encountered, then turn the number into a negative number.
      // This should only occur at index 0;
      sum *= -1;
    } else {
      sum += ((int32_t)ceil(pow(10,num_digits - index)))*char_to_int(buffer_char[index]);
    }
    index--;
  }
  buffer_int = sum;
}


void Serial_Stream_Reader::parse_double(){
  double sum = 0;
  int8_t num_digits = 0;
  int8_t point_index = 0;
  int8_t offset = 0; 
  index--;
  num_digits = index;
  //Locate the decimal, store location and zero out location;
  while(index > -1){
    if(buffer_char[index] == 0x2E){
      buffer_char[index] = 0x0;
      point_index = index;
      break;
    }
    index--;
  }
  offset = num_digits - point_index - 1;
  index++;
  while(index < BUFFER_SIZE && buffer_char[index] != 0x0){
    sum += ((double)char_to_int(buffer_char[index]))/((double)ceil(pow(10,index - point_index)));
    index++;
  }
  index = point_index;
  parse_int();
  sum += ((double)abs(buffer_int));
  if(buffer_int < 0){
    sum *= -1;
  }
  buffer_double = sum;
}


int32_t Serial_Stream_Reader::char_to_int(char value) {
  switch(value){
    case 0x30:
      return 0;
    case 0x31:
      return 1;
    case 0x32:
      return 2;
    case 0x33:
      return 3;
    case 0x34:
      return 4;
    case 0x35:
      return 5;
    case 0x36:
      return 6;
    case 0x37:
      return 7;
    case 0x38:
      return 8;
    case 0x39:
      return 9;
  }
}


void Serial_Stream_Reader::clear_buffer(){
  decimal_count   = 0;
  index           = 0;
  buffer_string   = "";
  buffer_int      = 0;
  buffer_double   = 0.0;
  rdy             = false;
  _is_int         = true;
  _is_double      = true;
  _is_string      = true;
  for(int i = 0; i < BUFFER_SIZE; i++){
    buffer_char[i] = 0x0;
  }
  rdy = false;
}

