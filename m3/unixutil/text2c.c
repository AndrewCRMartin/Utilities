/*

Article: 8963 of alt.sources
Newsgroups: alt.sources
Path: bcc.ac.uk!uknet!EU.net!howland.reston.ans.net!sol.ctr.columbia.edu!news.columbia.edu!lamont!anshar.shadow.net!cklaus
From: cklaus@anshar.shadow.net (Christopher Klaus)
Subject: text2C filter (v1.1 update)
Message-ID: <1994Feb22.173514.24141@lamont.ldgo.columbia.edu>
Sender: news@lamont.ldgo.columbia.edu
Organization: Lamont-Doherty Earth Observatory
X-Newsreader: Tin 1.1 PL4
Date: Tue, 22 Feb 1994 17:35:14 GMT
Lines: 139


This program converts text files into C printf statements to be included
within your C programs.  Useful for when you need to do a lot
of explaining and documenting in your program.

*/

#include <stdio.h>

FILE *fp;
char *progname, *arg;


usage()
{
    (void) printf("\n\nUsage: %s [-acfh] input_file\n", progname);
    (void) printf(" -a Author Information\n -c Number of Chars Within p");
    (void) printf("rintf statement. (default is 50).\n -f Printf State");
    (void) printf("ment. (ie. -f \"(void) fprintf(fp,\")\n -h Help Scr");
    (void) printf("een. \n\nV1.1 txt2C\n\nThis is a txt2C filter that ");
    (void) printf("converts a regular text file into printf\nstatement");
    (void) printf("s to be included into your C programs.  \n\nIf you ");
    (void) printf("have any suggestions, comments, or updates to this ");
    (void) printf("utility,\nplease email me at cklaus@shadow.net\n\nC");
    (void) printf("hristopher Klaus \nFeb 22, 1994\n\n\n");
}


authorinfo()
{
    printf("\nWritten By Christopher Klaus <cklaus@shadow.net>\n");
}


t2c(filename, numchar, cprint)
    char filename[256], cprint[256];
    int numchar;
{
    int c = 0, i = 0;
    if ((fp = fopen(filename, "rb")) == NULL)
    {
	printf("Error: cant open file %s\n", filename);
	usage();
	exit(1);

    }
    printf("\n%s\"", cprint);
    while (!feof(fp))
    {

	c = getc(fp);
	if (c == '\n')
	{
	    i++;
	    printf("\\n");
	} else if (c == '\t')
	{
	    i++;
	    printf("\\t");
	} else if (c == '"')
	{
	    i++;
	    printf("\\\"");
	} else if (c == '%')
	{
	    i++;
	    printf("%%%%");
	} else if (c == '\\')
	{
	    i++;
	    printf("\\\\");
	} else
	    printf("%c", c);

	if (i > numchar)
	{
	    i = 0;
	    printf("\");\n");
	    printf(cprint);
	    printf("\"");
	}
	i++;
    }
    printf("\");\n\n");
}



main(argc, argv)
    int argc;
    char **argv;
{
    int numchar = 50;
    char cprint[256];
    progname = argv[0];

    strcpy(cprint, "printf(");

    while ((arg = *++argv))
    {
	if (*arg == '-')
	    for (arg++; *arg; arg++)
		switch (*arg)
		{
		case 'a':
		    authorinfo();
		    exit(0);
		    break;
		case 'h':
		    usage();
		    exit(0);
		    break;
		case 'c':
		    if (!*++argv)
		    {
			(void) printf("Missing Number of Chars For Within Printf Statement\n");
		    } else
			numchar = atoi(*argv++);
		    break;
		case 'f':
		    if (!*++argv)
		    {
			(void) printf("Missing printf statement\n");
		    } else
			strcpy(cprint, *argv++);
		    break;
		}
	t2c(*argv, numchar, cprint);
    }

}

/*
--
Christopher William Klaus  Email: cklaus@shadow.net  Author:Inet Sec. Scanner
2209 Summit Place Drive,Dunwoody, GA 30350-2430. (404)206-1513.
*/


