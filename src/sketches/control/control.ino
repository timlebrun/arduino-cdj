#include <SPI.h>

SPISettings spiSettings(16000000, MSBFIRST, SPI_MODE0);

byte pixels[5][9] = {
	{ B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 },
	{ B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 },
	{ B00000000, B00000000, B01000000, B01000000, B01000100, B01000000, B01000000, B01000100, B01000000 },
	{ B10000110, B01010100, B01010100, B01001110, B00110001, B10000101, B00010100, B01010010, B01101010 },
	{ B10000110, B10000010, B10101010, B10101010, B10001010, B10101010, B10101010, B00001010, B00001010 }
};

byte displayPackage[6][18] = {
	{ 0x00, pixels[0][0], pixels[0][0], pixels[0][0], pixels[0][0], pixels[0][0], pixels[0][0], pixels[0][0], pixels[0][0], pixels[0][0], B00000000, 0, 0, 0, 0, 0, 0, 0},
	{ 0x10, pixels[1][0], pixels[1][0], pixels[1][0], pixels[1][0], pixels[1][0], pixels[1][0], pixels[1][0], pixels[1][0], pixels[1][0], B00000000, 0, 0, 0, 0, 0, 0, 0},
	{ 0x20, pixels[2][0], pixels[2][0], pixels[2][0], pixels[2][0], pixels[2][0], pixels[2][0], pixels[2][0], pixels[2][0], pixels[2][0], B00000000, 0, 0, 0, 0, 0, 0, 0},
	{ 0x30, pixels[3][0], pixels[3][0], pixels[3][0], pixels[3][0], pixels[3][0], pixels[3][0], pixels[3][0], pixels[3][0], pixels[3][0], B00000000, 0, 0, 0, 0, 0, 0, 0},
	{ 0x40, pixels[4][0], pixels[4][0], pixels[4][0], pixels[4][0], pixels[4][0], pixels[4][0], pixels[4][0], pixels[4][0], pixels[4][0], B00000000, 0, 0, 0, 0, 0, 0, 0},
	{ 0x50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

byte test[114] = {
	0x00, 0x00, 0x00, 0xee, 0xee, 0xee, 0xee, 0x00, 0xfe, 0xfe, 0xba, 0x00, 0x40, 0xee, 0xee, 0x00, 0x00, 0x00, 0x8a,
	0x10, 0x00, 0x00, 0x00, 0x3f, 0x03, 0x0c, 0x30, 0x00, 0x3f, 0x00, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x29, 0x96,
	0x20, 0x04, 0x10, 0x40, 0x3e, 0x07, 0xaf, 0x3e, 0x50, 0x3f, 0x00, 0xee, 0xee, 0xee, 0x00, 0x00, 0x00, 0x29, 0x08, 
	0x30, 0x56, 0x11, 0x50, 0x6a, 0x57, 0xee, 0x3a, 0x44, 0x6a, 0x00, 0xfe, 0xba, 0x24, 0x00, 0x00, 0x00, 0x29, 0x53,
	0x40, 0x06, 0x80, 0x0a, 0x2a, 0xa8, 0x80, 0xa0, 0xaa, 0xaa, 0x00, 0x3c, 0xee, 0x00, 0x00, 0x00, 0x00, 0x29, 0x29,
	0x50, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0xf0, 0xff, 0x00, 0xd0, 0x77, 0x00, 0x00, 0x00, 0x29, 0x9b
};


void setup() {
	pinMode(LED_BUILTIN, OUTPUT);

	pinMode(SCK, OUTPUT);
	pinMode(MOSI, INPUT);
	pinMode(MISO, OUTPUT);
	pinMode(SS, OUTPUT);

	// SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.attachInterrupt();
	// SPI.setDataMode(SPI_MODE0);qwm:;=	
	SPI.begin();

	Serial.begin(115200);
}

byte checkSum(byte array[], byte size, byte start = 0) {
	int sum;

	for (byte i = start; i < size; i++) sum += array[i];

	return sum % 256;
}

ISR (SPI_STC_vect) {
	// Serial.print(SPDR);
	Serial.print("DATA: ");
	Serial.println(SPDR);
}

void loop() {

	SPI.beginTransaction(spiSettings);
	// digitalWrite(SS, LOW); // Try to invert
	// digitalWrite(LED_BUILTIN, true);
	for (byte i = 0; i < 6; i++)
	{
		byte hehe[19];
		byte package[19];

		byte checksum = checkSum(displayPackage[i], 1, 17);
		for (byte j = 0; i < 18; i++) package[j] = displayPackage[j];
		package[18] = checksum;
		
		SPI.transfer(package, 19);

		// Serial.print(":");
		// for (byte j = 0; i < 18; i++) Serial.print(hehe[i], HEX);
		// Serial.println("");
		
		// send
		/* code */
	}
	
	// digitalWrite(SS, HIGH);
	SPI.endTransaction();
}
