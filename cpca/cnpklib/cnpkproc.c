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


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdarg.h>
#include <sys/wait.h>

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
#include "buftool.h"

#if HAVE_CONFIG_H
#include <config.h>
#endif

#define MODULE_PATH		PROG_PATH

#ifdef _UFR2_
	#define MODULE_NAME		"cnpkmoduleufr2"
#elif _LIPS_
	#define MODULE_NAME		"cnpkmodulelips"
#elif _PS_
	#define MODULE_NAME		"cnpkmoduleps"
#else
	#define MODULE_NAME		"cnpkmodule"
#endif

#define MODULE_NAME_BIDI	"cnpkbidi"

#define FORMAT_STR	"--format=%d"
#define OUTPUT_STR	"--output=%d"
#define CMD_PIPE_STR "--cmd_fd=%d"
#define RES_PIPE_STR "--res_fd=%d"
#define DAT_PIPE_STR "--dat_fd=%d"
#define OPT_LIST_STR "--opt_list=%s"

#define CMD_HEADER_SIZE	4
#define RESPONSE_SIZE 6

#define EXEC_FAILURE 44

static int WritePipe( int fdPipe, char *pData, int nDataSize );
static int ReadPipe( int fdPipe, char *pData, int nDataSize );


int cnprocCreateProcess( int *pPId, CnpkPipeFds *pFds, int format, int fdOut )
{


	int command_pipe[2];
	int response_pipe[2];
	int data_pipe[2];
	int pid;


	if( pipe(command_pipe) == -1 ){
		DebugPrint( "Pipe Create Error1 \n");
		goto pipe_error;
	}
	if( pipe(response_pipe) == -1 ){
		DebugPrint( "Pipe Create Error2 \n");
		goto pipe_error;
	}
	if( pipe(data_pipe) == -1 ){
		DebugPrint( "Pipe Create Error3 \n");
		goto pipe_error;
	}

	if( (pid=fork()) == -1 ){
		DebugPrint( "Fork Error: \n");
		goto fork_error;
	}
	else if( pid == 0 ){
		char cmdpipe_opt[32];
		char respipe_opt[32];
		char datpipe_opt[32];
		char format_opt[32];
		char output_opt[32];
		char *module_arg[7];
		char *module_buf = NULL;
		int ret;

		close( command_pipe[1] );
		close( response_pipe[0] );
		close( data_pipe[1] );

		snprintf( format_opt, 31, FORMAT_STR, format );
		snprintf( output_opt, 31, OUTPUT_STR, fdOut );
		snprintf( cmdpipe_opt,31, CMD_PIPE_STR, command_pipe[0] );
		snprintf( respipe_opt,31, RES_PIPE_STR, response_pipe[1] );
		snprintf( datpipe_opt,31, DAT_PIPE_STR, data_pipe[0] );

		module_buf = malloc(strlen(MODULE_PATH)+strlen(MODULE_NAME)+2);
		if( module_buf == NULL ){
			goto malloc_error;
		}

		memset(module_buf, 0, sizeof(module_buf));
		strncpy(module_buf, MODULE_PATH, strlen(MODULE_PATH)+1);
		strcat(module_buf, "/");
		strncat(module_buf, MODULE_NAME, strlen(MODULE_NAME)+1);

		module_arg[0] = module_buf;
		module_arg[1] = format_opt;
		module_arg[2] = output_opt;
		module_arg[3] = cmdpipe_opt;
		module_arg[4] = respipe_opt;
		module_arg[5] = datpipe_opt;
		module_arg[6] = NULL;

		ret = execv(module_buf, module_arg);
		if( ret == -1 ){
			DebugPrint( "exec Error \n");
			exit(-1);

		}
	}
	else{
		close( command_pipe[0] );
		close( response_pipe[1] );
		close( data_pipe[0] );
	}

	*pPId = pid;
	pFds->cmd_fd = command_pipe[1];
	pFds->res_fd = response_pipe[0];
	pFds->dat_fd = data_pipe[1];

	return 0;

 malloc_error:
 fork_error:
 pipe_error:
	return -1;

}


int cnprocTermProcess( int nPId )
{
	int result = -1;

	if( nPId > 0 )
	{
		result = kill( nPId, SIGTERM );
	}
	return result;
}


