/*
 * Copyright (C) 2004-2011 CANON INC.
 * Output Printing Data Library.
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *  OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef _CNPKLIB_H_
#define _CNPKLIB_H_


#define CNPK_SUCCESS	0
#define CNPK_ERROR		-1


#define MAX_BUF_SIZE	4096

typedef enum {

	CNPK_DOC_FORMAT_LIPS,
	CNPK_DOC_FORMAT_PS,
	CNPK_DOC_FORMAT_BDL
} CnpkDocFormat;


typedef struct cnpk_page_info_t{

	char page_name[64];
	char page_orient[64];
	char resolution[64];
	char color_mode[64];
	char media_type[64];
	char toner_save[64];
	char bind_edge[64];
	char bind_edge_shift[64];
	char duplex[64];

} CnpkPageInfo;

typedef struct cnpk_pipe_fds_t{

	int cmd_fd;
	int res_fd;
	int dat_fd;

} CnpkPipeFds;


typedef struct option_list_t OptionList;

struct option_list_t {

	char *key;
	char *value;
	short change_flg;
	short updf_flg;
	OptionList *next;
};

typedef struct cnpk_ctx_t{

	int flgProc;

	OptionList *pOptList;
	int fdOut;
	int numPage;

	int pid;
	CnpkPipeFds pipe_fds;
	char bufPDLData[MAX_BUF_SIZE];
	int nBufSize;

} CnpkCtx;


CnpkCtx*  cnpkNew( CnpkDocFormat format, int fdOut );
int cnpkDestroy( CnpkCtx* pCnpk );
int cnpkSetJobAttr( CnpkCtx* pCnpk, char* pJobAttr );
int cnpkSetDocAttr( CnpkCtx* pCnpk, char* pDocAttr );
int cnpkSetPageAttr( CnpkCtx* pCnpk, char* pPageAttr );
int cnpkStartOutput( CnpkCtx* pCnpk );
int cnpkSendData( CnpkCtx* pCnpk, char* pData, int nDataSize );
int cnpkFlushSendData( CnpkCtx* pCnpk );
int cnpkEndOutput( CnpkCtx* pCnpk );
int cnpkNextPage( CnpkCtx* pCnpk );
int cnpkGetPageInfo( CnpkCtx* pCnpk, CnpkPageInfo* pPageInfo );
CnpkCtx*  cnpkbidiNew( CnpkDocFormat format, int fdOut, char* pOptList );
int cnpkBidiStart( CnpkCtx* pCnpk, int VerNo );
int cnpkBidiCmdCtrl( CnpkCtx* pCnpk, int CmdNo );
int cnpkBidiDestroy( CnpkCtx* pCnpk );


#endif

