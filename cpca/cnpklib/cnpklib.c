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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <unistd.h>

#ifdef _UFR2_
	#include "cnpklibufr2.h"
#elif _LIPS_
	#include "cnpkliblips.h"
#elif _PS_
	#include "cnpklibps.h"
#else
	#include "cnpklib.h"
#endif

#include "cnpkproc.h"
#include "cnpkopt.h"

#define METHOD_MEDIA_TYPE 0x01
#define METHOD_TONER_SAVE 0x02

#define MAX_PAGE_INFO 64

CnpkCtx*  cnpkNew( CnpkDocFormat format, int fdOut )
{

	CnpkCtx* pCnpk = NULL;
	int isModule = 0;

#ifdef DEBUG_MESSAGE
	fputs("DEBUG: cnpkNew\n", stderr);
#endif

	pCnpk = (CnpkCtx*)malloc(sizeof(CnpkCtx));
	if( pCnpk == NULL)
		goto malloc_error;
	memset(pCnpk, 0, sizeof(CnpkCtx));


	isModule = cnprocCanExecModule();

	if( isModule == 1 ){
		pCnpk->flgProc = 1;
		if( cnprocCreateProcess(&pCnpk->pid, &pCnpk->pipe_fds,
								format, fdOut) < 0 ){
				goto process_error;
		}
	}else{
		pCnpk->flgProc = 0;
		pCnpk->fdOut = fdOut;
	}

	return pCnpk;

 process_error:
	if( pCnpk )
		free(pCnpk);
 malloc_error:
	return NULL;

}

int  cnpkDestroy( CnpkCtx* pCnpk )
{
	int result = 0;

#ifdef DEBUG_MESSAGE
	fputs("DEBUG: cnpkDestroy\n", stderr);
#endif

	if( pCnpk->flgProc )
	{
#ifdef COVERAGE
		if( cnprocWriteCommand(pCnpk->pipe_fds, CNPK_ID_EXIT_PROC, NULL, 0) < 0)
		{
			return CNPK_ERROR;
		}
#else
		if( cnprocTermProcess(pCnpk->pid) != 0 )
		{
			if( cnprocWriteCommand(pCnpk->pipe_fds, CNPK_ID_EXIT_PROC, NULL, 0) < 0)
			{
				return CNPK_ERROR;
			}
		}
#endif
		waitpid(pCnpk->pid, NULL, 0);
	}
	else
	{
		if( pCnpk->pOptList != NULL )
		{
			option_list_free( pCnpk->pOptList );
		}
	}

 	if( pCnpk )
 	{
		free(pCnpk);
	}
	return result;
}


int cnpkSetJobAttr( CnpkCtx* pCnpk, char* pJobAttr )
{
	int result = 0;

#ifdef DEBUG_MESSAGE
	fputs("DEBUG: cnpkSetJobAttr\n", stderr);
#endif

	if( pCnpk->flgProc ){
		if( cnprocWriteCommand(pCnpk->pipe_fds, CNPK_ID_SET_JOB_ATTR,
							   pJobAttr, strlen(pJobAttr)+1) < 0 )
			return CNPK_ERROR;

		result = cnprocCheckResponse(pCnpk->pipe_fds, CNPK_ID_SET_JOB_ATTR,
									 NULL, NULL );
	}else{
		if( parse_option(&pCnpk->pOptList, pJobAttr) < 0 )
			result = -1;
	}

	return result;
}



int cnpkSetDocAttr( CnpkCtx* pCnpk, char* pDocAttr )
{
	int result = 0;

#ifdef DEBUG_MESSAGE
	fputs("DEBUG: cnpkSetDocAttr\n", stderr);
#endif

	if( pCnpk->flgProc ){
		if( cnprocWriteCommand(pCnpk->pipe_fds, CNPK_ID_SET_DOC_ATTR,
							   pDocAttr, strlen(pDocAttr)+1) < 0 )
			return CNPK_ERROR;

		result = cnprocCheckResponse(pCnpk->pipe_fds, CNPK_ID_SET_DOC_ATTR,
									 NULL, NULL );
	}else{
		if( parse_option(&pCnpk->pOptList, pDocAttr) < 0 )
			result = -1;
	}

	return result;
}



