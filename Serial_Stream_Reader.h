/****************************************************
The purpose of this library is to be able to read
information over serial communication and resolve
integers and double precision numbers to their proper
data types. The intention of this file is for use in
a larger project where GPS coordinates would be sent
from a stationary PC to a remote autonmous vehicle.

Anybody attempting to use these class files should
review the test stored in this location.


****************************************************/



#ifndef SERIAL_STREAM_READER
#define SERIAL_STREAM_READER

#include "Arduino.h"
using namespace std;


class Serial_Stream_Reader
{
private:
 
	// Numbers containing more than 9 characters
  // Become less accurate on unreliable.
	const static uint8_t BUFFER_SIZE = 10;
	
	// The char buffer is only used for parsing
	// integers and doubles.
	char buffer_char[BUFFER_SIZE];
  
	int8_t index;
	uint8_t decimal_count;
	String buffer_string;
	int32_t buffer_int;
	double buffer_double;
	bool rdy;
	bool _is_int;
	bool _is_double;
	bool _is_string;

	// Private Helper Functions
	void clear_buffer();
	void resolve();
	void parse_int();
	void parse_double();
	int32_t char_to_int(char);
 
public:

	//Constructor
	Serial_Stream_Reader();
	
	//Adds incoming byte from serial communication
	void add_byte(char);
	
	/*
	 * ready() should be checked after adding a byte.
	 * If the stream has results and is in a ready state, 
	 * and another byte is added, the information
	 * stored in the buffer will be discarded. 
	 * Arriving character that are outside the range of 32-126
	 * (which is most characters on the keyboard) are what trigger the
	 * state change from not ready to ready. The intention is that a
	 * carraige return (enter key on the keyboard) would trigger this state
	 * change.
	 */
	bool ready();
	
	/*
	 * The following boolean functions are used to determine what type
	 * of information is in the information stream.
	 */
	bool is_string();
	bool is_double();
	bool is_int();
	
	/*
	 * The following accessors are used for accessing the the information
	 * in the buffer. Performing a read with any one of the functions 
	 * will cause the buffer to be cleared. Before reading, the data
	 * type should be checked first. However, if the designer feels
	 * that it is prudent, the string buffer can be reliabley read
	 * regardless of the data type.
	 */
	String get_string();
	double get_double();
	int32_t get_int();

};
#endif 
