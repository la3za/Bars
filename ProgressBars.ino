// Progress bars of various sorts
// counts up and down, and shows that it can also start in arbitrary position on bar
// uses custom character set for LCD
// 
// Sverre Holm December 2023, github/la3za/ProgressBars
//
// 9 different progress bars
// loadGapLessCharacters7() is an example of LCD characters in PROGMEM to save dynamic memory
//

// Put call required by your LCD here:
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

// size of LCD:
#define NCOLS 20  
#define NROWS 4

//#define DEBUG             // for displaying integer segment and subsegment numbers
byte framedBar = 0;  

#define LCD_PWM 45          // for backlight (if needed)
byte backlightVal = 100;    // (0...255) initial backlight value

int imax = 100;             // no of units to show in bar
int col1 = 2;  // -99       // first column of bar
int col2 = 15; // 299       // end of bar
int line = 0;               // lineno for bar
boolean flip = true;        // flips start index, ibegin, for bar
int ibegin;

byte filled;      
byte empty;

byte buffer[8];  // temporary storage for PROGMEM characters for loadGapLessCharacters7();

////////////////////////////////////////////////////////////////////////////
//Define LCD character sets
///////////////////////////////////////////////////////////////////////////////////////////


// every other vertical line filled, advantage: space between characters in display are no longer visible
// only 6 dots high, not the full 8
// https://robodoupe.cz/2015/progress-bar-pro-arduino-a-lcd-displej/
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


void loadGapLessCharacters6() {  // height 6 dots
  lcd.createChar(0, g0);  //0 |::
  lcd.createChar(1, g1);  //1 ||:
  lcd.createChar(2, g2);  //2 |||
  filled = 2;
  lcd.createChar(3, g3);  //3 :::
  empty = 3;
  lcd.createChar(4, g4);  //4 ::|
  lcd.createChar(5, g5);  //5 |:|
}

//////////////////////////////////////////////////////////////
// every other vertical line filled, advantage: space between characters in display are no longer visible
// only 5 dots high, not the full 8
// https://robodoupe.cz/2015/progress-bar-pro-arduino-a-lcd-displej/
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

void loadGapLessCharacters8() {  // height 8 dots
  lcd.createChar(0, g80);  //0 |::
  lcd.createChar(1, g81);  //1 ||:
  lcd.createChar(2, g82);  //2 |||
  filled = 2;
  lcd.createChar(3, g83);  //3 :::
  empty = 3;
  lcd.createChar(4, g84);  //4 ::|
  lcd.createChar(5, g85);  //5 |:|
}

// Progress bar with every other vertical line filled, 
// advantage: space between characters in display are no longer visible
// only 7 dots high, not the full 8. 
// https://robodoupe.cz/2015/progress-bar-pro-arduino-a-lcd-displej/
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



//////////////////////////////////////////////////////////
void loadGapLessCharacters7()
{
  memcpy_P(buffer,g70, 8);
  lcd.createChar(0, buffer);
  memcpy_P(buffer,g71, 8);
  lcd.createChar(1, buffer);
  memcpy_P(buffer,g72, 8);
  lcd.createChar(2, buffer);
  memcpy_P(buffer,g73, 8);
  lcd.createChar(3, buffer);
  memcpy_P(buffer,g74, 8);
  lcd.createChar(4, buffer);
  memcpy_P(buffer,g75, 8);
  lcd.createChar(5, buffer);

  filled = 2;
  empty = 3;
}


//////////////////////////////////////////////////////////////
byte zeroBar[] = {
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111
};
byte oneBar[] = {
  B11111,
  B00000,
  B10000,
  B10000,
  B10000,
  B10000,
  B00000,
  B11111
};

byte twoBar[] = {
  B11111,
  B00000,
  B11000,
  B11000,
  B11000,
  B11000,
  B00000,
  B11111
};

byte threeBar[] = {
  B11111,
  B00000,
  B11100,
  B11100,
  B11100,
  B11100,
  B00000,
  B11111
};

byte fourBar[] = {
  B11111,
  B00000,
  B11110,
  B11110,
  B11110,
  B11110,
  B00000,
  B11111
};

