#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D1 6
#define EEPROM_D2 7
#define EEPROM_D3 8
#define EEPROM_D4 9
#define EEPROM_D5 10
#define EEPROM_D6 11
#define EEPROM_D7 12
#define WRITE_EN 13
#define CHIP_SELECT A7

#define DFT 0b0000000000000111

#define IO  0b0000000000000000
#define RO  0b0000000000000001
#define BO  0b0000000000000010
#define EO  0b0000000000000011
#define AO  0b0000000000000100
#define CO  0b0000000000000101
#define UO0 0b0000000000000110

#define II  0b0000000000001000
#define RI  0b0000000000010000
#define MI  0b0000000000100000
#define HLT 0b0000000001000000
#define OI  0b0000000010000000
#define BI  0b0000000100000000
#define SU  0b0000001000000000
#define AI  0b0000010000000000
#define CE  0b0000100000000000
#define CI  0b0001000000000000
#define FI  0b0010000000000000
#define MCR 0b0100000000000000
#define UC0 0b1000000000000000

void setAddress(int address, bool outputEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address >> 8 | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);
  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}

void setDataPinMode(bool mode) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, mode); 
  }
}

byte readEEPROM(int address) {
  // Make sure Arduino is not outputting before making EEPROM output
  setDataPinMode(INPUT);
  setAddress(address, true);

  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}

void writeEEPROM(int address, byte data) {
  // Make sure EEPROM is not outputting before making Arduino output
  setAddress(address, false);
  setDataPinMode(OUTPUT);
  
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }

  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(10);
}

void printContents() {
  for (int base = 0; base <= 2047; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }
    char buf[80];
    sprintf(buf, "%03x: %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
      base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
      data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]
    );
    Serial.println(buf);
    if (base % 256 == 240) {
      Serial.println();
    }
  }
}

void eraseEEPROM() {
  Serial.print("Erasing EEPROM");
  for (int address = 0; address <= 2047; address += 1) {
    writeEEPROM(address, 0xff);  
    if (address % 64 == 0) {
      Serial.print(".");
    }  
  }
  Serial.println(" done!");
}

bool left_chip;

uint16_t data[] = {
  CO|MI, RO|II|CE, MCR, DFT, DFT, DFT, DFT, DFT,           // NOP 0000
  CO|MI, RO|II|CE, IO|MI, RO|AI, MCR, DFT, DFT, DFT,       // LDA 0001
  CO|MI, RO|II|CE, IO|MI, RO|BI, MCR, DFT, DFT, DFT,       // LDB 0010
  CO|MI, RO|II|CE, EO|AI, MCR, DFT, DFT, DFT, DFT,         // ADD 0011
  CO|MI, RO|II|CE, SU|EO|AI, MCR, DFT, DFT, DFT, DFT,      // SUB 0100
  CO|MI, RO|II|CE, IO|MI, AO|RI, MCR, DFT, DFT, DFT,       // STA 0101
  CO|MI, RO|II|CE, IO|AI, MCR, DFT, DFT, DFT, DFT,         // LDI 0110
  CO|MI, RO|II|CE, IO|CI, MCR, DFT, DFT, DFT, DFT,         // JMP 0111
  CO|MI, RO|II|CE, MCR, DFT, DFT, DFT, DFT, DFT,           //     1000
  CO|MI, RO|II|CE, MCR, DFT, DFT, DFT, DFT, DFT,           //     1001
  CO|MI, RO|II|CE, MCR, DFT, DFT, DFT, DFT, DFT,           //     1010
  CO|MI, RO|II|CE, MCR, DFT, DFT, DFT, DFT, DFT,           //     1011
  CO|MI, RO|II|CE, MCR, DFT, DFT, DFT, DFT, DFT,           //     1100
  CO|MI, RO|II|CE, MCR, DFT, DFT, DFT, DFT, DFT,           //     1101
  CO|MI, RO|II|CE, IO|MI, RO|OI, MCR, DFT, DFT, DFT,       // OUT 1110
  CO|MI, RO|II|CE, HLT, MCR, DFT, DFT, DFT, DFT,           // HLT 1111
};



 
void setup() {
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  setDataPinMode(INPUT);

  pinMode(CHIP_SELECT, INPUT);
  left_chip = analogRead(CHIP_SELECT) > 100;

  Serial.begin(57600);
  
  // eraseEEPROM();

  if (left_chip) {
    Serial.print("Programming Left EEPROM");
  } else {
    Serial.print("Programming Right EEPROM");
  }
  for (int address = 0; address < sizeof(data)/sizeof(data[0]); address += 1) {
      if (left_chip) {
        writeEEPROM(address, data[address] >> 8);
      } else {
        writeEEPROM(address, data[address]);
      }

    if (address % 64 == 0) {
      Serial.print(".");
    }
  }
  Serial.println();

  printContents();
  
}

void loop() { 
}
