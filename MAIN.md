MAIN → DISPLAY
===

## What i know for now

|#   |0x01|0x02|0x03|0x04|0x05|0x06|0x07|0x08|0x09|0x0a|0x0b|0x0c|0x0d|0x0e|0x0f|0x10|0x11|CRC |
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
| 00 | PX | PX | PX | PX | PX | PX | PX | PX | PX | 00 | 37 | 00 | 58 | 00 | 00 | 00 | 57 | e6 | 
| 10 | PX | PX | PX | PX | PX | PX | PX | PX | PX | 00 |BITS| 00 | 00 | 00 | 00 | 00 | 57 | 07 | 
| 20 | PX | PX | PX | PX | PX | PX | PX | PX | PX | 00 |TMP3|TMP2|TMP1| 00 | 00 | 00 | 57 | e9 | 
| 30 | PX | PX | PX | PX | PX | PX | PX | PX | PX | 00 |BPM3|BPM2|BPM1| 00 | 00 | 00 | 57 | 35 | 
| 40 | PX | PX | PX | PX | PX | PX | PX | PX | PX | 00 |TRK2|TRK1| 00 | 00 | 00 | 00 | 57 | cf | 
| 50 | 00 | 00 | 00 | 00 | fc | ff | ff | ff | ff | ff | 00 | d0 | 77 | 00 | 00 | 00 | 57 | 95 | 


### CRC

A basic checksum (all information bytes summed % 256).  
Always present.


### Jog Display

`0x11` looks like its the jog display position.  
I guess, like the CDJ1000 it has values which trigger animations (still to find).

### 7 Segment digits

All the 7 segment displays fron the panel are driven as such (1 bit for each segment).  
Here's a basic cheatsheet for bytes applicable to all.

| 0x00 | 0xee | 0x24 | 0xd6 | 0xb6 | 0x3c | 0xba | 0xfa | 0x26 | 0xfe | 0xbe |
|------|------|------|------|------|------|------|------|------|------|------|
|*null*|   0  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |

Later maybe i will make a small schema to link each segment to its respective bit.

Using these i was able to identify the said displays in the payload (`BPM`, `TMP`, `TRK`), and some bits all around (commas, minus, plus, words).

_Note: all digits are driven from right to left. Even pixel display it seems._


### The Bits

There are a bunch of lighty bits all around the display to show words, signs, borders etc.  
Some states are defined in `[0x10][0x0b]`.

Some of them were easy to find, some of them im still searching for :

|	| ,2| ,1|	|MT |WIDE| + | - |
|---|---|---|---|---|----|---|---|
| 1 | 1 | 1 | 1 | 1	| 1  | 1 | 1 |


### Progress Bar

The bottom progress bar (red stuff) goes from `[0x50][0x05]` to `[0x50][0x0a]`.  

_Theory to check: 2 bits for each pixel. `0b01` = Blinking & `0b11` = On_


### Pixels

The whole pixely screen zone (`PX`) seems to be pixel driven.  
Its is composed of 9 5x7 pixels digits.

For now i have not found the pattern and thus don't know how they are driven.


### Maybies

The missing displays are the Play, Cue, Vinyl, Loop In & Out and Autoloop LEDs.  
The MIGHT be in `[0x00][0x0b]` and they seem to be driven internally (blinked by `DISPLAY` panel itself).


## Whole Dumped Frame

This one is on pause, with a CD loaded, on the Text mode, displaying "_(o)_ Mix".

| #    |0x01|0x02|0x03|0x04|0x05|0x06|0x07|0x08|0x09|0x0a|0x0b|0x0c|0x0d|0x0e|0x0f|0x10|0x11|0x12|CRC |
|------|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
| [00] | 00 | 00 | 00 | 00 | 00 | 00 | 00 | 00 | 00 | 00 | 00 | 37 | 00 | 58 | 00 | 00 | 00 | 57 | e6 | 
| [10] | 10 | 00 | 00 | 00 | 00 | 00 | 00 | 00 | 00 | 00 | 00 | b0 | 00 | 00 | 00 | 00 | 00 | 57 | 07 | 
| [20] | 20 | 00 | 00 | 40 | 40 | 44 | 40 | 40 | 44 | 40 | 00 | ee | ee | ee | 00 | 00 | 00 | 57 | e9 | 
| [30] | 30 | 86 | 54 | 54 | 4e | 31 | 85 | 14 | 52 | 6a | 00 | fe | ba | 24 | 00 | 00 | 00 | 57 | 35 | 
| [40] | 40 | 86 | 82 | aa | aa | 8a | aa | aa | 0a | 0a | 00 | 3c | ee | 00 | 00 | 00 | 00 | 57 | cf | 
| [50] | 50 | 00 | 00 | 00 | 00 | fc | ff | ff | ff | ff | ff | 00 | d0 | 77 | 00 | 00 | 00 | 57 | 95 | 
