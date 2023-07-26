
/*
 * TarSplit -- split up tar files (creating directories as needed)
 *
 * usage: TarSplit [pathname]
 *
 * semantics: splits up tar file taken from stdin (or pathname, if
 * specified) and creates the files therein under the working data
 * directory.
 *
 * AmigaDOS Version - no support for stdin.
 */

#include <stdio.h>
#include "/version.h"

IDENT(".00");

#ifdef	AMIGA
#include <stdlib.h>
#include <exec/types.h>
#include <proto/all.h>
typedef int	bool ;
bool	ChkSumOK() ;
#include <libraries/dosextens.h>
#else
#include <modes.h>
#include <bool.h>
#define DIRMODE 	(S_IREAD | S_IWRITE | S_IEXEC | S_IOREAD | S_IOEXEC)
#endif
#define TBLOCK	512
#define NAMSIZ	100

union hblock {
	char dummy[TBLOCK];
	struct header {
		char name[NAMSIZ];
		char mode[8];
		char uid[8];
		char gid[8];
		char size[12];
		char mtime[12];
		char chksum[8];
		char linkflag;
		char linkname[NAMSIZ];
	} dbuf;
};

#define BLKSIZE 	(sizeof (union hblock))
#define HARDLINK	'1'
#define SYMBLINK	'2'
#define NORMAL		'\0'

void DoFile();

main(argc, argv)
int	argc;
char	*argv[];
{
	FILE		*TarFP;
	union hblock	TarBlock;

#ifndef AMIGA
	/* make the compiler happy about formatted I/O for longs... */
	pflinit();
#endif

	switch(argc) {
#ifndef AMIGA
	case 1:
		TarFP = stdin;
		break;
#endif
	case 2:
		if ((TarFP = fopen(argv[1], "r")) == NULL) {
			fprintf(stderr, "TarSplit: can't open %s\n", argv[1]);
			exit(1);
		}
		break;
	default:
		fprintf(stderr, "usage: TarSplit [pathname]\n");
		exit(1);
	}

	for (;;) {
		if (fread((char *)&TarBlock, BLKSIZE, 1, TarFP) == NULL) {
			fprintf(stderr, "TarSplit: premature EOF\n");
			exit(1);
		} else if (IsZero(&TarBlock)) {
			while (fread((char *)&TarBlock, BLKSIZE, 1, TarFP) != NULL)
				;
			break;
		} else
			DoFile(&TarBlock, TarFP);
	}

}

bool
IsZero(block)
union hblock	*block;
{
	int	i;
	char	*cblock;

	cblock = block->dummy;
	for (i = 0; i < TBLOCK; i++)
		if (*cblock++ != '\0')
			return(FALSE);

	return (TRUE);

}

void
DoFile(block, TarFP)
union hblock	*block;
FILE		*TarFP;
{
	long	FSize;
	char	FName[NAMSIZ], *RefName;
	int	i;
	bool	IsDir, OpenOK;
	FILE	*NewFP;

	if (!ChkSumOK(block)) {
		fprintf(stderr, "TarSplit: bad checksum, name %s?\n",
			block->dbuf.name);
		exit(1);
	}

	switch(block->dbuf.linkflag) {
	case HARDLINK:
	case SYMBLINK:
		fprintf(stderr, "TarSplit: can't handle links\n");
		exit(1);
	case NORMAL:
		break;
	default:
		fprintf(stderr, "TarSplit: unknown linkflag\n");
		exit(1);
	}

#ifdef	AMIGA
	if (sscanf(block->dbuf.size, "%12lo", &FSize) != 1) {
#else
	if (sscanf(block->dbuf.size, "%12O", &FSize) != 1) {
#endif
		fprintf(stderr, "TarSplit: bad size\n");
		exit(1);
	}

	for (i = 0, RefName = block->dbuf.name; *RefName; i++, RefName++)
		FName[i] = *RefName;

	if (IsDir = (*(RefName - 1) == '/')) {
		FName[i - 1] = '\0';
		if (strcmp(FName, ".") == 0)
			OpenOK = TRUE;
		else
#ifdef	AMIGA
			{
			    BPTR Lock;
			    OpenOK = (Lock = CreateDir(FName)) != 0;
			    UnLock(Lock) ;
			}
#else
			OpenOK = mknod(FName, DIRMODE) == 0;
#endif
	} else {
		FName[i] = '\0';
		OpenOK = (NewFP = fopen(FName, "w")) != NULL;
	}

	if (!OpenOK) {
		fprintf(stderr, "TarSplit: can't create %s\n", FName);
		exit(1);
	}

	for (; FSize > 0; FSize -= TBLOCK) {
		if (fread((char *)block, BLKSIZE, 1, TarFP) == NULL) {
			fprintf(stderr, "TarSplit: premature EOF\n");
			exit(1);
		}
		if (!IsDir)
			fwrite(block->dummy, 1,
				(FSize > TBLOCK ? TBLOCK : (int) FSize), NewFP);
	}

	if (!IsDir)
		fclose(NewFP);
}

bool
ChkSumOK(block)
union hblock	*block;
{
	long	Accum, ChkSumVal;
	int	i;

#ifdef	AMIGA
	sscanf(block->dbuf.chksum, "%8lo", &ChkSumVal);
#else
	sscanf(block->dbuf.chksum, "%8O", &ChkSumVal);
#endif
	for (i = 0; i < 8; i++)
		block->dbuf.chksum[i] = ' ';

	Accum = 0;
	for (i = 0; i < TBLOCK; i++)
		Accum += 0xff & block->dummy[i];

	return(Accum == ChkSumVal);

}