byte fiveBar[] = {
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

void loadCurvedFramedBarCharacters() {  // ( xxxx )
  empty = 0;
  filled = 5;
  lcd.createChar(empty, zeroBar);
  lcd.createChar(1, oneBar);
  lcd.createChar(2, twoBar);
  lcd.createChar(3, threeBar);
  lcd.createChar(4, fourBar);
  lcd.createChar(filled, fiveBar);

  lcd.createChar(6, beg1);
  lcd.createChar(7, end1);
}

///////////////////////////////////////////////////////////////////////////////////////////

// straight beginning symbol: |xxx|
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

///////////////////////////////////////////////
// completely filled bar:
//
byte oneFilled[]{
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
};

byte twoFilled[]{
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000
};

byte threeFilled[]{
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100
};

byte fourFilled[]{
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110
};

void loadSimpleBarCharacters() {
  empty = 254;  // 0;
  filled = 255;
  //lcd.createChar(empty, zero);
  lcd.createChar(1, oneFilled);
  lcd.createChar(2, twoFilled);
  lcd.createChar(3, threeFilled);
  lcd.createChar(4, fourFilled);
}

// straight beginning symbol, full height: |xxx|
byte beg3[] = {
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001,
  B00001
};
byte end3[] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
};

void loadFramedSimpleBarCharacters()
// filled bars with frame that marks end of bar
// simplified version of  https://www.norwichhackspace.org/blog/post/textstar-serial-lcd-display
{               // | XXX |
  empty = 254;  // 0;
  filled = 255;
  //lcd.createChar(empty, zero);
  lcd.createChar(1, oneFilled);
  lcd.createChar(2, twoFilled);
  lcd.createChar(3, threeFilled);
  lcd.createChar(4, fourFilled);

  lcd.createChar(6, beg3);
  lcd.createChar(7, end3);
}


void loadStraightFramedBarCharacters() {  // | xxxx |
  empty = 0;
  filled = 5;
  lcd.createChar(empty, zeroBar);
  lcd.createChar(1, oneBar);
  lcd.createChar(2, twoBar);
  lcd.createChar(3, threeBar);
  lcd.createChar(4, fourBar);
  lcd.createChar(filled, fiveBar);

  lcd.createChar(6, beg3);  // or beg2
  lcd.createChar(7, end3);
}



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


