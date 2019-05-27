/*  CAT28C64BD EEPROM Programmer sketch:
 *   
 *   The CAT28C64BD is an 8 kB EEPROM chip with a fast 32 byte write feature.
 *  This feature allows up to 32 consecutive byte writes to improve throuput.
 *  
 *  The page write feature is fully encompassed in the writeEEPROM() function
 *  and will accept any page size from 1 to 32 bytes.  The 2 pages of example
 *  data, in conjunction with the function printContents(), will allow you to 
 *  test your hardware.
 *  
 *  Direct port manipulation was required. Using digitalRead()/digitalWrite() 
 *  was too slow to meet the timing requirements when performing page writes.
 *  
 *  Data pins are on Port A (Mega2560 pins 22-29). Address pins are on Port F
 *  (LoByte), and PortK (HiByte) (Mega2560 pins A0-A12).
 *  
 */

#define SZ_PAGE 32

// Control pins
#define _WE A15
#define _OE A14
// unused   A13

int addr = 0;
//byte data[SZ_PAGE] = {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
//                      0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
//                      0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
//                      0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};

byte data[SZ_PAGE] = {0xAA, 0xA9, 0xAA, 0xA9, 0xAA, 0xA9, 0xAA, 0xA9,
                      0xAA, 0xA9, 0xAA, 0xA9, 0xAA, 0xA9, 0xAA, 0xA9,
                      0xAA, 0xA9, 0xAA, 0xA9, 0xAA, 0xA9, 0xAA, 0xA9,
                      0xAA, 0xA9, 0xAA, 0xA9, 0xAA, 0xA9, 0xAA, 0xA9};


// Ports F & K are the low & high bytes of the address, respectively.
// Port K's 3 msb's are EEPROM control bits, _WE (pin A15), _OE (pin A14),
// and _CE (not used).  Port A pins are used for the data bus.
void setAddress(unsigned int address)  {
    // Arduino:
    DDRF = 0xFF;      // Arduino: lowByte(address) pins as outputs.
    DDRK = 0xFF;      // Arduino: highByte(address) and control pins as outputs.

    // Arduino: Put Address onto the bus.
    PORTF = lowByte(address);
    PORTK = PORTK | (highByte(address) & B00011111);    // Protect 3 msb control pins.
}



// Read a byte from the EEPROM at the specified address
// using direct port manipulation.
byte readEEPROM(int EEAddress) {
    DDRK = 0xFF;                    // Arduino: highByte(address) and control pins as outputs.
    PORTK = B10000000;              //  EEPROM: Address & Control: WE=off, OE=on, CE=on, 5 bits of Addr=0
    DDRA = 0x00;                    // Arduino: data pins are inputs.
    PORTA = 0xFF;                   // Arduino: Input pullup resistors enabled.
    setAddress(EEAddress);
    return PINA;                    // Arduino: Read the byte found on port A (Data pins).
}



// Page write for EEPROM (1 to 32 sequential bytes)
// using direct port manipulation.
int writeEEPROM(int far, byte *dataArray, int numBytes) {
    if (numBytes > SZ_PAGE) {
        return -1;  // Exceeded CAT28C64BD EEPROM max page size. Allowable page sizes are 1 to 32 bytes.
    }

    PORTK = B11000000;                // EEPROM: Address & Control: WE=off, OE=off, CE=on, 5 bits of Addr=0
    DDRA = 0xFF;                      // Arduino data pins are outputs.

    for (int near=0; near<numBytes; near++) {
        setAddress(far + near);
        bitWrite(PORTK, 7, LOW);        // Set _WE = LOW latching the address into the EEPROM. (_WE = 0)
        PORTA = dataArray[near];        // Place data byte from array onto data pins.
        bitWrite(PORTK, 7, HIGH);       // Set _WE = HIGH latching the data byte into the EEPROM. (_WE = 1)
    }
    
    delayMicroseconds(5100);    // Each page write requires a delay for the Write Cycle Time, Twc = 5 ms.

    return 0; // Wrote numBytes to external EEPROM.
}



void printContents(int upperAddr) {
    for (int far=0; far<upperAddr; far+=32) {
        for (int near=0; near<32; near++) {
            data[near] = readEEPROM(far + near);
        }
        char buf[128];
        sprintf(buf, "%04x: %02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x",
                       far, data[0],  data[1],  data[2],  data[3],  data[4],  data[5],  data[6],  data[7],
                            data[8],  data[9],  data[10], data[11], data[12], data[13], data[14], data[15],
                            data[16], data[17], data[18], data[19], data[20], data[21], data[22], data[23],
                            data[24], data[25], data[26], data[27], data[28], data[29], data[30], data[31]);

        Serial.println(buf);
    }
}




void setup() {
    Serial.begin(1000000);

    // Preset EEPROM control pins to some neutral state.
    PORTK = B11000000;          // EEPROM Address & Control: WE=off, OE=off, CE=on, 5 bits of Addr=0
    PORTF = B00000000;          // Arduino: Address LowByte pins cleared.

    int topAddr = 8192;

    writeEEPROM(0, data, 32);

//    for (int far=0; far<topAddr; far+=32) {
//        writeEEPROM(far, data, 32);    
//    }

    printContents(topAddr);
}




void loop() {

}

