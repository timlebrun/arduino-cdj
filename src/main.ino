#include <CircularBuffer.h>
#include <SPI.h>

#include "./structs/controls/jog.h"
#include "./structs/controls/pitch.h"
#include "./structs/controls/button.h"
#include "./structs/controls/release.h"

#include "./configs/cdj800mk2.c"

const byte panelButtonControlsCount = sizeof(panelButtonControls) / sizeof(CDJPanelButtonControl);
bool panelButtonControlStates[panelButtonControlsCount];

// Test circular buffer for dumping
CircularBuffer<byte, 255> circularBuffer;

CircularBuffer<byte, 20> displayBuffer;
CircularBuffer<byte, 19> mainBuffer; // 6 packages of 19 bytes = 114

// @todo maybe make an actual struct for packages ?

bool gotFreshDisplayPackage = false;
volatile byte displayPackage[20]; // Package data from DISPLAY board
byte displayPackageSize = 0;


// Packages from MAIN board (19 bytes - checksum and number)
// Dumb variable names because i want to check i dont just suck at arrays
byte firstMainPackage[19];
byte secondMainPackage[19];
byte thirdMainPackage[19];
byte fourthMainPackage[19];
byte fifthMainPackage[19];
byte sixthMainPackage[19];

bool gotFreshMainPackage = false;
byte mainPackageSize = 0;

// Used for display debug and RE
volatile byte previousPackage[255];

// Actual values (not booleans)
unsigned int jogPosition = 0xffff;
unsigned int jogAcceleration = 0xffff;
unsigned int pitch = 0x00;
byte vinylRelease = 0x00;

SPISettings spiSettings(16000000, MSBFIRST, SPI_MODE2); // Probably not useful since we slave

void setup (void)
{
	pinMode(SCK, INPUT);
	pinMode(MOSI, INPUT);
	pinMode(MISO, OUTPUT); // useless since were slave
	pinMode(SS, INPUT);
	
	// Set SPI in slave mode ('cause were listening)
	SPCR |= bit (SPE);
	// SPI.begin();
	SPI.attachInterrupt();
	// SPI.setDataMode(SPI_MODE2); meh

	Serial.begin (230400); // Start sending stuff to computer

	Serial.println(SCK);
	Serial.println(MOSI);
	Serial.println(MISO);
	Serial.println(SS);

	Serial.print("Number of buttons registered: ");
	Serial.println(panelButtonControlsCount);
}

ISR (SPI_STC_vect) {
	// byte payload = SPDR;

	// grabDisplayBufferByte(SPDR);
	// grabMainBufferByte(SPDR); // Cant do both for now
	mainBuffer.push(SPDR);

}

void grabMainBufferByte(byte payload) {
	// @todo FIND HOW ??
	// maybe check first buffer byte, if counter value then try checksum ?
	// have to find a way to do this OUTSIDE the interrupt
	// if (payload == 0xa6 && mainBuffer.last() == 0x30) mainBuffer.clear();

}

void grabDisplayBufferByte(byte payload) {
	if (payload == 0x10 && displayBuffer.last() == 0x01) displayBuffer.clear();

	displayBuffer.push(payload);
}

void loop () {
	// delay(1);
	noInterrupts();
	// checkDisplayBuffer();
	checkMainBuffer();
	interrupts();

	if (gotFreshMainPackage) {
		Serial.println("");
		Serial.print("[00] "); dumpToSerial(firstMainPackage, 19);
		Serial.print("[10] "); dumpToSerial(secondMainPackage, 19);
		Serial.print("[20] "); dumpToSerial(thirdMainPackage, 19);
		Serial.print("[30] "); dumpToSerial(fourthMainPackage, 19);
		Serial.print("[40] "); dumpToSerial(fifthMainPackage, 19);
		Serial.print("[50] "); dumpToSerial(sixthMainPackage, 19);
		Serial.println("");
		Serial.println("");

		gotFreshMainPackage = false; // Sorry dude :/
		delay(10);

	}

	// 
	// check display buffer
	// check main buffer

	// byte size = mainBuffer.size();
	// byte test[255];

	// for (byte i = 0; i < size; i++) test[i] = mainBuffer[i]; // Store as quick as possible
	// mainBuffer.clear();

	// for (byte i = 0; i < size; i++) {
	// 	char hexByteString[3];
	// 	sprintf(hexByteString, "%02x ", test[i]);
	// 	Serial.print(hexByteString);
	// }

	// Serial.println("");
	// Serial.println(size);
	// Serial.println("");

	// // maybe try something like
	// // stop interrupts
	// // check first byte, if counter value check checksum
	// // if okay store
	// // else continue

	// delay(100);
}

void checkMainBuffer() {
	byte firstByte = mainBuffer.first();

	// Store directly 
	if (firstByte == 0x00) checkAndStoreMainPackage(firstMainPackage);
	if (firstByte == 0x10) checkAndStoreMainPackage(secondMainPackage);
	if (firstByte == 0x20) checkAndStoreMainPackage(thirdMainPackage);
	if (firstByte == 0x30) checkAndStoreMainPackage(fourthMainPackage);
	if (firstByte == 0x40) checkAndStoreMainPackage(fifthMainPackage);
	if (firstByte == 0x50) checkAndStoreMainPackage(sixthMainPackage);

	return;
}

