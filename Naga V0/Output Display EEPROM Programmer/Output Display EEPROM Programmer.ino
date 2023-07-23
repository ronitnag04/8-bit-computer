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
  delay(1);
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

//              0     1     2     3     4     5     6     7
//              8     9     A     b     C     d     E     F
//byte data[] = {0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 
//               0x7f, 0x7b, 0x77, 0x1f, 0x4e, 0x3d, 0x4f, 0x47};

byte digits[] = {0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 
                 0x7f, 0x7b};


void setup() {
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  setDataPinMode(INPUT);

  Serial.begin(57600);
  
  // eraseEEPROM();
  
  int base = 0;
  Serial.println("Programming one's place (unsigned)");
  for (int value = 0; value <= 255; value += 1) {
    writeEEPROM(value + base, digits[value % 10]);    
  }
  base = 256;
  Serial.println("Programming ten's place (unsigned)");
  for (int value = 0; value <= 255; value += 1) {
    writeEEPROM(value + base, digits[(value / 10) % 10]);    
  }
  base = 256*2;
  Serial.println("Programming hundred's place (unsigned)");
  for (int value = 0; value <= 255; value += 1) {
    writeEEPROM(value + base, digits[(value / 100) % 10]);    
  }
  base = 256*3;
  Serial.println("Programming sign place (unsigned)");
  for (int value = 0; value <= 255; value += 1) {
    writeEEPROM(value + base, 0);    
  }

  base = 256*4;
  Serial.println("Programming one's place (signed)");
  for (int value = -128; value <= 127; value += 1) {
    writeEEPROM((byte)value + base, digits[abs(value) % 10]);    
  }
  base = 256*5;
  Serial.println("Programming ten's place (signed)");
  for (int value = -128; value <= 127; value += 1) {
    writeEEPROM((byte)value + base, digits[abs(value / 10) % 10]);    
  }
  base = 256*6;
  Serial.println("Programming hundred's place (signed)");
  for (int value = -128; value <= 127; value += 1) {
    writeEEPROM((byte)value + base, digits[abs(value / 100) % 10]);    
  }
  base = 256*7;
  Serial.println("Programming sign place (signed)");
  for (int value = -128; value <= 127; value += 1) {
    if (value < 0) {
      writeEEPROM((byte)value + base, 0x01);       
    } else {
      writeEEPROM((byte)value + base, 0);
    }
  }
  
  printContents();
  
}

void loop() {
}
