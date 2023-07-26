/*

Program:	MFUUD ver. 3.5.2  (Last Update: 01/30/94)
Author:		Paul Erickson
Affiliation:	Department of Computer Engineering
		Carnegie Mellon University
Email:		pe0e+@andrew.cmu.edu

**************************************************************

Copyright (c) 1993 Paul Erickson

All rights reserved.


This is the intellectually property of Paul Erickson.  You may use and 
distribute this program freely in either source code or binary form provided 
that the above copywrite notice and this paragraph are duplicated in all such 
forms.  This program cannot be modified without the consent of the author, and 
such modifications include, but are not limited to, incorporating the 
following code into other sources.  THIS SOFTWARE IS PROVIDED "AS IS" WITHOUT 
ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT WITHOUT LIMITATION, THE 
IMPLIED WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.

**************************************************************


-----------------WHAT IS MFUUD?------------------

MFUUD (Multiple File UUdecoder) is a program designed specifically to decode 
uuencoded files posted as usenet articles.  MFUUD can also decode regular 
uuencoded files as well and is designed to satisfy all uudecoding needs.


------------CHANGES SINCE VERSION 3.0.x-------------

FIXED BUG:	
MFUUD refused to decode certain files

FIXED BUG:
MFUUD would on occasion misinterpret part numbers

REMOVED OPTION:
-input option has been removed.  If you want to decode from standard input do 
not specify any filenames.

REMOVED OPTION:
-temp option has been removed.  MFUUD no longer requires a temporary 
directory.

ADDED OPTION:
-extended option has been added.  The -extended option replaces: 


	MFUUD -input < filename


Please see documentation on the -extended option for more details.

----------------HOW TO USE MFUUD-----------------

MFUUD is simple to use.  The syntax goes as follows:


	mfuud [-del] [-quiet] [-dest directory_name] [-mode ###] [-plain]
		[-dir] [-keywords "(keywords)"] file_1 ... file_n


The text in brackets represent command line options.  (When typing the 
options, do not include the brackets.)  The options must be typed in lowercase 
and can be abbreviated so long as there is no ambiguity.  Some of the options 
require arguments: -temp, -dest, -mode, and  -keywords.  The proper argument 
must follow each of these options if they are used, and the option and 
argument must be separated by a space character.


If you do not specify any options, you must at least specify the path of each 
file you wish to decode.  You may use the wildcard (*) character if you like. 
MFUUD will be expecting message files (i.e. text files that contain message 
headers).  This is important because MFUUD will not be able to decode files 
without the headers intact (unless you specify the [-plain] option).  The 
order of the files you specify does not matter since MFUUD is "smart" enough 
to combine files in the proper order and rebuild the original uuencoded file. 
You can also specify more than one group of files.  MFUUD is programmed to 
combine all related parts in the proper order to reconstruct each  uuencoded 
file. 

If you have ever decoded files "by hand", then you are no doubt aware of the 
tediousness of the process.  MFUUD decodes files just as a human would.  It 
reads the caption or subject line of each message, determining which files 
will be combined in the proper order. MFUUD in addition will uudecode the 
files while stripping the message headers and extraneous text.


--------------COMMAND LINE OPTIONS---------------

OPTION: -del

This option tells MFUUD that you wish the uuencoded parts to the original file 
be deleted after it has been reconstructed.  The removal of the parts is only 
done when MFUUD is confident that the original file has been properly 
reconstructed.  If not, MFUUD leaves the uuencoded parts for you to either 
delete yourself or edit to fix errors in the subject lines.


OPTION: -quiet

This option prevents MFUUD from displaying any output.  Error messages, 
however, will still be sent to the standard error device (stderr).


OPTION: -extended

This option is used if you want MFUUD to decode a file that contains all the 
message files concatenated together.  Many newsreaders will append message 
files you select to a specified file.  In order to decode that type of file, 
you must use the following syntax:


	MFUUD -e filename


OPTION: -dest directory_name

This option is used to specify a destination directory for the decoded files. 
By default, all uudecoded files are written into the current directory.


OPTION: -mode ###

This option will override the default file permission of each decoded file. 
This default file information is contained in the uuencoded file and is used 
to set its file permission when created.  A three digit octal number must 
follow the -mode option, and each file created by MFUUD will be set to this 
specified file mode.  Consult your operating system manual for more 
information on file modes.


OPTION: -plain

If you just want to decode standard uuencoded files (files with no message 
headers), use this option.  MFUUD will behave like uudecode, except you can 
specify one or more files.  For example: 


	MFUUD -p part1 part2 part3


where part1, part2, and part3 are uuencoded parts of a file.  In addition, 
MFUUD will remove extraneous text while it decodes.   You can also use:


	MFUUD -p Cindy1of1 Steph1of2 Steph2of2


This could produce cindy.jpg and steph.jpg.


OPTION: -dir

This tells MFUUD to honor filenames that contain a path.  For example, 
uuencoded files can have a begin line as follows:


	begin 644 /users/usr5/pe0e/picture.jpg


MFUUD will only look at the basename and ignore the pathname unless you 
specify this option.  Most of the time, the uuencoders tree structure will not 
be the same as your tree structure, so it makes no sense to honor full 
pathnames.


OPTION: -keywords "word1 word2 ... wordn"

This option is useful when you are performing "mass decoding".  See the 
section on mass decoding for more information.  This option lets MFUUD know 
which files to decode.  The keywords you specify must be in double quotes, and 
each must begin with either a "+" or a "-".  The "-" preceding a keyword, 
tells MFUUD to ignore messages whose subject line contains this keyword.  Any 
keyword preceded by a "+" are decoded regardless.  For example:


	mfuud -keywords "-male +female" *


Specifying this command line will decode all message files in the current 
directory that don't contain the word "male" in the captions unless they 
contain "female", in which case it makes no difference as MFUUD will decode it 
regardless.  MFUUD, when scanning for keywords, is not case sensitive so it 
make no difference if the keywords are uppercase or lowercase.  You should 
note that the following syntax will get you undesired results:


	mfuud -keywords "-male" *


The keyword "female" also contains the word "male"; therefore, mfuud will not 
decode message files that contain the word "female" in the caption.  You can 
instruct mfuud to be more strict in matching keywords by using the "~" 
character. If you precede and end the keyword with a tilde, then MFUUD will 
match complete words.  For example:


	mfuud -keywords "-~male~" *


If you are searching for specific files then you can use a special notation. 
 If you specify a "-" followed by no keyword than MFUUD will ignore all 
messages.  This is not useful unless you specify at lease one keyword preceded 
by a "+".  For example,


	mfuud -keywords "- +cindy +crawford" *


This instructs MFUUD to ignore all messages except those that contain "cindy" 
or "crawford" in their caption.


-----------DECODING FROM STANDARD INPUT------------

If you don't specify filenames then MFUUD will assume you want to decode from 
the standard input.  For example:


	cat filename | MFUUD -dest /usr/tmp/pictures


When decoding from standard input, MFUUD strips extraneous text.


-----------------MASS DECODING-------------------

Mass decoding is the process of uudecoding message files by skipping the 
intermediate steps. If you know the directory on your file system that 
contains the message files that interest you, then you can decode messages 
straight from this directory.  I recommend that you create a temporary 
directory where you would store the decoded files, especially if you are 
planning to decode many files.  Switch to the directory containing the message 
files and using a syntax similar to the following:


	mfuud -dest /usr/tmp/pictures *


/usr/tmp/pictures is the temporary directory.  You could also be in 
/usr/tmp/pictures and specify the path of the directory you want to decode 
from followed by a wildcard.  This may or may not work.  It depends on the 
length of the path you specify, but in many cases you will overflow the 
command line.  For example, if you use say:


	mfuud /afs/andrew.cmu.edu/usr0/netbb/.MESSAGES/netnews/alt/binaries/pictures/supermodels/*


you may overflow the command line depending on how many files are in 
a.b.p.supermodels.  (NOTE:  If you are on AFS, I have just specified a path 
for the supermodels directory.  It is very easy to find where the other binary 
bboards are located.  I advise, however, that you don't use this path unless 
you cannot find where the messages are located on your local system.)


----------------COMPILING MFUUD------------------

I use the following command line:


	cc -o mfuud mfuud.c

*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>


/* Added ACRM for Solbourne
*/
#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif


#define SUBJECT "Subject: "
#define LINELEN 80
#define NAMELEN 30
#define MAXFILE 1000
#define SUBJLEN strlen(SUBJECT)
#define TRANS(c) ( (c - ' ') & 077 )
#define MAXKEYW 10
#define MEMLOC (char *) (mem->head + mem->offset)
typedef struct {
	int post_id,num,del,sub_id,maxp;
	long time,seek;
	char *uuname,*filename,*subline;
} 
FINFO;

typedef struct {
	char *head;
	int offset;
	int blocks;
	int bsize;
	int avail;
	int items;
}
MEMORY;

char DEL = 0;
char QUIET = 0;
char *DESTDIR = NULL;
int FMODE = 0;
char PLAIN = 0;
char DIR = 0;
char EXT = 0;

	FILE *fopen();
	void GetNum();
	int GetId();
	unsigned int src();
	void swapint();
	void decode();
	void SortFiles();
	long GetTime();
	void swaplong();
	int uuline();
	unsigned int additem();
	void meminit();
	int GetOption();
	char *FormatStr();
	void KeySplit();
	int keycheck();
	char *upcase();
	char *userid();

int main(argc,argv)
int argc;
char *argv[];
{

	FILE *file;
	int fcount,aindex,i,count,keynum,uuchk,header,closed,newline,temp;
	long spos;
	char line[LINELEN],subline[LINELEN],name[NAMELEN],*keystr,*keyword[MAXKEYW],*cpos,*fn;
	FINFO filelist[MAXFILE];
	unsigned int offsets1[MAXFILE],offsets2[MAXFILE];
	MEMORY table,namelist;


	meminit(&table,256);
	meminit(&namelist,256);
	additem(&namelist,"*");
	keynum = 0;
	for (count = 1; count < argc; count++) {
		if ( *argv[count] != '-' ) break;
		switch(GetOption(argv[count])) {
		case 0:
			fprintf(stderr,"mfuud: ambiguous or unknown command line option \"%s\"\n",argv[count]);
			exit(1);
		case 1:
			DEL = 1;
			break;
		case 2:
			QUIET = 1;
			break;
		case 3:
			EXT = 1;
			break;
		case 4:
			if (++count < argc) DESTDIR = argv[count];
			break;
		case 5:
			if (++count < argc) sscanf(argv[count],"%o",&FMODE);
			break;
		case 6:
			PLAIN = 1;
			break;
		case 7:
			DIR = 1;
			break;
		case 8:
			if (++count < argc) keystr = argv[count];
			KeySplit(keyword,keystr,&keynum);
			break;
		}
	}
	if ( !QUIET ) printf("Multiple File UUDecoder ver 3.5.2\n");
	i = argc;
	argc -= count;
	aindex = count;
	if ( argc == 0 ) { 
		decode(NULL, 0);
		return(0); 
	}
	if ( PLAIN ) {
		for (fcount = 0; count < i; fcount++, count++) {
			filelist[fcount].filename = argv[count];
			filelist[fcount].del = 0;
			filelist[fcount].sub_id = 1;
		}
		decode(filelist,argc);
		return(0);
	}
	closed = 1;
	for ( fcount = count = 0; count < argc;) {
		if ( !EXT || closed ) {
			spos = header = 0;
			if ( (file = fopen(fn = argv[count+aindex],"r")) == NULL ) {
				fprintf(stderr,"mfuud: cannot access file %s\n",argv[count+aindex]);
				count++;
				continue;
			}
		}
		filelist[fcount].uuname = NULL;
		offsets1[fcount] = 0;
		filelist[fcount].del = 0;
		filelist[fcount].sub_id = -1;
		for ( uuchk = closed = 0, newline = 1, *subline = '\0'; (uuchk < 6) && (fgets(line,LINELEN,file)); spos = ftell(file)) {
			if ( (*line == '\n') && ( header > 5 ) ) filelist[fcount].seek = spos;
			if ( newline )
				if ( (cpos = strchr(line,' ')) && (cpos[-1] == ':') ) header++; 
				else header = 0;
			newline = ( line[strlen(line)-1] == '\n' );
			if ( !strncmp(line,SUBJECT,SUBJLEN) )
				if ( keycheck(line,keyword,keynum) >= 0 ) strcpy(subline,line);
				else break;
			if ( (*subline != '\0') && uuline(line) ) uuchk++;
			if ( !strncmp(line,"Date: ",6) ) filelist[fcount].time = GetTime(line);
			if ( !strncmp(line,"From: ",6) ) filelist[fcount].post_id = GetId(&table,userid(line+6));
			if ( (*subline != '\0') && !strncmp(line,"begin",5) )
				if ( sscanf(line, "begin %o %s",&temp,name) == 2 ) offsets1[fcount] = src(&namelist,name);
		}
		if ( !EXT || feof(file) ) {
			fclose(file);
			closed = 1;
			count++;
		}
		if ( (uuchk >= 6) && (*subline != '\0') ) {
			filelist[fcount].filename = fn;
			if ( !QUIET ) printf("%s",subline+SUBJLEN);
			offsets2[fcount] = additem(&namelist,upcase(strtok(subline+SUBJLEN,"\n")));
			GetNum(subline,&filelist[fcount].num,&filelist[fcount].maxp);
			fcount++;
		}

	}
	if ( argc == 0 ) {
		fprintf(stderr,"Nothing decoded.\n");
		return(2);
	}
	for (count = 0; count < fcount; count++) {
		if ( offsets1[count] ) filelist[count].uuname = namelist.head + offsets1[count];
		filelist[count].subline = namelist.head + offsets2[count];
	}
	if ( !QUIET ) printf("Found %d uuencoded files.\n",fcount);
	if ( !fcount ) return(2);
	SortFiles(filelist,fcount);
	decode(filelist,fcount);
	return(0);

}

