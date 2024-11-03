// BARS:
// Progress bars of various sorts, moving from left to right
// counts up and down, and shows that it can also start in arbitrary position on bar
// uses custom character set for LCD
// 
// Sverre Holm December 2023, github/la3za/Bars
//
// 10 different progress bars
// loadGapLessCharacters7() is an example of LCD characters in PROGMEM to save dynamic memory
//
// Some character sets are stored in PROGMEM in order to save dynamic memory
//
// 17.12.2023: First version
// 06.10.2024: Less flicker in bars 1-3 by modification of gapLessBar().
// 03.11.2024: Added min, round in gapLessBar(), fullBar(), framedProgressBar() for smoother jumps
//             More character sets in PROGMEM
//             Cleaned up and commented code better, put character definitions in Definitions.h

#include <Wire.h>
// Put call required by your LCD here:
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
// size of LCD:
#define NCOLS 20  
#define NROWS 4

#include "Definitions.h"  // character set definitions

//#define DEBUG             // debug info: integer segment and subsegment numbers
//#define SLOW              // slow speed for debug   

byte framedBar = 0;         // initial bar to display (usually 0)

int imax = 50;              // no of units to show in bar (resolution)
int col1 = 7;  // -99       // first column of bar
int col2 = 16; // 299       // end of bar
int line = 0;               // lineno for bar
boolean flip = true;        // flips value of start index (ibegin) for bar
int ibegin;

byte filled;                // address of 'filled' LCD character
byte empty;                 // address of 'empty' LCD character

byte buffer[8];  // temporary storage for PROGMEM characters loading character sets to LCD

#define LCD_PWM 45          // pin for backlight (if needed)
byte backlightVal = 20;     // (0...255) initial backlight value. 
                            //value = 20 works fine for photo: Manual f/3.8, 1/5 sec, ISO100

/////////////////////////////////////////////////////////
/* For each bar there is one load ...() routine to load character set into LCD
   and one routine for actual printing.
   Example, bar 0:
      loadSimpleBarCharacters()
      fullBar(int nr, int total, int firstPos, int lastPos, int line)
*/

/////////////////////////////////////////////////////////
// 1. Routines for loading character sets
/////////////////////////////////////////////////////////

void loadSimpleBarCharacters() {  // Bar 0 characters
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

/////////////////////////////////////////////////////////
void loadGapLessCharacters6() {  // Bar 1, height 6 dots
  lcd.createChar(0, g0);  //0 |::
  lcd.createChar(1, g1);  //1 ||:
  lcd.createChar(2, g2);  //2 |||
  filled = 2;
  lcd.createChar(3, g3);  //3 :::
  empty = 3;
  lcd.createChar(4, g4);  //4 ::|
  lcd.createChar(5, g5);  //5 |:|
}

//////////////////////////////////////////////////////////
void loadGapLessCharacters7()  // Bar 2
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

/////////////////////////////////////////////////////////
void loadGapLessCharacters8() {  // Bar 3, height 8 dots
  lcd.createChar(0, g80);  //0 |::
  lcd.createChar(1, g81);  //1 ||:
  lcd.createChar(2, g82);  //2 |||
  filled = 2;
  lcd.createChar(3, g83);  //3 :::
  empty = 3;
  lcd.createChar(4, g84);  //4 ::|
  lcd.createChar(5, g85);  //5 |:|
}

////////////////////////////////////////////////////////
void loadFramedSimpleBarCharacters()  // Bar 4
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

/////////////////////////////////////////////////////////
void loadCurvedFramedBarCharacters() {  // Bar 5, ( xxxx )
  empty = 0;
  filled = 5;
  memcpy_P(buffer,zeroBar, 8);
  lcd.createChar(empty, buffer);
  memcpy_P(buffer,oneBar, 8);
  lcd.createChar(1, buffer);
  memcpy_P(buffer,twoBar, 8);
  lcd.createChar(2, buffer);
  memcpy_P(buffer,threeBar, 8);
  lcd.createChar(3, buffer);
  memcpy_P(buffer,fourBar, 8);
  lcd.createChar(4, buffer);
  memcpy_P(buffer,fiveBar, 8);
  lcd.createChar(filled, buffer);

  lcd.createChar(6, beg1);
  lcd.createChar(7, end1);
}

/////////////////////////////////////////////////////////
void loadStraightFramedBarCharacters() {  // Bar 6, | xxxx |
  empty = 0;
  filled = 5;
  memcpy_P(buffer,zeroBar, 8);
  lcd.createChar(empty, buffer);
  memcpy_P(buffer,oneBar, 8);
  lcd.createChar(1, buffer);
  memcpy_P(buffer,twoBar, 8);
  lcd.createChar(2, buffer);
  memcpy_P(buffer,threeBar, 8);
  lcd.createChar(3, buffer);
  memcpy_P(buffer,fourBar, 8);
  lcd.createChar(4, buffer);
  memcpy_P(buffer,fiveBar, 8);
  lcd.createChar(filled, buffer);
  memcpy_P(buffer,beg3, 8);
  lcd.createChar(6, buffer);
  memcpy_P(buffer,end3, 8);
  lcd.createChar(7, buffer);
}


/////////////////////////////////////////////////////////
void loadSquares() {  // Bars 7, 8
  lcd.createChar(1, fullSquare);
  lcd.createChar(2, smallFill);
}


/////////////////////////////////////////////////////////
void loadSignalStrengthCharacters()  // Bar 9
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
// 2. Now follows the actual drawing functions
///////////////////////////////////////////////////////////////////////////////////////////

void fullBar(int nr, int total, int firstPos, int lastPos, int line)  // Bar 0
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
  byte subSegmentNo = min(4,(int)round(5 * (segmentNoReal - segmentNoInt)));  // added min, round 2.11.2024

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
    lcd.print("    ");
  }
