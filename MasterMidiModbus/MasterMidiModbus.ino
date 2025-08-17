#include "MIDIUSB.h"        
#define MB_SERIAL Serial1
#include <SimpleModbusMaster.h>
#define DEF_TO_STR(A) #A

// modbus
#define connection_error_led LED_BUILTIN

#define baud 1000000
#define timeout 1000
#define polling 5 // the scan rate

#define retry_count 10

#define TxEnablePin 2

enum {
    PACKET1,
    PACKET2,
    // leave this last entry
    TOTAL_NO_OF_PACKETS
};

enum {
    // just add or remove registers and you're good to go...
    // The first register starts at address 0
    LED_STATE,
    BD_BUTTON1_STATE,
    SN_BUTTON2_STATE,
    SN_BUTTON3_STATE,
    TOTAL_ERRORS,
    // leave this one
    TOTAL_REGS_SIZE
    // total number of registers for function 3 and 16 share the same register array
};



Packet packets[TOTAL_NO_OF_PACKETS];

packetPointer packet1 = &packets[PACKET1];
packetPointer packet2 = &packets[PACKET2];

unsigned int regs[4];
unsigned int write_regs[1];
unsigned long last_toggle = 0;

// midi
#define BD 36
#define OH 46
#define SN 38

void setup() {
	Serial.begin(9600);
	Serial.print("MB_SERIAL: ");
#if 0
	while(1) {
		if ((void*)&MB_SERIAL == (void*)&Serial) {
			Serial.println ("it's usb serial...");
		} 
		else if ((void*)&MB_SERIAL == (void*)&Serial1) {
			Serial.println("it's serial 1");
		}
		else {
			Serial.println("WTF, mate?");
		}
		delay(1000);
	}
#endif
	// read 3 registers starting at address 0
	packet1->id = 1;
	packet1->function = READ_HOLDING_REGISTERS;
	packet1->address = 0;
	packet1->no_of_registers = 4;
	packet1->register_array = regs;

	// write the 1 registers to the PLC starting at address 0
	packet2->id = 1;
	packet2->function = PRESET_MULTIPLE_REGISTERS;
	packet2->address = 0;
	packet2->no_of_registers = 1;
	packet2->register_array = write_regs;

	// Initialize communication settings etc...
	modbus_configure(baud, timeout, polling, retry_count, TxEnablePin, packets, TOTAL_NO_OF_PACKETS);

	pinMode(connection_error_led, OUTPUT);

}

bool sn = false;
bool bd = false;
bool sn2 = false;

bool led_on = false;
void loop() {
	unsigned int connection_status = modbus_update(packets);

#if 1
	int bd_button = regs[BD_BUTTON1_STATE];
	Serial.println(bd_button);
	int sn_button = regs[SN_BUTTON2_STATE];
	int sn_button2 = regs[SN_BUTTON3_STATE];

	if (millis() - last_toggle > 1000) {
		last_toggle = millis();
		write_regs[0] = led_on;
		led_on = !led_on;
	}

	if (connection_status != TOTAL_NO_OF_PACKETS) {
		digitalWrite(connection_error_led, HIGH);
		// You could re-enable the connection by:
		//packets[connection_status].connection = true;
	} else {
		digitalWrite(connection_error_led, LOW);
	}


	if (bd_button && !bd) {
		noteOn(9, BD, 127);
		bd = true;
	}

	if (sn_button && !sn) {
		noteOn(9, SN, 127);
		sn = true;
	}

	if (sn_button2 && !sn2) {
		noteOn(9, SN, 127);
		sn2 = true;
	}

	if (!bd_button) {
		noteOff(9, BD, 0);
		bd = false;
	}

	if (!sn_button) {
		noteOff(9, SN, 0);
		sn = false;
	}

	if (!sn_button2) {
		noteOff(9, SN, 0);
		sn2 = false;
	}
	MidiUSB.flush();
#endif
}

//MIDIUSB library
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = { 0x09, 0x90 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = { 0x08, 0x80 | channel, pitch, velocity };
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = { 0x0B, 0xB0 | channel, control, value };
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}
