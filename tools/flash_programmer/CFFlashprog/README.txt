This is a utility to program NAND flash device via BDM interface.

Usage:
cf

Flash devices:
   NAND, NOR or SER

Flash commands:
   reset, erase, write, read, or verify
-- reset configfile
-- erase configfile offset<hex> size<hex>
-- write configfile offset<hex> size<hex> flags filename
-- read configfile offset<hex> size<hex> flags
-- verify configfile offset<hex> size<hex> flags filename
   Example: cf nand cf.cfg erase 0 20000


The utility program currently only support NAND interface and it has to work under Command
prompt. The sequence for programming the NAND are erase->write/verify. First, the erase 
feature will erase block(s) and detect bad block(s) at the same time and store the bad block(s) offset at M54418 internal SRAM. At later time, this/these bad block(s) will be referred by
write/verify features. If the first erase offset and size are given, it cannot be used at later time for write/verify at different offset and size.

To erase at NAND offset 0 and 4 blocks, size is set to 0x80000. (0x20000 block aligned)

	d:> cf nand erase m54418twr_nand 0 80000

When the actual file size is smaller than a given size (0x80000), the file size will be used.
If a given size is smaller than file size, the given size will be used. The flag set to 1 to
indicate the programmer to program the boot pages (8-bit width bus for the 4 boot pages). If 
it is 0, the 4 pages will become regular 16-bit width bus pages.

	d:> cf nand write m54418twr_nand 0 80000 1 u-boot.bin
