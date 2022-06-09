#pragma once

#include <stdbool.h>

#define ZONE_LONG_NAME_STD_INDEX 0
#define ZONE_SHORT_NAME_STD_INDEX 1

#define ZONE_LONG_NAME_DTS_INDEX 2
#define ZONE_SHORT_NAME_DTS_INDEX 3

static const char *const UTC_ZONE_NAMES[] = {"Coordinated Universal Time", "UTC",
                                             "Coordinated Universal Time", "UTC"};

static const char *const GMT_ZONE_NAMES[] = {"Greenwich Mean Time", "GMT",
                                             "Greenwich Mean Time", "GMT"};

#ifdef ENABLE_TIME_ZONE_SUPPORT
static const char *const PST_ZONE_NAMES[] = {"Pacific Standard Time", "PST",
                                             "Pacific Daylight Time", "PDT"};

static const char *const MST_ZONE_NAMES[] = {"Mountain Standard Time", "MST",
                                             "Mountain Daylight Time", "MDT"};

static const char *const CST_ZONE_NAMES[] = {"Central Standard Time", "CST",
                                             "Central Daylight Time", "CDT"};

static const char *const EST_ZONE_NAMES[] = {"Eastern Standard Time", "EST",
                                             "Eastern Daylight Time", "EDT"};

static const char *const HST_ZONE_NAMES[] = {"Hawaii Standard Time", "HST",
                                             "Hawaii Daylight Time", "HDT"};

static const char *const AKST_ZONE_NAMES[] = {"Alaska Standard Time", "AKST",
                                              "Alaska Daylight Time", "AKDT"};

static const char *const AST_ZONE_NAMES[] = {"Atlantic Standard Time", "AST",
                                             "Atlantic Daylight Time", "ADT"};

static const char *const NST_ZONE_NAMES[] = {"Newfoundland Standard Time", "NST",
                                             "Newfoundland Daylight Time", "NDT"};

static const char *const CET_ZONE_NAMES[] = {"Central European Time", "CET",
                                             "Central European Summer Time", "CEST"};

static const char *const WET_ZONE_NAMES[] = {"Western European Time", "WET",
                                             "Western European Summer Time", "WEST"};

static const char *const IST_ZONE_NAMES[] = {"Israel Standard Time", "IST",
                                             "Israel Daylight Time", "IDT"};

static const char *const JST_ZONE_NAMES[] = {"Japan Standard Time", "JST",
                                             "Japan Daylight Time", "JDT"};

static const char *const EET_ZONE_NAMES[] = {"Eastern European Time", "EET",
                                             "Eastern European Summer Time", "EEST"};

static const char *const EAT_ZONE_NAMES[] = {"Eastern African Time", "EAT",
                                             "Eastern African Summer Time", "EAST"};

static const char *const WAT_ZONE_NAMES[] = {"Western African Time", "WAT",
                                             "Western African Summer Time", "WAST"};

static const char *const CAT_ZONE_NAMES[] = {"Central African Time", "CAT",
                                             "Central African Summer Time", "CAST"};

static const char *const SAST_ZONE_NAMES[] = {"South Africa Standard Time", "SAST",
                                              "South Africa Summer Time", "SAST"};

static const char *const BRT_ZONE_NAMES[] = {"Brasilia Time", "BRT",
                                             "Brasilia Summer Time", "BRST"};

static const char *const ART_ZONE_NAMES[] = {"Argentine Time", "ART",
                                             "Argentine Summer Time", "ARST"};

static const char *const PYT_ZONE_NAMES[] = {"Paraguay Time", "PYT",
                                             "Paraguay Summer Time", "PYST"};

static const char *const AMT_ZONE_NAMES[] = {"Amazon Time", "AMT",
                                             "Amazon Summer Time", "AMST"};

static const char *const COT_ZONE_NAMES[] = {"Colombia Time", "COT",
                                             "Colombia Summer Time", "COST"};

static const char *const VET_ZONE_NAMES[] = {"Venezuela Time", "VET",
                                             "Venezuela Summer Time", "VEST"};

static const char *const GFT_ZONE_NAMES[] = {"French Guiana Time", "GFT",
                                             "French Guiana Summer Time", "GFST"};

static const char *const ACT_ZONE_NAMES[] = {"Acre Time", "ACT",
                                             "Acre Summer Time", "ACST"};

static const char *const WGT_ZONE_NAMES[] = {"Western Greenland Time", "WGT",
                                             "Western Greenland Summer Time", "WGST"};

static const char *const ECT_ZONE_NAMES[] = {"Ecuador Time", "ECT",
                                             "Ecuador Summer Time", "ECST"};

