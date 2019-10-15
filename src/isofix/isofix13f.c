/******************************************************/
/*                                                    */
/*            ISO start LBA fixing utility            */
/*                                                    */
/* This tool will take an ISO image with PVD pointing */
/* to bad DR offset and add padding data so actual DR */
/* gets located in right absolute address.            */
/*                                                    */
/* Original boot area, PVD, SVD and VDT are copied to */
/* the start of new, fixed ISO image.                 */
/*                                                    */
/* Supported input image formats are: 2048, 2336,     */
/* 2352 and 2056 bytes per sector. All of them are    */
/* converted to 2048 bytes per sector when writing    */
/* excluding 2056 format which is needed by Mac users.*/
/*                                                    */
/******************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "isofix.h"


int main( int argc, char **argv )
{
	int sector_size = 2048, mode = 1;
	long image_length, remaining_length, progress, last_pos, start_lba, i;
	char lba_parameter = 0;

	char destfname[256];
	char string[256];
	char buffer[4096];

	struct opts_s opts = { false, false, false, false, false };

	FILE *fsource, *fdest, *fboot, *fheader;


printf("ISO LBA fixer 1.3 - (C) 2001 by DeXT\n\n");

// process command line

strcpy(destfname,"fixed.iso");

if (argc < 2)
   opts.askforimage = true;
else
   {
   for (i = 1; i < argc; i++)
       {
       if (atol(argv[i]) <= 0)    // not a number, or negative one
          {
          if (!stricmp(argv[i], "/boot"))
             {
             opts.extractbootonly = true;
             if (i == 1) opts.askforimage = true;
             }
          else if (!stricmp(argv[i], "/header"))
             {
             opts.extractheaderonly = true;
             if (i == 1) opts.askforimage = true;
             }
          else if (!stricmp(argv[i], "/mac"))
             {
             opts.macformat = true;
             if (i == 1) opts.askforimage = true;
             }
          else if (!stricmp(argv[i], "/iso"))
             {
             opts.isoformat = true;
             if (i == 1) opts.askforimage = true;
             }
          else
             {
             if (i == 1) strcpy(string, argv[i]);
             else strcpy(destfname, argv[i]);
             }
          }
       else lba_parameter = i;
       }
   }

if (opts.askforimage == true)
   {
   printf("\nPlease enter name of image to fix (Return to exit): ");
   fgets(string, 256-1, stdin);
   if (strlen(string) == 0) exit(1);
   }

printf("Processing file: '%s'\n",string);

fsource = fopen(string,"rb");

if (fsource == NULL) { printf("Image not found!"); exit(1); }

fseek(fsource, 0L, SEEK_END);
image_length = ftell(fsource);
fseek(fsource, 0L, SEEK_SET);

// detect format

printf("Scanning image...\n");

fread(buffer, 1, 16, fsource);
if (!memcmp(SYNC_DATA, buffer, 12)) // raw (2352)
   {
   sector_size = 2352;
   switch(buffer[15])
         {
         case 2: mode = 2; break;
         case 1: mode = 1; break;
         default: { printf("Unsupported track mode (%d)", buffer[15]); exit (1); }
         }
   if (seek_pvd(buffer, 2352, mode, fsource) == 0) { printf("Could not find PVD!\n"); exit(1); }
   }
else if (seek_pvd(buffer, 2048, 1, fsource))
   {
   sector_size = 2048;
   mode = 1;
   }
else if (seek_pvd(buffer, 2336, 2, fsource))
   {
   sector_size = 2336;
   mode = 2;
   }
else if (seek_pvd(buffer, 2056, 2, fsource))
   {
   sector_size = 2056;
   mode = 2;
   opts.macformat = true;
   }
else { printf("Could not find PVD!\n"); exit(1); }

if (opts.isoformat == true) opts.macformat = false;

printf("sector size = %d, mode = %d\n", sector_size, mode);

if (opts.extractbootonly == false && opts.extractheaderonly == false)
   {
   if (lba_parameter != 0)
      start_lba = atol(argv[lba_parameter]);
   else
      {
      printf("\nPlease enter starting LBA value: ");
      fgets(string, 256-1, stdin);
      start_lba = atol(string);
      if (start_lba <= 0) { printf("Bad LBA value"); exit(1); }
      }

   printf("Creating destination file '%s'...\n",destfname);
   fdest = fopen(destfname, "wb");
   if (fdest == NULL) { printf("Can't open destination file!"); exit(1); }
   }

if (opts.extractheaderonly == false || (opts.extractheaderonly == true && opts.extractbootonly == true))
   {
   printf("Saving boot area to file 'bootfile.bin'...\n");
   fboot = fopen("bootfile.bin", "wb");
   }
if (opts.extractbootonly == false || (opts.extractheaderonly == true && opts.extractbootonly == true))
   {
   printf("Saving ISO header to file 'header.iso'...\n");
   fheader = fopen("header.iso", "wb");
   }

// save boot area

fseek(fsource, 0L, SEEK_SET);
for (i = 0; i < 16 ; i++)
    {
    sector_read(buffer, sector_size, mode, fsource);
    if (opts.extractbootonly == false && opts.extractheaderonly == false)
       {
       if (opts.macformat == true) fwrite (SUB_HEADER, 8, 1, fdest);
       fwrite(buffer, 2048, 1, fdest);
       }
    if (opts.extractheaderonly == false || (opts.extractheaderonly == true && opts.extractbootonly == true))
       {
       if (opts.macformat == true) fwrite (SUB_HEADER, 8, 1, fboot);
       fwrite(buffer, 2048, 1, fboot);
       }
    if (opts.extractbootonly == false || (opts.extractheaderonly == true && opts.extractbootonly == true))
       {
       if (opts.macformat == true) fwrite (SUB_HEADER, 8, 1, fheader);
       fwrite(buffer, 2048, 1, fheader);
       }
    }

if (opts.extractheaderonly == false || (opts.extractheaderonly == true && opts.extractbootonly == true))
   fclose(fboot);
if (opts.extractbootonly == true && opts.extractheaderonly == false) exit(0); // boot saved, exit

// seek & copy pvd etc.

last_pos = ftell(fsource); // start of pvd

do {
   sector_read(buffer, sector_size, mode, fsource);

   if      (!memcmp(PVD_STRING, buffer, 8)) { printf("Found PVD at sector %ld\n", last_pos/sector_size); }
   else if (!memcmp(SVD_STRING, buffer, 8)) { printf("Found SVD at sector %ld\n", last_pos/sector_size); }
   else if (!memcmp(VDT_STRING, buffer, 8)) { printf("Found VDT at sector %ld\n", last_pos/sector_size); opts.last_vd = true; }
   else    { printf("Error: Found unknown Volume Descriptor"); exit(1); }

   if (opts.extractbootonly == false && opts.extractheaderonly == false)
      {
      if (opts.macformat == true) fwrite (SUB_HEADER, 8, 1, fdest);
      fwrite(buffer, 2048, 1, fdest);
      }
   if (opts.macformat == true) fwrite (SUB_HEADER, 8, 1, fheader);
   fwrite(buffer, 2048, 1, fheader);
   last_pos = ftell(fsource);
   }
   while (opts.last_vd == false);

// add padding data to header file

for(i = 0; i < (long)sizeof(buffer); i++)  // clear buffer
   buffer[i] = 0;

remaining_length = 300 - (last_pos/sector_size);
for(i = 0; i < remaining_length; i++)
   {
   if (opts.macformat == true) fwrite (SUB_HEADER, 8, 1, fheader);
   fwrite(buffer, 2048, 1, fheader);
   }

fclose(fheader);
if (opts.extractheaderonly == true) exit(0); // header saved, exit

// add padding data to iso image

if (last_pos > start_lba*sector_size)
   { printf("Sorry, LBA value is too small...\nIt should be at least %d for current ISO image (probably greater)", last_pos/sector_size); exit(1); }
if (start_lba < 11700)
   printf("Warning! LBA value should be greater or equal to 11700 for multisession images\n");

printf("Adding padding data up to start LBA value...");

remaining_length = start_lba - (last_pos/sector_size);

printf("   ");
for(i = 0; i < remaining_length; i++)
   {
   if (!(i%512))
      {
      progress = i*100/remaining_length;
      printf("\b\b\b%02d%%", progress);
      }
   if (opts.macformat == true) fwrite (SUB_HEADER, 8, 1, fdest);
   if (!fwrite(buffer, 2048, 1, fdest))  { printf("\nCan't write file! (disk full?)"); exit(1); }
   }
printf("\b\b\b   \b\b\b");

printf("Done!\n");

// append original iso image

printf("Appending ISO image...");

fseek(fsource, 0L, SEEK_SET);

remaining_length = image_length/sector_size;

printf("   ");
for(i = 0; i < remaining_length; i++)
   {
   if (!(i%512))
      {
      progress = i*100/remaining_length;
      printf("\b\b\b%02d%%", progress);
      }
      if (!sector_read(buffer, sector_size, mode, fsource)) { printf("\nCan't read image!"); exit(1); }
      if (opts.macformat == true) fwrite (SUB_HEADER, 8, 1, fdest);
      if (!fwrite(buffer, 2048, 1, fdest))  { printf("\nCan't write file! (disk full?)"); exit(1); }
   }
printf("\b\b\b   \b\b\b");

printf("Done!\n");

printf("\nImage is now fixed!\n");

fclose(fsource);
fclose(fdest);

return 0;
}


//////////////////////////////////////////////////////////////////////////////

// seek_pvd() will search for valid PVD in sector 16 of source image

int seek_pvd(char *buffer, int sector_size, int mode, FILE *fsource)
{
   fseek(fsource, 0L, SEEK_SET);   
   fseek(fsource, 16*sector_size, SEEK_CUR);              // boot area
   if (sector_size == 2352) fseek(fsource, 16, SEEK_CUR); // header
   if (mode == 2) fseek(fsource, 8, SEEK_CUR);            // subheader
   fread(buffer, 1, 8, fsource);
   if (!memcmp(PVD_STRING, buffer, 8)) return 1;

return 0;
}

// sector_read() will put user data into buffer no matter the source format

int sector_read(char *buffer, int sector_size, int mode, FILE *fsource)
{
int status;

   if (sector_size == 2352) fseek(fsource, 16, SEEK_CUR); // header
   if (mode == 2) fseek(fsource, 8, SEEK_CUR);            // subheader
   status = fread(buffer, 2048, 1, fsource);
   if (sector_size >= 2336)
      {
      fseek(fsource, 280, SEEK_CUR);
      if (mode == 1) fseek(fsource, 8, SEEK_CUR);
      }

return status;
}

