# Bars
This is a collection of and an implementation of a number of different progressbars or bargraphs for Arduino using customized LCD character sets. Bars can start and stop on user-selectable columns. 
 
The main design principle is that no more than 8 custom characters should be required per bar. That means that the custom character set is uploaded just once for each bar, giving much less probability for wearing out the LCD character memory with its limited number of write cycles. This excludes some fancier bars that require more or less continuous updates of character sets during progression of the bar.


