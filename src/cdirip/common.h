#ifndef __COMMON_H__
#define __COMMON_H__

/* Error codes */

#define ERR_GENERIC   0L
#define ERR_OPENIMAGE 0x01
#define ERR_SAVETRACK 0x02
#define ERR_READIMAGE 0x03
#define ERR_SAVEIMAGE 0x04
#define ERR_PATH      0x05


/* For Debug only! */

// #define DEBUG_CDI



/* Basic structures */

typedef struct image_s
       {
       long               header_offset;
       long               header_position;
       long               length;
       unsigned long      version;
       unsigned short int sessions;
       unsigned short int tracks;
       unsigned short int remaining_sessions;
       unsigned short int remaining_tracks;
       unsigned short int global_current_session;
       } image_s;

typedef struct track_s
       {
       unsigned short int global_current_track;
       unsigned short int number;
       long               position;
       unsigned long      mode;
       unsigned long      sector_size;
       unsigned long      sector_size_value;
       long               length;
       long               pregap_length;
       long               total_length;
       unsigned long      start_lba;
       unsigned char      filename_length;
       } track_s;


/* Functions */

void error_exit(long errcode, char *string);

#endif
