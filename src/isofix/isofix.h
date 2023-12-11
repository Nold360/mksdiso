#if defined(__linux__) || defined(__APPLE__)
  #define stricmp strcasecmp
#endif


typedef enum bool_t { false, true } bool;

struct opts_s {
              bool last_vd;
              bool askforimage;
              bool extractbootonly;
              bool extractheaderonly;
              bool macformat;
              bool isoformat;
              };

static const char PVD_STRING[8] = { 0x01, 0x43, 0x44, 0x30, 0x30, 0x31, 0x01, 0 }; //"\x01" "CD001" "\x01" "\0";
static const char SVD_STRING[8] = { 0x02, 0x43, 0x44, 0x30, 0x30, 0x31, 0x01, 0 }; //"\x02" "CD001" "\x01" "\0";
static const char VDT_STRING[8] = { 0xff, 0x43, 0x44, 0x30, 0x30, 0x31, 0x01, 0 }; //"\xFF" "CD001" "\x01" "\0";
static const char SYNC_DATA[12] = { 0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0 };
static const char SUB_HEADER[8] = { 0, 0, 0x08, 0, 0, 0, 0x08, 0 };

int seek_pvd(char *buffer, int sector_size, int mode, FILE *fsource);
int sector_read(char *buffer, int sector_size, int mode, FILE *fsource);