int cnprocWriteCommand( CnpkPipeFds tFds, int nCmdID,
						char *pData, int nDataSize)
{
	int result = -1;

	int write_size = CMD_HEADER_SIZE + nDataSize;
	BufTool *buftool = buftool_new( write_size, BUFTOOL_LITTLE_ENDIAN );

	if( buftool ){

		buftool_write_short(buftool, nCmdID);
		buftool_write_short(buftool, nDataSize);
		buftool_write(buftool, pData, nDataSize);

		if( WritePipe(tFds.cmd_fd, (char *)buftool_data(buftool),
			       buftool_pos(buftool) ) < 0 ){
			DebugPrint("cnprocWriteCommand\n");
			result = -1;
		}
		else
			result = 0;
		buftool_destroy( buftool );

	}
	return result;
}


int cnprocCheckResponse( CnpkPipeFds tFds,  int nCmdID,
						 int* pDataSize, char** pRespData )
{

	char response[RESPONSE_SIZE+1];
	short result;
	short data_size;
	int fdRes = tFds.res_fd;
	short cmd;

	if( ReadPipe( fdRes, response, RESPONSE_SIZE ) == 0  ){
		BufTool *buf_tool = buftool_new(RESPONSE_SIZE, BUFTOOL_LITTLE_ENDIAN);

		memcpy(buftool_data(buf_tool), response, RESPONSE_SIZE);

		buftool_set_pos(buf_tool, 0);
		buftool_read_short(buf_tool, &cmd);
		buftool_read_short(buf_tool, &result);
		buftool_read_short(buf_tool, &data_size);

		if( cmd != nCmdID ){
			buftool_destroy(buf_tool);
			return -1;
		}

		if( data_size > 0 ){
			*pRespData = (char*)malloc(data_size);
			if((char*)*pRespData != NULL )
				ReadPipe( fdRes, (char*)*pRespData, data_size );
		}
		if( pDataSize != NULL )
			*pDataSize	= data_size;

		buftool_destroy(buf_tool);
		return result;
	}
	else
		return -1;

}


int cnprocWriteData(  CnpkPipeFds tFds, char *pData, int nDataSize )
{

	 if( WritePipe(tFds.dat_fd, pData, nDataSize) < 0 ){
		DebugPrint( "cnprocWriteData: Error\n");
		return -1;
	 }
	 return 0;
}


static int WritePipe( int fdPipe, char *pData, int nDataSize )
{

	int write_size;
	int total_size = nDataSize;

	while( total_size > 0 ){
		write_size = write( fdPipe, pData, total_size );
		if( write_size == -1 )
			goto write_error;
		total_size -= write_size;
		pData += write_size;
	}
	return 0;

 write_error:
	return -1;
}


static int ReadPipe( int fdPipe, char *pData, int nDataSize )
{

	int read_size;
	int total_size = nDataSize;

	while( total_size > 0 ){
		read_size = read( fdPipe, pData, total_size );
		if( read_size == -1 )
			goto read_error;
		total_size -= read_size;
		pData += read_size;
	}

	return 0;

 read_error:
	return -1;
}


void DebugPrint(char *pFormat,...)
{

#ifdef DEBUG
	va_list ap;

	fprintf(stderr, "DEBUG: ");

	va_start(ap, pFormat);
	vfprintf(stderr, pFormat, ap);
	va_end(ap);

	fprintf(stderr, "\n");
#endif
}


void ErrorPrint(char *pFormat,...)
{

	va_list ap;

	fprintf(stderr, "ERROR: ");

	va_start(ap, pFormat);
	vfprintf(stderr, pFormat, ap);
	va_end(ap);

	fprintf(stderr, "\n");

}


