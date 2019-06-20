#define DATAARRSIZE 9
 
// 74HC595 pins
int latchPin = 12;
int clockPin = 13;
int dataPin = 11;
int OE = 10;
 
int outPins[4] = {A2, A3, 4, 5};
byte dataArray[DATAARRSIZE];

int maxAmp = 0;
int t0, t1, t2, t3, t4, t5, t6, t7;
float maxThresh = 150.0;
 
void setup() {
  //set pins to output because they are addressed in the main loop
  // config GPIOs
  pinMode(latchPin, OUTPUT);
  pinMode(OE, OUTPUT);
  digitalWrite(OE, LOW);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(A5, INPUT);

  Serial.begin(9600);
 
  // LED settings
  dataArray[0] = 0xFF; //0b11111111
  dataArray[1] = 0xFE; //0b11111110
  dataArray[2] = 0xFC; //0b11111100
  dataArray[3] = 0xF8; //0b11111000
  dataArray[4] = 0xF0; //0b11110000
  dataArray[5] = 0xE0; //0b11100000
  dataArray[6] = 0xC0; //0b11000000
  dataArray[7] = 0x80; //0b10000000
  dataArray[8] = 0x00; //0b00000000

  // volume thresholds
  maxAmp = 0;
  t1 = (int)(0.32 * maxThresh);
  t2 = (int)(0.50 * maxThresh);
  t3 = (int)(0.63 * maxThresh);
  t4 = (int)(0.73 * maxThresh);
  t5 = (int)(0.82 * maxThresh);
  t6 = (int)(0.89 * maxThresh);
  t7 = (int)(0.95 * maxThresh);
  t0 = t1 - (t2 - t1);
}
 
void loop() {
  // sample volume a few times and take highest
  int volume = 0;
  for (int i=0; i<5; i++) {
//    int v = abs(460 - analogRead(A5)); // usb
    int v = abs(460 - analogRead(A5)); // battery
    if (v > volume) {volume = v;}
  }

//  int volume = abs(511 - analogRead(A5));

  // calculate VU LED level using thresholds
  uint8_t level = 0;
  if (volume > t0) {
    if (volume < t1) {
      level = 1;
    } else if (volume < t2) {
      level = 2;
    } else if (volume < t3) {
      level = 3;
    } else if (volume < t4) {
      level = 4;
    } else if (volume < t5) {
      level = 5;
    } else if (volume < t6) {
      level = 6;
    } else if (volume < t7) {
      level = 7;
    } else {
      level = 8;
    }
  }
  
  setLevel(level);
}
 
void setLevel(uint8_t level) {
  // print level for testing
  // Serial.print("level: ");
  // Serial.println(level);

  // shift out LEDs to match level
  // turn on LEDs based on level
  uint8_t data = dataArray[level];
  
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, data);
  digitalWrite(latchPin, 1);
  
  // cycle output controllers
  for (int k=0; k<4; k++) {
    digitalWrite(outPins[k], HIGH);
    delay(1);
    digitalWrite(outPins[k], LOW);
  }
}

// ShiftOut and starting point for 74HC595 control
// from https://www.arduino.cc/en/Tutorial/ShiftOut
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first,
  // on the rising edge of the clock,
  // clock idles low
 
  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);
 
  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, LOW);
  digitalWrite(myClockPin, LOW);
 
  //for each bit in the byte myDataOut
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights.
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, LOW);
 
    //if the value passed to myDataOut and a bitmask result
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= HIGH;
    }
    else {  
      pinState= LOW;
    }
 
    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, HIGH);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, LOW);
  }
 
  //stop shifting
  digitalWrite(myClockPin, LOW);
}