int cnpkSetPageAttr( CnpkCtx* pCnpk, char* pPageAttr )
{
	int result = 0;

#ifdef DEBUG_MESSAGE
	fputs("DEBUG: cnpkSetPageAttr\n", stderr);
#endif

	if( pCnpk->flgProc ){
		if( cnprocWriteCommand(pCnpk->pipe_fds, CNPK_ID_SET_PAGE_ATTR,
							   pPageAttr, strlen(pPageAttr)+1) < 0 )
			return CNPK_ERROR;

		result = cnprocCheckResponse(pCnpk->pipe_fds, CNPK_ID_SET_PAGE_ATTR,
									 NULL, NULL);

	}else{
		if( parse_option(&pCnpk->pOptList, pPageAttr) < 0 )
			result = -1;
	}

	return result;
}


int cnpkStartOutput( CnpkCtx* pCnpk )
{
	int result = 0;

#ifdef DEBUG_MESSAGE
	fputs("DEBUG: cnpkStartOutput\n", stderr);
#endif

	if( pCnpk->flgProc ){

		if( cnprocWriteCommand(pCnpk->pipe_fds,
							   CNPK_ID_START_OUTPUT, NULL, 0) < 0 )
			return CNPK_ERROR;

		result = cnprocCheckResponse(pCnpk->pipe_fds, CNPK_ID_START_OUTPUT,
									 NULL, NULL);
	}

	return result;
}



int cnpkSendData( CnpkCtx* pCnpk, char* pData, int nDataSize )
{

	int nTotalSize;
	char *pBuf;
	int result;

#ifdef DEBUG_MESSAGE
	fputs("DEBUG: cnpkSendData\n", stderr);
#endif

	if( pCnpk->flgProc ){
		nTotalSize = pCnpk->nBufSize + nDataSize;
		pBuf = (char*)pCnpk->bufPDLData;

		if( nTotalSize < MAX_BUF_SIZE ){
			pBuf += pCnpk->nBufSize;
			memcpy( pBuf, pData, nDataSize );
			pCnpk->nBufSize += nDataSize;

		}else{
			char size_str[32];
			char *pSrc = NULL;
			int tmp_size = pCnpk->nBufSize;

			if( tmp_size > 0 ){
				snprintf(size_str, 31, "%d", tmp_size);
				if( cnprocWriteCommand(pCnpk->pipe_fds, CNPK_ID_SEND_DATA,
									   size_str, strlen(size_str)+1) == 0 ){
					cnprocWriteData(pCnpk->pipe_fds,
									pCnpk->bufPDLData, tmp_size );
				}
				result = cnprocCheckResponse(pCnpk->pipe_fds,
											 CNPK_ID_SEND_DATA, NULL, NULL );

				if( result != CNPK_SUCCESS )
					goto send_error;

				nTotalSize -= tmp_size;
			}

			pSrc = pData;
			while( nTotalSize >= MAX_BUF_SIZE ){
				pBuf = (char*)&pCnpk->bufPDLData;
				memcpy(pBuf, pSrc, MAX_BUF_SIZE);
				snprintf(size_str, 31, "%d", MAX_BUF_SIZE);
				if( cnprocWriteCommand(pCnpk->pipe_fds, CNPK_ID_SEND_DATA,
									   size_str, strlen(size_str)+1) == 0 ){
					cnprocWriteData(pCnpk->pipe_fds,
									pCnpk->bufPDLData, MAX_BUF_SIZE );
				}
				result = cnprocCheckResponse(pCnpk->pipe_fds,
											 CNPK_ID_SEND_DATA, NULL, NULL );
				if( result != CNPK_SUCCESS )
					goto send_error;
				nTotalSize -= MAX_BUF_SIZE;
				pSrc += MAX_BUF_SIZE;
			}
			pBuf = (char*)&pCnpk->bufPDLData;
			memcpy(pBuf, pSrc, nTotalSize);
			pCnpk->nBufSize = nTotalSize;
		}
	}else{
		int total_size = nDataSize;
		int tmp_size = 0;

		while( total_size > 0 ){
			tmp_size = write(pCnpk->fdOut, pData, total_size);
			if( tmp_size < 0 )
				goto send_error;
			else
				total_size -= tmp_size;
		}
	}

	return CNPK_SUCCESS;

 send_error:
	return CNPK_ERROR;
}