#endif
}
///////////////////////////////////////////////////////////////////////////////////////////

void gapLessBar(unsigned nr, unsigned total, unsigned firstPos, unsigned lastPos, unsigned line) // Bar 1, 2, 3
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
    byte subSegmentNo   = min((int)noOfSubSegments-1,(byte)round(noOfSubSegments *(segmentNoReal-segmentNoInt)));  // added min, round 2.11.2024 

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
        startPos = firstPos + segmentNoInt + 1;   // +1 added - removes flickering, 06.10.1024
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
        lcd.setCursor(firstPos+6,line+1); lcd.print(segmentNoInt); lcd.print(" ");lcd.print(subSegmentNo);lcd.print(F("    "));
      }
    #endif
}


////////////////////////////////////////////////////////////////////////////////////////////////////////

void framedProgressBar(int nr, int total, int firstPos, int lastPos, int line)  // Bars 4, 5, 6
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
  byte subSegmentNo = min(4,(int)round(5 * (segmentNoReal - segmentNoInt))); // added min, round 2.11.2024

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
    lcd.print("    ");
  }
#endif
}

////////////////////////////////////////////////////////////////////////

void singleCharacterBar(int nr, int total, int firstPos, int lastPos, int line, byte fill, byte open)  // Bars 7, 8
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
    lcd.print("    ");
  }
#endif
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
void signalStrength(int nr, int total, int firstPos, int line)  // Bar 9
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

///////////////////////////////////////////////////////////////////////////
// 3. Auxilliary routine
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



/////////////////////////////////////////////////////////////////////
// 4. setup and loop
/////////////////////////////////////////////////////////////////////

// S E T U P //

void setup() {
  // initialize the LCD and allocate the 5 arrays to a number.
  lcd.begin(NCOLS, NROWS);
  pinMode(LCD_PWM, OUTPUT);
  analogWrite(LCD_PWM, backlightVal);
  
  if (NROWS > 2) lcd.setCursor(0,3); lcd.print("github/la3za/Bars ");
  
  //Serial.begin(115200);
}


///////////////// L O O P ////////////////////

// Prints a number from 0 to 100 & displays the progress bar
// The loop counts up, pauses  then counts down again.
void loop() { 
  lcd.setCursor(0, line); lcd.print("                    ");  // erase old stuff

  //framedBar = 1;  // for test
  //imax = 100;

  // load character sets
  switch (framedBar) {
    case 0:  loadSimpleBarCharacters();         break;   // =====
    case 1:  loadGapLessCharacters6();          break;   // -----
    case 2:  loadGapLessCharacters7();          break;
    case 3:  loadGapLessCharacters8();          break;
    case 4:  loadFramedSimpleBarCharacters();   break;  // |===|
    case 5:  loadCurvedFramedBarCharacters();   break;  // (---) nicest?
    case 6:  loadStraightFramedBarCharacters(); break;  // |---|
    case 9:  loadSignalStrengthCharacters();    break;
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
    delay(50); // 50, delay inside loop
    #ifdef SLOW
      delay(500); 
    #endif  
  }
  delay(1000);
  lcd.setCursor(2, line+1);lcd.print(" ");
  for (int i = imax; i >= 0; i--)  // count down
  {
    lcd.setCursor(2, line+1); PrintFixedWidth(lcd, i, 3);
    displayBars(i);
    delay(50); //50, delay inside loop
    #ifdef SLOW
      delay(500); 
    #endif  
  }

  delay(1000);  // delay between bars

  framedBar = framedBar + 1;
  if (framedBar > 9) { framedBar = 0; flip = !flip; }
}

////////////////////////////////////////////////////////////////////
void displayBars(int i)
{
    if (framedBar == 0)                   fullBar(i, imax, col1, col2, line);        // progress bar.
    if (framedBar >= 1 & framedBar <= 3)  gapLessBar(i, imax, col1, col2, line);
    if (framedBar >= 4 & framedBar <= 6)  framedProgressBar(i, imax, col1-1, col2+1, line);  // progress bar.
    if (framedBar == 7)                   singleCharacterBar(i, imax, col1, col2, line, 255, 1); // full height square
                                                                                                 // Note different last two parameters:
    if (framedBar == 8)                   singleCharacterBar(i, imax, col1, col2, line, 2, 219); // reduced height square. 
    if (framedBar == 9)                   signalStrength(i, imax, col1, line);
}