int cnprocCanExecModule(void)
{

	int pid;
	int ret = 0;

	if( (pid=fork()) == -1 ){
		DebugPrint( "Fork Error: \n");
		goto fork_error;
	}
	else if( pid == 0 ){
		char *module_arg[2];
		char *module_buf = NULL;

		module_buf = malloc(strlen(MODULE_PATH)+strlen(MODULE_NAME)+2);
		if( module_buf == NULL ){
			goto fork_error;
		}

		memset(module_buf, 0, sizeof(module_buf));
		strncpy(module_buf, MODULE_PATH, strlen(MODULE_PATH)+1);
		strcat(module_buf, "/");
		strncat(module_buf, MODULE_NAME, strlen(MODULE_NAME)+1);

		module_arg[0] = MODULE_NAME;
		module_arg[1] = NULL;

		execv(module_buf, module_arg );
		exit(EXEC_FAILURE);

	}

	if( pid > 0 ){
		int status;
		waitpid(pid, &status, 0);

		if( WEXITSTATUS(status) == EXEC_FAILURE )
			ret = 0;
		else
			ret = 1;
	}

	return ret;

 fork_error:
	return -1;

}

int cnprocCreateProcess_BIDI( int *pPId, CnpkPipeFds *pFds, int format, int fdOut, char *pOptList )
{

	int command_pipe[2];
	int response_pipe[2];
	int data_pipe[2];
	int pid;


	if( pipe(command_pipe) == -1 ){
		DebugPrint( "Pipe Create Error1 \n");
		goto pipe_error;
	}
	if( pipe(response_pipe) == -1 ){
		DebugPrint( "Pipe Create Error2 \n");
		goto pipe_error;
	}
	if( pipe(data_pipe) == -1 ){
		DebugPrint( "Pipe Create Error3 \n");
		goto pipe_error;
	}

	if( (pid=fork()) == -1 ){
		DebugPrint( "Fork Error: \n");
		goto fork_error;
	}
	else if( pid == 0 ){
		char cmdpipe_opt[32];
		char respipe_opt[32];
		char datpipe_opt[32];
		char format_opt[32];
		char output_opt[32];
		char *optlist_opt = NULL;
		char *module_arg[8];
		char *module_buf = NULL;
		int ret;

		int optlist_opt_size = 0;

		optlist_opt_size += sizeof( OPT_LIST_STR );
		optlist_opt_size += strlen( pOptList );
		optlist_opt_size += 1;

		optlist_opt = calloc( 1, optlist_opt_size );
		if( optlist_opt == NULL ) {
			goto malloc_error;
		}

		close( command_pipe[1] );
		close( response_pipe[0] );
		close( data_pipe[1] );

		snprintf( format_opt, 31, FORMAT_STR, format );
		snprintf( output_opt, 31, OUTPUT_STR, fdOut );
		snprintf( cmdpipe_opt,31, CMD_PIPE_STR, command_pipe[0] );
		snprintf( respipe_opt,31, RES_PIPE_STR, response_pipe[1] );
		snprintf( datpipe_opt,31, DAT_PIPE_STR, data_pipe[0] );
		snprintf( optlist_opt, optlist_opt_size, OPT_LIST_STR, pOptList );

		module_buf = malloc(strlen(MODULE_PATH)+strlen(MODULE_NAME_BIDI)+2);
		if( module_buf == NULL ){
			goto malloc_error;
		}

		memset(module_buf, 0, sizeof(module_buf));
		strncpy(module_buf, MODULE_PATH, strlen(MODULE_PATH)+1);
		strcat(module_buf, "/");
		strncat(module_buf, MODULE_NAME_BIDI, strlen(MODULE_NAME_BIDI)+1);

		module_arg[0] = module_buf;
		module_arg[1] = format_opt;
		module_arg[2] = output_opt;
		module_arg[3] = cmdpipe_opt;
		module_arg[4] = respipe_opt;
		module_arg[5] = datpipe_opt;
		module_arg[6] = optlist_opt;
		module_arg[7] = NULL;

		ret = execv(module_buf, module_arg);
		if( ret == -1 ){
			DebugPrint( "exec Error \n");
			exit(-1);

		}

		if( optlist_opt ) free( optlist_opt );
	}
	else{
		close( command_pipe[0] );
		close( response_pipe[1] );
		close( data_pipe[0] );
	}

	*pPId = pid;
	pFds->cmd_fd = command_pipe[1];
	pFds->res_fd = response_pipe[0];
	pFds->dat_fd = data_pipe[1];

	return 0;

 malloc_error:
 fork_error:
 pipe_error:
	return -1;

}
