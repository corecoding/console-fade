/* Routines to fade the first 16 palette colors to and from black,
   using the Linux console routines.  (See drivers/char/console.c, look
   for ESpalette) */

#include <stdio.h> /* do we need this anymore? */
#include <unistd.h> /* ioperm */

/* the default colour table, for VGA+ colour systems */
int default_red[] = {0x00,0xaa,0x00,0xaa,0x00,0xaa,0x00,0xaa,0x55,0xff,0x55,0xff,0x55,0xff,0x55,0xff};
int default_grn[] = {0x00,0x00,0xaa,0x55,0x00,0x00,0xaa,0xaa,0x55,0x55,0xff,0xff,0x55,0x55,0xff,0xff};
int default_blu[] = {0x00,0x00,0x00,0x00,0xaa,0xaa,0xaa,0xaa,0x55,0x55,0x55,0x55,0xff,0xff,0xff,0xff};

unsigned int steps=64; /* How many steps to fade in.  Can be decreased for
			 less smooth but quicker fading. */

/* Requires two args.  One is boolean (true for fade in, false for fade out),
   second is fading time in ms. A third, if provided, will be used as nubmer 
   of steps.*/

int main(int argc, char *argv[])
{
	if (argc > 3) { steps=atoi(argv[3]); }
	return ( atoi(argv[1]) ? fadein(atoi(argv[2])) : fadeout(atoi(argv[2])) );
}

int fadein(int fadetime)
{
	unsigned long restsize;
	unsigned int i;
	unsigned char j, stepsize, red, green, blue;

	stepsize=256/steps;
	restsize=1000 * fadetime/steps;
	for (i=0; i<=255; i+=stepsize) {
		printf("\e[A\n"); /* Because we get no output w/o newlines */
		for (j=1; j<=15; j++) {
			red=default_red[j]*i/256;
			green=default_grn[j]*i/256;
			blue=default_blu[j]*i/256;
			/* printf("P%1XR%dG%dB%d\t", j, red, green, blue); */
			printf("\e]P%1X%02X%02X%02X",j,\
			 red, green, blue); 
		}
		usleep(restsize);
	}
	printf("\e]R");
}

int fadeout(int fadetime)
{
        unsigned long restsize;
        int i;
        unsigned char j, stepsize, red, green, blue;

        steps=64;
        stepsize=256/steps;
        restsize=1000 * fadetime/steps;
        for (i=256; i>=0; i-=stepsize) {
                printf("\e[A\n"); /* Because we get no output w/o newlines */
                for (j=1; j<=15; j++) {
                        red=default_red[j]*i/256;
                        green=default_grn[j]*i/256;
                        blue=default_blu[j]*i/256;
                        /* printf("P%1XR%dG%dB%d\t", j, red, green, blue); */
                        printf("\e]P%1X%02X%02X%02X",j,\
                         red, green, blue);
                }
                usleep(restsize);
        }
}
