/*
 *  CUPS add-on module for Canon UFR II printer.
 *  Copyright CANON INC. 2004-2011
 *  All Rights Reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <cups/cups.h>
#include <cups/ppd.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>

#include "paramlist.h"
#include "buflist.h"



#define	INDENT_NONE	0
#define	INDENT_RIGHT	1
#define	INDENT_LEFT	2

#if HAVE_CONFIG_H
#include <config.h>
#endif

#define	EXEC_PATH		PROG_PATH
#define	EXEC_GS			"gs"

#define	TABLE_BUF_SIZE	(1024 * 2)
#define	LINE_BUF_SIZE	1024
#define	DATA_BUF_SIZE	(1024 * 256)
#define	PARAM_BUF_SIZE	256
#define	BUFSIZE	256

#define IS_BLANK(c)	(c == ' '  || c == '\t')
#define IS_RETURN(c)	(c == '\r' || c == '\n')

#define ACRO_UTIL "%%BeginResource: procset Adobe_CoolType_Utility_MAKEOCF"
#define ACRO_UTIL_LEN 55

#define	BANNER_DESC "%%BeginResource procset bannerprint"
#define	BANNER_DESC_LEN 35

#define	NUMOF( array )	( sizeof( array ) / sizeof( ( array )[ 0 ] ) )

#define OPTIONLIST_VALUE_DELIMITER ';'
#define EQUAL_AND_SEMICOLON_SIZE 2U
#define LEN_KEY_OPT_LIST_INFO_NUM 2

#define PS_NAME_SPACE_SIZE		3U
#define UPDF_NAME_SPACE_SIZE	5U

typedef struct
{
	const char *key1;
	const char *value1;
	const char *key2;
	const char *value2;
	const char *key3;
	const char *value3;
	const char *change_key;
	const char *change_value;
} ChangeOptListTable;

static ChangeOptListTable *get_change_option_list_table( const int id );
static ParamList *get_p_job_attr_list( const char * const p_job_attr );
static void change_p_job_attr_list( ParamList **p_list );


enum {
	ARGV_GS,
	ARGV_RESO,
	ARGV_WIDTH,
	ARGV_HEIGHT,
	ARGV_DEVICE,
	ARGV_DRIVER,
	ARGV_MODEL,
	ARGV_JOB,
};

char *org_cmd_arg[] = {
NULL,
"-r",
"-dDEVICEWIDTH=",
"-dDEVICEHEIGHT=",
"-sDEVICE=",
"-sDriver=",
"-sModel=",
"-sJobInfo=",
"-q",
"-dBATCH",
"-dSAFER",
"-dQUIET",
"-dNOPAUSE",
"-sFastImage=All",
"-sOutputFile=-",
"-",
NULL
};

#define	ARGV_NUM	(sizeof(org_cmd_arg)/sizeof(char*))

int g_filter_pid = -1;
int g_signal_received = 0;

#ifdef	DEBUG_IN_PS
int g_in_log_fd = -1;
#endif


char *ppd_opt_name[] =
{
	"PageSize",
	"MediaType",
	"InputSlot",
	"Collate",
	"OutputBin",
	"Resolution",
	"CNTonerSaving",
	"CNHalftone",
	"BindEdge",
	"Duplex",
	"CNBackPaperPrint",
	"CNRotatePrint",
	"CNColorMode",
	"CNColorHalftone",
	"StapleLocation",
	"CNPunch",
	"CNPunchLocation",
	"CNOutputPartition",
	"CNPureBlackProcess",
	"CNJobExecMode",
	"CNSaddleStitch",
	"CNZfolding",
	"CNTrimming",
	"CNTrimmingValue",
	"CNShiftPrint",
	"CNBlackSubstitute",
	"CNOutputFace",
	"CNImageSpecialProcess",
	"CNCopySetNumbering",
	"CNDraftMode",
	"CNInterleafSheet",
	"CNInterleafMediaType",
	"CNInterleafPrint",
	"CNImageRefinement",
	"CNLineRefinement",
	"CNSpotColorMatching",
	"CNOHPPrintMode",
	"CNAdvancedSmoothing",
	"CNSpecialPrintMode",
	"CNSetFrontSheet",
	"CNFrontPrintOn",
	"CNFrontPaperSrc",
	"CNSetBackSheet",
	"CNBackPrintOn",
	"CNBackPaperSrc",
	"CNFinisher",
	"BookLet",
	"CNInserter",
	"CNSmallSizeCurl",
	"CNShiftStartPrintPosition",
	"CNSharpness",
	"CNCreep",
	"CNDisplacementCorrection",
	"CNAdjustTrim",
	"CNCfolding",
	"CNCfoldSetting",
	"CNTrustPrint",
	"CNLineControl",
	"CNTonerVolumeAdjustment",
	"CNSpecialPrintAdjustmentA",
	"CNSpecialPrintAdjustmentB",
	"CNSpecialSmooth",
	"CNGlossyPlainPaperProc",
	"CNMultiPunch",
	"CNHalfFolding",
	"CNAccordionZfolding",
	"CNDoubleParallelFolding",
	"CNHalfFoldSetting",
	"CNAccordionZfoldSetting",
	"CNDoubleParallelFoldSetting",
	"CNVfolding",
	"CNTonerDensity",
	"CNPlainPaperCurlCorrect",
	"CNMonitorProfile",
	"CNMatchingMethod",
	"CNOutputAdjustment",
	"CNSpecialPrintAdjustment"
};


char *comma_chg_opt_name[] =
{
	"CNBindEdgeShift",
	"CNShiftFrShortEdge",
	"CNShiftFrLongEdge",
	"CNShiftBkShortEdge",
	"CNShiftBkLongEdge",
	"CNGutterShiftNum",
	"CNAdjustTrimNum",
};


char *cmd_opt_name[] =
{
	"CNBindEdgeShift",
	"CNJobAccount",
	"CNSecuredPrint",
	"CNMailBox",
	"CNBookletShift",
	"CNDocName",
	"CNUsrName",
	"CNProofPrint",
	"number-up",
	"CNStartingNumber",
	"CNOffsetNum",
	"CNShiftFrShortEdge",
	"CNShiftFrLongEdge",
	"CNShiftBkShortEdge",
	"CNShiftBkLongEdge",
	"CNGutterShiftNum",
	"CNAdjustTrimNum",
	"orientation-requested",
};

char *drv_tbl_name[] =
{
	"CNTblFormat",
	"CNTblInputSlot",
	"CNTblDuplex",
	"CNTblOutputBin",
	"CNTblSubstitute",
	"CNTblMediaType",
	"CNModelMethod",
	"CNMaxBoxNum",
	"CNMaxDocStr",
	"CNTblOutputPartition",
	"CNUSType",
	"CNMaxCopies",
	"CNMaxGutter",
	"CNExecuteMethod",
	"CNPrinterName",
	"CNPDLType",
	"CNOEFLibName",
	"CNPrtColorSpace",
	"CNOptDevType",
	"CNOptCalibType",
	"CN_PdlWrapper_PdlPath",
	"CN_PdlWrapper_DebugMode",
	"CNUFR2ModelMethod",
	"CNUIOffsetMax",
	"CNShiftStartPrintPosType",
	"CNShiftPositionMin",
	"CNShiftPositionMax",
	"CNMINGutterShiftNum",
	"CNMAXGutterShiftNum",
	"CNDefaultGutterShiftNum",
	"CNUITrimValMin",
	"CNUITrimValMax",
	"CNUIAdjustTrimNumDefault",
	"CNTblRetouchType",
	"CN_PDL_SLC_FI_RN_NL_K",
	"CN_PDL_SLC_FI_GN_NL_K",
	"CN_PDL_SLC_FI_CE_NL_K",
	"CN_PDL_SLC_FI_RN_HHA_K",
	"CN_PDL_SLC_FI_RN_HHA_Y",
	"CN_PDL_SLC_FI_RN_HHA_M",
	"CN_PDL_SLC_FI_RN_HHA_C",
	"CN_PDL_SLC_FI_GN_HHA_K",
	"CN_PDL_SLC_FI_GN_HHA_Y",
	"CN_PDL_SLC_FI_GN_HHA_M",
	"CN_PDL_SLC_FI_GN_HHA_C",
	"CN_PrinterInfo_Path",
	"CN_PrinterInfo_DefaultPath",
	"CNHostDraftMode",
	"CNNeedInterpData",
	"CNLandRotation",
	"CNHostRotation",
	"CNDR_FINM",
	"CNDR_TINM",
	"CNDR_LINM",
	"CNDR_BINM",
	"CNDR_VRNM",
	"CNDR_ATNM",
	"CNDR_SFLT",
	"CNDR_SFCT",
	"CNDR_SFRT",
	"CNDR_TRLL",
	"CNDR_TRLU",
	"CNDR_SFWD",
	"CNDR_AJLN",
	"CNDR_EXLN",
	"CNDR_BKPX",
	"CNDR_SGMX",
	"CNDR_TOMD",
	"CNDR_TOPX",
	"CNDR_BTMX",
	"CNDR_BPMX",
	"CNDR_BCMX",
	"CNDR_BDMX",
	"CNDR_THNP",
	"CNDR_SMDT",
	"CNDR_EXBP",
	"CNTblTrayAlias",
	"CN_PDL_RP",
	"CNTblTonerDensity",
	"CN_PDL_TL",
	"CN_PDL_CTN",
	"CN_PDL_BV",
	"CN_PDL_CV",
	"CNOutputDepth",
	"CNChangeOptList"
};

enum {
	DRV_LIST_CNFEEDDIRECTION,

	DRV_LIST_NUM,
};

char *drv_list_name[] =
{
	"CNFeedDirection",
};

char *org_page_size_table[] =
{
	"jenv_you_chou3",
};

enum {
	OPVP_MODEL,
	OPVP_DRIVER,
	OPVP_DEVICE,
};

char *mdl_tbl_name[] =
{
	"opvpModel",
	"opvpDriver",
	"opvpDevice",
};

char *drv_cs_name = "CNDefaultCS";

char *drv_cs_val[] = {
	"RGB",
	"Gray",
};

enum {
	CS_RGB,
	CS_GRAY,
};

#define CHANGE_OPTION_LIST_TABLE_0_PARAM_NUM 1

static ChangeOptListTable change_option_list_table_0[CHANGE_OPTION_LIST_TABLE_0_PARAM_NUM] =
{
	{NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
};

#define CHANGE_OPTION_LIST_TABLE_1_PARAM_NUM 57

static ChangeOptListTable change_option_list_table_1[CHANGE_OPTION_LIST_TABLE_1_PARAM_NUM] =
{
	{"CNSpecialPrintAdjustment", "Settings1",		 NULL, NULL, NULL, NULL, "CNOutputAdjustment", "True"},
	{"PageSize",				 "Postcard",		 NULL, NULL, NULL, NULL, "CNOutputAdjustment", "True"},
	{"PageSize",				 "dbl_postcard",	 NULL, NULL, NULL, NULL, "CNOutputAdjustment", "True"},
	{"PageSize",				 "4x_postcard",		 NULL, NULL, NULL, NULL, "CNOutputAdjustment", "True"},
	{"PageSize",				 "Monarch",		 NULL, NULL, NULL, NULL, "CNOutputAdjustment", "True"},
	{"PageSize",				 "Com10",		 NULL, NULL, NULL, NULL, "CNOutputAdjustment", "True"},
	{"PageSize",				 "dl_envelope",		 NULL, NULL, NULL, NULL, "CNOutputAdjustment", "True"},
	{"PageSize",				 "Envelope_C5",		 NULL, NULL, NULL, NULL, "CNOutputAdjustment", "True"},
	{"PageSize",				 "Envelope_B5",		 NULL, NULL, NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "LABELS",			 NULL, NULL, NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "ENVELOPE",			 NULL, NULL, NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "ROUGH",			 "PageSize", "Letter",			 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "ROUGH",			 "PageSize", "Legal",			 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "ROUGH",			 "PageSize", "A4",				 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "ROUGH",			 "PageSize", "jenv_you_chou3",	 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "ROUGH",			 "PageSize", "jenv_chou3",		 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "ROUGH",			 "PageSize", "Monarch",			 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "ROUGH",			 "PageSize", "Com10",			 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "ROUGH",			 "PageSize", "dl_envelope",		 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "ROUGH",			 "PageSize", "Envelope_C5",		 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "ROUGH",			 "PageSize", "Envelope_B5",		 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "ROUGH",			 "PageSize", "16K",				 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "ROUGH",			 "PageSize", "Foolscap",		 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY1",			 "PageSize", "Letter",			 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY1",			 "PageSize", "Legal",			 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY1",			 "PageSize", "A4",				 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY1",			 "PageSize", "jenv_you_chou3",	 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY1",			 "PageSize", "jenv_chou3",		 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY1",			 "PageSize", "Monarch",			 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY1",			 "PageSize", "Com10",			 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY1",			 "PageSize", "dl_envelope",		 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY1",			 "PageSize", "Envelope_C5",		 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY1",			 "PageSize", "Envelope_B5",		 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY1",			 "PageSize", "16K",				 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY1",			 "PageSize", "Foolscap",		 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY2",			 "PageSize", "Letter",			 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY2",			 "PageSize", "Legal",			 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY2",			 "PageSize", "A4",				 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY2",			 "PageSize", "jenv_you_chou3",	 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY2",			 "PageSize", "jenv_chou3",		 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY2",			 "PageSize", "Monarch",			 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY2",			 "PageSize", "Com10",			 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY2",			 "PageSize", "dl_envelope",		 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY2",			 "PageSize", "Envelope_C5",		 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY2",			 "PageSize", "Envelope_B5",		 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY2",			 "PageSize", "16K",				 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "HEAVY2",			 "PageSize", "Foolscap",		 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "PlainPaper",		 "PageSize", "A5",		 	 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "PlainPaper",		 "PageSize", "Executive",	 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "PlainPaper",		 "PageSize", "B5",			 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "PlainPaper",		 "PageSize", "Statement",	 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "PlainPaperL",		 "PageSize", "A5",		 	 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "PlainPaperL",		 "PageSize", "Executive",	 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "PlainPaperL",		 "PageSize", "B5",			 NULL, NULL, "CNOutputAdjustment", "True"},
	{"MediaType",				 "PlainPaperL",		 "PageSize", "Statement",	 NULL, NULL, "CNOutputAdjustment", "True"},
	{"CNOutputAdjustment",		 "False",		 NULL, NULL, NULL, NULL, "CNPrtColorSpace", "Gray"},
	{NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
};

#define CHANGE_OPTION_LIST_TABLE_2_PARAM_NUM 7

static ChangeOptListTable change_option_list_table_2[CHANGE_OPTION_LIST_TABLE_2_PARAM_NUM] =
{
	{"Collate", "StapleCollate", "MediaType", "HEAVY2",		 "orientation-requested", "4", "Collate", "True"},
	{"Collate", "StapleCollate", "MediaType", "HEAVY2",		 "orientation-requested", "5", "Collate", "True"},
	{"Collate", "StapleCollate", "MediaType", "BOND",		 "orientation-requested", "4", "Collate", "True"},
	{"Collate", "StapleCollate", "MediaType", "BOND",		 "orientation-requested", "5", "Collate", "True"},
	{"Collate", "StapleCollate", "orientation-requested", "4",	NULL, NULL, "StapleLocation", "BottomLeft"},
	{"Collate", "StapleCollate", "orientation-requested", "5",	NULL, NULL, "StapleLocation", "BottomLeft"},
	{NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL}
};

#define CHANGE_OPTION_LIST_TABLE_LIST_NUM 3

static ChangeOptListTable* change_option_list_table_list[CHANGE_OPTION_LIST_TABLE_LIST_NUM] =
{
	change_option_list_table_0,
	change_option_list_table_1,
	change_option_list_table_2
};

#ifdef DEBUGLOG
	#include <stdarg.h>

	static void debuglog(int indent, const char *message, ...)
	{
		static int debug_indent;
		va_list	ap;
		int	i;
		FILE *fp = fopen("/tmp/pstoufr2cpca.log", "a");

		if (indent == INDENT_LEFT)
		{
			debug_indent--;
			if (debug_indent < 0){
				debug_indent = 0;
			}
		}

		for (i = 0; i < debug_indent; i++)
		{
			fprintf(fp, "\t");
		}

		va_start(ap, message);
		vfprintf(fp, message, ap);

		fclose(fp);

		if (indent == INDENT_RIGHT)
		{
			debug_indent++;
		}
	}
#else
	#define	debuglog(a, b, ...)
#endif

static
int is_comma_chg_opt_name(char *name)
{
	int num = sizeof(comma_chg_opt_name) / sizeof(char *);
	int i;

	for( i = 0; i < num; i++ )
	{
		if( strcmp(name, comma_chg_opt_name[i]) == 0 )
		{
			return 1;
		}
	}
	return 0;
}


static
int get_comma_chg_env()
{
	double num = 0.11;
	char * tmp = NULL;
	int ret = 0;
	char * value = (char *)malloc(8);
	snprintf(value, 8, "%.2f", num);

	if( (tmp = strchr(value, ',') ) != NULL)
	{
      	ret = 1;
	}
	else if( (tmp = strchr(value, '.') ) != NULL)
	{
		ret = 2;
	}
	free(value);

	return ret;
}


static
bool is_cmd_opt_name(char *name)
{
	int num = sizeof(cmd_opt_name) / sizeof(char *);
	int i;

	for( i = 0; i < num; i++ )
	{
		if( strcmp(name, cmd_opt_name[i]) == 0 )
		{
			return true;
		}
	}
	return false;
}

static
bool is_drv_tbl_name(char *name)
{
	int num = sizeof(drv_tbl_name) / sizeof(char *);
	int i;

	for( i = 0; i < num; i++ )
	{
		if( strcmp(name, drv_tbl_name[i]) == 0 )
		{
			return true;
		}
	}
	return false;
}

static
bool is_org_page_size_table(ppd_file_t *p_ppd)
{
	int num = sizeof(org_page_size_table) / sizeof(char *);
	ppd_choice_t *p_choice = NULL;
	int i;

	p_choice = ppdFindMarkedChoice(p_ppd, "PageSize");

	if(p_choice != NULL)
	{
		for( i = 0; i < num; i++ )
		{
			if( strncmp(p_choice->choice, org_page_size_table[i], strlen(org_page_size_table[i]) ) == 0 )
			{
				return true;
			}
		}
	}
	return false;
}


static
int get_CNCopies_value(cups_option_t *p_cups_opt, int num_opt, int copies, char *p_value_buf, int value_len)
{
	cups_option_t *p_opt = p_cups_opt;
	char *cncopies = NULL;
	int cncopies_len = 0;
	int i;

	for( i = 0; i < num_opt; i++)
	{
		if(strcmp(p_opt->name, "CNCopies") == 0)
		{
			cncopies = p_opt->value;
			break;
		}
		p_opt++;
	}

	if(copies > 1)
	{
		snprintf(p_value_buf, value_len, "%d", copies);
		cncopies_len = strlen(p_value_buf);
	}
	else if(copies == 1)
	{
		if(cncopies == NULL)
		{
			snprintf(p_value_buf, value_len, "%d", copies);
			cncopies_len = strlen(p_value_buf);
		}
		else
		{
			strncpy(p_value_buf, cncopies, value_len);
			cncopies_len = strlen(p_value_buf);
		}
	}
	else if(copies == -1 && cncopies != NULL)
	{
		strncpy(p_value_buf, cncopies, value_len);
		cncopies_len = strlen(p_value_buf);
	}
	else if(copies == -2)
	{
		strcpy(p_value_buf, "1");
		cncopies_len = strlen(p_value_buf);
	}

	return cncopies_len;
}

static ChangeOptListTable* get_change_option_list_table( const int id )
{
	int table_id = id;
	const int num = sizeof(change_option_list_table_list) / sizeof(ChangeOptListTable *);

	if ((0 > table_id) || (table_id > num))
	{
		table_id = 0;
	}
	return change_option_list_table_list[table_id];
}

static ParamList *get_p_job_attr_list( const char * const p_job_attr )
{
	ParamList *p_list = NULL;
	char *pName = NULL;
	char *pValue = NULL;
	char *pTemp = NULL;
	char *pOptList = NULL;
	unsigned int idx = 0;
	int add_value_size = 0;

	pTemp = strdup(p_job_attr);
	pOptList = pTemp;

	struct _OptListInfo {
		const char	*pKeyName;
		const char	*pKeyNameValueLen;
	}
	const OptListInfo[LEN_KEY_OPT_LIST_INFO_NUM] = {
		{ "CNUsrName",		"CNUsrNameLen"		},
		{ "CNDocName",		"CNDocNameLen"		}
	};

	if( pOptList != NULL ) {
		while( *pOptList != '\0') {
			while( isspace(*pOptList) != 0 ) {
				pOptList++;
			}

			if( strncasecmp(pOptList, "ps:", PS_NAME_SPACE_SIZE) == 0 ) {
				pOptList += PS_NAME_SPACE_SIZE;
			}
			pName = pOptList;

			while( (*pOptList != '=')  && (*pOptList != '\0') ) {
				pOptList++;
			}
			*pOptList = '\0';
			pOptList++;

			while( isspace(*pOptList) != 0 ) {
				pOptList++;
			}
			pValue = pOptList;

			for( idx = 0; idx < NUMOF(OptListInfo); idx++ ) {
				if( strcasecmp( pName, OptListInfo[ idx ].pKeyName ) == 0 ) {
					if( strstr( p_job_attr, OptListInfo[ idx ].pKeyNameValueLen ) != NULL) {
						break;
					}
				}
			}

			if( idx < NUMOF(OptListInfo) ) {
				char *pLenValue = NULL;
				pLenValue = strstr( p_job_attr, OptListInfo[ idx ].pKeyNameValueLen );
				pLenValue += ( strlen( OptListInfo[ idx ].pKeyNameValueLen ) + 1U );

				pOptList += atoi( pLenValue );
				if( pOptList != NULL ) {
					if( *pOptList != '\0' ) {
						*pOptList = '\0';
						pOptList++;
					}
				}
			}
			else {
				while( (*pOptList != ';') && (*pOptList !='\0') ) {
					pOptList++;
				}
				if( *pOptList != '\0' ) {
					*pOptList = '\0';
					pOptList++;
				}
			}
			add_value_size = (int)(strlen(pValue)+1U) ;
			param_list_add( &p_list, pName, pValue, add_value_size );
		}
	}
	free(pTemp);
	return p_list;
}

static void change_p_job_attr_list( ParamList ** const p_list )
{
	int			table_id = 0;
	int			change_value_size = 0;
	ParamList*	curs = NULL;
	ParamList*	curs2 = NULL;
	ParamList*	curs3 = NULL;
	ChangeOptListTable*	change_option_list_table	= NULL;
	ChangeOptListTable*	p_table						= NULL;

	curs = param_list_find(*p_list, "CNChangeOptList");
	if (curs != NULL)
	{
		table_id = atoi(curs->value);
	}

	change_option_list_table = get_change_option_list_table(table_id);
	if (change_option_list_table != NULL)
	{
		p_table = change_option_list_table;

		while (p_table->key1 != NULL)
		{
			curs = param_list_find(*p_list, p_table->key1);
			if (curs != NULL)
			{
				if (strcasecmp(p_table->value1, curs->value) == 0)
				{
					if (p_table->key2 == NULL)
					{
						change_value_size = (int)(strlen(p_table->change_value)+1U);
						param_list_add(p_list, p_table->change_key, p_table->change_value, change_value_size);
					}
					else
					{
						curs2 = param_list_find(*p_list, p_table->key2);
						if (curs2 != NULL)
						{
							if (strcasecmp(p_table->value2, curs2->value) == 0)
							{
								if (p_table->key3 == NULL)
								{
									change_value_size = (int)(strlen(p_table->change_value)+1U);
									param_list_add(p_list, p_table->change_key, p_table->change_value, change_value_size);
								}
								else
								{
									curs3 = param_list_find(*p_list, p_table->key3);
									if (curs3 != NULL)
									{
										if (strcasecmp(p_table->value3, curs3->value) == 0)
										{
											change_value_size = (int)(strlen(p_table->change_value)+1U);
											param_list_add(p_list, p_table->change_key, p_table->change_value, change_value_size);
										}
									}
								}
							}
						}
					}
				}
			}
			p_table++;
		}
	}

	return;
}

static
char *ppd_mark_to_job_attr(ppd_file_t *p_ppd, cups_option_t *p_cups_opt,
	 int num_opt, char *p_table, int copies, char *user, char *title)
{
	int ppd_opt_num = sizeof(ppd_opt_name) / sizeof(char*);
	unsigned int opt_len = strlen(p_table);
	char *p_job_attr = NULL;
	char *p_cs = drv_cs_val[CS_GRAY];
	ppd_choice_t *p_choice;
	cups_option_t *p_opt;
	char value[MAX_VALUE_LEN + 1];
	int cncopies_len = 0;
	int is_docname = 0;
	int is_usrname = 0;
  	int tmp_env = 0;
  	char *p_tmp = NULL;
	char *p_device_uri = NULL;
	int i;
	char *p_size_name = NULL;
	ppd_size_t *p_size = NULL;
	char cBuf[255];

	unsigned int	docname_len = 0;
	unsigned int	usrname_len = 0;
	unsigned int	idx = 0;

	struct _LENKEYINFO {
		const char	*pKeyName;
		const char	*pKeyNameLen;
		unsigned int	*pDataSize;
		int		*pDataFlag;
	} len_key_info[LEN_KEY_OPT_LIST_INFO_NUM] = {
		{ "CNUsrName",		"CNUsrNameLen",		&usrname_len,		&is_usrname	},
		{ "CNDocName",		"CNDocNameLen",		&docname_len,		&is_docname	},
	};
	ParamList*	p_list = NULL;
	ParamList*	curs = NULL;

	memset(value, 0, MAX_VALUE_LEN + 1);

	cncopies_len
	 = get_CNCopies_value(p_cups_opt, num_opt, copies, value, MAX_VALUE_LEN);
	if(cncopies_len > 0) {
		opt_len += strlen("CNCopies") + (unsigned int)cncopies_len + EQUAL_AND_SEMICOLON_SIZE;
	}

	if(p_ppd != NULL){
		if(p_ppd->colorspace == PPD_CS_RGB)
			p_cs = drv_cs_val[CS_RGB];
	}

	opt_len += strlen(drv_cs_name) + strlen(p_cs) + 2;

	for( i = 0 ; i < ppd_opt_num ; i++ )
	{
		p_choice = ppdFindMarkedChoice(p_ppd, ppd_opt_name[i]);
		if( p_choice != NULL )
		{
			opt_len += strlen(ppd_opt_name[i])
					+ strlen(p_choice->choice)
					+ EQUAL_AND_SEMICOLON_SIZE;
		}
	}
	for( p_opt = p_cups_opt, i = 0 ; i < num_opt ; i++ )
	{
		if( is_cmd_opt_name(p_opt->name) )
		{
			opt_len += strlen(p_opt->name)
					+ strlen(p_opt->value)
					+ EQUAL_AND_SEMICOLON_SIZE;

			for( idx = 0; idx < NUMOF( len_key_info ); idx++ ) {
				if( strcasecmp( p_opt->name, len_key_info[idx].pKeyName ) == 0 ) {
					if( len_key_info[idx].pDataFlag != NULL ) {
						*len_key_info[idx].pDataFlag = 1;
					}
		        		p_tmp = strchr( p_opt->value, OPTIONLIST_VALUE_DELIMITER );
					if( p_tmp != NULL ) {
						*len_key_info[idx].pDataSize = strlen( p_opt->value );
						sprintf( cBuf, "%d",  *len_key_info[idx].pDataSize );
						opt_len += strlen( len_key_info[idx].pKeyNameLen );
						opt_len += strlen( cBuf );
						opt_len += EQUAL_AND_SEMICOLON_SIZE;
					}
					break;
				}
			}
		}
		p_opt++;
	}

	if( (is_docname == 0) && (title != NULL) ){
		opt_len += strlen("CNDocName") + strlen(title) + EQUAL_AND_SEMICOLON_SIZE;
       	p_tmp = strchr( title, OPTIONLIST_VALUE_DELIMITER );
       	if( p_tmp  != NULL ) {
			docname_len = strlen( title );
			sprintf( cBuf, "%d", docname_len );
			opt_len += strlen( "CNDocNameLen" );
			opt_len += strlen( cBuf );
			opt_len += EQUAL_AND_SEMICOLON_SIZE;
		}
	}
	if( (is_usrname == 0) && (user != NULL) ){
		opt_len += strlen("CNUsrName") + strlen(user) + EQUAL_AND_SEMICOLON_SIZE;
       	p_tmp = strchr( user, OPTIONLIST_VALUE_DELIMITER );
       	if( p_tmp != NULL ) {
			usrname_len = strlen( user );
			sprintf( cBuf, "%d", usrname_len );
			opt_len += strlen( "CNUsrNameLen" );
			opt_len += strlen( cBuf );
			opt_len += EQUAL_AND_SEMICOLON_SIZE;
		}
	}

	if(user) {
		opt_len += strlen("CNJobUser") + strlen(user) + EQUAL_AND_SEMICOLON_SIZE;
	}

	if(is_org_page_size_table(p_ppd))
	{
		p_choice = ppdFindMarkedChoice(p_ppd, "PageSize");
		if( p_choice != NULL )
		{
			opt_len += strlen("CNOrgPageSize") + strlen(p_choice->choice) + EQUAL_AND_SEMICOLON_SIZE;
		}
	}

	if((p_device_uri = getenv("DEVICE_URI")) != NULL) {
		opt_len += strlen("DEVICE_URI") + strlen(p_device_uri) + EQUAL_AND_SEMICOLON_SIZE;
	}

	if( strstr( p_table, "CNDR_" ) ) {
		if( ( p_choice = ppdFindMarkedChoice(p_ppd, "PageSize") ) != NULL ) {
			p_size_name = p_choice->choice;
			p_size = ppdPageSize(p_ppd, p_size_name);

			opt_len += strlen("CNPageWidth") + EQUAL_AND_SEMICOLON_SIZE;
			opt_len += strlen("CNPageLength") + EQUAL_AND_SEMICOLON_SIZE;
			opt_len += strlen("CNPageLeft") + EQUAL_AND_SEMICOLON_SIZE;
			opt_len += strlen("CNPageTop") + EQUAL_AND_SEMICOLON_SIZE;
			opt_len += strlen("CNPageRight") + EQUAL_AND_SEMICOLON_SIZE;
			opt_len += strlen("CNPageBottom") + EQUAL_AND_SEMICOLON_SIZE;

			sprintf(cBuf, "%f", p_size->width);
			opt_len += strlen(cBuf);
			sprintf(cBuf, "%f", p_size->length);
			opt_len += strlen(cBuf);
			sprintf(cBuf, "%f", p_size->left);
			opt_len += strlen(cBuf);
			sprintf(cBuf, "%f", p_size->top);
			opt_len += strlen(cBuf);
			sprintf(cBuf, "%f", p_size->right);
			opt_len += strlen(cBuf);
			sprintf(cBuf, "%f", p_size->bottom);
			opt_len += strlen(cBuf);
		}
	}

	if( opt_len > 0 )
	{
		opt_len += 3;

		if( (p_job_attr = malloc(opt_len + 1)) != NULL )
		{
			strcpy(p_job_attr, "ps:");
			strcat(p_job_attr, p_table);

			strcat(p_job_attr, drv_cs_name);
			strcat(p_job_attr, "=");
			strcat(p_job_attr, p_cs);
			strcat(p_job_attr, ";");

			for( i = 0 ; i < ppd_opt_num ; i++ )
			{
				p_choice = ppdFindMarkedChoice(p_ppd, ppd_opt_name[i]);
				if( p_choice != NULL )
				{
					strcat(p_job_attr, ppd_opt_name[i]);
					strcat(p_job_attr, "=");
					strcat(p_job_attr, p_choice->choice);
					strcat(p_job_attr, ";");
				}
			}

			if(cncopies_len > 0){
				strcat(p_job_attr, "CNCopies");
				strcat(p_job_attr, "=");
				strcat(p_job_attr, value);
				strcat(p_job_attr, ";");
			}

			tmp_env = get_comma_chg_env();

			for( p_opt = p_cups_opt, i = 0 ; i < num_opt ; i++ )
			{
      			if( is_comma_chg_opt_name(p_opt->name) == 1 )
      			{
        				if( tmp_env == 1 && (p_tmp = strchr(p_opt->value, '.') ) != NULL)
        				{
          				*p_tmp = ',';
        				}
					else if( tmp_env == 2 && (p_tmp = strchr(p_opt->value, ',') ) != NULL)
        				{
          				*p_tmp = '.';
        				}
      			}
				if( is_cmd_opt_name(p_opt->name) )
				{
					strcat(p_job_attr, p_opt->name);
					strcat(p_job_attr, "=");
					strcat(p_job_attr, p_opt->value);
					strcat(p_job_attr, ";");
				}
				p_opt++;
			}
			if( docname_len > 0 )
			{
				sprintf( cBuf, "%d", docname_len );

				strcat(p_job_attr, "CNDocNameLen");
				strcat(p_job_attr, "=");
				strcat(p_job_attr, cBuf);
				strcat(p_job_attr, ";");
			}
			if( usrname_len > 0 )
			{
				sprintf( cBuf, "%d", usrname_len );

				strcat(p_job_attr, "CNUsrNameLen");
				strcat(p_job_attr, "=");
				strcat(p_job_attr, cBuf);
				strcat(p_job_attr, ";");
			}

			if(is_usrname == 0 && user != NULL)
			{
				strcat(p_job_attr, "CNUsrName");
				strcat(p_job_attr, "=");
				strcat(p_job_attr, user);
				strcat(p_job_attr, ";");
			}
			if(is_docname == 0 && title != NULL)
			{
				strcat(p_job_attr, "CNDocName");
				strcat(p_job_attr, "=");
				strcat(p_job_attr, title);
				strcat(p_job_attr, ";");
			}
			if(user)
			{
				strcat(p_job_attr, "CNJobUser");
				strcat(p_job_attr, "=");
				strcat(p_job_attr, user);
				strcat(p_job_attr, ";");
			}
        		if(is_org_page_size_table(p_ppd))
			{
				p_choice = ppdFindMarkedChoice(p_ppd, "PageSize");
				if( p_choice != NULL )
				{
					strcat(p_job_attr, "CNOrgPageSize");
					strcat(p_job_attr, "=");
					strcat(p_job_attr, p_choice->choice);
					strcat(p_job_attr, ";");
				}
			}
			if(p_device_uri)
			{
				strcat(p_job_attr, "DEVICE_URI");
				strcat(p_job_attr, "=");
				strcat(p_job_attr, p_device_uri);
				strcat(p_job_attr, ";");
			}
			if( strstr( p_table, "CNDR_" ) ) {
				if( ( p_choice = ppdFindMarkedChoice(p_ppd, "PageSize") ) != NULL ) {
					p_size_name = p_choice->choice;
					p_size = ppdPageSize(p_ppd, p_size_name);

					strcat(p_job_attr, "CNPageWidth");
					strcat(p_job_attr, "=");
					sprintf(cBuf, "%f", p_size->width);
					strcat(p_job_attr, cBuf);
					strcat(p_job_attr, ";");

					strcat(p_job_attr, "CNPageLength");
					strcat(p_job_attr, "=");
					sprintf(cBuf, "%f", p_size->length);
					strcat(p_job_attr, cBuf);
					strcat(p_job_attr, ";");

					strcat(p_job_attr, "CNPageLeft");
					strcat(p_job_attr, "=");
					sprintf(cBuf, "%f", p_size->left);
					strcat(p_job_attr, cBuf);
					strcat(p_job_attr, ";");

					strcat(p_job_attr, "CNPageTop");
					strcat(p_job_attr, "=");
					sprintf(cBuf, "%f", p_size->top);
					strcat(p_job_attr, cBuf);
					strcat(p_job_attr, ";");

					strcat(p_job_attr, "CNPageRight");
					strcat(p_job_attr, "=");
					sprintf(cBuf, "%f", p_size->right);
					strcat(p_job_attr, cBuf);
					strcat(p_job_attr, ";");

					strcat(p_job_attr, "CNPageBottom");
					strcat(p_job_attr, "=");
					sprintf(cBuf, "%f", p_size->bottom);
					strcat(p_job_attr, cBuf);
					strcat(p_job_attr, ";");
				}
			}

			p_job_attr[opt_len - 1] = '\0';

			debuglog(INDENT_NONE, "p_job_attr = [%s]\n", p_job_attr);

			if( (strstr( p_job_attr, "CNChangeOptList" ) != NULL) ||
			    (strstr( p_job_attr, "CNOutputAdjustment" ) != NULL) )
			{
				p_list = get_p_job_attr_list( p_job_attr );

				change_p_job_attr_list( &p_list );

				free( p_job_attr );

				curs = param_list_find(p_list, "CNOutputAdjustment");
				if( curs != NULL ) {
					if( strcasecmp(curs->value, "False") == 0 ) {
						param_list_delete(&p_list, "CNOutputDepth");
					}
				}

				opt_len = PS_NAME_SPACE_SIZE;
				for( curs = p_list; curs != NULL; curs = curs->next )
				{
					opt_len += strlen(curs->key) + strlen(curs->value) + EQUAL_AND_SEMICOLON_SIZE;
				}

				p_job_attr = malloc(opt_len + 1U);
				if( p_job_attr != NULL )
				{
					strcpy(p_job_attr, "ps:");

					for (curs = p_list; curs != NULL; curs = curs->next)
					{
						strcat(p_job_attr, curs->key);
						strcat(p_job_attr, "=");
						strcat(p_job_attr, curs->value);
						strcat(p_job_attr, ";");
					}
					p_job_attr[opt_len - 1U] = '\0';
					param_list_free( p_list );

					debuglog(INDENT_NONE, "p_job_attr(change) = [%s]\n", p_job_attr);
				}
			}
		}
	}
	return p_job_attr;
}

static
char* add_drv_list(char *buf, char *value)
{
	char *tmp = NULL;

	if(buf == NULL)	{
		tmp = strdup(value);
	}else{
		int size = strlen(buf) + strlen(value) + 1;
		tmp = malloc(size + 1);
		memset(tmp, 0, size + 1);
		strcpy(tmp, buf);
		strcat(tmp, ",");
		strcat(tmp, value);
		free(buf);
	}
	return tmp;
}

static
int GetKey(char *ptr, char *key, int size)
{
	int	key_len = 0;
	char	*p_key = NULL;

	p_key = key;
	while(1){
		if(*ptr == '*'){
			ptr++;
		}
		if(*ptr == '\n' || *ptr == '\r' || *ptr == '\0'){
			break;
		}
		if(*ptr == ' ' || *ptr == '\t'){
			break;
		}
		if(*ptr == ':'){
			break;
		}
		if(p_key - key >= size - 1){
			break;
		}
		*p_key = *ptr;
		ptr++;
		p_key++;
		key_len++;
	}
	*p_key = '\0';

	return key_len;
}

static
int GetValue(char *p_line_buf, char *p_value_buf, int value_len)
{
	int pos = 0;
	int val_pos = 0;

	while( IS_BLANK(p_line_buf[pos]) )
		pos++;
	while( p_line_buf[pos] == ':' )
		pos++;
	while( IS_BLANK(p_line_buf[pos]) )
		pos++;
	while( p_line_buf[pos] == '"')
		pos++;

	while( !IS_RETURN(p_line_buf[pos])
		&& p_line_buf[pos] != '"'
		&& p_line_buf[pos] != '\n'
		&& val_pos < value_len - 1 )
			p_value_buf[val_pos++] = p_line_buf[pos++];
	p_value_buf[val_pos++] = 0;

	return val_pos;
}

static
int GetUIValue(char *ptr, char *key, char *val, int size)
{
	char *p_key = NULL;
	char *p_val = NULL;
	int opt_flag = 0;

	p_key = key;
	while(1){
		if(*ptr == '\n' || *ptr == '\r' || *ptr == '\0')
			return 1;
		if(*ptr == ' ' || *ptr == '\t')
			ptr++;
		if(*ptr == '*')
			ptr++;
		if(*ptr == '(')
			break;
		if(p_key - key == size - 1)
			return 1;
		*p_key = *ptr;
		ptr++;
		p_key++;
	}
	*p_key = '\0';

	ptr++;
	p_val = val;
	while(1){
		if(*ptr == '\n' || *ptr == '\r' || *ptr == '\0')
			return 1;
		if(*ptr == ')')
			break;
		if(p_val - val == size - 1)
			break;
		*p_val = *ptr;
		ptr++;
		p_val++;
	}
	*p_val = '\0';

	if(ptr != NULL){
		if(strstr(ptr, "True") != NULL)
			opt_flag = 1;
	}

	return opt_flag;
}

static
int get_driver_table(char *ppd_file, char *p_buf, int buf_len)
{
	FILE	*fp = NULL;
	int	got_tbl_num = 0;

	if( ppd_file == NULL || p_buf == NULL || buf_len <= 0 )
	{
		return 0;
	}

	if( (fp = fopen(ppd_file, "r")) )
	{
		char	line_buf[LINE_BUF_SIZE];
		char	key[BUFSIZE];
		char	value[BUFSIZE];

		while( fgets(line_buf, sizeof(line_buf), fp) )
		{
			if( line_buf[0] == '*' )
			{
				int key_len = 0;
				int value_len = 0;
				int p_buf_len = 0;

				memset(key, 0, sizeof(key));
				memset(value, 0, sizeof(value));

				if( strncmp(line_buf, "*%CNUIValue:", 12 ) == 0 )
				{
					(void)GetUIValue(line_buf + 12, key, value, sizeof(key));
					key_len = strlen(key);
				}
				else
				{
					key_len = GetKey(line_buf + 1, key, sizeof(key));
					if(key_len)
					{
						(void)GetValue(line_buf + 1 + key_len, value, sizeof(value));
					}
				}

				value_len = strlen(value);

				if (key_len > 0 && value_len > 0)
				{
					p_buf_len = strlen(p_buf);

					debuglog(INDENT_NONE, "key:value = [%s]:[%s]\n", key, value);

					if( is_drv_tbl_name(key) )
					{
						int new_len = p_buf_len + key_len + value_len + 2;

						if( new_len < buf_len - 1 )
						{
							strcat(p_buf, key);
							strcat(p_buf, "=");
							strcat(p_buf, value);
							strcat(p_buf, ";");
						}
						got_tbl_num++;
					}
				}
			}
		}
		fclose(fp);
	}
	return got_tbl_num;
}

static
int get_driver_list(char *ppd_file, char *p_buf, int buf_len)
{
	FILE *fp = fopen(ppd_file, "r");
	int drv_list_num = sizeof(drv_list_name) / sizeof(char*);
	char *list[DRV_LIST_NUM];
	char line_buf[LINE_BUF_SIZE];
	int i;

	if(strlen(p_buf) > buf_len - 1)
	{
		return 1;
	}

	for(i = 0; i < drv_list_num; i++)
	{
		list[i] = NULL;
	}

	if( fp && p_buf != NULL && buf_len > 0 )
	{
		int cc;
		int pos = 0;

		do
		{
			cc = fgetc(fp);

			if( cc != '\n' && cc != EOF )
			{
				if( pos < LINE_BUF_SIZE - 1 )
					line_buf[pos++] = cc;
			}
			else
			{
				int i;

				line_buf[pos] = 0;

				for( i = 0 ; i < drv_list_num ; i++ )
				{
					int name_len = strlen(drv_list_name[i]);
					char value[BUFSIZE];

					if( line_buf[0] == '*'
					 && strncmp(line_buf + 1, drv_list_name[i], name_len) == 0 )
					{
						if( GetValue(line_buf+1+name_len, value, sizeof(value)) > 0 )
						{
							list[i] = add_drv_list(list[i], value);
						}
					}
}

				pos = 0;
			}
		}
		while( cc != EOF );

		fclose(fp);

		for( i = 0 ; i < drv_list_num ; i++ )
		{
			if(list[i] != NULL)
			{
				int name_len = strlen(drv_list_name[i]);
				int new_len = strlen(p_buf) + name_len + strlen(list[i]) + 2;
				if( new_len < buf_len - 1)
				{
					strcat(p_buf, drv_list_name[i]);
					strcat(p_buf, "=");
					strcat(p_buf, list[i]);
					strcat(p_buf, ";");
				}
				free(list[i]);
			}
		}
	}
	return 0;
}

static
void append_lib_ext(char *p_driver, int buf_len)
{
	char *p_ext;
	int len = buf_len - strlen(p_driver) - 1;

	if( (p_ext = strrchr(p_driver, '.')) )
	{
		if( !strncmp(p_ext, ".so", 3) )
			*p_ext = '\0';
	}
#ifdef	APPLE_OS
	strncat(p_driver, ".dylib", len);
#else
	strncat(p_driver, ".so", len);
#endif
}

static
int get_driver_model(char *ppd_file,
	char *p_model, char *p_driver, char *p_device, int buf_len)
{
	FILE *fp = fopen(ppd_file, "r");
	int mdl_tbl_num = sizeof(mdl_tbl_name) / sizeof(char*);
	char line_buf[LINE_BUF_SIZE];
	int got_tbl_num = 0;

	if( fp && p_model != NULL && p_driver != NULL
	 	   && p_device != NULL && buf_len > 0 )
	{
		int cc;
		int pos = 0;

		*p_model  = '\0';
		*p_driver = '\0';
		*p_device = '\0';

		do
		{
			cc = fgetc(fp);

			if( cc != '\n' && cc != EOF )
			{
				if( pos < LINE_BUF_SIZE - 1 )
					line_buf[pos++] = cc;
			}
			else
			{
				int i;

				line_buf[pos] = 0;

				for( i = 0 ; i < mdl_tbl_num ; i++ )
				{
					int name_len = strlen(mdl_tbl_name[i]);
					char value[BUFSIZE];

					if( line_buf[0] == '*'
					 && strncmp(line_buf + 1, mdl_tbl_name[i], name_len) == 0 )
					{
						if( GetValue(line_buf + 1 + name_len, value, sizeof(value)) > 0 )
						{
							if( strlen(value) < buf_len - 1 )
							{
								switch( i )
								{
								case OPVP_MODEL:
									memset(p_model, 0, buf_len);
									strncpy(p_model, value, buf_len - 1);
									break;
								case OPVP_DRIVER:
									memset(p_driver, 0, buf_len);
									strncpy(p_driver, value, buf_len - 1);
									append_lib_ext(p_driver, buf_len);
									break;
								case OPVP_DEVICE:
									memset(p_device, 0, buf_len);
									strncpy(p_device, value, buf_len - 1);
									break;
								}
							}
							got_tbl_num++;
						}
					}
				}
				pos = 0;
			}
		}
		while( cc != EOF && got_tbl_num < mdl_tbl_num );

		fclose(fp);
	}
	return (got_tbl_num == mdl_tbl_num)? 1 : 0;
}

static
char *make_job_attr(ppd_file_t *p_ppd, char *p_ppd_name,
	 cups_option_t *p_cups_opt, int num_opt, int copies, char *user, char *title)
{
	char *p_job_attr = NULL;
	char table_buf[TABLE_BUF_SIZE];

	table_buf[0] = '\0';
	if( get_driver_table(p_ppd_name, table_buf, sizeof(table_buf)) )
	{
		get_driver_list(p_ppd_name, table_buf, sizeof(table_buf));

		debuglog(INDENT_NONE, "table_buf = [%s]\n", table_buf);

		p_job_attr = ppd_mark_to_job_attr(p_ppd, p_cups_opt,
				num_opt, table_buf, copies, user, title);
	}
	return p_job_attr;
}

static
int is_acro_util(char *p_data_buf, int read_bytes)
{
	if( strncmp(p_data_buf, ACRO_UTIL, ACRO_UTIL_LEN) == 0 )
		return 1;
	else
		return 0;
}

static
int is_end_resource(char *p_data_buf, int read_bytes)
{
	if( strncmp(p_data_buf, "%%EndResource", 13) == 0 )
		return 1;
	else
		return 0;
}

static
int read_line(int fd, char *p_buf, int bytes)
{
	static char read_buf[DATA_BUF_SIZE];
	static int buf_bytes = 0;
	static int buf_pos = 0;
	int read_bytes = 0;

	while( bytes > 0 )
	{
		if( buf_bytes == 0 && fd != -1 )
		{
			buf_bytes = read(fd, read_buf, DATA_BUF_SIZE);

			if( buf_bytes > 0 )
			{
#ifdef	DEBUG_IN_PS
				if( g_in_log_fd > 0 )
					write(g_in_log_fd, read_buf, buf_bytes);
#endif
				buf_pos = 0;
			}
		}

		if( buf_bytes > 0 )
		{
			*p_buf = read_buf[buf_pos++];
			bytes--;
			buf_bytes--;
			read_bytes++;

			if( IS_RETURN(*p_buf) )
				break;

			p_buf++;
		}
		else if( buf_bytes < 0 )
		{
			if( errno == EINTR )
				continue;
		}
		else
			break;
	}

	return read_bytes;
}

static
int get_copies(char *p_buf)
{
	char *p_code = p_buf;
	char value[MAX_VALUE_LEN + 1];
	int value_len = 0;
	int copies = -1;
	memset(value, 0, MAX_VALUE_LEN + 1);

	while(IS_BLANK(*p_code))
		p_code++;

	while(isdigit(*p_code))
	{
		value[value_len++] = *p_code++;
		if(value_len == MAX_VALUE_LEN)
			break;
	}

	if(value_len > 0)
		copies = atoi(value);

	return copies;
}

static
ParamList *get_ps_params(int ifd, BufList **ps_data, int *copies)
{
	char read_buf[DATA_BUF_SIZE];
	ParamList *p_list = NULL;
	int begin_page = 0;
	int read_bytes;
	int acro_util = 0;
	BufList *bl = NULL;
	BufList *prev_bl = NULL;
	int numcopies = -1;
	int is_banner = 0;
	char *p_read_buf;

	while( (read_bytes = read_line(ifd, read_buf, DATA_BUF_SIZE - 1)) > 0 )
	{
		{
			if( is_acro_util(read_buf, read_bytes) ){
				acro_util = 1;
			}
			else if( acro_util && is_end_resource(read_buf, read_bytes) ){
				acro_util = 0;
			}

			if( acro_util ){
				int line_bytes=0;
				while( line_bytes+29 < read_bytes ){
					if(!strncmp(&read_buf[line_bytes],
								" ct_BadResourceImplementation?", 30)){
						strcpy(&read_buf[line_bytes], " false");
						line_bytes+=6;
						strcpy(&read_buf[line_bytes],
							   &read_buf[line_bytes+24]);
						read_bytes-=24;
					}
					else{
						line_bytes++;
					}
				}
			}
		}

		bl = buflist_new((unsigned char *)read_buf, read_bytes);

		if( *ps_data == NULL )
			*ps_data = bl;
		else
			buflist_add_tail(prev_bl, bl);

		prev_bl = bl;

		if( read_bytes > 0 )
		{
			if( read_buf[read_bytes - 1] == '\n' )
				read_buf[read_bytes - 1] = '\0';
			else
				read_buf[read_bytes] = '\0';
		}
		else
		{
			read_buf[0] = '\0';
		}

		if( strncmp(read_buf, "%%BeginFeature:", 15) == 0 )
		{
			char key_buf[MAX_KEY_LEN + 1];
			char value_buf[MAX_VALUE_LEN + 1];
			int key_len = 0;
			int value_len = 0;
			char *p_code;

			p_code = read_buf + 15;

			while( *p_code != '\0' )
				if( *p_code++ == '*' )
					break;

			while( *p_code != '\0' )
			{
				if( IS_BLANK(*p_code)
				 || key_len >= MAX_KEY_LEN )
					break;
				key_buf[key_len++] = *p_code++;
			}
			while( *p_code != '\0' )
			{
				if( !IS_BLANK(*p_code)  )
					break;
				p_code++;
			}
			while( *p_code != '\0' )
			{
				if( IS_BLANK(*p_code)
				 || value_len >= MAX_VALUE_LEN )
					break;
				value_buf[value_len++] = *p_code++;
			}
			if( key_len > 0 && value_len > 0 )
			{
				key_buf[key_len] = '\0';
				value_buf[value_len] = '\0';

				param_list_add(&p_list, key_buf, value_buf, value_len + 1);
			}
		}
		else if( !begin_page && strncmp(read_buf, "%%Page:", 7) == 0 )
		{
			begin_page = 1;
		}
		else if((p_read_buf = strstr(read_buf, "NumCopies")) != NULL)
		{
			int tmp = get_copies(p_read_buf + 9);
			if(tmp > 0)
				numcopies = tmp;
		}
		else if(strncmp(read_buf, "/#copies", 8) == 0)
		{
			int tmp = get_copies(read_buf + 8);
			if(tmp > 0)
				numcopies = tmp;
		}
		else if(strncmp(read_buf, BANNER_DESC, BANNER_DESC_LEN) == 0)
		{
			is_banner = 1;
		}
		else if( begin_page )
		{
			if( strncmp(read_buf, "%%EndPageSetup", 14) == 0 )
				break;
			else if( strncmp(read_buf, "gsave", 5) == 0 )
				break;
			else if( read_buf[0] >= '0' && read_buf[0] <= '9' )
				break;
		}
	}

	while( (read_bytes = read_line(-1, read_buf, DATA_BUF_SIZE - 1)) > 0 )
	{
		BufList *bl = buflist_new((unsigned char *)read_buf, read_bytes);

		if( *ps_data == NULL )
			*ps_data = bl;
		else
			buflist_add_tail(prev_bl, bl);

		prev_bl = bl;

		if( read_buf[read_bytes - 1] == '\n' )
			read_buf[read_bytes - 1] = '\0';
		else
			read_buf[read_bytes] = '\0';
	}
	if(numcopies > 0)
	{
		*copies = numcopies;
	}
	if(is_banner)
	{
		*copies = -2;
	}

	return p_list;
}

static
void mark_ps_param_options(ppd_file_t *p_ppd, ParamList *p_param)
{
	int num = param_list_num(p_param);

	if( num > 0 )
	{
		cups_option_t *options
			= (cups_option_t*)malloc(num * sizeof(cups_option_t));
		if( options )
		{
			int i;
			for( i = 0 ; i < num ; i++ )
			{
				options[i].name = p_param->key;
				options[i].value = p_param->value;
				p_param = p_param->next;
			}
			for( i = num - 1 ; i >= 0 ; i-- )
			{
				cupsMarkOptions(p_ppd, 1, &options[i]);
			}

			free(options);
		}
	}
}

static
char *strdup2(char *str1, char *str2)
{
	int len;
	char *p_str;

	if( str1 == NULL || str2 == NULL )
		return NULL;

	len = strlen(str1) + strlen(str2) + 1;
	if( (p_str = malloc(len)) != NULL )
	{
		strcpy(p_str, str1);
		strcat(p_str, str2);
	}
	return p_str;
}

static
void init_cmd_arg(char *cmd_arg[], char *org_arg[])
{
	int i;

	for( i = 0 ; i < ARGV_NUM ; i++ )
	{
		if( org_arg[i] != NULL )
			cmd_arg[i] = strdup(org_arg[i]);
		else
			cmd_arg[i] = NULL;
	}
}

static
void free_cmd_arg(char *cmd_arg[])
{
	int i;

	for( i = 0 ; i < ARGV_NUM ; i++ )
	{
		if( cmd_arg[i] != NULL )
			free(cmd_arg[i]);
	}
}

static
void set_cmd_arg(char *cmd_arg[], int index, char *str)
{
	if( cmd_arg[index] != NULL )
		free(cmd_arg[index]);

	cmd_arg[index] = str;
}

static
int make_cmd_arg(cups_option_t *p_cups_opt, int num_opt,
	ParamList *p_param, char *cmd_arg[], int copies, char *user, char *title)
{
#ifdef DEBUG_PPD
	char *p_ppd_name = "debug.ppd";
#else
	char *p_ppd_name = getenv("PPD");
#endif
	ppd_file_t *p_ppd;
	char device[PARAM_BUF_SIZE];
	char driver[PARAM_BUF_SIZE];
	char model[PARAM_BUF_SIZE];
	char reso_buf[PARAM_BUF_SIZE];
	char width_buf[PARAM_BUF_SIZE];
	char height_buf[PARAM_BUF_SIZE];
	ppd_choice_t *p_choice;
	ppd_size_t *p_size;
	char *p_size_name;
	int reso;
	char *p_job_attr = NULL;

	if( (p_ppd = ppdOpenFile(p_ppd_name)) == NULL )
		return -1;

	ppdMarkDefaults(p_ppd);
	cupsMarkOptions(p_ppd, num_opt, p_cups_opt);
	mark_ps_param_options(p_ppd, p_param);

	p_choice = ppdFindMarkedChoice(p_ppd, "PageSize");
	p_size_name = p_choice->choice;
	p_size = ppdPageSize(p_ppd, p_size_name);

	p_choice = ppdFindMarkedChoice(p_ppd, "Resolution");
	reso = atoi(p_choice->choice);

	snprintf(reso_buf, PARAM_BUF_SIZE-1, "%s%d",
		cmd_arg[ARGV_RESO], reso);
	snprintf(width_buf, PARAM_BUF_SIZE-1, "%s%d",
		cmd_arg[ARGV_WIDTH], (int)(p_size->width * (float)reso / 72.0));
	snprintf(height_buf, PARAM_BUF_SIZE-1, "%s%d",
		cmd_arg[ARGV_HEIGHT], (int)(p_size->length * (float)reso / 72.0));

	p_job_attr = make_job_attr(
		p_ppd, p_ppd_name, p_cups_opt, num_opt, copies, user, title);
	ppdClose(p_ppd);

	if( get_driver_model(
			p_ppd_name, model, driver, device, PARAM_BUF_SIZE) == 0 )
	{
		if( p_job_attr != NULL )
			free(p_job_attr);
		return -1;
	}

	if( p_job_attr != NULL )
	{
		{
			char *p_gs_path = strdup2(EXEC_PATH, "/");
			set_cmd_arg(cmd_arg, ARGV_GS,
						strdup2(p_gs_path, EXEC_GS));
			free(p_gs_path);
		}
		set_cmd_arg(cmd_arg, ARGV_RESO, strdup(reso_buf));
		set_cmd_arg(cmd_arg, ARGV_WIDTH, strdup(width_buf));
		set_cmd_arg(cmd_arg, ARGV_HEIGHT, strdup(height_buf));
		set_cmd_arg(cmd_arg, ARGV_DEVICE,
						strdup2(cmd_arg[ARGV_DEVICE], device));
		set_cmd_arg(cmd_arg, ARGV_DRIVER,
						strdup2(cmd_arg[ARGV_DRIVER], driver));
		set_cmd_arg(cmd_arg, ARGV_MODEL,
						strdup2(cmd_arg[ARGV_MODEL], model));
		set_cmd_arg(cmd_arg, ARGV_JOB,
						strdup2(cmd_arg[ARGV_JOB], p_job_attr));

		free(p_job_attr);

		return 0;
	}
	else
		return -1;
}

int exec_filter(char *cmd_arg[], int ofd, int fds[2])
{
	int	child_pid = -1;

	if( pipe(fds) >= 0 )
	{
		child_pid = fork();

		if( child_pid == 0 )
		{
			close(0);
			dup2(fds[0], 0);
			close(fds[0]);
			close(fds[1]);

			if( ofd != 1 )
			{
				close(1);
				dup2(ofd, 1);
				close(ofd);
			}

			execv(cmd_arg[0], cmd_arg);

			fprintf(stderr, "execl() error\n");
		}
		else if( child_pid != -1 )
		{
			close(fds[0]);
		}
	}
	return child_pid;
}

static
void
sigterm_handler(int sigcode)
{
	if( g_filter_pid != -1 )
		kill(g_filter_pid, SIGTERM);

	g_signal_received = 1;
}

int	main(int argc, char *argv[])
{
	cups_option_t *p_cups_opt = NULL;
	int num_opt = 0;
	ParamList *p_ps_param = NULL;
	BufList *p_ps_data = NULL;
	char *p_data_buf;
	int ifd = 0;
	int fds[2];
	struct sigaction sigact;
	char *cmd_arg[ARGV_NUM];
	int copies = -1;
#ifdef	UNIT_TEST
	freopen( "/tmp/unit_test/pstoufr2cpca_leak_check.log", "a+", stderr);
#endif

#ifdef	DEBUG_SLEEP
	sleep(30);
#endif

	setbuf(stderr, NULL);
	fprintf(stderr, "DEBUG: pstoufr2cpca start.\n");

	memset(&sigact, 0, sizeof(sigact));
	sigact.sa_handler = sigterm_handler;
	if( sigaction(SIGTERM, &sigact, NULL) )
	{
		fputs("ERROR: pstoufr2cpca can't register signal hander.\n", stderr);
		return 1;
	}

	if( argc < 6 || argc > 7 )
	{
		fputs("ERROR: pstoufr2cpca illegal parameter number.\n", stderr);
		return 1;
	}

	if( argv[5] != NULL )
	{
		num_opt = cupsParseOptions(argv[5], 0, &p_cups_opt);
		if( num_opt < 0 )
		{
			fputs("ERROR: illegal option.\n", stderr);
			return 1;
		}
	}

	if( argc == 7 )
	{
		if( (ifd = open(argv[6], O_RDONLY)) == -1 )
		{
			fputs("ERROR: can't open file.\n", stderr);
			return 1;
		}
	}

	init_cmd_arg(cmd_arg, org_cmd_arg);

#ifdef	DEBUG_IN_PS
	g_in_log_fd = open("/tmp/debug_in_log.ps", O_WRONLY);
#endif
	p_ps_param = get_ps_params(ifd, &p_ps_data, &copies);

	if( make_cmd_arg(p_cups_opt, num_opt,
			 p_ps_param, cmd_arg, copies, argv[2], argv[3]) < 0 )
	{
		fputs("ERROR: can't make parameter.\n", stderr);
		goto error_return;
	}

	if( g_signal_received )
		goto error_return;

	if( (p_data_buf = (char*)malloc(DATA_BUF_SIZE)) != NULL )
	{
#ifdef	DEBUG_PS
		int log_fd = open("/tmp/debug_data_log.ps", O_WRONLY);
#endif
		g_filter_pid = exec_filter(cmd_arg, 1, fds);

		buflist_write(p_ps_data, fds[1]);

#ifdef	DEBUG_PS
		buflist_write(p_ps_data, log_fd);
#endif

		while( !g_signal_received )
		{
			int read_bytes = read(ifd, p_data_buf, DATA_BUF_SIZE);

			if( read_bytes > 0 )
			{
				int write_bytes;
				char *p_data = p_data_buf;
#ifdef	DEBUG_PS
				write(log_fd, p_data_buf, read_bytes);
#endif
#ifdef	DEBUG_IN_PS
				if( g_in_log_fd > 0 )
					write(g_in_log_fd, p_data_buf, read_bytes);
#endif
				do
				{
					write_bytes = write(fds[1], p_data, read_bytes);

					if( write_bytes < 0 )
					{
						if( errno == EINTR )
							continue;
						fprintf(stderr,
							"ERROR: pstoufr2cpca write error,%d.\n", errno);
						goto error_exit;
					}
					read_bytes -= write_bytes;
					p_data += write_bytes;
				}
				while( !g_signal_received && read_bytes > 0 );
			}
			else if( read_bytes < 0 )
			{
				if( errno == EINTR )
					continue;
				fprintf(stderr, "ERROR: pstoufr2cpca read error,%d.\n", errno);
				goto error_exit;
			}
			else
				break;
		}

		free(p_data_buf);

#ifdef	DEBUG_PS
		if( log_fd != -1 ) close(log_fd);
#endif
	}

#ifdef	DEBUG_IN_PS
	if( g_in_log_fd > 0 )
		close(g_in_log_fd);
#endif

	free_cmd_arg(cmd_arg);

	if( p_ps_param != NULL )
		param_list_free(p_ps_param);
	if( p_ps_data != NULL )
		buflist_destroy(p_ps_data);
	if( ifd != 0 )
		close(ifd);

	close(fds[1]);

	if( g_filter_pid != -1 )
		waitpid(g_filter_pid, NULL, 0);

	return 0;

error_exit:
	if( p_data_buf != NULL )
		free(p_data_buf);

error_return:
	free_cmd_arg(cmd_arg);

	if( p_ps_param != NULL )
		param_list_free(p_ps_param);
	if( p_ps_data != NULL )
		buflist_destroy(p_ps_data);
	if( ifd != 0 )
		close(ifd);

	return 1;
}