int cnpkFlushSendData( CnpkCtx* pCnpk )
{
	int result = 0;
	char size_str[32];

	if( pCnpk->flgProc ){
		if( pCnpk->nBufSize > 0 ){
			snprintf(size_str, 31, "%d", pCnpk->nBufSize);
			if( cnprocWriteCommand(pCnpk->pipe_fds, CNPK_ID_SEND_DATA,
								   size_str, strlen(size_str)+1) == 0 ){
				cnprocWriteData(pCnpk->pipe_fds,
								pCnpk->bufPDLData, pCnpk->nBufSize );
			}
			if( (result = cnprocCheckResponse(pCnpk->pipe_fds,
										CNPK_ID_SEND_DATA, NULL, NULL)) == 0 )
				pCnpk->nBufSize = 0;
		}

		if( cnprocWriteCommand(pCnpk->pipe_fds, CNPK_ID_FLUSH_SEND_DATA,
							   NULL, 0) < 0 ){
			ErrorPrint( "cnpklib -->cnpkFlushSendData\n");
			goto write_error;
		}
		result = cnprocCheckResponse(pCnpk->pipe_fds, CNPK_ID_FLUSH_SEND_DATA,
									 NULL, NULL);
	}
	return result;

 write_error:
	return CNPK_ERROR;
}


int cnpkEndOutput( CnpkCtx* pCnpk )
{

	int result = 0;

#ifdef DEBUG_MESSAGE
	fputs("DEBUG: cnpkEndOutput\n", stderr);
#endif

	if( pCnpk->flgProc ){
		if( pCnpk->nBufSize > 0 ){
			char size_str[32];
			snprintf(size_str, 31, "%d", pCnpk->nBufSize);
			if( cnprocWriteCommand(pCnpk->pipe_fds, CNPK_ID_SEND_DATA,
								   size_str, strlen(size_str)+1) == 0 ){
				cnprocWriteData(pCnpk->pipe_fds,
								pCnpk->bufPDLData, pCnpk->nBufSize );
			}
			if( (result = cnprocCheckResponse(pCnpk->pipe_fds,
										CNPK_ID_SEND_DATA, NULL, NULL)) == 0 )
				pCnpk->nBufSize = 0;
		}

		if( cnprocWriteCommand(pCnpk->pipe_fds, CNPK_ID_END_OUTPUT,
							   NULL, 0) < 0 ){
			ErrorPrint( "cnpklib -->cnpkEndOutput\n");
			goto write_error;
		}
		result = cnprocCheckResponse(pCnpk->pipe_fds, CNPK_ID_END_OUTPUT,
									 NULL, NULL);
	}

	return result;

 write_error:
	return CNPK_ERROR;

}


int cnpkNextPage( CnpkCtx* pCnpk )
{

	int result = 0;

#ifdef DEBUG_MESSAGE
	fputs("DEBUG: cnpkNextPage\n", stderr);
#endif

	if( pCnpk->flgProc ){
		if( pCnpk->nBufSize > 0 ){
			char size_str[32];
			snprintf(size_str, 31, "%d", pCnpk->nBufSize);
			if( cnprocWriteCommand(pCnpk->pipe_fds, CNPK_ID_SEND_DATA,
								   size_str, strlen(size_str)+1) == 0 ){
				cnprocWriteData(pCnpk->pipe_fds,
								pCnpk->bufPDLData, pCnpk->nBufSize);

				if( (result=cnprocCheckResponse(pCnpk->pipe_fds,
										CNPK_ID_SEND_DATA, NULL, NULL)) == 0 )
					pCnpk->nBufSize = 0;
				else
						goto flush_error;

			}
		}

		if( cnprocWriteCommand(pCnpk->pipe_fds, CNPK_ID_NEXT_PAGE,
							   NULL, 0) < 0 ){
				ErrorPrint( "cnpklib -->cnpkNextPage\n");
				goto write_error;
		}
		result = cnprocCheckResponse(pCnpk->pipe_fds, CNPK_ID_NEXT_PAGE,
									 NULL, NULL);


	}else{
		pCnpk->numPage++;
	}

	return result;

 flush_error:
 write_error:
	return CNPK_ERROR;

}


