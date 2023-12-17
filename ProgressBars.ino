// Progress bars of various sorts
// Sverre Holm Nov 2023
//

// Put call required by your LCD here:
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

// size of LCD:
#define NCOLS  20
#define NROWS   4

//#define DEBUG  // for displaying numbers

byte framedBar = 0; // 0;

//Define LCD character sets  
////////////////////////////////////////
// framed bar: 1, 2

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

//////// beginning-end symbols ///////////////////////////////

 
 // frame type 1: (xxx)
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

  // frame type 1: |xxx|
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

  // frame type 1: (xxx)
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

///////////////////////////////////////////////
// completely filled bar:
//
byte oneFilled[] {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
};

byte twoFilled[] {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000
};

byte threeFilled[] {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100
};

byte fourFilled[] {
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
// only 5 dots high, not the full 8
// https://robodoupe.cz/2015/progress-bar-pro-arduino-a-lcd-displej/
  byte g0[8] = {
    B00000, 
    B00000, 
    B00000, 
    B10101, 
    B10000, 
    B10000, 
    B10000, 
    B10101}; //0 |:: 
  byte g1[8] = {
    B00000, 
    B00000, 
    B00000, 
    B10101, 
    B10100, 
    B10100, 
    B10100, 
    B10101}; //1 ||: 
  byte g2[8] = {
    B00000, 
    B00000, 
    B00000, 
    B10101, 
    B10101, 
    B10101, 
    B10101, 
    B10101}; //2 ||| 
  byte g3[8] = {
    B00000, 
    B00000, 
    B00000, 
    B10101, 
    B00000, 
    B00000, 
    B00000, 
    B10101}; //3 :::
  byte g4[8] = {
    B00000, 
    B00000, 
    B00000, 
    B10101, 
    B00001, 
    B00001, 
    B00001, 
    B10101}; //4 ::| 
  byte g5[8] = {
    B00000, 
    B00000, 
    B00000, 
    B10101, 
    B10001, 
    B10001, 
    B10001, 
    B10101}; //5 |:| 

  byte fillSquare[8]= {
    B11111,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B11111};  

#define LCD_PWM 45
byte    backlightVal    = 100; // (0...255) initial backlight value
byte filled;
byte empty;

///////////////////////////////////////////////////////////////////////////////////////////
void filledSquare()
{
  lcd.createChar(1,fillSquare);
}
///////////////////////////////////////////////////////////////////////////////////////////

void simpleBarCharacters()
{
  empty   = 254; // 0;
  filled  = 255;
  //lcd.createChar(empty, zero);
  lcd.createChar(1, oneFilled);
  lcd.createChar(2, twoFilled);
  lcd.createChar(3, threeFilled);
  lcd.createChar(4, fourFilled);
}
//////////////////////////////////////////////////////////

void simpleBarFramedCharacters()
// filled bars with frame that marks end of bar
// simplified version of  https://www.norwichhackspace.org/blog/post/textstar-serial-lcd-display
{ // | XXX |
  empty   = 254; // 0;
  filled  = 255;
  //lcd.createChar(empty, zero);
  lcd.createChar(1, oneFilled);
  lcd.createChar(2, twoFilled);
  lcd.createChar(3, threeFilled);
  lcd.createChar(4, fourFilled);

  lcd.createChar(6, beg3);
  lcd.createChar(7, end3);
}

///////////////////////////////////////////////////////////////////////////////////////////


void framedBar1Characters()
{    // ( xxxx )
  empty  = 0;
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

void framedBar2Characters()
{   // | xxxx |
  empty  = 0;
  filled = 5;
  lcd.createChar(empty, zeroBar);
  lcd.createChar(1, oneBar);
  lcd.createChar(2, twoBar);
  lcd.createChar(3, threeBar);
  lcd.createChar(4, fourBar);
  lcd.createChar(filled, fiveBar);

  lcd.createChar(6, beg3); // or beg2
  lcd.createChar(7, end3);
}

//////////////////////////////////////////////////////////
void gapLessCharacters()
{
  lcd.createChar(0, g0); //0 |:: 
  lcd.createChar(1, g1); //1 ||:
  lcd.createChar(2, g2); //2 |||
      filled = 2;
  lcd.createChar(3, g3); //3 :::
      empty  = 3;
  lcd.createChar(4, g4); //4 ::| 
  lcd.createChar(5, g5); //5 |:| 
  
}

///////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  // initialize the LCD and allocate the 5 arrays to a number.
  lcd.begin(NCOLS, NROWS);  
  pinMode(LCD_PWM, OUTPUT);
  analogWrite(LCD_PWM, backlightVal);

  //Serial.begin(115200);
}

///////////////////////////////////////////////////////////////////////////////////////////

 void updateProgressBarFramed(unsigned nr, unsigned total, unsigned firstPos, unsigned lastPos, unsigned line)
 // framed progress bar
 // data is displayed in columns firstPos ... lastPos,
 // and frame ends on firstPos-1, lastPos+1

 {
    firstPos = max(firstPos, 0);        // 1...NCOLS-1, first position with data
    lastPos  = min(lastPos,  NCOLS-1);  // ... NCOLS-2, last position with data
    line     = max(line,0); line = min(line,NROWS);

    int Nseg = lastPos - firstPos -1 ;  // no of positions to use on LCD (first, last = frame)

    float segmentNoReal = Nseg*float(nr)/float(total);//
    int segmentNoInt    = int(segmentNoReal);
    byte subSegmentNo   = int(5*(segmentNoReal-segmentNoInt));

    // draw on LCD

    // First: left-hand symbol
      lcd.setCursor(firstPos, line); lcd.write(byte(6)); // initial symbol
    
    // Second: upper and lower frame
      for (int j = firstPos+1; j < lastPos; j++)
        {
          lcd.setCursor(j,line);lcd.write(byte(empty));  // over- and underbar - or- empty
        }
    
    // Third: draw closing, right-hand symbol
      lcd.setCursor(lastPos,  line);lcd.write(byte(7)); // final   symbol  

    // Fourth draw 0 ... Nseg completely filled segments
    for (int j = firstPos+1; j <= firstPos + segmentNoInt; j++)
    {
       lcd.setCursor(j, line); lcd.write(filled);
    }

    // Third draw sub segment
    if (segmentNoInt != Nseg)  // nothing if final segment has been filled
    {
      lcd.setCursor(firstPos + 1 + segmentNoInt, line);
      if (subSegmentNo == 0)  lcd.write(empty);
      else                    lcd.write(subSegmentNo); // 1...4
    }
   
    
    // debug info on screen
    #ifdef DEBUG
      if (line < NROWS)
      {
        lcd.setCursor(firstPos,line+1); lcd.print(segmentNoReal); lcd.print("    ");
        lcd.setCursor(firstPos+6,line+1); lcd.print(segmentNoInt); lcd.print(" ");lcd.print(subSegmentNo);lcd.print("      ");
      }
    #endif
 }

///////////////////////////////////////////////////////////////////////////////////////////

void updateProgressBar(unsigned nr, unsigned total, unsigned firstPos, unsigned lastPos, unsigned line)
// Plain progressbar
// data is displayed in columns firstPos ... lastPos

 {
    firstPos = max(firstPos, 0);        // 1...NCOLS-1, first position with data
    lastPos  = min(lastPos,  NCOLS-1);  // ... NCOLS-2, last position with data
    line     = max(line,0); line = min(line,NROWS);

    int Nseg = lastPos - firstPos + 1;  // no of positions to use on LCD

    float segmentNoReal = Nseg*float(nr)/float(total);//
    int segmentNoInt    = int(segmentNoReal);
    byte subSegmentNo   = int(5*(segmentNoReal-segmentNoInt));

    // draw on LCD

    // 1. Empty frame
    for (int j = firstPos; j <=lastPos; j++)
        {
          lcd.setCursor(j,line);lcd.write(byte(empty));  // over- and underbar - or- empty
        }
    // 2. Draw 0 ... Nseg completely filled segments
    for (int j = firstPos; j < firstPos + segmentNoInt; j++)
    {
       lcd.setCursor(j, line); lcd.write(filled);
    }

    // 3. Draw sub segment
    if (segmentNoInt != Nseg)  // nothing if final segment has been filled
    {
      lcd.setCursor(firstPos + segmentNoInt, line);
      if (subSegmentNo == 0)  lcd.write(empty);        // 0 
      else                    lcd.write(subSegmentNo); // 1...4
    }

    // debug info on screen
    #ifdef DEBUG
      if (line < NROWS)
      {
        lcd.setCursor(firstPos,line+1); lcd.print(segmentNoReal); lcd.print("    ");
        lcd.setCursor(firstPos+6,line+1); lcd.print(segmentNoInt); lcd.print(" ");lcd.print(subSegmentNo);lcd.print("      ");
      }
    #endif
    
 }

 
////////////////////////////////////////////////////////////////////////


 void updateProgressBarFramedStandard(unsigned nr, unsigned total, unsigned firstPos, unsigned lastPos, unsigned line)
 // framed progress bar with standard characters
 // data is displayed in columns firstPos ... lastPos,
 // and frame ends on firstPos-1, lastPos+1

 {
    firstPos = max(firstPos, 0);        // 1...NCOLS-1, first position with data
    lastPos  = min(lastPos,  NCOLS-1);  // ... NCOLS-2, last position with data
    line     = max(line,0); line = min(line,NROWS);

    int Nseg = lastPos - firstPos + 1 ;  // no of positions to use on LCD (first, last = frame)

    float segmentNoReal = Nseg*float(nr)/float(total);//
    int segmentNoInt    = int(segmentNoReal);

    // draw on LCD

    
    // First: left-hand symbol
    //  lcd.setCursor(firstPos, line); lcd.print(" ");  // initial symbol
    
    // Second: upper and lower frame
      for (int j = firstPos; j <= lastPos; j++)
        {
          lcd.setCursor(j,line); 
 //         lcd.write((char)165);  // dot// lcd.print(" ");
 //       lcd.write((char)219);    // 6x8 square
            lcd.write(1);          // 8x8 square  
        }
    
    // Third: draw closing, right-hand symbol
    //  lcd.setCursor(lastPos,  line);lcd.print(" "); // final   symbol  

    // Fourth draw 0 ... Nseg completely filled segments
    for (int j = firstPos; j <= firstPos + segmentNoInt - 1; j++)
    {
       lcd.setCursor(j, line); lcd.write((char)255); //lcd.write((char)219); //square // lcd.print(">");
    }
    
    // debug info on screen
    #ifdef DEBUG
      if (line < NROWS)
      {
        lcd.setCursor(firstPos,line+1); lcd.print(segmentNoReal); lcd.print("    ");
        lcd.setCursor(firstPos+6,line+1); lcd.print(segmentNoInt); lcd.print("      ");
      }
    #endif
 }

   
/////////////////////////////////////

void updateProgress3(unsigned nr, unsigned total, unsigned firstPos, unsigned lastPos, unsigned line)
// https://robodoupe.cz/2015/progress-bar-pro-arduino-a-lcd-displej/
// framed progress bar, every other segment only
// data is displayed in columns firstPos ... lastPos,

{
    firstPos = max(firstPos, 0);        // 1...NCOLS-1, first position with data
    lastPos  = min(lastPos,  NCOLS-1);  // ... NCOLS-1, last position with data
    lastPos  = max(lastPos, firstPos+1); // cannot go right to left!!
    line     = max(line,0); line = min(line,NROWS);

    int Nseg = lastPos - firstPos +1 ;  // no of positions to use on LCD (first, ..., last)

    float segmentNoReal = (float(nr)/float(total) )* Nseg;//
    int segmentNoInt    = int(segmentNoReal);
    byte subSegmentNo   = int(3*(segmentNoReal-segmentNoInt));

    // draw on LCD

    // 1: left-hand symbol
    lcd.setCursor(firstPos, line);
    if (segmentNoInt == 0) lcd.write(byte(0)); // initial left-hand symbol, |::
    else                   lcd.write(filled);  // filled,                   |||
    
    // 2: upper and lower frame :::
    {  
    for (int j = firstPos + segmentNoInt; j < lastPos; j++) // 
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
    }
    else if (segmentNoInt == Nseg-1)
    {  // 4 = ::|, 5 = |:|, filled = |||
      lcd.setCursor(firstPos + segmentNoInt, line);
      if (subSegmentNo == 0) lcd.write(5);      // 
      if (subSegmentNo == 1) lcd.write(filled); // 
      if (subSegmentNo == 2) lcd.write(filled); // |||
     }

    // debug info on screen
    #ifdef DEBUG
      if (line < NROWS)
      {
        lcd.setCursor(firstPos,line+1); lcd.print(segmentNoReal); lcd.print("    ");
        lcd.setCursor(firstPos+6,line+1); lcd.print(segmentNoInt); lcd.print(" ");lcd.print(subSegmentNo);lcd.print("      ");
      }
    #endif
}

/////////////////////////////////////

// Print a number from 0 to 100 & displays the progress bar
// The loop counts up, pauses  then counts down again.
void loop() {
  int imax = 100;
  int col1 = 0;
  int col2 = 13;
  unsigned line = 1;

lcd.setCursor(0,line); lcd.print("                    "); // erase old stuff

// load character sets
  if (framedBar == 0) filledSquare();
  if (framedBar == 1) gapLessCharacters();          // -----
  if (framedBar == 2) simpleBarCharacters();        // =====

  // Framed bar, i.e. no data in first and last position:
  if (framedBar == 3) simpleBarFramedCharacters();  // |===|
  if (framedBar == 4) framedBar1Characters();       // (---) nicest?
  if (framedBar == 5) framedBar2Characters();       // |---|
  
  // Ideas: 
  // ordinary characterset: (==== **) using centered dot, () or || as frame 
  
  delay(100); 

  lcd.setCursor(0,0);
  lcd.print("Bar "); lcd.print(framedBar); lcd.print(":");
  
  for(int i=imax/2; i <= imax; i++)                              // count up
  {
    lcd.setCursor(7,0);
    lcd.print(i);
    lcd.print("   ");
  // lcd.setCursor(0,line); lcd.print(framedBar);

   if (framedBar==0) updateProgressBarFramedStandard(i, imax, col1, col2, line);
   if (framedBar==1) updateProgress3(i, imax, col1, col2, line);
   if (framedBar==2) updateProgressBar(i, imax, col1, col2, line);   // progress bar. 
   if (framedBar>=3) updateProgressBarFramed(i, imax, col1, col2, line);   // progress bar. 
   delay(50); 
  }
  delay(1000);
  for(int i=imax; i >= 0; i--)                              // count down
  {
    lcd.setCursor(7,0);
    lcd.print(i);
    lcd.print("   ");

   if (framedBar==0) updateProgressBarFramedStandard(i, imax, col1, col2, line);
   if (framedBar==1) updateProgress3(i, imax, col1, col2, line);
   if (framedBar==2) updateProgressBar(i, imax, col1, col2, line);   // progress bar. 
   if (framedBar>=3) updateProgressBarFramed(i, imax, col1, col2, line);   // progress bar. 
  delay(50); //50
  }
  
  delay(1000);
  framedBar = framedBar + 1;
  if (framedBar > 5) framedBar = 0;
}
