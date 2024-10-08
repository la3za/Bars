// BARS:
// Progress bars of various sorts, moving from left to right
// counts up and down, and shows that it can also start in arbitrary position on bar
// uses custom character set for LCD
// 
// Sverre Holm December 2023, github/la3za/ProgressBars
//
// 9 different progress bars
// loadGapLessCharacters7() is an example of LCD characters in PROGMEM to save dynamic memory
//
// a couple of character sets are stored in PROGMEM in order to save dynamic memory
//

// 06.10.2024: less flicker in bars 1-3 by modification of gapLessBar().



// Put call required by your LCD here:
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

// size of LCD:
#define NCOLS 20  
#define NROWS 4

//#define DEBUG             // debug info: integer segment and subsegment numbers
byte framedBar = 0;         // initial bar to display

#define LCD_PWM 45          // pin for backlight (if needed)
byte backlightVal = 20;     // (0...255) initial backlight value. 
                            //value = 20 works fine for photo: Manual f/3.8, 1/5 sec, ISO100

int imax = 50;              // no of units to show in bar
int col1 = 6;  // -99       // first column of bar
int col2 = 15; // 299       // end of bar
int line = 0;               // lineno for bar
boolean flip = true;        // flips start index, ibegin, for bar
int ibegin;

byte filled;                // address of 'filled' LCD character
byte empty;                 // address of 'empty' LCD character

byte buffer[8];  // temporary storage for PROGMEM characters for loadGapLessCharacters7();

////////////////////////////////////////////////////////////////////////////
//Define LCD character sets
///////////////////////////////////////////////////////////////////////////////////////////


// every other vertical line filled, advantage: space between characters in display are no longer visible
// 6 dots high, not the full 8
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
// full height of 8 dots high
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
// 7 dots high, not the full 8. 
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

void loadSimpleBarCharacters() {
  empty = 254;  // 0;
  filled = 255;
  // upload characters to the lcd
  memcpy_P(buffer,oneFilled, 8);
  lcd.createChar(1, buffer);
  memcpy_P(buffer,twoFilled, 8);
  lcd.createChar(2, buffer);
  memcpy_P(buffer,threeFilled, 8);
  lcd.createChar(3, buffer);
  memcpy_P(buffer,fourFilled, 8);
  lcd.createChar(4, buffer);
}

// straight beginning symbol, full height: |xxx|
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

