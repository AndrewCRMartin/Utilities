Return-Path: <"BIOPD@UK.AC.OXFORD.VAX"@nimsn01>
Received: from uk.ac.mrc.nimr (nimsn01) by nimsn41. (4.0/SMI-4.0)
	id AA05735; Tue, 10 Apr 90 17:47:26 BST
Via:  uk.ac.mrc.nimr; Tue, 10 Apr 90 17:48:08 BST
Message-Id: <9004101648.AA00658@uk.ac.mrc.nimr>
Via:        oxford.vax    ; 10 Apr 1990 17:48:05-BST
Date:           Tue, 10 Apr 90  17:50 GMT
From: BIOPD@UK.AC.OXFORD.VAX
To: A-MARTIN@UK.AC.MRC.NATIONAL-INSTITUTE-FOR-MEDICAL-RESEARCH
Status: RO

#ifndef lint
static char sccsid[] = "@(#)uuencode.c	5.3-1 (Berkeley) 1/22/85";
#endif
/* modified by ajr to include checksums */
/* modified by Mark Hobart NPDSA to take out checksums. If you wan't them
*  compile with CHECKSUMS defined - Nov 89
*/

/*
 * uuencode [input] output
 *
 * Encode a file so it can be mailed to a remote system.
 */
#include <stdio.h>

#ifdef unix
#include <sys/types.h>
#include <sys/stat.h>
#endif

/* Used by checksums but I couldn't be bothered taking it out */
#define SUMSIZE 64

/* ENC is the basic 1 character encoding function to make a char printing */
#define ENC(c) ((c) ? ((c) & 077) + ' ': '`')

main(argc, argv)
char **argv;
{
	FILE *in;
#ifdef unix
	struct stat sbuf;
#endif
	int mode;

	/* optional 1st argument */
	if (argc > 2) {
		if ((in = fopen(argv[1], "r")) == NULL) {
			perror(argv[1]);
			exit(1);
		}
		argv++; argc--;
	} else
		in = stdin;

	if (argc != 2) {
		printf("Usage: uuencode [infile] remotefile\n");
		exit(2);
	}

#ifdef unix
	/* figure out the input file mode */
	fstat(fileno(in), &sbuf);
	mode = sbuf.st_mode & 0777;
#else
	mode = 0777;
#endif
	printf("begin %o %s\n", mode, argv[1]);

	encode(in, stdout);

	printf("end\n");
	exit(0);
}

/*
 * copy from in to out, encoding as you go along.
 */
encode(in, out)
FILE *in;
FILE *out;
{
	char buf[80];
	int i, n;
#ifdef CHECKSUMS
	int checksum;
#endif
	for (;;) {
		/* 1 (up to) 45 character line */
		n = fr(in, buf, 45);

		putc(ENC(n), out);
#ifdef CHECKSUMS
		checksum = 0;
		for (i=0; i<n; i += 3)
			checksum = (checksum+outdec(&buf[i], out)) % SUMSIZE;

		putc(ENC(checksum), out);
#else
		for (i=0; i<n; i += 3)
			outdec(&buf[i], out);
#endif
		putc('\n', out);
		if (n <= 0)
			break;
	}
}

/*
 * output one group of 3 bytes, pointed at by p, on file f.
 * return the checksum increment.
 */
int outdec(p, f)
char *p;
FILE *f;
{
	int c1, c2, c3, c4;

	c1 = *p >> 2;
	c2 = (*p << 4) & 060 | (p[1] >> 4) & 017;
	c3 = (p[1] << 2) & 074 | (p[2] >> 6) & 03;
	c4 = p[2] & 077;
	putc(ENC(c1), f);
	putc(ENC(c2), f);
	putc(ENC(c3), f);
	putc(ENC(c4), f);

#ifdef CHECKSUMS
	return((p[0]+p[1]+p[2]) % SUMSIZE);
#else
	/* In the non-checksums version this value is ignored anyway */
	return((p[0]+p[1]+p[2]));
#endif
}

/* fr: like read but stdio */
int
fr(fd, buf, cnt)
FILE *fd;
char *buf;
int cnt;
{
	int c, i;

	for (i=0; i<cnt; i++) {
		c = getc(fd);
		if (c == EOF)
			return(i);
		buf[i] = c;
	}
	return (cnt);
}

#if (!unix && MANX)

perror (sp)
char *sp;
{
	if (sp && *sp) {
		fprintf (stderr, "%s: ");
	}
	fprintf (stderr, "<unknown error>\n");
}

#endif	/* Unix */
