#pragma once


/*
Command	    Argument	            Response	Data	Description
CMD0	    None / Zero	            R1          No      Software reset
ACMD41	    Zero	                R1          No      Initialise SDHC
CM8	        000001AA	            R7          No      Check card type / voltage
CMD12	    None / Zero	            R1          No      Stop reading data
CMD16	    Block length [31:00]	R1          No      Set read / write block length
CMD17	    Address [31:00]	        R1          Yes     Single block read
CMD18    Address [31:00]	        R1          Yes     Multiple block read
ACMD23	    Block quantity [22:00]	R1          No      Set number of blocks to erase
CMD24	    Address [31:00]	        R1          Yes     Single block write
CMD25    Address [31:00]	        R1          Yes     Multiple block write
CMD55	    None / Zero	            R1          No      Sent after a ACMD command
CMD58	    None / Zero	            R3          No      Read OCR
*/


#define COMMAND_BASE 0X40
#define CMD0 COMMAND_BASE+0x0
#define ACMD41 COMMAND_BASE+0x29
#define CMD8 COMMAND_BASE+0x08
#define CMD12 COMMAND_BASE+0x0C
#ifdef USE_SDSC // Command 16 is only used for SDSC cards, SDHC and SDXC cards have fixed block length of 512 bytes
#define CMD16 COMMAND_BASE+0x10
#endif
#define CMD17 COMMAND_BASE+0x11
#define CMD18 COMMAND_BASE+0x12
#define CMD23 COMMAND_BASE+0x17
#define CMD24 COMMAND_BASE+0x18
#define CMD25 COMMAND_BASE+0x19
#define CMD55 COMMAND_BASE+0x37
#define CMD58 COMMAND_BASE+0x3A