void loadFramedSimpleBarCharacters()
// filled bars with frame that marks end of bar
// simplified version of  https://www.norwichhackspace.org/blog/post/textstar-serial-lcd-display
{               // | XXX |
  empty = 254;  // 0;
  filled = 255;
  //lcd.createChar(empty, zero);
  memcpy_P(buffer,oneFilled, 8);
  lcd.createChar(1, buffer);
  memcpy_P(buffer,twoFilled, 8);
  lcd.createChar(2, buffer);
  memcpy_P(buffer,threeFilled, 8);
  lcd.createChar(3, buffer);
  memcpy_P(buffer,fourFilled, 8);
  lcd.createChar(4, buffer);
  memcpy_P(buffer,threeFilled, 8);

  memcpy_P(buffer,beg3, 8);
  lcd.createChar(6, buffer);
  memcpy_P(buffer,end3, 8);
  lcd.createChar(7, buffer);
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

  memcpy_P(buffer,beg3, 8);
  lcd.createChar(6, buffer);
  memcpy_P(buffer,end3, 8);
  lcd.createChar(7, buffer);
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

void loadVerticalBarCharacters()
// vertical lines, thicker and thicker
{               
  empty = 254;  
  filled = 255;
  lcd.createChar(1, vert1);
  lcd.createChar(2, vert2);
  lcd.createChar(3, vert3);
  lcd.createChar(4, vert4);
  lcd.createChar(5, vert5);
  lcd.createChar(6, vert6);
  lcd.createChar(7, vert7);
}
///////////////////////////////////////////////////////////////////////////////////////////
// Now follows the actual drawing functions
///////////////////////////////////////////////////////////////////////////////////////////

void gapLessBar(unsigned nr, unsigned total, unsigned firstPos, unsigned lastPos, unsigned line)
// https://robodoupe.cz/2015/progress-bar-pro-arduino-a-lcd-displej/
// framed progress bar, every other segment only, so that gap between characters melts in
// data is displayed in columns firstPos ... lastPos

{
    firstPos = max(firstPos, 0);        // 1...NCOLS-1, first position with data
    lastPos  = min(lastPos,  NCOLS-1);  // ... NCOLS-1, last position with data
    lastPos  = max(lastPos, firstPos+1); // cannot go right to left!!
    line     = max(line,0); line = min(line,NROWS);

    int Nseg = lastPos - firstPos +1 ;  // no of positions to use on LCD (first, ..., last)

    float noOfSubSegments = 3.0; // no of subsegments used per character, out of a total of 5, i.e. 2 gaps
    float segmentNoReal = (float(nr)/float(total) )* (Nseg-1.0/noOfSubSegments); // 1/segmentNoReal 19.12.2023
    int segmentNoInt    = int(segmentNoReal);
    byte subSegmentNo   = byte(noOfSubSegments *(segmentNoReal-segmentNoInt));

    int startPos;

    /*
    Example: 
    total = 111, firstPos = 0, lastPos = 19 => Nseg = 20
    
    nr =   0 => segmentNoReal= 0.0 , segmentNoInt= 0, subSegmentNo=0.
    nr =   1 => segmentNoReal= 0.18, segmentNoInt= 0, subSegmentNo=0.53 -> 0
    nr =   2 => segmentNoReal= 0.35, segmentNoInt= 0, subSegmentNo=1.06 -> 1
    nr =   3 => segmentNoReal= 0.53, segmentNoInt= 0, subSegmentNo=1.59 -> 1
    nr =   4 => segmentNoReal= 0.71, segmentNoInt= 0, subSegmentNo=2.13 -> 2
    nr =   5 => segmentNoReal= 0.89, segmentNoInt= 0, subSegmentNo=2.65 -> 2

    nr =   6 => segmentNoReal= 1.06, segmentNoInt= 1, subSegmentNo=0.19 -> 0

    nr = 111 => segmentNoReal=19.67, segmentNoInt=19, subSegmentNo=2

    */

    // draw on LCD

    // 1: left-hand symbol
    lcd.setCursor(firstPos, line);
    if (segmentNoInt == 0) 
    {
        lcd.write(byte(0)); // initial left-hand symbol, |::
        startPos = firstPos + segmentNoInt;
    }
    else 
    {
        lcd.write(filled);  // filled,                   |||
        startPos = firstPos + segmentNoInt + 1;   // +1 added - removes flickering
    }
    
    // 2: upper and lower frame, after actual segment :::
     {  

     for (int j = startPos ; j < lastPos; j++) // 8.10.2024:  
       {
         lcd.setCursor(j,line);lcd.write(empty);  // :::
       }
     }
     
    // 3: draw 0 ... Nseg completely filled segments

    int jmax = firstPos + segmentNoInt ;
    for (int j = firstPos+1; j < jmax; j++)
      {
       // if (j>0)
        {
        // #ifdef DEBUG
        //   lcd.setCursor(0,3); lcd.print(j);lcd.print(" "); 
        //   lcd.setCursor(6,3); lcd.print(segmentNoInt);lcd.print(" ");
        //   lcd.print(firstPos);lcd.print(" ");lcd.print(lastPos);lcd.print(" ");
        // #endif
        lcd.setCursor(j, line); lcd.write(filled);
        }
      }

     // 4: draw closing, right-hand symbol
    if (segmentNoInt < Nseg-2) 
    {
      lcd.setCursor(lastPos,line); lcd.write(byte(4)); // ::|, final   symbol  
    }

    // 5 draw sub segment
    if (segmentNoInt < Nseg-1)  // nothing if final segment has been filled
    {
      lcd.setCursor(firstPos + segmentNoInt, line);                        
      lcd.write(subSegmentNo); // 0, 1, 2 =|::, ||:, |||
      if (segmentNoInt == Nseg-2) lcd.write(4);           // added 19.12.2023 - only important when counting down. = end-character
    }
    else if (segmentNoInt == Nseg-1)
    {  // 4 = ::|, 5 = |:|, filled = |||
      lcd.setCursor(firstPos + segmentNoInt, line);
      if (subSegmentNo == 0) lcd.write(5);      // |__|
      if (subSegmentNo == 1) lcd.write(filled); // 
      if (subSegmentNo == 2) lcd.write(filled); // |||
     }

    // debug info on screen
    #ifdef DEBUG
      if (line < NROWS)
      {
        lcd.setCursor(firstPos,line+1); lcd.print(segmentNoReal); lcd.print(F("    "));
        lcd.setCursor(firstPos+6,line+1); lcd.print(segmentNoInt); lcd.print(" ");lcd.print(subSegmentNo);lcd.print(F("      "));
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

  int Nseg = lastPos - firstPos + 1;  // no of positions to use on LCD 

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
/////////////////////////////////////////////////////////////////////////////////////////////////////
void verticalBar(int nr, int total, int firstPos, int line)
// Vertical progressbar in 8 steps
// data is displayed in columns firstPos ... lastPos = firstPos+7

{
  firstPos = max(firstPos, 0);        // 1...NCOLS-1, first position with data
  int lastPos  = firstPos + 7; 
  lastPos  = min(lastPos, NCOLS - 1); 
  line = max(line, 0);
  line = min(line, NROWS);

  int Nseg = lastPos - firstPos + 1;  // no of positions to use on LCD

  float segmentNoReal = Nseg * float(nr) / float(total);  //
  int segmentNoInt = int(segmentNoReal);

  // draw on LCD
 
  // Draw 0 ... Nseg completely filled segments
  for (int j = firstPos; j < firstPos + segmentNoInt; j++) {
    lcd.setCursor(j, line);
    if (j < firstPos + 7) lcd.write(j-firstPos+1);
    else                  lcd.write(filled);
  }
  // blank out non-used ones
  for (int j = firstPos+segmentNoInt; j <= lastPos; j++) {
    lcd.setCursor(j, line);
    lcd.write(empty);
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
  }
#endif
}



////////////// S E T U P /////////////////////////////////////////////////////////////////////////////

void setup() {
  // initialize the LCD and allocate the 5 arrays to a number.
  lcd.begin(NCOLS, NROWS);
  pinMode(LCD_PWM, OUTPUT);
  analogWrite(LCD_PWM, backlightVal);
  
  if (NROWS > 2) lcd.setCursor(0,3); lcd.print("github/la3za/Bars ");
  
  //Serial.begin(115200);
}

///////////////////////////////////////////////////////////////////////////
void PrintFixedWidth(Print &out, int number, byte width, char filler = ' ') {
  int temp = number;
  //
  // Sverre Holm 2022
  // call like this to print number to lcd: PrintFixedWidth(lcd, val, 3);
  // or for e.g. minutes PrintFixedWidth(lcd, minute, 2, '0')
  //
  // Default filler = ' ', can also be set to '0' e.g. for clock
  // If filler = ' ', it handles negative integers: width = 5 => '   -2'
  // but not if filler = '0': width = 5 => '000-2'
  //
  // https://forum.arduino.cc/t/print-lcd-text-justify-from-right/398351/5
  // https://forum.arduino.cc/t/u8glib-how-to-display-leading-zeroes/396694/3

  //do we need room for a minus?
  if (number < 0) {
    width--;
  }

  //see how wide the number is
  if (temp == 0) {
    width--;
  }
  else
  {
    while (temp && width) {
      temp /= 10;
      width--;
    }
  }

  //start by printing the rest of the width with filler symbol
  while (width) {
    out.print(filler);
    width--;
  }

  out.print(number); // finally print signed number
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
    case 9:  loadVerticalBarCharacters();       break;
    default: loadSquares();                     break;
  }
  delay(100);   // to avoid flash

  lcd.setCursor(0, line); lcd.print("Bar"); PrintFixedWidth(lcd, framedBar, 2);

  if (flip) ibegin = 0;       // first time:  full loop
  else      ibegin = imax/2;  // second time: start at 50%
  
  lcd.setCursor(1, line+1);lcd.print(" ");
  for (int i = ibegin; i <= imax; i++)  // count up
  { 
    lcd.setCursor(2, line+1); PrintFixedWidth(lcd, i, 3);
    displayBars(i);
    delay(50); // 50
  }
  delay(1000);
  lcd.setCursor(2, line+1);lcd.print(" ");
  for (int i = imax; i >= 0; i--)  // count down
  {
    lcd.setCursor(2, line+1); PrintFixedWidth(lcd, i, 3);
    displayBars(i);
    delay(50); //50
  }

  delay(1000);
  framedBar = framedBar + 1;
  if (framedBar > 9) { framedBar = 0; flip = !flip; }
}

////////////////////////////////////////////////////////////////////
void displayBars(int i)
{
    if (framedBar == 0)                   fullBar(i, imax, col1, col2, line);        // progress bar.
    if (framedBar >= 1 & framedBar <= 3)  gapLessBar(i, imax, col1, col2, line);
    if (framedBar >= 4 & framedBar <= 6)  framedProgressBar(i, imax, col1, col2, line);  // progress bar.
    if (framedBar == 7)                   singleCharacterBar(i, imax, col1, col2, line, 255, 1); // full height square
    if (framedBar == 8)                   singleCharacterBar(i, imax, col1, col2, line, 2, 219); // reduced height square
    if (framedBar == 9)                   verticalBar(i, imax, col1, line);
}
