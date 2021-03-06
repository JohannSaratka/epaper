
# Driver for waveshare e-Paper Module

[Waveshare Wiki](https://www.waveshare.com/wiki/1.54inch_e-Paper_Module)
[Waveshare Github](https://github.com/waveshare/e-Paper)

# Hardware Connection

| e-Paper	| MSP-EXP430G2	| Function |
| -------	| ------------	| -------- 	|
|Vcc		| VCC			|3.3V 	|
|GND	| GND			|GND 	|
|DIN	| P1.7			|SPI MOSI|
|CLK	| P1.5			|SPI SCK 	|
|CS		| P1.4			|SPI CS 	|
|DC		| P2.5			|Digital Out |
|RST	| P2.4			|Digital Out |
|BUSY	| P2.3			|Digital In |

# mspdebug
## Start GDB
    opt gdb_loop true
    gdb

## Flash elf file
    prog ./build/bin/app.elf
    run

## Code Size 
   
|text | data | bss | dec | hex | language  | optimization | note|
|--|--|--|--|--|--|--|--|
| 2800 | 4 | 26 | 2830 | b0e | C | -O0 | |
| 1790 | 4 | 26 | 1820 | 71c | C | -O1 | |
| 1710 | 4 | 26 | 1740 | 6cc | C | -Os | |
| | | | | | | | |
| 2442 | 144 | 12 | 2598 | a26| C++ | -O1 | Initial Version |
| 2330 | 144 | 12 | 2486 | 9b6 | C++ | -Os | |
| 1782 | 8 | 30 | 1820 | 71c | C++ | -Os | Compiler flags |

## TODO

- Horizontal lines with line weight > 1px is broken because drawLine uses drawPoint which will overwrite data in frame buffer if points overlap
- Empty circles can have small missing section as each drawn segment can overwrite frame buffer values of previous section. Drawing circle as lines parallel to x-axis would solve this.
- Implement paged drawing as explained here: [pictureloop](https://github.com/olikraus/u8glib/wiki/tpictureloop). This would be computationally more intensive but would also remove any artefacts caused by not having a frame buffer in RAM.  