static const char *const GYT_ZONE_NAMES[] = {"Guyana Time", "GYT",
                                             "Guyana Summer Time", "GYST"};

static const char *const BOT_ZONE_NAMES[] = {"Bolivia Time", "BOT",
                                             "Bolivia Summer Time", "BOST"};

static const char *const PET_ZONE_NAMES[] = {"Peru Time", "PET",
                                             "Peru Summer Time", "PEST"};

static const char *const PMST_ZONE_NAMES[] = {"Pierre & Miquelon Standard Time", "PMST",
                                              "Pierre & Miquelon Daylight Time", "PMDT"};

static const char *const UYT_ZONE_NAMES[] = {"Uruguay Time", "UYT",
                                             "Uruguay Summer Time", "UYST"};

static const char *const FNT_ZONE_NAMES[] = {"Fernando de Noronha Time", "FNT",
                                             "Fernando de Noronha Summer Time", "FNST"};

static const char *const SRT_ZONE_NAMES[] = {"Suriname Time", "SRT",
                                             "Suriname Summer Time", "SRST"};

static const char *const GMT_03_00_ZONE_NAMES[] = {"Punta Arenas Standard Time", "GMT-03:00",
                                                   "Punta Arenas Standard Time", "GMT-03:00"};

static const char *const CLT_ZONE_NAMES[] = {"Chile Time", "CLT",
                                             "Chile Summer Time", "CLST"};

static const char *const EGT_ZONE_NAMES[] = {"Eastern Greenland Time", "EGT",
                                             "Eastern Greenland Summer Time", "EGST"};

static const char *const AWST_ZONE_NAMES[] = {"Australian Western Standard Time", "AWST",
                                              "Australian Western Daylight Time", "AWDT"};

static const char *const DAVT_ZONE_NAMES[] = {"Davis Time", "DAVT",
                                              "Davis Summer Time", "DAVST"};

static const char *const DDUT_ZONE_NAMES[] = {"Dumont-d'Urville Time", "DDUT",
                                              "Dumont-d'Urville Summer Time", "DDUST"};

static const char *const MIST_ZONE_NAMES[] = {"Macquarie Island Standard Time", "MIST",
                                              "Macquarie Island Daylight Time", "MIDT"};

static const char *const MAWT_ZONE_NAMES[] = {"Mawson Time", "MAWT",
                                              "Mawson Summer Time", "MAWST"};

static const char *const NZST_ZONE_NAMES[] = {"New Zealand Standard Time", "NZST",
                                              "New Zealand Daylight Time", "NZDT"};

static const char *const ROTT_ZONE_NAMES[] = {"Rothera Time", "ROTT",
                                              "Rothera Summer Time", "ROTST"};

static const char *const SYOT_ZONE_NAMES[] = {"Syowa Time", "SYOT",
                                              "Syowa Summer Time", "SYOST"};

static const char *const VOST_ZONE_NAMES[] = {"Vostok Time", "VOST",
                                              "Vostok Summer Time", "VOSST"};

static const char *const ALMT_ZONE_NAMES[] = {"Alma-Ata Time", "ALMT",
                                              "Alma-Ata Summer Time", "ALMST"};

static const char *const ANAT_ZONE_NAMES[] = {"Anadyr Time", "ANAT",
                                              "Anadyr Summer Time", "ANAST"};

static const char *const AQTT_ZONE_NAMES[] = {"Aqtau Time", "AQTT",
                                              "Aqtau Summer Time", "AQTST"};

static const char *const TMT_ZONE_NAMES[] = {"Turkmenistan Time", "TMT",
                                             "Turkmenistan Summer Time", "TMST"};

static const char *const GMT_05_00_ZONE_NAMES[] = {"Atyrau Time", "GMT+05:00",
                                                   "Atyrau Time", "GMT+05:00"};

static const char *const GMT_07_00_ZONE_NAMES[] = {"Barnaul Standard Time", "GMT+07:00",
                                                   "Barnaul Daylight Time", "GMT+07:00"};

static const char *const AZT_ZONE_NAMES[] = {"Azerbaijan Time", "AZT",
                                             "Azerbaijan Summer Time", "AZST"};

static const char *const ICT_ZONE_NAMES[] = {"Indochina Time", "ICT",
                                             "Indochina Summer Time", "ICST"};

static const char *const KGT_ZONE_NAMES[] = {"Kirgizstan Time", "KGT",
                                             "Kirgizstan Summer Time", "KGST"};

static const char *const BNT_ZONE_NAMES[] = {"Brunei Time", "BNT",
                                             "Brunei Summer Time", "BNST"};