int cnpkGetPageInfo( CnpkCtx* pCnpk, CnpkPageInfo* pPageInfo )
{
	int result = 0;
	int data_size = 0;
	char *pData;

#ifdef DEBUG_MESSAGE
	fputs("DEBUG: cnpkGetPageInfo\n", stderr);
#endif

	if( pCnpk->flgProc ){
		if( cnprocWriteCommand(pCnpk->pipe_fds, CNPK_ID_GET_PAGEINFO, NULL,
							   0) < 0 )
			return CNPK_ERROR;
		result = cnprocCheckResponse(pCnpk->pipe_fds, CNPK_ID_GET_PAGEINFO,
									 &data_size, &pData );

		if( result == 0 && pData != NULL ){
			char *pName;
			char *pValue;
			char *ptr = NULL;

			ptr = pData;

			while( *ptr != '\0'){

				while( isspace(*ptr) )
					ptr++;
				pName = ptr;

				while( *ptr != '='  && *ptr != '\0' )
					ptr++;
				*ptr = '\0'; ptr++;

				while( isspace(*ptr) )
					ptr++;

				pValue = ptr;
				while( *ptr != ';' && *ptr !='\0')
					ptr++;
				if( *ptr != '\0' ){
					*ptr = '\0';
					ptr++;
				}
				if( !strcmp(pName, "page_name") ){
					strncpy(pPageInfo->page_name, pValue, MAX_PAGE_INFO-1);
					pPageInfo->page_name[MAX_PAGE_INFO-1] = '\0';
				}else if( !strcmp(pName, "page_orient") ){
					strncpy(pPageInfo->page_orient, pValue, MAX_PAGE_INFO-1);
					pPageInfo->page_orient[MAX_PAGE_INFO-1] = '\0';
				}else if( !strcmp(pName, "resolution") ){
					strncpy(pPageInfo->resolution, pValue, MAX_PAGE_INFO-1);
					pPageInfo->resolution[MAX_PAGE_INFO-1] = '\0';
				}else if( !strcmp(pName, "color_mode") ){
					strncpy(pPageInfo->color_mode, pValue, MAX_PAGE_INFO-1);
					pPageInfo->color_mode[MAX_PAGE_INFO-1] = '\0';
				}else if( !strcmp(pName, "media_type") ){
					strncpy(pPageInfo->media_type, pValue, MAX_PAGE_INFO-1);
					pPageInfo->media_type[MAX_PAGE_INFO-1] = '\0';
				}else if( !strcmp(pName, "toner_save") ){
					strncpy(pPageInfo->toner_save, pValue, MAX_PAGE_INFO-1);
					pPageInfo->toner_save[MAX_PAGE_INFO-1] = '\0';
				}else if( !strcmp(pName, "bind_edge") ){
					strncpy(pPageInfo->bind_edge, pValue, MAX_PAGE_INFO-1);
					pPageInfo->bind_edge[MAX_PAGE_INFO-1] = '\0';
				}else if( !strcmp(pName, "bind_edge_shift") ){
					strncpy(pPageInfo->bind_edge_shift,
						   pValue, MAX_PAGE_INFO-1);
					pPageInfo->bind_edge_shift[MAX_PAGE_INFO-1] = '0';
				}else if( !strcmp(pName, "duplex") ){
					strncpy(pPageInfo->duplex, pValue, MAX_PAGE_INFO-1);
					pPageInfo->bind_edge_shift[MAX_PAGE_INFO-1] = '\0';
				}
			}
			free(pData);
		}
	}else{
		int model_method = 0;

		if( pPageInfo == NULL )
			return -1;

		memset(pPageInfo, 0, sizeof(CnpkPageInfo));

		{
			char *pValue = NULL;
			if( (pValue = option_list_get_value(pCnpk->pOptList,
											"CNModelMethod", NULL)) != NULL){
				model_method = atoi(pValue);
				free(pValue);
			}
		}

		{
			char *pValue = NULL;
			if( (pValue = option_list_get_value(pCnpk->pOptList,
											"PageSize", NULL)) != NULL){
				strncpy(pPageInfo->page_name, pValue, MAX_PAGE_INFO-1);
				pPageInfo->page_name[MAX_PAGE_INFO-1] = '\0';
				free(pValue);
			}else{
				strcpy(pPageInfo->page_name, "A4");
			}
		}

		{
			char *pValue = NULL;
			if( (pValue = option_list_get_value(pCnpk->pOptList,
											"Resolution", NULL)) != NULL){
				strncpy(pPageInfo->resolution, pValue, MAX_PAGE_INFO-1);
				pPageInfo->resolution[MAX_PAGE_INFO-1] = '\0';
				free(pValue);
			}else{
				strcpy(pPageInfo->resolution, "600");
			}
		}

		{
			char *pValue = NULL;
			if( (pValue = option_list_get_value(pCnpk->pOptList,
											"Orientation", NULL)) != NULL){
				strncpy(pPageInfo->page_orient, pValue, MAX_PAGE_INFO-1);
				pPageInfo->page_orient[MAX_PAGE_INFO-1] = '\0';
				free(pValue);
			}else{
				strcpy(pPageInfo->page_orient, "portrait");
			}
		}

		{
			char *pValue = NULL;
			if( (pValue = option_list_get_value(pCnpk->pOptList,
											"CNColorMode", NULL)) != NULL){
				if( !strcmp(pValue, "mono") ){
					strcpy(pPageInfo->color_mode, "gray");
				}
				else
					strcpy(pPageInfo->color_mode, "color");
				free(pValue);
			}else{
				strcpy(pPageInfo->color_mode, "gray");
			}
		}

		{
			char *pValue = NULL;
			pPageInfo->media_type[0] = '\0';
			if( model_method & METHOD_MEDIA_TYPE ){
				if( (pValue = option_list_get_value(pCnpk->pOptList,
												"MediaType", NULL)) != NULL){
					char *slot = NULL;
					if( (slot = option_list_get_value(pCnpk->pOptList,
											  "InputSlot", NULL)) != NULL){
						if( !strcasecmp(slot, "Auto") ||
							!strcasecmp(slot, "Manual") ){
							strncpy(pPageInfo->media_type,
									pValue, MAX_PAGE_INFO-1);
							pPageInfo->media_type[MAX_PAGE_INFO-1] = '\0';
						}
						free(slot);
					}
					free(pValue);
				}
			}else{
				pPageInfo->media_type[0] = '\0';
			}
		}

		{
			if( model_method & METHOD_TONER_SAVE ){
				char *pValue = NULL;
				if( (pValue = option_list_get_value(pCnpk->pOptList,
											"CNTonerSaving", NULL)) != NULL){
					strncpy(pPageInfo->toner_save, pValue, MAX_PAGE_INFO-1);
					pPageInfo->toner_save[MAX_PAGE_INFO-1] = '\0';
					free(pValue);
				}else{
					pPageInfo->media_type[0] = '\0';
				}
			}else{
				pPageInfo->media_type[0] = '\0';
			}
		}

		{

			char *pValue = NULL;
			if( (pValue = option_list_get_value(pCnpk->pOptList,
											"BindEdge", NULL)) != NULL){
				strncpy(pPageInfo->bind_edge, pValue, MAX_PAGE_INFO-1);
				pPageInfo->bind_edge[MAX_PAGE_INFO-1] = '\0';
				free(pValue);
			}else{
				strcpy(pPageInfo->bind_edge, "Left");
			}
		}



		{
			char *pMaxGutt = NULL;
			char *pValue = NULL;
			int shift = 0;
			int nMaxGutt = 30;

			if( (pMaxGutt = option_list_get_value(pCnpk->pOptList,
											  "CNMaxGutter", NULL)) != NULL){
				nMaxGutt =atoi(pMaxGutt);
				free(pMaxGutt);
			}

			if( (pValue = option_list_get_value(pCnpk->pOptList,
										"CNBindEdgeShift", NULL)) != NULL){
				shift =atoi(pValue);
				if( shift < 0 ){
					strcpy(pPageInfo->bind_edge_shift, "0");
				}
				else if( shift > nMaxGutt ){
					char bufTmp[64];
					snprintf(bufTmp, 63, "%d", nMaxGutt);
					strncpy(pPageInfo->bind_edge_shift,
							bufTmp, MAX_PAGE_INFO-1);
					pPageInfo->bind_edge[MAX_PAGE_INFO-1] = '\0';
				}
				else{
					strncpy(pPageInfo->bind_edge_shift,
							pValue, MAX_PAGE_INFO-1);
					pPageInfo->bind_edge_shift[MAX_PAGE_INFO-1] = '0';
				}
				free(pValue);
			}else{
				strcpy(pPageInfo->bind_edge_shift, "0");
			}
		}

		{

			char *pValue = NULL;
			if( (pValue = option_list_get_value(pCnpk->pOptList,
											"Duplex", NULL)) != NULL){
				if( (strcmp(pValue, "False") == 0 ) ||
					(strcmp(pValue, "None") == 0 ) ){
					strncpy(pPageInfo->duplex, "False", MAX_PAGE_INFO-1);
					pPageInfo->duplex[MAX_PAGE_INFO-1] = '\0';
				}else{
					strncpy(pPageInfo->duplex, "True", MAX_PAGE_INFO-1);
					pPageInfo->duplex[MAX_PAGE_INFO-1] = '\0';
				}
				free(pValue);
			}else{
				strcpy(pPageInfo->duplex, "False");
			}
		}



	}

		return result;
}

