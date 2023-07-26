Return-Path: <"BIOPD@UK.AC.OXFORD.VAX"@nimsn01>
Received: from uk.ac.mrc.nimr (nimsn01) by nimsn41. (4.0/SMI-4.0)
	id AA05687; Tue, 10 Apr 90 17:47:08 BST
Via:  uk.ac.mrc.nimr; Tue, 10 Apr 90 17:47:51 BST
Message-Id: <9004101647.AA00649@uk.ac.mrc.nimr>
Via:        oxford.vax    ; 10 Apr 1990 17:47:48-BST
Date:           Tue, 10 Apr 90  17:49 GMT
From: BIOPD@UK.AC.OXFORD.VAX
To: A-MARTIN@UK.AC.MRC.NATIONAL-INSTITUTE-FOR-MEDICAL-RESEARCH
Status: RO

#ifndef lint
static char sccsid[] = "@(#)uudecode.c	5.3-1 (Berkeley) 4/10/85";
#endif
/* modified by ajr to use checksums. Use #define CHECKSUMS - MHH Nov 89 */
/* modified by fnf to use Keith Pyle's suggestion for compatibility */
/* modified by Mark Hobart of NPDSA to work as original - Nov 89 */

/*
 * uudecode [input]
 *
 * create the specified file, decoding as you go.
 * used with uuencode.
 */
#include <stdio.h>

#ifdef unix
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

/* Used by checksums but left in to make code change easier - MHH */
#define SUMSIZE 64


/* single character decode */
#define DEC(c)	(((c) - ' ') & 077)

main(argc, argv)
char **argv;
{
	FILE *in, *out;
	int mode;
	char dest[128];
	char buf[80];

	/* optional input arg */
	if (argc > 1) {
		if ((in = fopen(argv[1], "r")) == NULL) {
			perror(argv[1]);
			exit(1);
		}
		argv++; argc--;
	} else
		in = stdin;

	if (argc != 1) {
		printf("Usage: uudecode [infile]\n");
		exit(2);
	}

	/* search for header line */
	for (;;) {
		if (fgets(buf, sizeof buf, in) == NULL) {
			fprintf(stderr, "No begin line\n");
			exit(3);
		}
		if (strncmp(buf, "begin ", 6) == 0)
			break;
	}
	sscanf(buf, "begin %o %s", &mode, dest);

#ifdef unix
	/* handle ~user/file format */
	if (dest[0] == '~') {
		char *sl;
		struct passwd *getpwnam();
		char *index();
		struct passwd *user;
		char dnbuf[100];

		sl = index(dest, '/');
		if (sl == NULL) {
			fprintf(stderr, "Illegal ~user\n");
			exit(3);
		}
		*sl++ = 0;
		user = getpwnam(dest+1);
		if (user == NULL) {
			fprintf(stderr, "No such user as %s\n", dest);
			exit(4);
		}
		strcpy(dnbuf, user->pw_dir);
		strcat(dnbuf, "/");
		strcat(dnbuf, sl);
		strcpy(dest, dnbuf);
	}
#endif

	/* create output file */
	out = fopen(dest, "w");
	if (out == NULL) {
		perror(dest);
		exit(4);
	}
#if (unix || !MANX)
	chmod(dest, mode);
#endif

	decode(in, out);

	if (fgets(buf, sizeof buf, in) == NULL || strcmp(buf, "end\n")) {
		fprintf(stderr, "No end line\n");
		exit(5);
	}
	exit(0);
}

/*
 * copy from in to out, decoding as you go along.
 */
decode(in, out)
FILE *in;
FILE *out;
{
	char buf[80];
	char *bp;
	int n, checksum, line;
	int warnings = 5;

	for (line = 1; ; line++) {
		/* for each input line */
		if (fgets(buf, sizeof buf, in) == NULL) {
			printf("Short file\n");
			exit(10);
		}
#ifdef CHECKSUMS
		checksum = 0;
#endif
		n = DEC(buf[0]);
		if (n <= 0)
			break;

		bp = &buf[1];

		while (n > 0) {
#ifdef CHECKSUMS
			checksum = (checksum+outdec(bp, out, n)) % SUMSIZE;
#else
			outdec(bp,out,n);
#endif
			bp += 4;
			n -= 3;
		}
#ifdef CHECKSUMS
		if (*bp != '\n' && checksum != DEC(*bp))
			if (warnings > 0) {
				printf("Checksum error, line %d.\n",line);
				warnings--;
			} else if (warnings == 0) {
				printf("more...\n");
				warnings--;
			}
#endif
	}
}

/*
 * output a group of 3 bytes (4 input characters).
 * the input chars are pointed to by p, they are to
 * be output to file f.  n is used to tell us not to
 * output all of them at the end of the file.
 * we return a checksum increment.
 */
int outdec(p, f, n)
char *p;
FILE *f;
{
	int c1, c2, c3;

	c1 = DEC(*p) << 2 | DEC(p[1]) >> 4;
	c2 = DEC(p[1]) << 4 | DEC(p[2]) >> 2;
	c3 = DEC(p[2]) << 6 | DEC(p[3]);

	if (n >= 1)
		putc(c1, f);
	if (n >= 2)
		putc(c2, f);
	if (n >= 3)
		putc(c3, f);

/* In the non-checksums option this return value is ignored */
	return((c1+c2+c3) % SUMSIZE);
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

/*
 * Return the ptr in sp at which the character c appears;
 * NULL if not found
 */

#ifndef NULL		/* MANX has bogus "#define NULL 0L" somewhere */
#define	NULL	0
#endif

char *
index(sp, c)
register char *sp, c;
{
	do {
		if (*sp == c)
			return(sp);
	} while (*sp++);
	return(NULL);
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
