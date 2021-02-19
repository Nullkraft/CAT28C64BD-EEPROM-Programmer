CAT28C64BD EEPROM Programmer sketch:

The CAT28C64BD is an 8 kB EEPROM chip with a fast 32 byte write feature. This feature allows up to 32 consecutive byte writes to improve throuput.

The page write feature is fully encompassed in the writeEEPROM() function and will accept any page size from 1 to 32 bytes.  The 2 pages of example data, along with the function printContents(), will let you to test your hardware setup.

Direct port manipulation was used so the Arduino could meet the pin timing requirements for performing page writes.

Data pins are on Port A (Mega2560 pins 22-29). Address pins are on Port F (LoByte), and PortK (HiByte) (Mega2560 pins A0-A12).