CnpkCtx*  cnpkbidiNew( CnpkDocFormat format, int fdOut, char* pOptList )
{

	CnpkCtx* pCnpk = NULL;

#ifdef DEBUG_MESSAGE
	fputs("DEBUG: cnpkbidiNew\n", stderr);
#endif

	pCnpk = (CnpkCtx*)malloc(sizeof(CnpkCtx));
	if( pCnpk == NULL)
		goto malloc_error;
	memset(pCnpk, 0, sizeof(CnpkCtx));


		pCnpk->flgProc = 1;
		if( cnprocCreateProcess_BIDI(&pCnpk->pid, &pCnpk->pipe_fds, format, fdOut, pOptList) < 0 ){
				goto process_error;
		}

	return pCnpk;

 process_error:
	if( pCnpk )
		free(pCnpk);
 malloc_error:
	return NULL;

}

int cnpkBidiStart( CnpkCtx* pCnpk, int VerNo )
{
	int result = 0;

#ifdef DEBUG_MESSAGE
	fputs("DEBUG: cnpkBidiStart\n", stderr);
#endif
	char *pData = (char*)&VerNo;

	if( pCnpk->flgProc ){
		if( cnprocWriteCommand(pCnpk->pipe_fds, CNPK_ID_BIDI_START, pData, sizeof(int)) < 0 )
			return CNPK_ERROR;

		result = cnprocCheckResponse(pCnpk->pipe_fds, CNPK_ID_BIDI_START, NULL, NULL );
	}else{
		if( parse_option(&pCnpk->pOptList, pData) < 0 )
			result = -1;
	}

	return result;
}

