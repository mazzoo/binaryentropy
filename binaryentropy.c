#include <sys/types.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>


#define BUF_SZ 4096
char buf[BUF_SZ];

void usage(void)
{
  printf("usage:\n");
  printf("\tbinaryentropy image\n");
  printf("\n");
  printf("\tdumps the entropy of image\n");
  printf("\tthe output is created by sweeping a 256-byte sliding window over image\n");
  printf("\tand may be used by e.g. gnuplot to visualize\n");
  printf("\n");
  printf("\te.g.:\n");
  printf("\t\t$ binaryentropy image > entropy_of_image.gnuplot\n");
  printf("\t\t$ gnuplot\n");
  printf("\t\tgnuplot> plot \"entropy_of_image.gnuplot\"\n");
  printf("\n");
  printf("\tor in one line, you may\n");
  printf("\t\t$ IMAGE=/bin/ls make plot\n");
  printf("\n");
}

void usage_exit(char * err)
{
  printf("\nERROR: ");
  printf(err);
  printf("\n\n");
  usage();
  exit(1);
}

void dump_bytes(uint8_t * image, int offset, int uniqcount)
{
  int i;
  printf("### %d unique bytes at offset 0x%.8x :\n", uniqcount, offset);
  for (i = 0; i < uniqcount; i++)
  {
    if ( !(i % 16) )
    {
      printf("0x%.8x :  ", offset + i);
    }
    printf("%.2x ", image[i]);
    if ( (i % 16) == 8)
    {
      printf(" ");
    }
    if ( (i % 16) == 15)
    {
      printf("\n");
    }
  }
  if ( uniqcount % 16 )
  {
    printf("\n");
  }
}

int main(int argc, char ** argv)
{
  int ret = 0;

  if (argc != 2)
  {
    usage_exit("one arguments required");
  }

  int fimage;
  int fsize;
  uint8_t * image;

  fimage = open(argv[1], O_RDONLY);
  if (fimage < 0)
  {
    snprintf(buf, BUF_SZ, "cannot open file %s", argv[1]);
    usage_exit(buf);
  }
  fsize = lseek(fimage, 0, SEEK_END);
  if (fimage < 0)
  {
    snprintf(buf, BUF_SZ, "cannot seek file %s", argv[1]);
    usage_exit(buf);
  }
  ret = lseek(fimage, 0, SEEK_SET);
  if (ret < 0)
  {
    snprintf(buf, BUF_SZ, "cannot rewind file %s", argv[1]);
    usage_exit(buf);
  }

  image = malloc(fsize);
  if (!image)
  {
    usage_exit("not enough memory");
  }

  ret = read(fimage, image, fsize);
  if (ret != fsize)
  {
    snprintf(buf, BUF_SZ, "read: expected %d but got %d", fsize, ret);
    usage_exit(buf);
  }
  close(fimage);

  printf("### gathering entrpopy in %s ...\n", argv[1]);

  char uniq[256];
  int uniqcount;

  /* gather */
  int i;
  for (i = 0; i < fsize-256; i++)
  {
    memset(uniq, 0, 256 * sizeof(char));
    uniqcount = 0;

    int j;
    for (j = 0; j <= 256; j++)
    {
      if (!uniq[image[i+j]])
      {
        uniq[image[i+j]] = 1;
        uniqcount++;
      }
    }
    printf("%d\n", uniqcount);
  }

  return 0;
}
