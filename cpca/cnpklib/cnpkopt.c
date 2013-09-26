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
#include <string.h>
#include <ctype.h>

#ifdef _UFR2_
	#include "cnpklibufr2.h"
#elif _LIPS_
	#include "cnpkliblips.h"
#elif _PS_
	#include "cnpklibps.h"
#else
	#include "cnpklib.h"
#endif

#include "cnpkopt.h"



typedef struct glue_page_name_tbl_t{
	char *glue_name;
	char *opt_name;
} GluePageNameTbl;

GluePageNameTbl  gluecode_page_name_table[] =
{

	{ "iso_a3_297x420mm",         "A3"          },
	{ "iso_a4_210x297mm",         "A4"          },
	{ "jis_b4_257x364mm",         "B4"          },
	{ "jis_b5_182x257mm",         "B5"          },
	{ "na_letter_8.5x11in",       "Letter"      },
	{ "na_legal_8.5x14in",        "Legal"       },
	{ "jpn_hagaki_100x148mm",     "Postcard"    },
	{ "iso_a5_148x210mm",         "A5"          },
	{ "iso_dl_110x220mm",         "dl_envelope" },
	{ "iso_b5_176x250mm",         "envelope_b5" },
	{ "iso_c5_162x229mm",         "envelope_c5" },
	{ "jpn_oufuku_148x200mm",     "dbl_postcard"},
	{ "na_number-10_4.125x9.5in", "Com10"       },
	{ "na_executive_7.25x10.5in", "Executive"   },
	{ "na_monarch_3.875x7.5in",   "Monarch"     },
	{ "na_invoice_5.5x8.5in",     "Statement"   },
	{ "na_ledger_11x17in",        "Ledger"      },
	{ "jpn_you4_105x235mm",       "jenv_you4"   },
	{ "jpn_kaku2_240x332mm",      "jenv_kaku2"  },
	{ "jpn_chou3_120x235mm",      "jenv_chou3"  },
	{ "na_arch-b_12x18in",        "12x18"       },
	{ NULL, NULL                                },
};

typedef struct glude_page_size_tbl_t{
		char* opt_name;
		int width;
		int height;
}GluePageSizeTbl;

GluePageSizeTbl  gluecode_page_size_table[] =
{
	{ "SRA3",        450, 320 },
	{ "4x_postcard", 200, 296 },
	{ "13x19",       482, 330 },
	{ "jenv_you2", 	 162, 114 },
	{ NULL,            0,   0 },
};

static int inch_to_mm(float value_in)
{
	int value_mm;

	value_mm = (int)(value_in*25.4+0.5);

	return value_mm;
}

static char* convert_page_size_name( char *pValue )
{

	int i;
	char *pOptName = NULL;
	char *pDup = NULL;

	for( i=0; gluecode_page_name_table[i].glue_name!=NULL; i++ ){
		if( !strcmp(gluecode_page_name_table[i].glue_name, pValue) ){
			pOptName = &gluecode_page_name_table[i].opt_name[0];
			return pOptName;
		}
	}

	if( !strncmp(pValue, "custom", 6) ){
		char *pTemp;
		char *pWidth, *pHeight;
		int width_mm, height_mm;
		short flgInch;

		pDup = strdup(pValue);

		pTemp = strrchr(pDup, '_');
		if( pTemp ){
			pTemp++;
			pWidth = pTemp;
			while( *pTemp != '\0' && *pTemp != 'x')
				pTemp++;

			if( *pTemp	 == 'x' ){
				*pTemp = '\0';pTemp++;

				pHeight = pTemp;
				while( isdigit(*pTemp) )
					pTemp++;

				if( !strcmp(pTemp, "in") ){
					flgInch = 1;
				}else if( !strcmp(pTemp, "mm") ){
					flgInch = 0;
				}else{
					goto opt_error;
				}
				*pTemp = '\0';

				if( flgInch ){
					int width_in, height_in;
					width_in = atoi(pWidth);
					height_in = atoi(pHeight);
					width_mm = inch_to_mm(width_in);
					height_mm = inch_to_mm(height_in);
				}else{
					width_mm = atoi(pWidth);
					height_mm = atoi(pHeight);
				}

				for( i=0; gluecode_page_size_table[i].opt_name!=NULL; i++ ){
					if( width_mm == gluecode_page_size_table[i].width &&
						height_mm == gluecode_page_size_table[i].height ){
						pOptName = &gluecode_page_size_table[i].opt_name[0];
						break;
					}
				}
				return pOptName;
			}
		}
	}

 opt_error:

	if( pDup )
		free(pDup);

	return NULL;
}



char* option_list_get_value( OptionList* pOptList, const char *key, short* flg)
{

	OptionList *plist = pOptList;
	char *pValue;

	for( ; plist != NULL; plist = plist->next ){
		if(	strcasecmp( plist->key, key ) == 0 ){
			pValue = strdup(plist->value);
			if( flg != NULL )
				*flg = plist->change_flg;
			return pValue;

		}
	}
	return NULL;
}


void option_list_change_value( OptionList* pOptList, const char *key,
							   const char *value, short flg )
{

	OptionList *plist = pOptList;

	for( ; plist != NULL; plist = plist->next ){
		if(	strcasecmp( plist->key, key ) == 0 ){
			free(plist->value);
			plist->value = strdup(value);
			plist->change_flg = 1;
			plist->updf_flg = flg;
		}
	}

}


void option_list_add( OptionList** ppOptList,
					  const char* key, const char* value, short flg )
{

	if( option_list_get_value(*ppOptList,key, NULL) == NULL ){
		OptionList *plist = (OptionList*)malloc(sizeof(OptionList));

		plist->key = strdup(key);
		plist->value = strdup(value);
		plist->change_flg = 0;
		plist->updf_flg = flg;

		plist->next = *ppOptList;
		*ppOptList = plist;
	}
	else{
		option_list_change_value(*ppOptList, key, value, flg);
	}


}

void option_list_free( OptionList* pOptList )
{

	OptionList *plist = pOptList;
	OptionList *temp = NULL;

	for( ; plist != NULL; plist=temp ){
		free(plist->key);
		free(plist->value);

	    temp = plist->next;
		free(plist);
	}
}



void option_list_clear_flg( OptionList* pOptList )
{
	OptionList *plist = pOptList;

	for( ; plist != NULL; plist = plist->next )
		plist->change_flg = 0;
}


int parse_option( OptionList **pOptList, char *pInfo )
{
	char *pName;
	char *pValue;
	char *pTemp = NULL;
	char *ptr = NULL;
	int i;
	short is_updf = 0;

	if( pInfo == NULL )
		return -1;
	pTemp = strdup(pInfo);
	ptr = pTemp;
	i = 0;

	while( *ptr != '\0'){

		while( isspace(*ptr) )
			ptr++;


		if( !strncmp(ptr, "ps:", 3) ){
			is_updf = 0;
			ptr += 3;
		}
		else if( !strncmp(ptr, "updf:", 5) ){
			is_updf = 1;
			ptr += 5;
		}
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
			*ptr = '\0'; ptr++;
		}
		if( is_updf ){
			if( !strcmp(pName, "MediaSize") ){
				char *pPageName = NULL;
				pPageName = convert_page_size_name(pValue);
				if( pPageName ){
					pName = "PageSize";
					pValue = pPageName;
				}
				else
					pValue = '\0';
			}
			else if( !strcmp(pName, "MediaPageRotation") ){
				pName = "Orientation";
			}

		}

		if( pValue != '\0' )
			option_list_add( pOptList, pName, pValue, is_updf );
		i++;
	}

	free(pTemp);
	return i;
}




