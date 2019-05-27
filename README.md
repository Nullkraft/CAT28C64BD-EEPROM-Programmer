CAT28C64BD EEPROM Programmer sketch:

The CAT28C64BD is an 8 kB EEPROM chip with a fast 32 byte write feature. This feature allows up to 32 consecutive byte writes to improve throuput.

The page write feature is fully encompassed in the writeEEPROM() function and will accept any page size from 1 to 32 bytes.  The 2 pages of example data, in conjunction with the function printContents(), will allow you to test your hardware.

Direct port manipulation was required. Using digitalRead()/digitalWrite() was too slow to meet the timing requirements when performing page writes.

Data pins are on Port A (Mega2560 pins 22-29). Address pins are on Port F (LoByte), and PortK (HiByte) (Mega2560 pins A0-A12).