static const char *const YAKT_ZONE_NAMES[] = {"Yakutsk Time", "YAKT",
                                              "Yakutsk Summer Time", "YAKST"};

static const char *const CHOT_ZONE_NAMES[] = {"Choibalsan Time", "CHOT",
                                              "Choibalsan Summer Time", "CHOST"};

static const char *const BDT_ZONE_NAMES[] = {"Bangladesh Time", "BDT",
                                             "Bangladesh Summer Time", "BDST"};

static const char *const TLT_ZONE_NAMES[] = {"Timor-Leste Time", "TLT",
                                             "Timor-Leste Summer Time", "TLST"};

static const char *const GST_ZONE_NAMES[] = {"Gulf Standard Time", "GST",
                                             "Gulf Daylight Time", "GDT"};

static const char *const TJT_ZONE_NAMES[] = {"Tajikistan Time", "TJT",
                                             "Tajikistan Summer Time", "TJST"};

static const char *const HKT_ZONE_NAMES[] = {"Hong Kong Time", "HKT",
                                             "Hong Kong Summer Time", "HKST"};

static const char *const HOVT_ZONE_NAMES[] = {"Hovd Time", "HOVT",
                                              "Hovd Summer Time", "HOVST"};

static const char *const IRKT_ZONE_NAMES[] = {"Irkutsk Time", "IRKT",
                                              "Irkutsk Summer Time", "IRKST"};

static const char *const TRT_ZONE_NAMES[] = {"Turkey Time", "TRT",
                                             "Turkey Summer Time", "TRST"};

static const char *const WIB_ZONE_NAMES[] = {"West Indonesia Time", "WIB",
                                             "West Indonesia Summer Time", "WIST"};

static const char *const WIT_ZONE_NAMES[] = {"East Indonesia Time", "WIT",
                                             "East Indonesia Summer Time", "EIST"};

static const char *const AFT_ZONE_NAMES[] = {"Afghanistan Time", "AFT",
                                             "Afghanistan Summer Time", "AFST"};

static const char *const PETT_ZONE_NAMES[] = {"Petropavlovsk-Kamchatski Time", "PETT",
                                              "Petropavlovsk-Kamchatski Summer Time", "PETST"};

static const char *const PKT_ZONE_NAMES[] = {"Pakistan Time", "PKT",
                                             "Pakistan Summer Time", "PKST"};

static const char *const XJT_ZONE_NAMES[] = {"Xinjiang Standard Time", "XJT",
                                             "Xinjiang Daylight Time", "XJDT"};

static const char *const NPT_ZONE_NAMES[] = {"Nepal Time", "NPT",
                                             "Nepal Summer Time", "NPST"};

static const char *const KRAT_ZONE_NAMES[] = {"Krasnoyarsk Time", "KRAT",
                                              "Krasnoyarsk Summer Time", "KRAST"};

static const char *const MYT_ZONE_NAMES[] = {"Malaysia Time", "MYT",
                                             "Malaysia Summer Time", "MYST"};

static const char *const MAGT_ZONE_NAMES[] = {"Magadan Time", "MAGT",
                                              "Magadan Summer Time", "MAGST"};

static const char *const WITA_ZONE_NAMES[] = {"Central Indonesia Time", "WITA",
                                              "Central Indonesia Summer Time", "CIST"};

static const char *const NOVT_ZONE_NAMES[] = {"Novosibirsk Time", "NOVT",
                                              "Novosibirsk Summer Time", "NOVST"};

static const char *const ORAT_ZONE_NAMES[] = {"Oral Time", "ORAT",
                                              "Oral Summer Time", "ORAST"};

static const char *const OMST_ZONE_NAMES[] = {"Omsk Time", "OMST",
                                              "Omsk Summer Time", "OMSST"};

static const char *const KST_ZONE_NAMES[] = {"Korea Standard Time", "KST",
                                             "Korea Daylight Time", "KDT"};

static const char *const QOST_ZONE_NAMES[] = {"Kostanay Standard Time", "QOST",
                                              "Kostanay Summer Time", "QOSST"};

static const char *const QYZT_ZONE_NAMES[] = {"Qyzylorda Time", "QYZT",
                                              "Qyzylorda Summer Time", "QYZST"};

static const char *const MMT_ZONE_NAMES[] = {"Myanmar Time", "MMT",
                                             "Myanmar Summer Time", "MMST"};

static const char *const SAKT_ZONE_NAMES[] = {"Sakhalin Time", "SAKT",
                                              "Sakhalin Summer Time", "SAKST"};

static const char *const UZT_ZONE_NAMES[] = {"Uzbekistan Time", "UZT",
                                             "Uzbekistan Summer Time", "UZST"};

