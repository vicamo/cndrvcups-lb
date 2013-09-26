#ifndef _papertbl_
#define _papertbl_


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

#endif