void GetNum(string,n,m)
char *string;
int *n,*m;
{
	int aindex, end, brk, start;
	char *pos,*temp;

	brk = 0;
	for ( end = 0, aindex = 0; string[aindex] != '\0'; aindex++ ) {
		if ( (isdigit(string[aindex])) && (end != -1) ) {
			start = aindex;
			end = -1; 
		}
		if ( (!isdigit(string[aindex])) && (end == -1) ) end = aindex; 
		if ( ((string[aindex] == '/') || brk || (strncmp(string+aindex,"OF",2) == 0)) && (end > start) ) { 
			aindex = -1; 
			break;
		} 
		if ( (!isdigit(string[aindex])) && (end > start) && (string[aindex] != ' ') )end = 0;
	}
	if ( aindex == -1 ) {
		string[end] = '\0';
		*n = atoi(string + start);
		for (pos = string+end+1; ( !isdigit(*pos) ) && ( *pos ); pos++ );
		for (temp = pos; isdigit(*pos); pos++ );
		*pos = '\0';
		if ( *temp ) *m = atoi(temp); else *m = -1;
		
	}
	else *m = *n = -1; 

}

void swapint(x,y)
int *x,*y;
{
	int temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

int GetId(table,str)
char *str;
MEMORY *table;
{
	int count;
	char *aindex;

	for (aindex = (table->head), count = 1; count <= (table->items); count++) {
		if ( !strcmp(aindex,str) ) return(count);
		aindex += strlen(aindex) + 1; 
	}
	additem(table,str);
	return(table->items);
}

unsigned int src(table,str)
char *str;
MEMORY *table;
{
	int count;
	unsigned int aindex;

	for (aindex = 0, count = 1; count <= (table->items); count++) {
		if ( !strcmp((table->head)+aindex,str) ) return(aindex);
		aindex += strlen((table->head)+aindex) + 1; 
	}
	return(additem(table,str));
}

void decline(cptr,file)
char *cptr;
FILE *file;
{
	int n = TRANS(*cptr++);

	while ( n > 0 ) {
		if ( n >= 1 ) putc(TRANS(cptr[0]) << 2 | TRANS(cptr[1]) >> 4,file);
		if ( n >= 2 ) putc(TRANS(cptr[1]) << 4 | TRANS(cptr[2]) >> 2,file);
		if ( n >= 3 ) putc(TRANS(cptr[2]) << 6 | TRANS(cptr[3]),file);
		cptr += 4;
		n -= 3;
	}
}

long maxseek(filelist, aindex, num, spos)
FINFO *filelist;
int aindex, num, spos;
{
	int count;

	for ( count = 0; count < num; count++ )
		if ( (count != aindex) && 
		    (filelist[count].filename == filelist[aindex].filename) && 
		    (filelist[count].seek > filelist[aindex].seek) &&
		    ( spos > filelist[count].seek ) ) spos = filelist[count].seek;
	return(spos);
}

void clean(filelist,num)
FINFO *filelist;
int num;
{
	int i,j,del;

	for ( i = 0; i < num; i++ )
		if ( filelist[i].del ) {
			del = 1;
			for ( j = 0; j < num; j++) 
				if ( ( filelist[i].filename == filelist[j].filename ) && (!filelist[j].del) ) del = 0;
			if ( del ) unlink(filelist[i].filename);
		}
}

void decode(filelist,num)
FINFO *filelist;
int num;
{
	char *CreatePath();
	void BaseName();

	int count;
	FILE *infile,*outfile;
	char line1[LINELEN],line2[LINELEN],line[LINELEN],name[LINELEN],*path;
	int begin,mode;
	long max;

	begin = count = 0;
	*line1 = '\0';
	*line2 = '\0';

	do {
		if ( 
num ) {
			if ( filelist[count].sub_id < 1 ) continue;
			if ( (infile = fopen(filelist[count].filename,"r")) == NULL ) {
				fprintf(stderr,"mfuud: cannot access file %s\n",filelist[count].filename);
				exit(1);
			}
			fseek(infile,0,SEEK_END);
			max = maxseek(filelist,count,num,ftell(infile));
			fseek(infile,filelist[count].seek,SEEK_SET);
		}
		else infile = stdin;
		while ( ( (infile == stdin) ? 1 : ftell(infile) < max ) && fgets(line,LINELEN,infile)  ) {
			if ( (strncmp(line,"begin",5) == 0) && begin ) {
				if (!QUIET) printf("Short file\n");
				fclose(outfile);
				begin = 0;
			}
			if ( (strncmp(line,"begin",5) == 0) && !begin ) {
				sscanf(line, "begin %o %s",&mode,name);
				if ( !DIR ) BaseName(name);
				if ( (outfile = fopen(path = CreatePath(name),"wb")) == NULL ) {
					fprintf(stderr,"mfuud: unable to create %s\n",path);
					break;
				}
				begin = 1;
				if (!QUIET) printf("UUdecoding %s\n",name);
				if ( FMODE ) chmod(path,FMODE); 
				else chmod(path,mode);
			}
			if ( begin && (uuline(line)) ) decline(line,outfile);
			if ( (strncmp(line,"end",3) == 0) && begin ) {
				decline(line1,outfile);
				decline(line2,outfile);
				begin = 0;
				fclose(outfile);
			}
			strcpy(line1,line2);
			strcpy(line2,line); 
		}
		if ( infile != stdin ) {
			fclose(infile);
			filelist[count].del = 1;
			if ( DEL ) clean(filelist,num);
		}
	} 
	while ( ++count < num );
	if ( begin ) {
		if (!QUIET) printf("Short file\n");
		fclose(outfile);
	}
}

char *CreatePath(name)
char *name;
{
	static char pathname[LINELEN];

	if ( !DESTDIR ) return(name);
	strcpy(pathname,DESTDIR);
	strcat(pathname,"/");
	strcat(pathname,name);
	return(pathname);
}

char *FormatStr(str)
char *str;
{
	int aindex;
	for (aindex = 0; str[aindex] != '\0'; aindex++) 
		if ( (isalnum(str[aindex]))
		    || (str[aindex] == '+') 
		    || (str[aindex] == '-') ) str[aindex] = toupper(str[aindex]);
		else str[aindex] = ' ';
	return(str);
}

void swaplong(x,y)
long *x,*y;
{
	long temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

long GetTime(string)
char *string;
{
	long hour,min,sec;
	char *token,*s;

	strtok(string," ");
	while ( (token = strtok(NULL," \n")) != NULL) {
		if ( strchr(token,':') ) break;
	}

	if ( !token ) return(0);
	if ( ( s = strtok(token,":") ) != NULL ) hour = atol(s); 
	else hour = 0;
	if ( ( s = strtok(NULL,":") ) != NULL ) min = atol(s); 
	else min = 0;
	if ( ( s = strtok(NULL," ") ) != NULL ) sec = atol(s); 
	else sec = 0;
	return(hour*(long)3600+min*(long)60+sec);
}

int uuline(line)
char *line;
{
	if ( (*line == 'M') && ( (strlen(line)==62) || (strlen(line)==63) ) ) return(1); 
	else return(0);
}

void meminit(mem,bsize)
MEMORY *mem;
int bsize;
{
	mem->head = NULL;
	mem->offset = 0;
	mem->blocks = 0;
	mem->bsize = bsize;
	mem->avail = 0;
	mem->items = 0;
}

unsigned int additem(mem, item)
MEMORY *mem;
char *item;
{
	unsigned int retvalue;
	int size;

	if (( mem->avail ) < ( strlen(item) + 1 )) {
		(mem->blocks)++;
		size = (mem->blocks) * (mem->bsize);
		mem->avail += mem->bsize;
		if (mem->head)
			mem->head = (char*) realloc(mem->head,size);
		else
			mem->head = (char*) malloc(size);
	}
	strcpy(MEMLOC,item);
	retvalue = mem->offset;
	mem->offset += strlen(item) + 1;
	mem->avail -= strlen(item) + 1;
	(mem->items)++;
	return(retvalue);
}

int GetOption(str)
char *str;
{
	int i,match;
	char *ptr;
	char *optlist = "-delete\0-quiet\0-extended\0-dest\0-mode\0-plain\0-dir\0-keywords\0";

	for ( i = 1, match = 0, ptr = optlist; *ptr != '\0'; i++, ptr += strlen(ptr) + 1 ) {
		if ( !strncmp(ptr,str,strlen(str)) )
			if ( match ) return(0);
			else match = i;
	}
	return(match);
}

void BaseName(path)
char *path;
{
	char *cpos,*lpos,temp[NAMELEN];

	lpos = path;
	cpos = path;

	for (; cpos; ) if ( cpos = strchr(cpos + 1,'/') ) lpos = cpos + 1;
	strcpy(temp,lpos);
	strcpy(path,temp);
}

void KeySplit(strarr,str,keynum)
char **strarr,*str;
int *keynum;
{
	char *ptr;

	ptr = strtok(str," ");
	while (ptr) {
		strarr[(*keynum)++] = FormatStr(ptr);
		ptr = strtok(NULL," ");
	}
}

int keycheck(str,keyword,keynum)
char *str,**keyword;
int keynum;
{
	char tstr[LINELEN];
	int aindex;
	int value = 0;

	if ( !keynum ) return(keynum);
	strcpy(tstr,str);
	FormatStr(tstr);

	for (aindex = 0; aindex < keynum; aindex++)
		if (strstr(tstr,keyword[aindex]+1))
			value += ( (*(keyword[aindex]) == '-') ? -1 : 1 );
	return(value);
}

char *upcase(str)
char *str;
{
	int aindex;
	for (aindex = 0; str[aindex] != '\0'; aindex++) str[aindex] = toupper(str[aindex]);
	return(str);
}

int same(s1,s2)
char *s1,*s2;
{
	int cont = 0;

	while (*s1 && *s2) {
		if ( !isalnum(*s1) ) { 
			s1++; 
			cont = 1; 
		}
		if ( !isalnum(*s2) ) { 
			s2++; 
			cont = 1; 
		}
		if ( cont ) {
			cont = 0;
			continue;
		}
		if ( *s1 != *s2 ) return(0);
		s1++; 
		s2++;
	}
	return(*s1 == *s2);
}

void swapitems(filelist,i,j)
FINFO *filelist;
int i,j;
{
	char *tchar;

	swapint(&filelist[i].post_id,&filelist[j].post_id);
	swapint(&filelist[i].num,&filelist[j].num);
	swapint(&filelist[i].maxp,&filelist[j].maxp);
	swapint(&filelist[i].sub_id,&filelist[j].sub_id);
	swaplong(&filelist[i].time,&filelist[j].time);
	swaplong(&filelist[i].seek,&filelist[j].seek);
	tchar = filelist[i].subline;
	filelist[i].subline = filelist[j].subline;
	filelist[j].subline = tchar;
	tchar = filelist[i].uuname;
	filelist[i].uuname = filelist[j].uuname;
	filelist[j].uuname = tchar;
	tchar = filelist[i].filename;
	filelist[i].filename = filelist[j].filename;
	filelist[j].filename = tchar;
}

void SortFiles(filelist,argc)
FINFO *filelist;
int argc;
{
	int i,j,result,match,bmatch,ncount,lnum,id,pos,len,idnum,dcount,start,maxp,idmatch;
	char idstr[LINELEN],pstr[LINELEN],*fname;

	for ( i = 0; i < argc; i++)
		for ( j = i + 1; j < argc; j++) {
			if ( (result = filelist[i].post_id - filelist[j].post_id) == 0 )
				if ( (result = filelist[i].num - filelist[j].num) == 0 )
					result = filelist[i].time - filelist[j].time;
			if ( result > 0 ) swapitems(filelist,i,j);
		}

	for (i = 0; i < argc; ) {
		id = filelist[i].post_id;
		for (; filelist[i].post_id == id; i++) {
			if ( filelist[i].num == -1 ) continue;
			for ( j = i + 1; filelist[j].post_id == id; j++)
				if ( same(filelist[i].subline,filelist[j].subline) ) {
					filelist[i].sub_id = 0;
					break;
				}
		}
	}

	i = 0;
	idnum = 1;
	while ( i < argc ) {
		id = filelist[i].post_id;
		start = i;
		for (; filelist[i].post_id == id; i++ ) {
			if ( filelist[i].sub_id != -1 ) continue;
			match = 0;
			idmatch = 0;
			pos = -1;
			len = 3;
			do {
				if ( match && (len < strlen(filelist[i].subline)) ) len++; 
				else { 
					pos++; 
					len = 3; 
				}
				strncpy(pstr,filelist[i].subline + pos,len);
				pstr[len] = '\0';
				lnum = 0;
				fname = NULL;
				for ( j = start, match = dcount = bmatch = ncount = 0, maxp = -1; filelist[j].post_id == id; j++ ) {
					if ( filelist[j].sub_id != -1 ) continue;
					if ( strstr(filelist[j].subline,pstr) ) {
						match++;
						if ( (filelist[j].uuname != NULL) && (filelist[j].uuname != fname) ) {
							bmatch++;
							fname = filelist[j].uuname;
						}
						if ( filelist[j].maxp != -1 ) maxp = filelist[j].maxp;
						if ( filelist[j].num == -1 ) dcount++;
						else if ( lnum != filelist[j].num ) {
							ncount += ( filelist[j].num - lnum - 1 );
							lnum = filelist[j].num;
						}
						else if ( strstr(filelist[j].subline,"REPOST") ) match--;
					}
				}
				if ( (match > idmatch) && ( bmatch == 1 ) && (ncount <= dcount) && (maxp != -1 ? match == maxp : 1) ) {
					strcpy(idstr,pstr);
					idmatch = match;
				}
				if ( idmatch && ( match == maxp ) ) break;
			}
			while (pos < strlen(filelist[i].subline) );
			if ( idmatch ) {
				for ( j = start; filelist[j].post_id == id; j++ ) {
					if ( filelist[j].sub_id != -1 ) continue;
					if ( strstr(filelist[j].subline,idstr) ) filelist[j].sub_id = idnum;
				}
				idnum++;
			}
		}
		lnum = 0;
		match = 1;
		for ( j = start; filelist[j].post_id == id; j++ ) {
			if ( (filelist[j].sub_id == -1) && (lnum + 1 != filelist[j].num) ) match = 0;
			lnum = filelist[j].num;
		}
		if ( match ) {
			for ( j = start; filelist[j].post_id == id; j++ )
				if ( filelist[j].sub_id == -1 ) filelist[j].sub_id = idnum;
			idnum++;
		}
	}
	if ( !QUIET ) for ( j = 1, i = 0; i < argc; i++ )
		if ( filelist[i].sub_id == -1 ) {
			if ( j ) {
				printf("Parts missing from the following segments:\n");
				j = 0;
			}
			printf("%s\n",filelist[i].subline);
		}
	for ( i = 0; i < argc; i++) {
		for ( j = i + 1; j < argc; j++) {
			if ( (result = filelist[i].post_id - filelist[j].post_id) == 0 ) {
				if ( (result = filelist[i].sub_id - filelist[j].sub_id) == 0 ) {
					if ( filelist[j].num == filelist[i].num ) {
						if ( strstr(filelist[j].subline,"REPOST") ) filelist[i].sub_id = 0;
						if ( strstr(filelist[i].subline,"REPOST") ) filelist[j].sub_id = 0;
					}
					if ( ( filelist[i].num == -1 ) || ( filelist[j].num == -1 ) )
						result = filelist[i].time - filelist[j].time;
					else result = filelist[i].num - filelist[j].num;
				}
			}
			if ( result > 0 ) swapitems(filelist,i,j);
		}
	}
}

char *userid(str)
char *str;
{
	char *token,*pos;
	char *cset = "() <>,\n";

	while ( (token = strtok(str,cset)) != NULL ) {
		if (str) str = NULL;
		if ( pos = strchr(token,'@') ) {
			*pos = '\0';
			return(token);
		}
	}
	return("NONE!");
}