static const char *const SGT_ZONE_NAMES[] = {"Singapore Time", "SGT",
                                             "Singapore Summer Time", "SGST"};

static const char *const SRET_ZONE_NAMES[] = {"Srednekolymsk Time", "SRET",
                                              "Srednekolymsk Daylight Time", "SREDT"};

static const char *const GET_ZONE_NAMES[] = {"Georgia Time", "GET",
                                             "Georgia Summer Time", "GEST"};

static const char *const IRST_ZONE_NAMES[] = {"Iran Standard Time", "IRST",
                                              "Iran Daylight Time", "IRDT"};

static const char *const BTT_ZONE_NAMES[] = {"Bhutan Time", "BTT",
                                             "Bhutan Summer Time", "BTST"};

static const char *const ULAT_ZONE_NAMES[] = {"Ulaanbaatar Time", "ULAT",
                                              "Ulaanbaatar Summer Time", "ULAST"};

static const char *const VLAT_ZONE_NAMES[] = {"Ust-Nera Time", "VLAT",
                                              "Ust-Nera Summer Time", "VLAST"};

static const char *const YEKT_ZONE_NAMES[] = {"Yekaterinburg Time", "YEKT",
                                              "Yekaterinburg Summer Time", "YEKST"};

static const char *const AZOT_ZONE_NAMES[] = {"Azores Time", "AZOT",
                                              "Azores Summer Time", "AZOST"};

static const char *const CVT_ZONE_NAMES[] = {"Cape Verde Time", "CVT",
                                             "Cape Verde Summer Time", "CVST"};

static const char *const FKT_ZONE_NAMES[] = {"Falkland Is. Time", "FKT",
                                             "Falkland Is. Summer Time", "FKST"};

static const char *const AEST_ZONE_NAMES[] = {"Australian Eastern Standard Time (New South Wales)", "AEST",
                                              "Australian Eastern Daylight Time (New South Wales)", "AEDT"};

static const char *const ACST_ZONE_NAMES[] = {"Australian Central Standard Time (South Australia)", "ACST",
                                              "Australian Central Daylight Time (South Australia)", "ACDT"};

static const char *const ACWST_ZONE_NAMES[] = {"Australian Central Western Standard Time", "ACWST",
                                               "Australian Central Western Daylight Time", "ACWDT"};

static const char *const LHST_ZONE_NAMES[] = {"Lord Howe Standard Time", "LHST",
                                              "Lord Howe Daylight Time", "LHDT"};

static const char *const EAST_ZONE_NAMES[] = {"Easter Is. Time", "EAST",
                                              "Easter Is. Summer Time", "EASST"};

static const char *const GMT_04_00_ZONE_NAMES[] = {"Astrakhan Standard Time", "GMT+04:00",
                                                   "Astrakhan Standard Time", "GMT+04:00"};

static const char *const MSK_ZONE_NAMES[] = {"Moscow Standard Time", "MSK",
                                             "Moscow Daylight Time", "MSD"};

static const char *const SAMT_ZONE_NAMES[] = {"Samara Time", "SAMT",
                                              "Samara Summer Time", "SAMST"};

static const char *const IOT_ZONE_NAMES[] = {"Indian Ocean Territory Time", "IOT",
                                             "Indian Ocean Territory Summer Time", "IOST"};

static const char *const CXT_ZONE_NAMES[] = {"Christmas Island Time", "CXT",
                                             "Christmas Island Summer Time", "CXST"};

static const char *const CCT_ZONE_NAMES[] = {"Cocos Islands Time", "CCT",
                                             "Cocos Islands Summer Time", "CCST"};

static const char *const TFT_ZONE_NAMES[] = {"French Southern & Antarctic Lands Time", "TFT",
                                             "French Southern & Antarctic Lands Summer Time", "TFST"};

static const char *const SCT_ZONE_NAMES[] = {"Seychelles Time", "SCT",
                                             "Seychelles Summer Time", "SCST"};

static const char *const MVT_ZONE_NAMES[] = {"Maldives Time", "MVT",
                                             "Maldives Summer Time", "MVST"};

static const char *const MUT_ZONE_NAMES[] = {"Mauritius Time", "MUT",
                                             "Mauritius Summer Time", "MUST"};

static const char *const RET_ZONE_NAMES[] = {"Reunion Time", "RET",
                                             "Reunion Summer Time", "REST"};

static const char *const MHT_ZONE_NAMES[] = {"Marshall Islands Time", "MHT",
                                             "Marshall Islands Summer Time", "MHST"};

static const char *const MET_ZONE_NAMES[] = {"Middle Europe Time", "MET",
                                             "Middle Europe Summer Time", "MEST"};