void loadSquares() {
  lcd.createChar(1, fullSquare);
  lcd.createChar(2, smallFill);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Now follows the actual drawing functions
///////////////////////////////////////////////////////////////////////////////////////////

void gapLessBar(int nr, int total, int firstPos, int lastPos, int line)
// https://robodoupe.cz/2015/progress-bar-pro-arduino-a-lcd-displej/
// framed progress bar, every other segment only, so that gap between characters melts in
// data is displayed in columns firstPos ... lastPos, 5 dots height
{
  firstPos = max(firstPos, 0);           // 1...NCOLS-1, first position with data
  lastPos = min(lastPos, NCOLS - 1);     // ... NCOLS-1, last position with data
  lastPos = max(lastPos, firstPos + 1);  // cannot go right to left!!
  line = max(line, 0);
  line = min(line, NROWS);  // not outside width of LCD

  int Nseg = lastPos - firstPos + 1;  // no of positions to use on LCD (first, ..., last)

  float noOfSubSegments = 3.0;                                                        // no of subsegments used per character
  float segmentNoReal = (float(nr) / float(total)) * (Nseg - 1.0 / noOfSubSegments);  // 1/segmentNoReal 19.12.2023
  int segmentNoInt = int(segmentNoReal);
  byte subSegmentNo = int(noOfSubSegments * (segmentNoReal - segmentNoInt));

  // draw on LCD

  // 1: left-hand symbol
  lcd.setCursor(firstPos, line);
  if (segmentNoInt == 0) lcd.write(byte(0));  // initial left-hand symbol, |::
  else lcd.write(filled);                     // filled,                   |||

  // 2: upper and lower frame :::
  {
    for (int j = firstPos + segmentNoInt; j < lastPos; j++)  //
    {
      lcd.setCursor(j, line);
      lcd.write(empty);  // :::
    }
  }

  // 3: draw 0 ... Nseg completely filled segments

  int jmax = firstPos + segmentNoInt;
  for (int j = firstPos + 1; j < jmax; j++) {
    {
      lcd.setCursor(j, line);
      lcd.write(filled);
    }
  }

  // 4: draw closing, right-hand symbol
  if (segmentNoInt < Nseg - 2) {
    lcd.setCursor(lastPos, line);
    lcd.write(byte(4));  // ::|, final   symbol
  }

  // 5 draw sub segment
  if (segmentNoInt < Nseg - 1)  // nothing if final segment has been filled
  {
    lcd.setCursor(firstPos + segmentNoInt, line);
    lcd.write(subSegmentNo);                     // 0, 1, 2 =|::, ||:, |||
    if (segmentNoInt == Nseg - 2) lcd.write(4);  // added 19.12.2023 - only important when counting down
  } else if (segmentNoInt == Nseg - 1) {         // 4 = ::|, 5 = |:|, filled = |||
    lcd.setCursor(firstPos + segmentNoInt, line);
    // order 5, filled, filled is correct
    // order 4, 5, filled misses one beat
    if (subSegmentNo == 0) lcd.write(5);       // |:|
    if (subSegmentNo == 1) lcd.write(filled);  //
    if (subSegmentNo == 2) lcd.write(filled);  // |||
  }

// debug info on screen
#ifdef DEBUG
  if (line < NROWS) {
    lcd.setCursor(6, line+1);
    lcd.print(segmentNoReal);
    lcd.print("    ");
    lcd.setCursor(12, line+1);
    lcd.print(segmentNoInt);
    lcd.print(" ");
    lcd.print(subSegmentNo);
    lcd.print("      ");
  }
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////

void fullBar(int nr, int total, int firstPos, int lastPos, int line)
// Plain progressbar, completely fills character
// data is displayed in columns firstPos ... lastPos

{
  firstPos = max(firstPos, 0);        // 1...NCOLS-1, first position with data
  lastPos = min(lastPos, NCOLS - 1);  // ... NCOLS-2, last position with data
  line = max(line, 0);
  line = min(line, NROWS);

  int Nseg = lastPos - firstPos + 1;  // no of positions to use on LCD

  float segmentNoReal = Nseg * float(nr) / float(total);  //
  int segmentNoInt = int(segmentNoReal);
  byte subSegmentNo = int(5 * (segmentNoReal - segmentNoInt));

  // draw on LCD

  // 1. Empty frame
  for (int j = firstPos; j <= lastPos; j++) {
    lcd.setCursor(j, line);
    lcd.write(byte(empty));  // over- and underbar - or- empty
  }
  // 2. Draw 0 ... Nseg completely filled segments
  for (int j = firstPos; j < firstPos + segmentNoInt; j++) {
    lcd.setCursor(j, line);
    lcd.write(filled);
  }

  // 3. Draw sub segment
  if (segmentNoInt != Nseg)  // nothing if final segment has been filled
  {
    lcd.setCursor(firstPos + segmentNoInt, line);
    if (subSegmentNo == 0) lcd.write(empty);  // 0
    else lcd.write(subSegmentNo);             // 1...4
  }

// debug info on screen
#ifdef DEBUG
  if (line < NROWS) {
    lcd.setCursor(6, line + 1);
    lcd.print(segmentNoReal);
    lcd.print("    ");
    lcd.setCursor(12, line + 1);
    lcd.print(segmentNoInt);
    lcd.print(" ");
    lcd.print(subSegmentNo);
    lcd.print("      ");
  }
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void framedProgressBar(int nr, int total, int firstPos, int lastPos, int line)
// framed progress bar
// data is displayed in columns firstPos+1 ... lastPos-1,
// and frame ends on firstPos, lastPos

{
  firstPos = max(firstPos, 0);        // 1...NCOLS-1, first position with data
  lastPos = min(lastPos, NCOLS - 1);  // ... NCOLS-2, last position with data
  line = max(line, 0);
  line = min(line, NROWS);

  int Nseg = lastPos - firstPos - 1;  // no of positions to use on LCD (first, last = frame)

  float segmentNoReal = Nseg * float(nr) / float(total);  //
  int segmentNoInt = int(segmentNoReal);
  byte subSegmentNo = int(5 * (segmentNoReal - segmentNoInt));

  // draw on LCD

  // First: left-hand symbol
  lcd.setCursor(firstPos, line);
  lcd.write(byte(6));  // initial symbol

  // Second: upper and lower frame
  for (int j = firstPos + 1; j < lastPos; j++) {
    lcd.setCursor(j, line);
    lcd.write(byte(empty));  // over- and underbar - or- empty
  }

  // Third: draw closing, right-hand symbol
  lcd.setCursor(lastPos, line);
  lcd.write(byte(7));  // final   symbol

  // Fourth draw 0 ... Nseg completely filled segments
  for (int j = firstPos + 1; j <= firstPos + segmentNoInt; j++) {
    lcd.setCursor(j, line);
    lcd.write(filled);
  }

  // Third draw sub segment
  if (segmentNoInt != Nseg)  // nothing if final segment has been filled
  {
    lcd.setCursor(firstPos + 1 + segmentNoInt, line);
    if (subSegmentNo == 0) lcd.write(empty);
    else lcd.write(subSegmentNo);  // 1...4
  }


// debug info on screen
#ifdef DEBUG
  if (line < NROWS) {
    lcd.setCursor(6, line + 1);
    lcd.print(segmentNoReal);
    lcd.print("    ");
    lcd.setCursor(12, line + 1);
    lcd.print(segmentNoInt);
    lcd.print(" ");
    lcd.print(subSegmentNo);
    lcd.print("      ");
  }
#endif
}

////////////////////////////////////////////////////////////////////////

void singleCharacterBar(int nr, int total, int firstPos, int lastPos, int line, byte fill, byte open)
// framed progress bar with nearly standard characters: 
// example: fill = char 255, open defined in char 1
// data is displayed in columns firstPos ... lastPos,

{
  firstPos = max(firstPos, 0);        // 1...NCOLS-1, first position with data
  lastPos = min(lastPos, NCOLS - 1);  // ... NCOLS-2, last position with data
  line = max(line, 0);
  line = min(line, NROWS);

  int Nseg = lastPos - firstPos + 1;  // no of positions to use on LCD (first, last = frame)

  float segmentNoReal = Nseg * float(nr) / float(total);  //
  int segmentNoInt = int(segmentNoReal);

  // draw on LCD


  // First: left-hand symbol
  //  lcd.setCursor(firstPos, line); lcd.print(" ");  // initial symbol

  // Second: upper and lower frame
  for (int j = firstPos; j <= lastPos; j++) {
    lcd.setCursor(j, line);
    //         lcd.write((char)165);  // dot// lcd.print(" ");
    //       lcd.write((char)219);    // 6x8 square
    lcd.write((char)open);  // 8x8 square  - same as Sansui DAB radio
  }

  // Then draw 0 ... Nseg completely filled segments
  for (int j = firstPos; j <= firstPos + segmentNoInt - 1; j++) {
    lcd.setCursor(j, line);
    lcd.write((char)fill);  //lcd.write((char)219); //square // lcd.print(">");
  }

// debug info on screen
#ifdef DEBUG
  if (line < NROWS) {
    lcd.setCursor(6, line + 1);
    lcd.print(segmentNoReal);
    lcd.print("    ");
    lcd.setCursor(12, line + 1);
    lcd.print(segmentNoInt);
    lcd.print("      ");
  }
#endif
}


////////////// S E T U P /////////////////////////////////////////////////////////////////////////////

void setup() {
  // initialize the LCD and allocate the 5 arrays to a number.
  lcd.begin(NCOLS, NROWS);
  pinMode(LCD_PWM, OUTPUT);
  analogWrite(LCD_PWM, backlightVal);

  //Serial.begin(115200);
}


///////////////// L O O P ////////////////////

// Prints a number from 0 to 100 & displays the progress bar
// The loop counts up, pauses  then counts down again.
void loop() {
  
  lcd.setCursor(0, line); lcd.print("                    ");  // erase old stuff

  // load character sets
  switch (framedBar) {
    case 0:  loadSimpleBarCharacters();         break;   // =====
    case 1:  loadGapLessCharacters6();          break;   // -----
    case 2:  loadGapLessCharacters7();          break;
    case 3:  loadGapLessCharacters8();          break;
    case 4:  loadFramedSimpleBarCharacters();   break;  // |===|
    case 5:  loadCurvedFramedBarCharacters();   break;  // (---) nicest?
    case 6:  loadStraightFramedBarCharacters(); break;  // |---|
    default: loadSquares();                     break;
  }

  delay(100);   // to avoid flash

  lcd.setCursor(0, line+1); lcd.print(framedBar);lcd.print(": ");

  if (flip) ibegin = 0;       // first time:  full loop
  else      ibegin = imax/2;  // second time: start at 50%
  
  lcd.setCursor(2, line+1);lcd.print("  ");
  for (int i = ibegin; i <= imax; i++)  // count up
  {
    lcd.setCursor(2, line+1); lcd.print(i); lcd.print(" "); 
    displayBars(i);
    delay(50); // 50
  }
  delay(1000);
  
  lcd.setCursor(2, line+1);lcd.print("  ");
  for (int i = imax; i >= 0; i--)  // count down
  {
    lcd.setCursor(2,line+1); lcd.print(i); lcd.print(" ");
    displayBars(i);
    delay(50); //50
  }

  delay(1000);
  framedBar = framedBar + 1;
  if (framedBar > 8) { framedBar = 0; flip = !flip; }
}

////////////////////////////////////////////////////////////////////
void displayBars(int i)
{
    if (framedBar == 0)                   fullBar(i, imax, col1, col2, line);        // progress bar.
    if (framedBar >= 1 & framedBar <= 3)  gapLessBar(i, imax, col1, col2, line);
    if (framedBar >= 4 & framedBar <= 6)  framedProgressBar(i, imax, col1, col2, line);  // progress bar.
    if (framedBar == 7)                   singleCharacterBar(i, imax, col1, col2, line, 255, 1); // full height square
    if (framedBar == 8)                   singleCharacterBar(i, imax, col1, col2, line, 2, 219); // reduced height square
   // if (framedBar == 9)                 singleCharacterBar(i, imax, col1, col2, line, 126, 219); // or '>', 219: not nice
}