int cnpkBidiCmdCtrl( CnpkCtx* pCnpk, int CmdNo )
{
	int result = 0;

#ifdef DEBUG_MESSAGE
	fputs("DEBUG: cnpkBidiCmdCtrl\n", stderr);
#endif
	char *pData = (char*)&CmdNo;

	if( pCnpk->flgProc ){
		if( cnprocWriteCommand(pCnpk->pipe_fds, CNPK_ID_BIDI_CMD_CTRL, pData, sizeof(int)) < 0 )
			return CNPK_ERROR;

		result = cnprocCheckResponse(pCnpk->pipe_fds, CNPK_ID_BIDI_CMD_CTRL, NULL, NULL );
	}else{
		if( parse_option(&pCnpk->pOptList, pData) < 0 )
			result = -1;
	}

	return result;
}

int  cnpkBidiDestroy( CnpkCtx* pCnpk )
{
	int result = 0;

#ifdef DEBUG_MESSAGE
	fputs("DEBUG: cnpkBidiDestroy\n", stderr);
#endif
	char cData = 0;
	char *pData = &cData;

	if( pCnpk != NULL )
	{
		if( pCnpk->flgProc != 0 )
		{
#ifdef COVERAGE
			if( cnprocWriteCommand(pCnpk->pipe_fds, CNPK_ID_EXIT_PROC, pData, sizeof(int)) < 0)
			{
				result = CNPK_ERROR;
			}
#else
			if( cnprocTermProcess(pCnpk->pid) != 0 )
			{
				if( cnprocWriteCommand(pCnpk->pipe_fds, CNPK_ID_EXIT_PROC, pData, sizeof(int)) < 0)
				{
					result = CNPK_ERROR;
				}
			}
#endif
			if( result == 0 )
			{
				waitpid(pCnpk->pid, NULL, 0);
			}
		}
		else
		{
			if( pCnpk->pOptList != NULL )
			{
				option_list_free( pCnpk->pOptList );
			}
		}
	}

 	if( pCnpk != NULL )
 	{
		free( pCnpk );
	}
	return result;
}