static const char *const WSST_ZONE_NAMES[] = {"West Samoa Standard Time", "WSST",
                                              "West Samoa Daylight Time", "WSDT"};

static const char *const CHAST_ZONE_NAMES[] = {"Chatham Standard Time", "CHAST",
                                               "Chatham Daylight Time", "CHADT"};

static const char *const BST_ZONE_NAMES[] = {"Bougainville Standard Time", "BST",
                                             "Bougainville Daylight Time", "BST"};

static const char *const CHUT_ZONE_NAMES[] = {"Chuuk Time", "CHUT",
                                              "Chuuk Summer Time", "CHUST"};

static const char *const VUT_ZONE_NAMES[] = {"Vanuatu Time", "VUT",
                                             "Vanuatu Summer Time", "VUST"};

static const char *const PHOT_ZONE_NAMES[] = {"Phoenix Is. Time", "PHOT",
                                              "Phoenix Is. Summer Time", "PHOST"};

static const char *const TKT_ZONE_NAMES[] = {"Tokelau Time", "TKT",
                                             "Tokelau Summer Time", "TKST"};

static const char *const FJT_ZONE_NAMES[] = {"Fiji Time", "FJT",
                                             "Fiji Summer Time", "FJST"};

static const char *const TVT_ZONE_NAMES[] = {"Tuvalu Time", "TVT",
                                             "Tuvalu Summer Time", "TVST"};

static const char *const GALT_ZONE_NAMES[] = {"Galapagos Time", "GALT",
                                              "Galapagos Summer Time", "GALST"};

static const char *const GAMT_ZONE_NAMES[] = {"Gambier Time", "GAMT",
                                              "Gambier Summer Time", "GAMST"};

static const char *const SBT_ZONE_NAMES[] = {"Solomon Is. Time", "SBT",
                                             "Solomon Is. Summer Time", "SBST"};

static const char *const ChST_ZONE_NAMES[] = {"Chamorro Standard Time", "ChST",
                                              "Chamorro Daylight Time", "ChDT"};

static const char *const LINT_ZONE_NAMES[] = {"Line Is. Time", "LINT",
                                              "Line Is. Summer Time", "LINST"};

static const char *const KOST_ZONE_NAMES[] = {"Kosrae Time", "KOST",
                                              "Kosrae Summer Time", "KOSST"};

static const char *const MART_ZONE_NAMES[] = {"Marquesas Time", "MART",
                                              "Marquesas Summer Time", "MARST"};

static const char *const SST_ZONE_NAMES[] = {"Samoa Standard Time", "SST",
                                             "Samoa Daylight Time", "SDT"};

static const char *const NRT_ZONE_NAMES[] = {"Nauru Time", "NRT",
                                             "Nauru Summer Time", "NRST"};

static const char *const NUT_ZONE_NAMES[] = {"Niue Time", "NUT",
                                             "Niue Summer Time", "NUST"};

static const char *const NFT_ZONE_NAMES[] = {"Norfolk Time", "NFT",
                                             "Norfolk Summer Time", "NFST"};

static const char *const NCT_ZONE_NAMES[] = {"New Caledonia Time", "NCT",
                                             "New Caledonia Summer Time", "NCST"};

static const char *const PWT_ZONE_NAMES[] = {"Palau Time", "PWT",
                                             "Palau Summer Time", "PWST"};

static const char *const PONT_ZONE_NAMES[] = {"Pohnpei Time", "PONT",
                                              "Pohnpei Summer Time", "PONST"};

static const char *const PGT_ZONE_NAMES[] = {"Papua New Guinea Time", "PGT",
                                             "Papua New Guinea Summer Time", "PGST"};

static const char *const CKT_ZONE_NAMES[] = {"Cook Is. Time", "CKT",
                                             "Cook Is. Summer Time", "CKHST"};

static const char *const TAHT_ZONE_NAMES[] = {"Tahiti Time", "TAHT",
                                              "Tahiti Summer Time", "TAHST"};

static const char *const GILT_ZONE_NAMES[] = {"Gilbert Is. Time", "GILT",
                                              "Gilbert Is. Summer Time", "GILST"};

static const char *const TOT_ZONE_NAMES[] = {"Tonga Time", "TOT",
                                             "Tonga Summer Time", "TOST"};

static const char *const WAKT_ZONE_NAMES[] = {"Wake Time", "WAKT",
                                              "Wake Summer Time", "WAKST"};

static const char *const WFT_ZONE_NAMES[] = {"Wallis & Futuna Time", "WFT",
                                             "Wallis & Futuna Summer Time", "WFST"};

#endif