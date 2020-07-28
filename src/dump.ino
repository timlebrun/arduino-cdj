/**
 * Startup dumping script.
 * 
 * Fills an entire buffer with bytes and dump them as soon as its full
 */

#include <CircularBuffer.h>
#include <SPI.h>

// Test circular buffer for dumping
CircularBuffer<byte, 255> buffer;

bool bufferDumped = false;

ISR (SPI_STC_vect) {
	if (buffer.isFull()) return; // Lets hope this does not takes too long
	
	buffer.push(SPDR);
}

void setup (void)
{
	pinMode(SCK, INPUT);
	pinMode(MOSI, INPUT);
	pinMode(MISO, OUTPUT); // useless since were slave
	pinMode(SS, INPUT);
	
	// Set SPI in slave mode ('cause were listening)
	SPCR |= bit (SPE);
	SPI.attachInterrupt();

	Serial.begin(230400); // Start sending stuff to computer
}

void loop () {
	wait(1); // lets not panic
	
	if (!buffer.isFull()) return; // If buffer not full, continue filling
	if (bufferDumped) return; // if buffer already dumped, ignore

	for (byte i = 0; i < buffer.size(); i++) {
		char hexByteString[3];
		sprintf(hexByteString, "%02x ", buffer[i]);
		Serial.print(hexByteString);
	}

	bufferDumped = true;

	Serial.println('');
	Serial.println('Done !');
	Serial.println('');
}
