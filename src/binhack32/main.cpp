/*
	IP.BIN/BOOT.BIN SB Hacker - 32bit BINHACK Clone - FamilyGuy 2010

	This is my attemp at creating a clone of binhack.exe in a 32-bit executable
	format to be compatible with non-16bit operating system (aka 64bit Win7).
	The Goal is to make it a *clone* so it can be used the same way as the
	original, just rename it BINHACK.EXE and it should works with old batch
	files. It'll also enable the VGA flag in the ip.bin, as there's no good
	reason not to do so.

	Here's what I found BINHACK to actually do to the files!

	What binhack does is writing exactly the same stuff for address:
	3704 -> 63EF	(With some bytes being left intact, probably 0x00)
	Where 639C,639D, 639E & 639F is reserved for the size of the boot.bin in
	bytes, swapped (00 18 38 E0 becomes E0 38 18 00)
	Of course it also make the ip.bin multiregion with the JUE and FOR ***
	Flags!

	It also hacks the boot.bin specified to the specified LBA+166 (in HEX).
	The address to hack is 8 bytes before CD001 in the bootbin, it's an integer.
	If WinCE is detected in the bootbin then it only creates the hacked ip file.

	I find the address to hack the LBA of the bootbin by finding CD001 in it
	and then hacking to the "C" offset-8.
            // To detect WinCE in a bootbin I look for the first two bytes, if it's D61A
            // it's WinCE.
1.0.0.2 FG: I figured out WinCE games doesn't need a LBA hack in the end, only bincon and
            ip.bin's WinCEOS flag set to 0. But bincon changes the first two bytes so it's
            better to detect WinCE-ness by looking at CD001 offset-8, if it's 0D 00 0A 00
            then it's WinCE. To detect if it has been bincon'd and change the flag in
            ip.bin (which binhack doesn't do but it's cool to make it automatic) the first
            two bytes will be looked for, if they're 09 00, it,s bincon'd!

	The goal was not to make the source code pretty, but to make it works.
	However Sizious rewrote it so it now working AND pretty.

	FamilyGuy 2010

	Thanks to Siz! for rewriting my source in a pretty and organized way!
*/

#include "binhack.hpp"

int main( int argc, char *argv[] ) {
	fstream boot;
    ifstream ipbin;
    ofstream iphak;

	unsigned int bootsize, lba;
	string bootname, ipname;

	char iphackbuf[ BOOTSECTOR_SIZE ];

	// Welcome !
	cout 	<< "IP.BIN/BOOT.BIN SB Hacker - 32bit BINHACK Clone - FamilyGuy 2010"
			<< endl << endl;

#ifdef DEBUG
	cout << "[DEBUG] ENABLED" << endl;
#endif

    // Ask stuff

	// This is the name of the binary to hack
    cout << "Enter name of binary: ";
    cin >> bootname;

	// This is the name of the output hacked bootsector, input bootsector always is ip.bin
    cout << "Enter name of bootsector: ";
    cin >> ipname;

	// Opening the IP.BIN
    ipbin.open( BOOTSECTOR_NAME, ios::in );
    if ( ipbin.fail() ) {
        cout << "Error opening ip file." << endl;
        return -1;
    }

// FG: I removed this as original binhack just takes the first 32KB of ip.bin without checking its size.
/*
    // Checking the IP.BIN filesize
	if ( filesize( ipbin ) != BOOTSECTOR_SIZE ) {
		ipbin.close();
		cout  << "Invalid " << BOOTSECTOR_NAME << " file. Size must be exactly "
			  << BOOTSECTOR_SIZE << " bytes." << endl;
		return -5;
	}
*/

	// Opening the BOOT.BIN
	boot.open ( bootname.c_str(), ios::binary | ios::in | ios::out );
	if ( boot.fail() ) {
		ipbin.close();
        cout << "Error opening " << bootname  << "." << endl;
        return -2;
	}

	// Get the BOOT.BIN filesize
	bootsize = filesize( boot );

    unsigned int hackoffset;
	// Getting the CD001 offset
	hackoffset = searchHackOffset( boot, bootsize );

#ifdef DEBUG
	cout 	<< "[DEBUG] filename: " << bootname.c_str() << endl
			<< "[DEBUG] opened: " << boot.is_open() << endl
			<< "[DEBUG] boot.bin filesize= " << bootsize << endl
            << "[DEBUG] Hack Offset: " << hackoffset << endl;
#endif

	// -------------------------------------------------------------------------
	// HACKING THE BOOT.BIN
	// -------------------------------------------------------------------------

	if ( isWinCE( boot, hackoffset ) ) {

		// WinCE Executable

		cout << "Found Windows CE" << endl;

		// SiZ: We don't need to modify the WinCE binary... ?
		// FG: Well original binhack doesn't. We only need to bincon it, and disable WinCEOS flag in ip.bin

	} else {

		// Katana Executable

        // This is the starting LBA of the second session, can be get with msinfo tool
        cout << "Enter msinfo value: ";
        cin >> lba;

		// Changes lba to lba+166 to get the real number to hack the bootbin too.
        lba += 166;

		// Hacking the Katana Binary !
		if ( hackKatanaBootBinary( boot, hackoffset, lba ) ) {
			// Hack OK !

			cout << "File " << bootname << " successfully hacked. " << endl;

		} else {
			// Hack Failed !

			// FG : This error is not in original binhack, instead there's an error dump I don't completely understand
			// SiZ: The error dump isn't a feature, it's a crash exception report done by the compiler library
			//      In clear, the ECHELON's binhacker don't manage invalid boot.bin files

			cout << "Invalid binary file." << endl;
            return -3;
		}

	}

	// -------------------------------------------------------------------------
	// HACKING THE IP.BIN
	// -------------------------------------------------------------------------

    // Copy ip.bin as ipname (called iphak)
    ipbin.read( iphackbuf, BOOTSECTOR_SIZE );
    ipbin.close(); // Closing it there makes it possible to overwrite ip.bin

	// Creating the IP.HAK bootsector file
	iphak.open( ipname.c_str(), ios::trunc | ios::out | ios::binary );
    if ( iphak.fail() ) {
        cout << "Error creating" << ipname << "." << endl;
        return -4;
    }

	// Writing the IP.HAK with the original content of the IP.BIN
    iphak.write( iphackbuf, BOOTSECTOR_SIZE );

	// Hack the bootstrap
	hackBootStrap( iphak, bootsize, boot );

	// Finishing...
    iphak.close();
	boot.close();
    cout << "File " << ipname << " successfully created." << endl;

	return 0;
}
