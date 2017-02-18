/***************************************************************
 * Copyright (C) 1984, 1985   -   All Rights Reserved          *
 *                                                             *
 * Lattice, Inc.                                               *
 * Glen Ellyn, Illinois                                        *
 * U. S. A.                                                    *
 *                                                             *
 * Author: Billy Shakespeare & Company                         *
 ***************************************************************/
/**********************************************
 *                  dbc.h                     *
 **********************************************/

#ifndef MSC
#define MSC /* Microsoft C Option */
#endif

#define dBASE3  /* dBASE III Option */

/*** dBC New Types ***/
typedef unsigned char     BYTE;     /* 1 byte data */
typedef long     RECNUM;   /* record number */
typedef BYTE     FLDWIDTH; /* field width in bytes or characters */
typedef BYTE     FLDDEC;   /* decimal places in field */
typedef BYTE     FLDNUM;   /* number of fields per record */
typedef unsigned int RECLEN;   /* record length */
typedef BYTE     KEYLEN;   /* key length */

/******************************************/
/* .DBF record field definition structure */
/******************************************/
typedef struct {
        char fieldnm[11]; /* field name terminating with NULL(0) */
        char type;        /* type of data */
			     /* 'C' - character
                                'N' - numeric
				'D' - date
                                'L' - logical
				'M' - memo    */
        FLDWIDTH width;   /* field width */
        FLDDEC   dec;     /* length of decimal places */
} dBFIELD;

/***** record status definition *****/
#define ACTIVE    0 /* record is active */
#define INACTIVE  1 /* record is inactive (marked deleted) */

/***** key type definitions *****/
#define CHARKEY   'C' /* CHARACTER key */
#define NDKEY     'N' /* NUMERIC or DATE key */

/***********************************
* return values from dBC functions *
************************************/
#define SUCCESS   0    /** successful function return code **/
#define d_ERROR   1    /** general error code **/
			  /* This error code is returned by a function that
                             failed due to a fundamental reason such as wrong
                             input values or the file to be accessed is not
		             open. */
#define d_NODBF   2    /** accessed file is not organized in the data **/
		       /*  file(.DBF) format */
#define d_BADREC  3    /** specified record number is out of range, **/
		       /*  i.e., larger than the current number of allocated
                           records */
#define d_WTFAIL  4    /** file-write operation failed, e.g., "disk full" **/
#define d_NONDX   5    /** accessed file is not organized in the index **/
		       /*  file(.NDX) format */
#define d_KYLONG  6    /** too long key expression **/
#define d_NOKEY   7    /** index file(.NDX) is empty or requested key is
                           not found **/
#define d_MAYBE   8    /** requested key was not found in the index file, **/
		       /*  but another key that may immediately follow the
			   requested key was located */
#define d_ENDKEY  9    /** no more key exists in the index file **/
		       /*  the sequential-read key pointer is placed
			   at the end of the index file */
#define d_NOSYNC  10   /** key exists in the index file, but its **/
		       /*  corresponding record does not exist in the
			   specified data file */
#define d_TOPKEY  11   /** top of the index file condition **/
		       /*  the sequential-read key pointer reached 
			   the top of the index file */



int dBcreat(char *,FLDNUM,dBFIELD *);
int dBopen(char *,char **);
int dBclose(char *);
int dBgetr(char *,RECNUM,char *,char *);
int dBgetf(char *,RECLEN *,char *,char *,char *,FLDNUM *,dBFIELD *);
int dBputr(char *,RECNUM,char *);
int dBupdr(char *,RECNUM,char *);
int dBdelete(char *,RECNUM);
int dBrecall(char *,RECNUM);
int dBsize(char *,RECNUM *);
int dBrmvr(char *,RECNUM);
int dBflush(char *);
int dBicreat(char *,char *,KEYLEN,char);
int dBiopen(char *,char **);
int dBiclose(char *);
int dBakey(char *,char *,RECNUM);
int dBrmvkey(char *,char *,RECNUM);
int dBtkey(char *,char *,RECNUM *);
int dBckey(char *,char *,RECNUM *);
int dBnkey(char *,char *,RECNUM *);
int dBpkey(char *,char *,RECNUM *);
int dBrewind(char *);
int dBfwd(char *);
int dBgetrk(char *,char *,char *,char *,char *);
int dBgetnr(char *,char *,char *,char *);
int dBgetpr(char *,char *,char *,char *);
int dBputrk(char *,char *,char *,char *);
int dBkexpr(char *,char *,char *,int *,KEYLEN *);
int dBiflsh(char *);
int dBmcreat(char *);
int dBmopen(char *,char **);
int dBmclose(char *);
int dBgetm(char *,char *,char *);
int dBputm(char *,char *,char *);

/**********  synonyms for dBC function names ************/

        /* synonym */	   /* actual name */
/*         ---------          -----------          */
#define    dbcreat            dBcreat
#define    dbopen             dBopen
#define    dbclose            dBclose
#define    dbgetr             dBgetr
#define    dbgetf             dBgetf
#define    dbputr             dBputr
#define    dbupdr             dBupdr
#define    dbdelete           dBdelete
#define    dbrecall           dBrecall
#define    dbsize             dBsize
#define    dbrmvr             dBrmvr
#define    dbflush            dBflush
#define    dbicreat           dBicreat
#define    dbiopen            dBiopen
#define    dbiclose           dBiclose
#define    dbakey             dBakey
#define    dbrmvkey           dBrmvkey
#define    dbtkey             dBtkey
#define    dbckey             dBckey
#define    dbnkey             dBnkey
#define    dbpkey             dBpkey
#define    dbrewind           dBrewind
#define    dbfwd              dBfwd
#define    dbgetrk            dBgetrk
#define    dbgetnr            dBgetnr
#define    dbgetpr            dBgetpr
#define    dbputrk            dBputrk
#define    dbkexpr            dBkexpr
#define    dbatofld           dBatofld
#define    dbfldtoa           dBfldtoa
#define    dbatokey           dBatokey
#define    dbkeytoa           dBkeytoa
#define    dbiflsh            dBiflsh
#define    dbmcreat           dBmcreat
#define    dbmopen            dBmopen
#define    dbmclose           dBmclose
#define    dbgetm             dBgetm
#define    dbputm             dBputm

#define _DBC_H /* turn _DBC_H switch on */
