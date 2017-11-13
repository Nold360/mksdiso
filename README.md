# mksdiso
This toolkit is a collection of free tools & scripts (for Linux), that can be used to create SDISO-Files which can run using Dreamshell on a Sega Dreamcast. Or burn a CDI-Image to disc.

mksdiso itself can be called the "SDISO Multimaker" for Linux. (Just without GUI & Libre)
To burn CDI-Images on Linux, there is also a included script called "burncdi".

## General
This Git-Repository includes:
 - binhack32 (IP.BIN/BOOT.BIN SB Hacker - 32bit BINHACK Clone by FamilyGuy 2010)
 - isofix (ISO LBA fixer 1.3 - (C) 2001 by DeXT)
 - scramble ("scrambler" for 1ST_READ.BIN)
 - cdirip (Ver. 0.6.2 - (C) 2001 by DeXT - Hacked for Linux by me 2012)
 - burncdi (Script for burning CDI-Files using Linux - By milksheik & me)
 - mksdiso (Script to convert a Dir/CDI/ISO to Dreamshell SD-ISO format)


## Dependencies
 - 7zip (for ISO extraction without need of root)
 - genisoimage or mkisofs (to repack ISOs)
 - wodim or cdrecord (optional for mksdiso, used for burning CDI-Files using burncdi)

### For Ubuntu/Debian:
```
$ sudo apt-get install p7zip wodim genisoimage
```

## Installation
### On Ubuntu/Debian
1. Download latest .deb-file from [Releases](https://github.com/Nold360/mksdiso/releases)
2. Install deb
```
$ sudo dpkg -i mksdiso-*-all.deb
```
3. Install dependencies
```
$ sudo apt-get -f install
```

On any other Distribution:
---------------------------------
0. Clone git repo:
```
$ git clone https://github.com/Nold360/mksdiso
$ cd mksdiso
```

1. Copy scripts & binaries into $PATH:
```
$ sudo make install
```

2. Copy mksdiso folder, including required files into your home: 
```
$ cp -r mksdiso ~/.mksdiso
```

4. Run mksdiso
5. Have fun! :)


## Usage
Convert (Katana) myImage.cdi to SDISO:
```
 $ mksdiso -k myImage.cdi
```
Convert (Homebrew) myImage.cdi to SDISO:
```
 $ mksdiso -h myImage.cdi
```
Convert (Katana) ISO to SDISO:
```
 $ mksdiso -k myImage.iso
```
Pack Homebrew-Folder into SDISO:
```
 $ mksdiso -h myFolder/ myGreatApp.iso
```

## Misc
Sources for all the binarys are also included.
Thanks to the creators of all these tools.