void checkAndStoreMainPackage(byte package[]) {
	// We already kno here the first byte is okay
	if (!mainBufferIsValidPackage()) return;

	// Starting at 1 to not store the index number (but keep package starting at 0)
	for (byte i = 0; i < mainBuffer.size(); i++) package[i] = mainBuffer[i];

	gotFreshMainPackage = true;
}

bool mainBufferIsValidPackage() {
	byte lastByte = mainBuffer.last();
	byte checkSum = getMainCheckSum(
		mainBuffer[1],
		mainBuffer[2],
		mainBuffer[3],
		mainBuffer[4],
		mainBuffer[5],
		mainBuffer[6],
		mainBuffer[7],
		mainBuffer[8],
		mainBuffer[9],
		mainBuffer[10],
		mainBuffer[11],
		mainBuffer[12],
		mainBuffer[13],
		mainBuffer[14],
		mainBuffer[15],
		mainBuffer[16],
		mainBuffer[17] // Grmbl not in C
	);

	// Serial.println("CHECK");
	// Serial.println(lastByte);
	// Serial.println(checkSum);
	// Serial.println("");

	return lastByte == checkSum;
}

void checkDisplayBuffer() {
	if (displayBuffer[0] != 0x01) return;
	if (displayBuffer[1] != 0x10) return;

	displayPackageSize = 0; // Reset size
	for (byte i = 0; i < displayBuffer.size(); i++) {
		displayPackage[i] = displayBuffer[i];
		displayPackageSize++; // Increment size
	}

	gotFreshDisplayPackage = true;

	return;
}

byte getMainCheckSum(byte b1, byte b2, byte b3, byte b4, byte b5, byte b6, byte b7, byte b8, byte b9, byte b10, byte b11, byte b12, byte b13, byte b14, byte b15, byte b16, byte b17) {
	return (b1 + b2 + b3 + b4 + b5 + b6 + b7 + b8 + b9 + b10 + b11 + b12 + b13 + b14 + b15 + b16 + b17) % 256;
}

void dumpCurrentDisplayPackageToSerial() {
	for (byte i = 0; i < displayPackageSize; i++) {
		char hexByteString[3];
		sprintf(hexByteString, "%02x ", displayPackage[i]);
		Serial.print(hexByteString);
	}

	char bytesCount[16];
	sprintf(bytesCount, "(%d bytes)", displayPackageSize);
	Serial.println(bytesCount);
}

void dumpToSerial(byte bytes[], byte size) {
	for (byte i = 0; i < size; i++) {
		char hexByteString[3];
		sprintf(hexByteString, "%02x ", bytes[i]);
		Serial.print(hexByteString);
	}

	Serial.println("");
}

void dumpChangesToPackage() {
	bool interesting = false;
	
	// Debug literal bytes
	for (byte j = 0; j < displayPackageSize; j++) {
		if (j == 1) continue; // Ignore 1 because i don't know yet
		// if (j == 1) continue; // Ignore 2 because i don't know either
		if (displayPackage[j] == previousPackage[j]) continue;

		interesting = true;
		char byteChange[16];
		sprintf(byteChange, "[%02x] %02x => %02x ", j, previousPackage[j], displayPackage[j]);
		Serial.print(byteChange);

		previousPackage[j] = displayPackage[j]; // Store new value in previousPackage
	}
	
	if (interesting) {
		char bytesCount[16];
		sprintf(bytesCount, "(%d bytes)", displayPackageSize);
		Serial.println(bytesCount);
	}
}

void matrix() {
	// Check all buttons
	for (byte i = 0; i < panelButtonControlsCount; i++) {
		const CDJPanelButtonControl *panelControl = &panelButtonControls[i]; // Get a pointer fo easier readability
		bool packageButtonState = displayPackage[panelControl->inputByte] & panelControl->inputMask;

		if (packageButtonState != panelButtonControlStates[i]) {
			panelButtonControlStates[i] = packageButtonState;
			if (packageButtonState) { Serial.print(panelControl->label); Serial.println(" pressed"); }
			else { Serial.print(panelControl->label); Serial.println(" released"); }
		}
	}

	unsigned int packageJogPosition = displayPackage[jogControl.inputPositionByteHigh] << 8 | displayPackage[jogControl.inputPositionByteLow];
	if (packageJogPosition != jogPosition) {
		jogPosition = packageJogPosition;
		Serial.print("JOG moved to "); Serial.println(jogPosition);
	}

	unsigned int packageJogAcceleration = displayPackage[jogControl.inputAccelerationByteHigh] << 8 | displayPackage[jogControl.inputAccelerationByteLow];
	if (packageJogAcceleration != jogAcceleration) {
		jogAcceleration = packageJogAcceleration;
		Serial.print("JOG accelerated to "); Serial.println(jogAcceleration);
	}

	unsigned int packagePitch = displayPackage[pitchControl.inputByteHigh] << 8 | displayPackage[pitchControl.inputByteLow];
	if (packagePitch != pitch) {
		pitch = packagePitch;
		Serial.print("PITCH changed to "); Serial.println(pitch);
	}

	byte packageVinylRelease = displayPackage[releaseControl.inputByte];
	if (packageVinylRelease != vinylRelease) {
		vinylRelease = packageVinylRelease;
		Serial.print("VINYLRELEASE changed to "); Serial.println(vinylRelease);
	}
}
