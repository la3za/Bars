////////////////////////////////////////////////////////////////////////////
//Define LCD character sets
///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////
// completely filled bar

// Bar 0, Bar 4:

const byte oneFilled[8] PROGMEM = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
};

const byte twoFilled[8] PROGMEM = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000
};

const byte threeFilled[8] PROGMEM = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100
};

const byte fourFilled[8] PROGMEM = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110
};

// every other vertical line filled, advantage: space between characters in display are no longer visible
// 6 dots high, not the full 8
// https://robodoupe.cz/2015/progress-bar-pro-arduino-a-lcd-displej/

// Bar 1:

byte g0[8] = {
  B00000,
  B00000,
  B10101,
  B10000,
  B10000,
  B10000,
  B10000,
  B10101
};  //0 |::
byte g1[8] = {
  B00000,
  B00000,
  B10101,
  B10100,
  B10100,
  B10100,
  B10100,
  B10101
};  //1 ||:
byte g2[8] = {
  B00000,
  B00000,
  B10101,
  B10101,
  B10101,
  B10101,
  B10101,
  B10101
};  //2 |||
byte g3[8] = {
  B00000,
  B00000,
  B10101,
  B00000,
  B00000,
  B00000,
  B00000,
  B10101
};  //3 :::
byte g4[8] = {
  B00000,
  B00000,
  B10101,
  B00001,
  B00001,
  B00001,
  B00001,
  B10101
};  //4 ::|
byte g5[8] = {
  B00000,
  B00000,
  B10101,
  B10101,
  B10001,
  B10001,
  B10001,
  B10101
};  //5 |:|

// Progress bar with every vertical line filled, 
// 7 dots high, not the full 8. 
// https://robodoupe.cz/2015/progress-bar-pro-arduino-a-lcd-displej/

// Bar 2:

  const byte g70[8] PROGMEM = {
    B00000,
    B10101,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10101}; //0 |__ 
  const byte g71[8] PROGMEM = {
    B00000,
    B10101,
    B10100,
    B10100, 
    B10100, 
    B10100, 
    B10100, 
    B10101}; //1 ||_ 
  const byte g72[8] PROGMEM = {
    B00000, 
    B10101,
    B10101,
    B10101, 
    B10101, 
    B10101, 
    B10101, 
    B10101}; //2 ||| 
  const byte g73[8] PROGMEM = {
    B00000,
    B10101,
    B00000,
    B00000,
    B00000, 
    B00000, 
    B00000, 
    B10101}; //3 ___ 
  const byte g74[8] PROGMEM = {
    B00000,
    B10101,
    B00001,
    B00001,
    B00001, 
    B00001, 
    B00001, 
    B10101}; //4 __| 
  const byte g75[8] PROGMEM = {
    B00000,
    B10101,
    B10001,
    B10001,
    B10001, 
    B10001, 
    B10001, 
    B10101}; //5 |_| 

//////////////////////////////////////////////////////////////
// every other vertical line filled, advantage: space between characters in display are no longer visible
// full height of 8 dots high
// https://robodoupe.cz/2015/progress-bar-pro-arduino-a-lcd-displej/

// Bar 3:

byte g80[8] = {
  B10101,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10101
};  //0 |::
byte g81[8] = {
  B10101,
  B10100,
  B10100,
  B10100,
  B10100,
  B10100,
  B10100,
  B10101
};  //1 ||:
byte g82[8] = {
  B10101,
  B10101,
  B10101,
  B10101,
  B10101,
  B10101,
  B10101,
  B10101
};  //2 |||
byte g83[8] = {
  B10101,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B10101
};  //3 :::
byte g84[8] = {
  B10101,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B10101
};  //4 ::|
byte g85[8] = {
  B10101,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10101
};  //8 |:|


// straight beginning symbol, full height: |xxx|
// Bar 4, Bar 6:

const byte beg3[8] PROGMEM = {
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001
};
const byte end3[8] PROGMEM = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
};

//////////////////////////////////////////////////////////////
// Bar 5, Bar 6
	
const byte zeroBar[8] PROGMEM = {
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111
};
const byte oneBar[8] PROGMEM = {
  B11111,
  B00000,
  B10000,
  B10000,
  B10000,
  B10000,
  B00000,
  B11111
};

const byte twoBar[8] PROGMEM = {
  B11111,
  B00000,
  B11000,
  B11000,
  B11000,
  B11000,
  B00000,
  B11111
};

const byte threeBar[8] PROGMEM = {
  B11111,
  B00000,
  B11100,
  B11100,
  B11100,
  B11100,
  B00000,
  B11111
};

const byte fourBar[8] PROGMEM = {
  B11111,
  B00000,
  B11110,
  B11110,
  B11110,
  B11110,
  B00000,
  B11111
};

const byte fiveBar[8] PROGMEM = {
  B11111,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B00000,
  B11111
};

// beginning symbol, curved (xxxx)
// Bar 5:
byte beg1[] = {
  B00001,
  B00010,
  B00010,
  B00010,
  B00010,
  B00010,
  B00010,
  B00001
};
// end symbol
byte end1[] = {
  B10000,
  B01000,
  B01000,
  B01000,
  B01000,
  B01000,
  B01000,
  B10000
};




///////////////////////////////////////////////////////////////////////////////////////////
// straight beginning symbol: |xxx|

// Not used:
/*
byte beg2[] = {
  B00000,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00000
};
byte end2[] = {
  B00000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B00000
};
*/



///////////////////////////////////////////////////////////////

byte fullSquare[8] = {
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111
};
byte smallFill[8] = {
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B00000
};

///////////////////////////////////////////////////////////////////////////////////////////
// Signalstrenght
// Bar 9:
byte vert1[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111
};
byte vert2[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111
};
byte vert3[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
};
byte vert4[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111
};
byte vert5[8] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
byte vert6[8] = {
  B00000,
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};
byte vert7[8] = {
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};