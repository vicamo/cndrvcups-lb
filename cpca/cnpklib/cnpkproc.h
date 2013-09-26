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


#ifndef _CNPK_PROC_H_
#define _CNPK_PROC_H_

#define CNPK_ID_START_OUTPUT		0x001
#define CNPK_ID_END_OUTPUT		0x002
#define CNPK_ID_SET_JOB_ATTR		0x003
#define CNPK_ID_SET_DOC_ATTR		0x004
#define CNPK_ID_SET_PAGE_ATTR		0x006
#define CNPK_ID_SEND_DATA		0X007
#define CNPK_ID_FLUSH_SEND_DATA		0X008
#define CNPK_ID_NEXT_PAGE		0x010
#define CNPK_ID_GET_PAGEINFO		0x011
#define CNPK_ID_BIDI_START		0x012
#define CNPK_ID_BIDI_CMD_CTRL		0x013
#define CNPK_ID_EXIT_PROC   		0x0f0

int cnprocCreateProcess(int *pPId, CnpkPipeFds *pFds, int format, int fdOut );
int cnprocTermProcess(int nPId);
int cnprocWriteCommand( CnpkPipeFds tFds, int nCmdID,
						char *pData, int nDataSize);
int cnprocReadCommand( CnpkPipeFds tFds );
int cnprocWriteData( CnpkPipeFds tFds, char *pData, int nDataSize );
int cnprocCheckResponse( CnpkPipeFds tFds,int nCmdID,
						 int* nDataSize, char** pData );
int cnprocCanExecModule(void);

int cnprocCreateProcess_BIDI(int *pPId, CnpkPipeFds *pFds, int format, int fdOut, char *pOptList);

void DebugPrint(char *pFormat,...);
void ErrorPrint(char *pFormat,...);

#endif
