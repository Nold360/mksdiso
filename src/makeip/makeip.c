#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NUM_FIELDS 11

struct field;

int check_areasym(char *, struct field *);

struct field {
  char *name;
  int pos;
  int len;
  int (*extra_check)(char *, struct field *);
} fields[NUM_FIELDS] = {
  { "Hardware ID", 0x0, 0x10, NULL },
  { "Maker ID", 0x10, 0x10, NULL },
  { "Device Info", 0x20, 0x10, NULL },
  { "Area Symbols", 0x30, 0x8, check_areasym },
  { "Peripherals", 0x38, 0x8, NULL },
  { "Product No", 0x40, 0xa, NULL },
  { "Version", 0x4a, 0x6, NULL },
  { "Release Date", 0x50, 0x10, NULL },
  { "Boot Filename", 0x60, 0x10, NULL },
  { "SW Maker Name", 0x70, 0x10, NULL },
  { "Game Title", 0x80, 0x80, NULL },
};

int filled_in[NUM_FIELDS];

int check_areasym(char *ptr, struct field *f)
{
  int i, a = 0;
  for(i=0; i<f->len; i++)
    switch(ptr[i]) {
     case 'J':
       a |= (1<<0);
       break;
     case 'U':
       a |= (1<<1);
       break;
     case 'E':
       a |= (1<<2);
       break;
     case ' ':
       break;
     default:
       fprintf(stderr, "Unknown area symbol '%c'.\n", ptr[i]);
       return 0;
    }
  for(i=0; i<f->len; i++)
    if((a & (1<<i)) == 0)
      ptr[i] = ' ';
    else
      ptr[i] = "JUE"[i];
  return 1;
}

void trim(char *str)
{
  int l = strlen(str);
  while(l>0 && (str[l-1] == '\r' || str[l-1] == '\n' ||
		str[l-1] == ' ' || str[l-1] == '\t'))
    str[--l]='\0';
}

int parse_input(FILE *fh, char *ip)
{
  static char buf[80];
  int i;
  memset(filled_in, 0, sizeof(filled_in));
  while(fgets(buf, sizeof(buf), fh)) {
    char *p;
    trim(buf);
    if(*buf)
      if((p = strchr(buf, ':'))) {
	*p++ = '\0';
	trim(buf);
	for(i=0; i<NUM_FIELDS; i++)
	  if(!strcmp(buf, fields[i].name))
	    break;
	if(i>=NUM_FIELDS) {
	  fprintf(stderr, "Unknown field \"%s\".\n", buf);
	  return 0;
	}
	memset(ip+fields[i].pos, ' ', fields[i].len);
	while(*p == ' ' || *p == '\t')
	  p++;
	if(strlen(p)>fields[i].len) {
	  fprintf(stderr, "Data for field \"%s\" is too long.\n", fields[i]);
	  return 0;
	}
	memcpy(ip+fields[i].pos, p, strlen(p));
	if(fields[i].extra_check!=NULL &&
	   !(*fields[i].extra_check)(ip+fields[i].pos, &fields[i]))
	  return 0;
	filled_in[i] = 1;
      } else {
	fprintf(stderr, "Missing : on line.\n");
	return 0;
      }
  }

  for(i=0; i<NUM_FIELDS; i++)
    if(!filled_in[i]) {
      fprintf(stderr, "Missing value for \"%s\".\n", fields[i]);
      return 0;
    }
      
  return 1;
}

int calcCRC(const unsigned char *buf, int size)
{
  int i, c, n = 0xffff;
  for (i = 0; i < size; i++)
  {
    n ^= (buf[i]<<8);
    for (c = 0; c < 8; c++)
      if (n & 0x8000)
        n = (n << 1) ^ 4129;
      else
        n = (n << 1);
  }
  return n & 0xffff;
}

void update_crc(char *ip)
{
  int n = calcCRC((unsigned char *)(ip+0x40), 16);
  char buf[5];
  sprintf(buf, "%04X", n);
  if(memcmp(buf, ip+0x20, 4)) {
    printf("Setting CRC to %s (was %.4s)\n", buf, ip+0x20);
    memcpy(ip+0x20, buf, 4);
  }
}

void makeip(char *ip_tmpl, char *in, char *out)
{
  static char ip[0x8000];
  FILE *fh = fopen(ip_tmpl, "rb");
  if(fh == NULL) {
    fprintf(stderr, "Can't open \"%s\".\n", ip_tmpl);
    exit(1);
  }
  if(fread(ip, 1, 0x8000, fh) != 0x8000) {
    fprintf(stderr, "Read error.\n");
    exit(1);
  }
  fclose(fh);
  fh = fopen(in, "r");
  if(fh == NULL) {
    fprintf(stderr, "Can't open \"%s\".\n", in);
    exit(1);
  }
  if(!parse_input(fh, ip))
    exit(1);
  fclose(fh);
  update_crc(ip);
  fh = fopen(out, "wb");
  if(fh == NULL) {
    fprintf(stderr, "Can't open \"%s\".\n", out);
    exit(1);
  }
  if(fwrite(ip, 1, 0x8000, fh) != 0x8000) {
    fprintf(stderr, "Write error.\n");
    exit(1);
  }
  fclose(fh);
}

int main(int argc, char *argv[])
{
  char *ip_tmpl;

  if(argc != 3) {
    fprintf(stderr, "Usage: %s ip.txt IP.BIN\n", argv[0]);
    exit(1);
  }

  ip_tmpl = getenv("IP_TEMPLATE_FILE");
  if(ip_tmpl == NULL)
    ip_tmpl = "IP.TMPL";

  makeip(ip_tmpl, argv[1], argv[2]);

  return 0;
}
