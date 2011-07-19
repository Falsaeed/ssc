#include "core.h"
#include "lib_financial.h"
#include <sstream>

#ifndef WIN32
#include <float.h>
#endif

static var_info vtab_ippppa[] = {
/*   VARTYPE           DATATYPE          NAME                        LABEL                                  UNITS         META                      GROUP            REQUIRED_IF                 CONSTRAINTS                      UI_HINTS*/
	{ SSC_INPUT,        SSC_NUMBER,		 "market",                   "Utility IPP or Commercial PPA",   "0/1",          "0=ipp,1=ppa", "ippppa",      "?=0",                     "INTEGER,MIN=0,MAX=1",            "" },

	{ SSC_INPUT,        SSC_NUMBER,      "energy_net",				"Annual energy produced by system",	"kWh",   "",                      "ippppa",             "*",						   "",                              "" },
	{ SSC_INPUT,        SSC_ARRAY,      "energy_availability",		"Annual energy availability",	"%",   "",                      "ippppa",             "*",						   "",                              "" },
	{ SSC_INPUT,        SSC_ARRAY,      "energy_degradation",		"Annual energy degradation",	"%",   "",                      "ippppa",             "*",						   "",                              "" },
	{ SSC_INPUT,        SSC_NUMBER,     "system_capacity",			"System nameplate capacity",		"kW",    "",                      "ippppa",             "*",						   "MIN=1e-3",                         "" },

	{ SSC_INPUT,        SSC_NUMBER,		"ppa_soln_mode",            "PPA solution mode",                "0/1",   "0=specify ppa,1=solve ppa", "ippppa",         "?=0",                     "INTEGER,MIN=0,MAX=1",            "" },
	{ SSC_INPUT,        SSC_NUMBER,		"ppa_soln_tolerance",            "PPA solution tolerance",                "",   "", "ippppa",         "?=1e-3",                     "",            "" },
	{ SSC_INPUT,        SSC_NUMBER,		"ppa_soln_min",            "PPA solution minimum ppa",                "cents/kWh",   "", "ippppa",         "?=0",                     "",            "" },
	{ SSC_INPUT,        SSC_NUMBER,		"ppa_soln_max",            "PPA solution maximum ppa",                "cents/kWh",   "", "ippppa",         "?=100",                     "",            "" },
	{ SSC_INPUT,        SSC_NUMBER,		"ppa_soln_max_iterations",            "PPA solution maximum number of iterations",                "",   "", "ippppa",         "?=100",                     "INTEGER,MIN=1",            "" },

	{ SSC_INPUT,        SSC_NUMBER,     "ppa_price_input",			"Initial year PPA price",			"cents/kWh",	 "",			  "ippppa",			 "?=10",         "",      			"" },
	{ SSC_INPUT,        SSC_NUMBER,     "ppa_escalation",           "PPA escalation",					"%",	 "",					  "ippppa",             "?=0",                     "MIN=0,MAX=100",      			"" },

	{ SSC_INPUT,       SSC_NUMBER,      "constr_total_financing",	"Construction financing total",	"$",	 "",					  "ippppa",			 "*",                         "",                             "" },



	{ SSC_INPUT,        SSC_NUMBER,      "total_installed_cost",                          "Total installed cost",                               "$",      "",                      "Cashloan",            "*",                      "MIN=0",                                         "" },
	{ SSC_INPUT,        SSC_NUMBER,      "salvage_percentage",                       "Salvage value percentage",                        "%",      "",                      "Cashloan",      "?=0.0",                  "MIN=0,MAX=100",                 "" },
	{ SSC_INPUT,        SSC_NUMBER,      "salvage_percentage",                       "Salvage value percentage",                        "%",      "",                      "Financials",      "?=0.0",                  "MIN=0,MAX=100",                 "" },

	{ SSC_INPUT,        SSC_NUMBER,      "annual_fuel_usage",        "Fuel usage",                         "kWht",         "",                      "ippppa",      "?=0",                     "MIN=0",                                         "" },

	{ SSC_INPUT,        SSC_NUMBER,      "min_dscr_target",                       "Minimum required DSCR",        "",      "",                      "ippppa",      "?=1.4",                  "",                 "" },
	{ SSC_INPUT,        SSC_NUMBER,      "min_irr_target",                       "Minimum required IRR",          "%",      "",                      "ippppa",      "?=15",                  "",                 "" },
	{ SSC_INPUT,        SSC_NUMBER,      "ppa_escalation",                       "PPA escalation",               "%",      "",                      "ippppa",      "?=0.6",                  "MIN=0,MAX=100",                 "" },
	{ SSC_INPUT,        SSC_NUMBER,      "min_dscr_required",              "Minimum DSCR required",        "0/1",      "0=no,1=yes",                      "ippppa",      "?=1",                  "INTEGER,MIN=0,MAX=1",                 "" },
	{ SSC_INPUT,        SSC_NUMBER,      "positive_cashflow_required",     "Positive cash flow required",  "0/1",      "0=no,1=yes",                      "ippppa",      "?=1",                  "INTEGER,MIN=0,MAX=1",                 "" },
	{ SSC_INPUT,        SSC_NUMBER,      "optimize_lcoe_wrt_debt_fraction","Optimize LCOE with respect to Debt Fraction",  "0/1",      "0=no,1=yes",                      "ippppa",      "?=0",                  "INTEGER,MIN=0,MAX=1",                 "" },
	{ SSC_INPUT,        SSC_NUMBER,      "optimize_lcoe_wrt_ppa_escalation","Optimize LCOE with respect to PPA Escalation",  "0/1",      "0=no,1=yes",                      "ippppa",      "?=0",                  "INTEGER,MIN=0,MAX=1",                 "" },

/* Dispatch */
	{ SSC_INPUT,        SSC_NUMBER,      "system_use_lifetime_output",		"Lifetime hourly system outputs",	"0/1",   "0=hourly first year,1=hourly lifetime",                      "ippppa",             "*",						   "INTEGER,MIN=0",                 "" },
	{ SSC_INPUT,        SSC_ARRAY,      "energy_net_hourly",	"Hourly energy produced by the system",	"%",   "",                      "ippppa",             "*",						   "",                 "" },
	{ SSC_INPUT,        SSC_ARRAY,      "dispatch_hourly",		"Hourly dispatch schedule for the system (1-9)",	"",   "",                      "ippppa",             "*",				   "",                 "" },
	{ SSC_INPUT,        SSC_NUMBER,      "dispatch_factor1",		"Dispatch payment factor 1",	"",   "",                      "ippppa",             "*",						   "",                 "" },
	{ SSC_INPUT,        SSC_NUMBER,      "dispatch_factor2",		"Dispatch payment factor 2",	"",   "",                      "ippppa",             "*",						   "",                 "" },
	{ SSC_INPUT,        SSC_NUMBER,      "dispatch_factor3",		"Dispatch payment factor 3",	"",   "",                      "ippppa",             "*",						   "",                 "" },
	{ SSC_INPUT,        SSC_NUMBER,      "dispatch_factor4",		"Dispatch payment factor 4",	"",   "",                      "ippppa",             "*",						   "",                 "" },
	{ SSC_INPUT,        SSC_NUMBER,      "dispatch_factor5",		"Dispatch payment factor 5",	"",   "",                      "ippppa",             "*",						   "",                 "" },
	{ SSC_INPUT,        SSC_NUMBER,      "dispatch_factor6",		"Dispatch payment factor 6",	"",   "",                      "ippppa",             "*",						   "",                 "" },
	{ SSC_INPUT,        SSC_NUMBER,      "dispatch_factor7",		"Dispatch payment factor 7",	"",   "",                      "ippppa",             "*",						   "",                 "" },
	{ SSC_INPUT,        SSC_NUMBER,      "dispatch_factor8",		"Dispatch payment factor 8",	"",   "",                      "ippppa",             "*",						   "",                 "" },
	{ SSC_INPUT,        SSC_NUMBER,      "dispatch_factor9",		"Dispatch payment factor 9",	"",   "",                      "ippppa",             "*",						   "",                 "" },

	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_jan",	"Energy produced by the system in January",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_jan",		"Revenue from the system in January",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_feb",	"Energy produced by the system in February",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_feb",		"Revenue from the system in February",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_mar",	"Energy produced by the system in March",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_mar",		"Revenue from the system in March",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_apr",	"Energy produced by the system in April",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_apr",		"Revenue from the system in April",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_may",	"Energy produced by the system in May",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_may",		"Revenue from the system in May",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_jun",	"Energy produced by the system in June",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_jun",		"Revenue from the system in June",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_jul",	"Energy produced by the system in July",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_jul",		"Revenue from the system in July",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_aug",	"Energy produced by the system in August",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_aug",		"Revenue from the system in August",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_sep",	"Energy produced by the system in September",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_sep",		"Revenue from the system in September",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_oct",	"Energy produced by the system in October",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_oct",		"Revenue from the system in October",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_nov",	"Energy produced by the system in November",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_nov",		"Revenue from the system in November",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_dec",	"Energy produced by the system in December",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_dec",		"Revenue from the system in December",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },

	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_dispatch1",	"Energy produced by the system in dispatch period 1",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_dispatch1",		"Revenue from the system in dispatch period 1",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_dispatch2",	"Energy produced by the system in dispatch period 2",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_dispatch2",		"Revenue from the system in dispatch period 2",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_dispatch3",	"Energy produced by the system in dispatch period 3",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_dispatch3",		"Revenue from the system in dispatch period 3",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_dispatch4",	"Energy produced by the system in dispatch period 4",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_dispatch4",		"Revenue from the system in dispatch period 4",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_dispatch5",	"Energy produced by the system in dispatch period 5",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_dispatch5",		"Revenue from the system in dispatch period 5",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_dispatch6",	"Energy produced by the system in dispatch period 6",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_dispatch6",		"Revenue from the system in dispatch period 6",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_dispatch7",	"Energy produced by the system in dispatch period 7",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_dispatch7",		"Revenue from the system in dispatch period 7",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_dispatch8",	"Energy produced by the system in dispatch period 8",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_dispatch8",		"Revenue from the system in dispatch period 8",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_energy_net_dispatch9",	"Energy produced by the system in dispatch period 9",	"",   "",                      "ippppa",             "*",						   "LENGTH_EQUAL=cf_length",                 "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_revenue_dispatch9",		"Revenue from the system in dispatch period 9",	"",   "",                      "ippppa",             "*",				   "LENGTH_EQUAL=cf_length",                 "" },


	/* financial outputs */
	{ SSC_OUTPUT,        SSC_NUMBER,     "cf_length",                "Number of periods in cashflow",      "",             "",                      "ippppa",      "*",                       "INTEGER",                                  "" },

	{ SSC_OUTPUT,        SSC_NUMBER,     "lcoe_real",                "Real LCOE",                          "cents/kWh",    "",                      "ippppa",      "*",                       "",                                         "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "lcoe_nom",                 "Nominal LCOE",                       "cents/kWh",    "",                      "ippppa",      "*",                       "",                                         "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "npv",                      "Net present value",				   "$",            "",                      "ippppa",      "*",                       "",                                         "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "ppa",                      "First year PPA",				   "cents/kWh",            "",                      "ippppa",      "*",                       "",                                         "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "npv",                      "Net present value",				   "$",            "",                      "ippppa",      "*",                       "",                                         "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "min_cashflow",                      "Minimum cash flow value",				   "$",            "",                      "ippppa",      "*",                       "",                                         "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "irr",                      "Internal rate of return",				   "%",            "",                      "ippppa",      "*",                       "",                                         "" },
	{ SSC_OUTPUT,        SSC_NUMBER,     "min_dscr",                      "Minimum DSCR",				   "",            "",                      "ippppa",      "*",                       "",                                         "" },

	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_om_fixed_expense",      "O&M Fixed expense",                  "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_om_production_expense", "O&M Production-based expense",       "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_om_capacity_expense",   "O&M Capacity-based expense",         "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_om_fuel_expense",       "O&M Fuel expense",                   "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_property_tax_assessed_value","Property tax net assessed value", "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_property_tax_expense",  "Property tax expense",               "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_insurance_expense",     "Insurance expense",                  "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_operating_expenses",    "Total operating expense",            "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_operating_income",    "Total operating income",            "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },

	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_deductible_expenses",   "Deductible expenses",                "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },

	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_debt_balance",          "Debt balance",                       "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_debt_payment_interest", "Interest payment",                   "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_debt_payment_principal","Principal payment",                  "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_debt_payment_total",    "Total P&I debt payment",             "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },

	{ SSC_OUTPUT,        SSC_NUMBER,      "ibi_total",             "Total IBI incentive income",         "$",            "",                      "ippppa",      "*",                     "",                "" },
	{ SSC_OUTPUT,        SSC_NUMBER,      "cbi_total",             "Total CBI incentive income",         "$",            "",                      "ippppa",      "*",                     "",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_pbi_total",             "Total PBI incentive income",         "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },

	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_ptc_fed",               "Federal PTC income",                 "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_ptc_sta",               "State PTC income",                   "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },

	{ SSC_OUTPUT,        SSC_NUMBER,      "itc_fed_total",         "Federal ITC income",                 "$",            "",                      "ippppa",      "*",                     "",                "" },
	{ SSC_OUTPUT,        SSC_NUMBER,      "itc_sta_total",         "State ITC income",                   "$",            "",                      "ippppa",      "*",                     "",                "" },

	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_sta_depr_sched",                        "State depreciation schedule",              "%",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_sta_depreciation",                      "State depreciation",                       "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_sta_incentive_income_less_deductions",  "State incentive income less deductions",   "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_sta_taxable_income_less_deductions",    "State taxable income less deductions",     "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_sta_tax_savings",                       "State tax savings",                        "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },

	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_fed_depr_sched",                        "Federal depreciation schedule",            "%",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_fed_depreciation",                      "Federal depreciation",                     "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_fed_incentive_income_less_deductions",  "Federal incentive income less deductions", "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_fed_taxable_income_less_deductions",    "Federal taxable income less deductions",   "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_fed_tax_savings",                       "Federal tax savings",                      "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },

	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_sta_and_fed_tax_savings",               "Total tax savings (Federal & State)",      "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_after_tax_net_equity_cash_flow",        "After tax net equity cash flow",           "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,      "cf_after_tax_cash_flow",                   "After tax cash flow",                      "$",            "",                      "ippppa",      "*",                     "LENGTH_EQUAL=cf_length",                "" },
	{ SSC_OUTPUT,        SSC_ARRAY,       "cf_ppa_price",            "PPA price",                     "cents/kWh",      "",                      "ippppa",             "*",                      "LENGTH_EQUAL=cf_length",                             "" },
	{ SSC_OUTPUT,        SSC_ARRAY,       "cf_pretax_dscr",            "Pre-tax DSCR",                     "",      "",                      "ippppa",             "*",                      "LENGTH_EQUAL=cf_length",                             "" },
	

var_info_invalid };

extern var_info
	vtab_standard_financial[],
	vtab_standard_loan[],
	vtab_oandm[],
	vtab_depreciation[],
	vtab_utility_rate[],
	vtab_tax_credits[],
	vtab_payment_incentives[];

enum {
	CF_energy_net,
	CF_energy_value,
	CF_ppa_price,
	CF_ppa_revenue,

	CF_om_fixed_expense,
	CF_om_production_expense,
	CF_om_capacity_expense,
	CF_om_fuel_expense,
	CF_property_tax_assessed_value,
	CF_property_tax_expense,
	CF_insurance_expense,
	CF_operating_expenses,

	CF_deductible_expenses,

	CF_debt_balance,
	CF_debt_payment_interest,
	CF_debt_payment_principal,
	CF_debt_payment_total,

	CF_pbi_fed,
	CF_pbi_sta,
	CF_pbi_uti,
	CF_pbi_oth,
	CF_pbi_total,

	CF_ptc_fed,
	CF_ptc_sta,

	CF_sta_depr_sched,
	CF_sta_depreciation,
	CF_sta_incentive_income_less_deductions,
	CF_sta_taxable_income_less_deductions,
	CF_sta_tax_savings,

	CF_fed_depr_sched,
	CF_fed_depreciation,
	CF_fed_incentive_income_less_deductions,
	CF_fed_taxable_income_less_deductions,
	CF_fed_tax_savings,

	CF_sta_and_fed_tax_savings,
	CF_after_tax_net_equity_cash_flow,
	CF_after_tax_cash_flow,

	// Dispatch
	CF_TOD1Energy,
	CF_TOD2Energy,
	CF_TOD3Energy,
	CF_TOD4Energy,
	CF_TOD5Energy,
	CF_TOD6Energy,
	CF_TOD7Energy,
	CF_TOD8Energy,
	CF_TOD9Energy,

	CF_TODJanEnergy,
	CF_TODFebEnergy,
	CF_TODMarEnergy,
	CF_TODAprEnergy,
	CF_TODMayEnergy,
	CF_TODJunEnergy,
	CF_TODJulEnergy,
	CF_TODAugEnergy,
	CF_TODSepEnergy,
	CF_TODOctEnergy,
	CF_TODNovEnergy,
	CF_TODDecEnergy,

	CF_TOD1JanEnergy,
	CF_TOD1FebEnergy,
	CF_TOD1MarEnergy,
	CF_TOD1AprEnergy,
	CF_TOD1MayEnergy,
	CF_TOD1JunEnergy,
	CF_TOD1JulEnergy,
	CF_TOD1AugEnergy,
	CF_TOD1SepEnergy,
	CF_TOD1OctEnergy,
	CF_TOD1NovEnergy,
	CF_TOD1DecEnergy,

	CF_TOD2JanEnergy,
	CF_TOD2FebEnergy,
	CF_TOD2MarEnergy,
	CF_TOD2AprEnergy,
	CF_TOD2MayEnergy,
	CF_TOD2JunEnergy,
	CF_TOD2JulEnergy,
	CF_TOD2AugEnergy,
	CF_TOD2SepEnergy,
	CF_TOD2OctEnergy,
	CF_TOD2NovEnergy,
	CF_TOD2DecEnergy,

	CF_TOD3JanEnergy,
	CF_TOD3FebEnergy,
	CF_TOD3MarEnergy,
	CF_TOD3AprEnergy,
	CF_TOD3MayEnergy,
	CF_TOD3JunEnergy,
	CF_TOD3JulEnergy,
	CF_TOD3AugEnergy,
	CF_TOD3SepEnergy,
	CF_TOD3OctEnergy,
	CF_TOD3NovEnergy,
	CF_TOD3DecEnergy,

	CF_TOD4JanEnergy,
	CF_TOD4FebEnergy,
	CF_TOD4MarEnergy,
	CF_TOD4AprEnergy,
	CF_TOD4MayEnergy,
	CF_TOD4JunEnergy,
	CF_TOD4JulEnergy,
	CF_TOD4AugEnergy,
	CF_TOD4SepEnergy,
	CF_TOD4OctEnergy,
	CF_TOD4NovEnergy,
	CF_TOD4DecEnergy,

	CF_TOD5JanEnergy,
	CF_TOD5FebEnergy,
	CF_TOD5MarEnergy,
	CF_TOD5AprEnergy,
	CF_TOD5MayEnergy,
	CF_TOD5JunEnergy,
	CF_TOD5JulEnergy,
	CF_TOD5AugEnergy,
	CF_TOD5SepEnergy,
	CF_TOD5OctEnergy,
	CF_TOD5NovEnergy,
	CF_TOD5DecEnergy,

	CF_TOD6JanEnergy,
	CF_TOD6FebEnergy,
	CF_TOD6MarEnergy,
	CF_TOD6AprEnergy,
	CF_TOD6MayEnergy,
	CF_TOD6JunEnergy,
	CF_TOD6JulEnergy,
	CF_TOD6AugEnergy,
	CF_TOD6SepEnergy,
	CF_TOD6OctEnergy,
	CF_TOD6NovEnergy,
	CF_TOD6DecEnergy,

	CF_TOD7JanEnergy,
	CF_TOD7FebEnergy,
	CF_TOD7MarEnergy,
	CF_TOD7AprEnergy,
	CF_TOD7MayEnergy,
	CF_TOD7JunEnergy,
	CF_TOD7JulEnergy,
	CF_TOD7AugEnergy,
	CF_TOD7SepEnergy,
	CF_TOD7OctEnergy,
	CF_TOD7NovEnergy,
	CF_TOD7DecEnergy,

	CF_TOD8JanEnergy,
	CF_TOD8FebEnergy,
	CF_TOD8MarEnergy,
	CF_TOD8AprEnergy,
	CF_TOD8MayEnergy,
	CF_TOD8JunEnergy,
	CF_TOD8JulEnergy,
	CF_TOD8AugEnergy,
	CF_TOD8SepEnergy,
	CF_TOD8OctEnergy,
	CF_TOD8NovEnergy,
	CF_TOD8DecEnergy,

	CF_TOD9JanEnergy,
	CF_TOD9FebEnergy,
	CF_TOD9MarEnergy,
	CF_TOD9AprEnergy,
	CF_TOD9MayEnergy,
	CF_TOD9JunEnergy,
	CF_TOD9JulEnergy,
	CF_TOD9AugEnergy,
	CF_TOD9SepEnergy,
	CF_TOD9OctEnergy,
	CF_TOD9NovEnergy,
	CF_TOD9DecEnergy,

	CF_TOD1Revenue,
	CF_TOD2Revenue,
	CF_TOD3Revenue,
	CF_TOD4Revenue,
	CF_TOD5Revenue,
	CF_TOD6Revenue,
	CF_TOD7Revenue,
	CF_TOD8Revenue,
	CF_TOD9Revenue,

	CF_TODJanRevenue,
	CF_TODFebRevenue,
	CF_TODMarRevenue,
	CF_TODAprRevenue,
	CF_TODMayRevenue,
	CF_TODJunRevenue,
	CF_TODJulRevenue,
	CF_TODAugRevenue,
	CF_TODSepRevenue,
	CF_TODOctRevenue,
	CF_TODNovRevenue,
	CF_TODDecRevenue,

	CF_Availability,
	CF_Degradation,

	CF_recapitalization,
	CF_recapitalization_boolean,
	
	CF_operating_income,
	CF_sta_income_taxes,
	CF_fed_income_taxes,
	CF_pretax_dscr,

	CF_max };




class cm_ippppa : public compute_module
{
private:
	util::matrix_t<double> cf;
	double ibi_fed_amount;
	double ibi_sta_amount;
	double ibi_uti_amount;
	double ibi_oth_amount;
	double ibi_fed_per;
	double ibi_sta_per;
	double ibi_uti_per;
	double ibi_oth_per;
	double cbi_fed_amount;
	double cbi_sta_amount;
	double cbi_uti_amount;
	double cbi_oth_amount;
public:
	cm_ippppa()
	{
		add_var_info( vtab_standard_financial );
		add_var_info( vtab_standard_loan );
		add_var_info( vtab_oandm );
		add_var_info( vtab_depreciation );
		add_var_info( vtab_tax_credits );
		add_var_info( vtab_payment_incentives );

		add_var_info( vtab_ippppa );
	}

	void exec( ) throw( general_error )
	{
		int i=0;

		bool is_commercialppa = (as_integer("market")==1);
		int ppa_soln_mode = as_integer("ppa_soln_mode");

		int nyears = as_integer("analysis_years");

		// initialize cashflow matrix
		cf.resize_fill( CF_max, nyears+1, 0.0 );
    	double nameplate = as_double("system_capacity"); // kW

		// initialize energy and revenue
		// initialize energy

		double first_year_energy = as_double("energy_net");
		size_t count_avail = 0;
		ssc_number_t *avail = 0;
		avail = as_array("energy_availability", &count_avail);
		size_t count_degrad = 0;
		ssc_number_t *degrad = 0;
		degrad = as_array("energy_degradation", &count_degrad);

		// degradation starts in year 2 for single value degradation - no degradation in year 1 - degradation =1.0
		if (count_degrad == 1)
		{
			if (as_integer("system_use_lifetime_output"))
			{
				if (nyears>=1) cf.at(CF_Degradation,1) = 1.0;
				for (i=2;i<=nyears;i++) cf.at(CF_Degradation,i) = 1.0 - degrad[0]/100.0;
			}
			else
				for (i=1;i<=nyears;i++) cf.at(CF_Degradation,i) = pow((1.0 - degrad[0]/100.0),i-1);
		}
		else if (count_degrad > 0)
		{
			for (i=0;i<nyears && i<(int)count_degrad;i++) cf.at(CF_Degradation,i+1) = (1.0 - degrad[i]/100.0);
		}

		if (count_avail == 1)
		{
			for (i=1;i<=nyears;i++) cf.at(CF_Availability,i)  = avail[0]/100.0;
		}
		else if (count_avail > 0)
		{
			for (i=0;i<nyears && i<(int)count_avail;i++) cf.at(CF_Availability,i+1) = avail[i]/100.0;
		}

		// dispatch
		if (as_integer("system_use_lifetime_output"))
		{
			compute_lifetime_dispatch_output(nyears);
		}
		else
		{
			for (i=1;i<=nyears;i++)
				cf.at(CF_energy_net,i) = first_year_energy * cf.at(CF_Degradation,i) * cf.at(CF_Availability,i);
			compute_dispatch_output(nyears);
		}

		double dispatch_factor1 = as_double("dispatch_factor1");
		double dispatch_factor2 = as_double("dispatch_factor2");
		double dispatch_factor3 = as_double("dispatch_factor3");
		double dispatch_factor4 = as_double("dispatch_factor4");
		double dispatch_factor5 = as_double("dispatch_factor5");
		double dispatch_factor6 = as_double("dispatch_factor6");
		double dispatch_factor7 = as_double("dispatch_factor7");
		double dispatch_factor8 = as_double("dispatch_factor8");
		double dispatch_factor9 = as_double("dispatch_factor9");

		for (i=1;i<=nyears;i++)
		{
			cf.at(CF_om_production_expense,i) *= cf.at(CF_energy_net,i);
			cf.at(CF_om_capacity_expense,i) *= nameplate;
		}


		double ppa = as_double("ppa_price_input"); // either initial guess for ppa_mode=1 or final ppa for ppa_mode=0

		double ppa_escalation = 0.01*as_double("ppa_escalation");


		double year1_fuel_use = as_double("annual_fuel_usage"); // kWht

		double inflation_rate = as_double("inflation_rate")*0.01;
		double property_tax = as_double("property_tax_rate")*0.01;
		double property_tax_decline_percentage = as_double("prop_tax_assessed_decline");
		double insurance_rate = as_double("insurance_rate")*0.01;
		double salvage_frac = as_double("salvage_percentage")*0.01;
		double federal_tax_rate = as_double("federal_tax_rate")*0.01;
		double state_tax_rate = as_double("state_tax_rate")*0.01;
		double effective_tax_rate = federal_tax_rate + (1-federal_tax_rate)*state_tax_rate;

		double real_discount_rate = as_double("real_discount_rate")*0.01;
		double nom_discount_rate = (1.0 + real_discount_rate) * (1.0 + inflation_rate) - 1.0;

		double min_dscr_target = as_double("min_dscr_target");
		double min_irr_target = as_double("min_irr_target");
		int min_dscr_required = as_integer("min_dscr_required");
		int positive_cashflow_required = as_integer("positive_cashflow_required");

		if (is_commercialppa)
		{
			min_dscr_required =0;
			positive_cashflow_required =0;
		}

		double total_cost = as_double("total_installed_cost");
		double property_tax_assessed_value = total_cost * as_double("prop_tax_cost_assessed_percent") * 0.01;

		int loan_term = as_integer("loan_term");
		double loan_rate = as_double("loan_rate")*0.01;
		double debt_frac = as_double("loan_debt")*0.01;

		// precompute expenses from annual schedules or value+escalation
		escal_or_annual( CF_om_fixed_expense, nyears, "om_fixed", inflation_rate, 1.0, false, as_double("om_fixed_escal")*0.01 );
		escal_or_annual( CF_om_production_expense, nyears, "om_production", inflation_rate, 0.001, false, as_double("om_production_escal")*0.01 );
		escal_or_annual( CF_om_capacity_expense, nyears, "om_capacity", inflation_rate, 1.0, false, as_double("om_capacity_escal")*0.01 );
		escal_or_annual( CF_om_fuel_expense, nyears, "om_fuel_cost", inflation_rate, as_double("system_heat_rate")*0.001, false, as_double("om_fuel_cost_escal")*0.01 );

		// ibi fixed
		ibi_fed_amount = as_double("ibi_fed_amount");
		ibi_sta_amount = as_double("ibi_sta_amount");
		ibi_uti_amount = as_double("ibi_uti_amount");
		ibi_oth_amount = as_double("ibi_oth_amount");

		// ibi percent
		ibi_fed_per = as_double("ibi_fed_percent")*0.01*total_cost;
		if (ibi_fed_per > as_double("ibi_fed_percent_maxvalue")) ibi_fed_per = as_double("ibi_fed_percent_maxvalue");
		ibi_sta_per = as_double("ibi_sta_percent")*0.01*total_cost;
		if (ibi_sta_per > as_double("ibi_sta_percent_maxvalue")) ibi_sta_per = as_double("ibi_sta_percent_maxvalue");
		ibi_uti_per = as_double("ibi_uti_percent")*0.01*total_cost;
		if (ibi_uti_per > as_double("ibi_uti_percent_maxvalue")) ibi_uti_per = as_double("ibi_uti_percent_maxvalue");
		ibi_oth_per = as_double("ibi_oth_percent")*0.01*total_cost;
		if (ibi_oth_per > as_double("ibi_oth_percent_maxvalue")) ibi_oth_per = as_double("ibi_oth_percent_maxvalue");

		// itc fixed
		double itc_fed_amount = as_double("itc_fed_amount");
		double itc_sta_amount = as_double("itc_sta_amount");

		// itc percent - max value used for comparison to qualifying costs
		double itc_fed_frac = as_double("itc_fed_percent")*0.01;
		double itc_fed_per = itc_fed_frac * total_cost;
		if (itc_fed_per > as_double("itc_fed_percent_maxvalue")) itc_fed_per = as_double("itc_fed_percent_maxvalue");
		double itc_sta_frac = as_double("itc_sta_percent")*0.01;
		double itc_sta_per = itc_sta_frac * total_cost;
		if (itc_sta_per > as_double("itc_sta_percent_maxvalue")) itc_sta_per = as_double("itc_sta_percent_maxvalue");

		// cbi
		cbi_fed_amount = 1000.0*nameplate*as_double("cbi_fed_amount");
		if (cbi_fed_amount > as_double("cbi_fed_maxvalue")) cbi_fed_amount = as_double("cbi_fed_maxvalue");
		cbi_sta_amount = 1000.0*nameplate*as_double("cbi_sta_amount");
		if (cbi_sta_amount > as_double("cbi_sta_maxvalue")) cbi_sta_amount = as_double("cbi_sta_maxvalue");
		cbi_uti_amount = 1000.0*nameplate*as_double("cbi_uti_amount");
		if (cbi_uti_amount > as_double("cbi_uti_maxvalue")) cbi_uti_amount = as_double("cbi_uti_maxvalue");
		cbi_oth_amount = 1000.0*nameplate*as_double("cbi_oth_amount");
		if (cbi_oth_amount > as_double("cbi_oth_maxvalue")) cbi_oth_amount = as_double("cbi_oth_maxvalue");

		// precompute pbi
		compute_production_incentive( CF_pbi_fed, nyears, "pbi_fed_amount", "pbi_fed_term", "pbi_fed_escal" );
		compute_production_incentive( CF_pbi_sta, nyears, "pbi_sta_amount", "pbi_sta_term", "pbi_sta_escal" );
		compute_production_incentive( CF_pbi_uti, nyears, "pbi_uti_amount", "pbi_uti_term", "pbi_uti_escal" );
		compute_production_incentive( CF_pbi_oth, nyears, "pbi_oth_amount", "pbi_oth_term", "pbi_oth_escal" );

		// precompute ptc
		compute_production_incentive_IRS_2010_37( CF_ptc_sta, nyears, "ptc_sta_amount", "ptc_sta_term", "ptc_sta_escal" );
		compute_production_incentive_IRS_2010_37( CF_ptc_fed, nyears, "ptc_fed_amount", "ptc_fed_term", "ptc_fed_escal" );


		double federal_depr_basis = total_cost
			- ( as_boolean("ibi_fed_amount_deprbas_fed")  ? ibi_fed_amount : 0 )
			- ( as_boolean("ibi_sta_amount_deprbas_fed")  ? ibi_sta_amount : 0 )
			- ( as_boolean("ibi_uti_amount_deprbas_fed")  ? ibi_uti_amount : 0 )
			- ( as_boolean("ibi_oth_amount_deprbas_fed")  ? ibi_oth_amount : 0 )
			- ( as_boolean("ibi_fed_percent_deprbas_fed") ? ibi_fed_per : 0 )
			- ( as_boolean("ibi_sta_percent_deprbas_fed") ? ibi_sta_per : 0 )
			- ( as_boolean("ibi_uti_percent_deprbas_fed") ? ibi_uti_per : 0 )
			- ( as_boolean("ibi_oth_percent_deprbas_fed") ? ibi_oth_per : 0 )
			- ( as_boolean("cbi_fed_deprbas_fed")  ? cbi_fed_amount : 0 )
			- ( as_boolean("cbi_sta_deprbas_fed")  ? cbi_sta_amount : 0 )
			- ( as_boolean("cbi_uti_deprbas_fed")  ? cbi_uti_amount : 0 )
			- ( as_boolean("cbi_oth_deprbas_fed")  ? cbi_oth_amount : 0 )
			- ( as_boolean("itc_fed_amount_deprbas_fed")   ? 0.5*itc_fed_amount : 0 )
			- ( as_boolean("itc_fed_percent_deprbas_fed")  ? 0.5*itc_fed_per : 0 )
			- ( as_boolean("itc_sta_amount_deprbas_fed")   ? 0.5*itc_sta_amount : 0 )
			- ( as_boolean("itc_sta_percent_deprbas_fed")  ? 0.5*itc_sta_per : 0 );

		double state_depr_basis = total_cost
			- ( as_boolean("ibi_fed_amount_deprbas_sta")  ? ibi_fed_amount : 0 )
			- ( as_boolean("ibi_sta_amount_deprbas_sta")  ? ibi_sta_amount : 0 )
			- ( as_boolean("ibi_uti_amount_deprbas_sta")  ? ibi_uti_amount : 0 )
			- ( as_boolean("ibi_oth_amount_deprbas_sta")  ? ibi_oth_amount : 0 )
			- ( as_boolean("ibi_fed_percent_deprbas_sta") ? ibi_fed_per : 0 )
			- ( as_boolean("ibi_sta_percent_deprbas_sta") ? ibi_sta_per : 0 )
			- ( as_boolean("ibi_uti_percent_deprbas_sta") ? ibi_uti_per : 0 )
			- ( as_boolean("ibi_oth_percent_deprbas_sta") ? ibi_oth_per : 0 )
			- ( as_boolean("cbi_fed_deprbas_sta")  ? cbi_fed_amount : 0 )
			- ( as_boolean("cbi_sta_deprbas_sta")  ? cbi_sta_amount : 0 )
			- ( as_boolean("cbi_uti_deprbas_sta")  ? cbi_uti_amount : 0 )
			- ( as_boolean("cbi_oth_deprbas_sta")  ? cbi_oth_amount : 0 )
			- ( as_boolean("itc_fed_amount_deprbas_sta")   ? 0.5*itc_fed_amount : 0 )
			- ( as_boolean("itc_fed_percent_deprbas_sta")  ? 0.5*itc_fed_per : 0 )
			- ( as_boolean("itc_sta_amount_deprbas_sta")   ? 0.5*itc_sta_amount : 0 )
			- ( as_boolean("itc_sta_percent_deprbas_sta")  ? 0.5*itc_sta_per : 0 );

		switch( as_integer("depr_sta_type") )
		{
		case 1: depreciation_sched_macrs_half_year( CF_sta_depr_sched, nyears ); break;
		case 2: depreciation_sched_straight_line( CF_sta_depr_sched, nyears, as_integer("depr_sta_sl_years") ); break;
		case 3:
			{
				size_t arr_len;
				ssc_number_t *arr_cust = as_array( "depr_sta_custom", &arr_len );
				depreciation_sched_custom( CF_sta_depr_sched, nyears, arr_cust, (int)arr_len );
				break;
			}
		}

		switch( as_integer("depr_fed_type") )
		{
		case 1: depreciation_sched_macrs_half_year( CF_fed_depr_sched, nyears ); break;
		case 2: depreciation_sched_straight_line( CF_fed_depr_sched, nyears, as_integer("depr_fed_sl_years") ); break;
		case 3:
			{
				size_t arr_len;
				ssc_number_t *arr_cust = as_array( "depr_fed_custom", &arr_len );
				depreciation_sched_custom( CF_fed_depr_sched, nyears, arr_cust, (int)arr_len );
				break;
			}
		}

		double state_tax_savings = 0.0;
		double federal_tax_savings = 0.0;

		double adjusted_installed_cost = total_cost
			- ibi_fed_amount
			- ibi_sta_amount
			- ibi_uti_amount
			- ibi_oth_amount
			- ibi_fed_per
			- ibi_sta_per
			- ibi_uti_per
			- ibi_oth_per
			- cbi_fed_amount
			- cbi_sta_amount
			- cbi_uti_amount
			- cbi_oth_amount;

		double loan_amount = debt_frac * adjusted_installed_cost;
		double first_cost = adjusted_installed_cost - loan_amount;

		cf.at(CF_after_tax_net_equity_cash_flow,0) = -first_cost + state_tax_savings + federal_tax_savings;
		cf.at(CF_after_tax_cash_flow,0) = cf.at(CF_after_tax_net_equity_cash_flow,0);

		double ibi_total = ibi_fed_amount + ibi_fed_per + ibi_sta_amount + ibi_sta_per + ibi_uti_amount + ibi_uti_per + ibi_oth_amount + ibi_oth_per;
		double cbi_total = cbi_fed_amount + cbi_sta_amount + cbi_uti_amount + cbi_oth_amount;
		double itc_fed_total = itc_fed_amount + itc_fed_per;
		double itc_sta_total = itc_sta_amount + itc_sta_per;

		double recapitalization_cost = as_double("recapitialization_cost");
		double recapitalization_escalation = as_double("recapitialization_escalation");
		if (as_integer("system_use_recapitalization"))
		{
			size_t count_recap = 0;
			ssc_number_t *recap = 0;
			recap = as_array("recapitalization_boolean", &count_recap);
			for (i=0;i<nyears && i<(int)count_recap;i++)
				cf.at(CF_recapitalization_boolean,i+1) = recap[i]; 
		}

		for (i=1; i<=nyears; i++)
		{

			// compute expenses
			cf.at(CF_om_production_expense,i) *= cf.at(CF_energy_net,i);
			cf.at(CF_om_capacity_expense,i) *= nameplate;
			cf.at(CF_om_fuel_expense,i) *= year1_fuel_use;
			double decline_percent = 100 - (i-1)*property_tax_decline_percentage;
			cf.at(CF_property_tax_assessed_value,i) = (decline_percent > 0) ? property_tax_assessed_value * decline_percent * 0.01:0.0;
			cf.at(CF_property_tax_expense,i) = cf.at(CF_property_tax_assessed_value,i) * property_tax;

			cf.at(CF_insurance_expense,i) = total_cost * insurance_rate * pow( 1 + inflation_rate, i-1 );


			cf.at(CF_deductible_expenses,i) = -cf.at(CF_operating_expenses,i);  // commercial
	
			if (i == 1)
			{
				cf.at(CF_debt_balance,i) = -loan_amount;
				cf.at(CF_debt_payment_interest,i) = loan_amount * loan_rate;
				cf.at(CF_debt_payment_principal,i) = -ppmt( loan_rate,       // Rate
																i,           // Period
																loan_term,   // Number periods
																loan_amount, // Present Value
																0,           // future Value
																0 );         // cash flow at end of period
			}
			else
			{
				if (i <= loan_term)
				{
					cf.at(CF_debt_balance,i) = cf.at(CF_debt_balance,i-1) + cf.at(CF_debt_payment_principal,i-1);
					cf.at(CF_debt_payment_interest,i) = -loan_rate * cf.at(CF_debt_balance,i);

					if (loan_rate != 0.0)
					{
						cf.at(CF_debt_payment_principal,i) = loan_rate * loan_amount/(1 - pow((1 + loan_rate),-loan_term))
							- cf.at(CF_debt_payment_interest,i);
					}
					else
					{
						cf.at(CF_debt_payment_principal,i) = loan_amount / loan_term - cf.at(CF_debt_payment_interest,i);
					}
				}
			}

			cf.at(CF_debt_payment_total,i) = cf.at(CF_debt_payment_principal,i) + cf.at(CF_debt_payment_interest,i);

			// compute pbi total
			cf.at(CF_pbi_total, i) = cf.at(CF_pbi_fed, i) + cf.at(CF_pbi_sta, i) + cf.at(CF_pbi_uti, i) + cf.at(CF_pbi_oth, i);

			// compute depreciation from basis and precalculated schedule
			cf.at(CF_sta_depreciation,i) = cf.at(CF_sta_depr_sched,i)*state_depr_basis;
			cf.at(CF_fed_depreciation,i) = cf.at(CF_fed_depr_sched,i)*federal_depr_basis;

			// recapitalization - if present
			if (as_integer("system_use_recapitalization"))
			{
				cf.at(CF_recapitalization,i) =  cf.at(CF_recapitalization_boolean,i) * 
					recapitalization_cost * pow((1 + inflation_rate + recapitalization_escalation),i-1);
			}

			// total - added lump 2/23/08
			cf.at(CF_operating_expenses,i) =
				+ cf.at(CF_om_fixed_expense,i)
				+ cf.at(CF_om_production_expense,i)
				+ cf.at(CF_om_capacity_expense,i)
				+ cf.at(CF_om_fuel_expense,i)
				+ cf.at(CF_property_tax_expense,i)
				+ cf.at(CF_insurance_expense,i)
				+ cf.at(CF_recapitalization,i);

			if (i == nyears) /* salvage value handled as negative operating expense in last year */
				cf.at(CF_operating_expenses,i) -= total_cost * salvage_frac; // updated to be consistent with DHF models - not inflated
//				cf.at(CF_operating_expenses,i) -= total_cost * salvage_frac * pow( 1+inflation_rate, i-1 );
		}

		double ppa_soln_tolerance = as_double("ppa_soln_tolerance");
		int ppa_soln_max_iteations = as_integer("ppa_soln_max_iterations");
		bool solved=true;
		double ppa_min=as_double("ppa_soln_min");
		double ppa_max=as_double("ppa_soln_max");
		int its=0;
		double weighting_factor = 1.0;
		double irr_weighting_factor = DBL_MAX;
		double dscr_weighting_factor = DBL_MAX;
		double positive_cashflow_weighting_factor = DBL_MAX;
		bool irr_is_minimally_met = false;
		bool irr_greater_than_target = false;
		double w0=1.0;
		double w1=1.0;
		double x0=ppa_min;
		double x1=ppa_max;
		double ppa_coarse_interval=10; // 10 cents/kWh
		bool ppa_interval_found=false;
		bool ppa_too_large=false;
		bool ppa_interval_reset=true;
		bool is_min_irr_satisfied=true;
		bool is_min_dscr_satisfied=true;
		bool is_positive_cashflow_satisfied=true;
		bool is_min_irr_minimally_satisfied=false;
		bool is_min_dscr_minimally_satisfied=false;
		bool is_positive_cashflow_minimally_satisfied=false;
		bool use_target_irr=true;
		double itnpv_target_irr=1;
		double itnpv_target_irr_plus_delta=1;
		double aftertax_irr=0;
		double min_dscr = DBL_MAX;
		double min_after_tax_cash_flow = DBL_MAX;

/***************** begin iterative solution *********************************************************************/

		do
		{

			if (ppa_interval_found)	ppa = (w0*x1+w1*x0)/(w0 + w1);



			for (i=1; i<=nyears; i++)
			{

				cf.at(CF_ppa_price,i) = ppa * pow( 1 + ppa_escalation, i-1 ); 

				if (is_commercialppa)
					cf.at(CF_energy_value,i) = cf.at(CF_energy_net,i) * cf.at(CF_ppa_price,i) /100.0;
				else
				// dispatch
					cf.at(CF_energy_value,i) = cf.at(CF_ppa_price,i) /100.0 * (
						cf.at(CF_TOD1Energy,i) * dispatch_factor1 +
						cf.at(CF_TOD2Energy,i) * dispatch_factor2 +
						cf.at(CF_TOD3Energy,i) * dispatch_factor3 +
						cf.at(CF_TOD4Energy,i) * dispatch_factor4 +
						cf.at(CF_TOD5Energy,i) * dispatch_factor5 +
						cf.at(CF_TOD6Energy,i) * dispatch_factor6 +
						cf.at(CF_TOD7Energy,i) * dispatch_factor7 +
						cf.at(CF_TOD8Energy,i) * dispatch_factor8 +
						cf.at(CF_TOD9Energy,i) * dispatch_factor9			);
	

			cf.at(CF_operating_income,i) = cf.at(CF_energy_value,i) - cf.at(CF_operating_expenses,i);

			// ************************************************
			// tax effect on equity (state)

			cf.at(CF_sta_incentive_income_less_deductions, i) =
				+ cf.at(CF_operating_income, i)
				+ cf.at(CF_pbi_total,i)
				- cf.at(CF_sta_depreciation,i)
				- cf.at(CF_debt_payment_interest,i);

			if (i==1) cf.at(CF_sta_incentive_income_less_deductions, i) += ibi_total + cbi_total;


			cf.at(CF_sta_taxable_income_less_deductions, i) = taxable_incentive_income( i, "sta" )
				+ cf.at(CF_operating_income,i)
				- cf.at(CF_sta_depreciation,i)
				- cf.at(CF_debt_payment_interest,i);

			cf.at(CF_sta_income_taxes,i) = state_tax_rate*cf.at(CF_sta_taxable_income_less_deductions,i);

			cf.at(CF_sta_tax_savings, i) = cf.at(CF_ptc_sta,i) - cf.at(CF_sta_income_taxes,i);
			if (i==1) cf.at(CF_sta_tax_savings, i) += itc_sta_amount + itc_sta_per;

			// ************************************************
			//	tax effect on equity (federal)

			cf.at(CF_fed_incentive_income_less_deductions, i) =
				+ cf.at(CF_operating_income, i)
				+ cf.at(CF_pbi_total,i)
				- cf.at(CF_fed_depreciation,i)
				- cf.at(CF_debt_payment_interest,i)
				+ cf.at(CF_sta_tax_savings, i);

			if (i==1) cf.at(CF_fed_incentive_income_less_deductions, i) += ibi_total + cbi_total;

			cf.at(CF_fed_taxable_income_less_deductions, i) = taxable_incentive_income( i, "fed" )
				+ cf.at(CF_operating_income,i)
				- cf.at(CF_fed_depreciation,i)
				- cf.at(CF_debt_payment_interest,i)
				+ cf.at(CF_sta_tax_savings, i);

			cf.at(CF_fed_income_taxes,i) = federal_tax_rate*cf.at(CF_fed_taxable_income_less_deductions,i);

			cf.at(CF_fed_tax_savings, i) = cf.at(CF_ptc_fed,i) - cf.at(CF_fed_income_taxes,i);
			if (i==1) cf.at(CF_fed_tax_savings, i) += itc_fed_amount + itc_fed_per;

			// ************************************************
			// combined tax savings and cost/cash flows

			cf.at(CF_sta_and_fed_tax_savings,i) = cf.at(CF_sta_tax_savings, i)+cf.at(CF_fed_tax_savings, i);

			cf.at(CF_after_tax_net_equity_cash_flow, i) =
				+ cf.at(CF_operating_income, i)
				+ cf.at(CF_sta_and_fed_tax_savings,i)
				- cf.at(CF_debt_payment_total, i)
				+ cf.at(CF_pbi_total, i);

			if (cf.at(CF_debt_payment_total,i) !=0.0)
				cf.at(CF_pretax_dscr,i) = cf.at(CF_operating_income,i) / cf.at(CF_debt_payment_total,i);
			if (i > loan_term)
				cf.at(CF_pretax_dscr,i) = 0;

			cf.at(CF_after_tax_cash_flow,i) =
				cf.at(CF_after_tax_net_equity_cash_flow, i)
				+ (1.0 - effective_tax_rate)*cf.at(CF_energy_value, i);

		}

		std::stringstream outm;

		aftertax_irr = irr( CF_after_tax_net_equity_cash_flow, nyears);
		min_dscr = min_cashflow_value( CF_pretax_dscr, nyears);
		min_after_tax_cash_flow=min_cashflow_value(CF_after_tax_net_equity_cash_flow,nyears);


		if (ppa_soln_mode == 1)
		{

			if (use_target_irr)
			{
				itnpv_target_irr = npv(CF_after_tax_net_equity_cash_flow,nyears,min_irr_target) +  cf.at(CF_after_tax_net_equity_cash_flow,0) ;
				itnpv_target_irr_plus_delta = npv(CF_after_tax_net_equity_cash_flow,nyears,min_irr_target+0.001) +  cf.at(CF_after_tax_net_equity_cash_flow,0);
				irr_weighting_factor = fabs(itnpv_target_irr);
				is_min_irr_minimally_satisfied=( irr_weighting_factor < ppa_soln_tolerance);
				is_min_irr_satisfied=((itnpv_target_irr >= 0.0) || is_min_irr_minimally_satisfied);
				if (is_min_dscr_minimally_satisfied)
				{
					if (itnpv_target_irr_plus_delta > itnpv_target_irr)
					{
						use_target_irr=false;
						is_min_irr_minimally_satisfied=false;
						is_min_irr_satisfied=false;
					}
				}

			}
			else
			{
				itnpv_target_irr = npv(CF_after_tax_net_equity_cash_flow,nyears,aftertax_irr) +  cf.at(CF_after_tax_net_equity_cash_flow,0) ;
				itnpv_target_irr_plus_delta = npv(CF_after_tax_net_equity_cash_flow,nyears,aftertax_irr+0.001) +  cf.at(CF_after_tax_net_equity_cash_flow,0) ;
				irr_weighting_factor = DBL_MAX;
				is_min_irr_minimally_satisfied = (fabs(itnpv_target_irr) <= ppa_soln_tolerance) && (itnpv_target_irr>itnpv_target_irr_plus_delta) && (aftertax_irr >= min_irr_target);
				is_min_irr_satisfied = (((itnpv_target_irr<=itnpv_target_irr_plus_delta) && (aftertax_irr >= 0)) || is_min_irr_minimally_satisfied);
			}

			if (min_dscr_required == 1)
			{
				dscr_weighting_factor = fabs( min_dscr - min_dscr_target );
				is_min_dscr_minimally_satisfied=( dscr_weighting_factor < ppa_soln_tolerance);
				is_min_dscr_satisfied=((min_dscr >= min_dscr_target) || is_min_dscr_minimally_satisfied);
				if (fabs(min_dscr)>ppa_soln_tolerance) dscr_weighting_factor /= fabs(min_dscr);
			}
			if (positive_cashflow_required == 1)
			{
				is_positive_cashflow_satisfied=(min_after_tax_cash_flow>=0.0);
//				is_positive_cashflow_minimally_satisfied= ((is_positive_cashflow_satisfied) && ( min_after_tax_cash_flow  < 100.0)); // somewhat arbitrary - consistent with finutility
				is_positive_cashflow_minimally_satisfied= ((is_positive_cashflow_satisfied) && ( fabs(min_after_tax_cash_flow)  < ppa_soln_tolerance)); // somewhat arbitrary - consistent with finutility
//				positive_cashflow_weighting_factor = fabs(min_after_tax_cash_flow);
				positive_cashflow_weighting_factor = 1.0; // switch to binary search
			}
			bool are_all_constraints_satisfied = (is_min_dscr_satisfied && is_min_irr_satisfied && is_positive_cashflow_satisfied);
			bool is_one_constraint_minimally_satisfied = (is_min_dscr_minimally_satisfied || is_min_irr_minimally_satisfied || is_positive_cashflow_minimally_satisfied);

			solved = ( ( (are_all_constraints_satisfied) && (is_one_constraint_minimally_satisfied) ) || ( fabs(x0-x1) < ppa_soln_tolerance) );




			if (!solved)
			{

				weighting_factor = min(positive_cashflow_weighting_factor, dscr_weighting_factor);
				weighting_factor = min(weighting_factor, irr_weighting_factor);
				if (ppa_interval_found)
				{// reset interval
					//if (irr_greater_than_target) // too large
					if (are_all_constraints_satisfied) // too large
					{
			// set endpoint of weighted interval x0<x1
						x1 = ppa;
						w1 = weighting_factor;
					}
					else // too small
					{
			// set endpoint of weighted interval x0<x1
						x0 = ppa;
						w0 = weighting_factor;
					}

				}
				else
				{ // find solution interval [x0,x1]
					if (ppa_interval_reset) 
					{
							if (irr_greater_than_target) ppa_too_large=true;
							ppa_interval_reset=false;
					}
					if (ppa_too_large) // too large
					{
						if (are_all_constraints_satisfied) // too large
						{
							x0 = ppa;
							w0 = weighting_factor;
							ppa = x0-ppa_coarse_interval;
						}
						else
						{
						  x1 = x0;
						  w1 = w0;
						  x0 = ppa;
						  w0 = weighting_factor;
						  ppa_interval_found=true;
						}
					}
					else
					{
						if (!are_all_constraints_satisfied)
						{
							x1 = ppa;
							w1 = weighting_factor;
							ppa = x1+ppa_coarse_interval;
						}
						else
						{
						  x0 = x1;
						  w0 = w1;
						  x1 = ppa;
						  w1 = weighting_factor;
						  ppa_interval_found=true;
						}
					}
					// for initial guess of zero
					if (fabs(x0-x1)<ppa_soln_tolerance) x0 = x1-2*ppa_soln_tolerance;
				}
					//std::stringstream outm;
					//outm << "iteration=" << its  << ", irr=" << cf.at(CF_project_return_aftertax_irr, flip_target_year)  << ", npvtarget=" << itnpv_target  << ", npvtarget_delta=" << itnpv_target_delta  
					//	//  << ", npvactual=" << itnpv_actual  << ", npvactual_delta=" << itnpv_target_delta  
					//	<< ", residual=" << residual << ", ppa=" << ppa << ", x0=" << x0 << ", x1=" << x1 <<  ",w0=" << w0 << ", w1=" << w1 << ", ppamax-ppamin=" << x1-x0;
					//log( outm.str() );
			}
/*					outm << "iteration=" << its  << ", irr=" << aftertax_irr  << ", npvtarget=" << itnpv_target_irr  << ", npvtarget_delta=" << itnpv_target_irr_plus_delta  
						<< ", min cash flow=" << min_after_tax_cash_flow  << ", is_positive_cashflow_satisfied=" << is_positive_cashflow_satisfied
						<< ", min dscr=" << min_dscr  << ", is_min_dscr_satisfied=" << is_min_dscr_satisfied
						<< "\ntarget_irr=" << min_irr_target  << ", is_min_irr_satisfied=" << is_min_irr_satisfied
						<< "\nare_all_constraints_satisfied=" << are_all_constraints_satisfied  << ", is_one_constraint_minimally_satisfied=" << is_one_constraint_minimally_satisfied
						<< ", solved=" << solved  << ", weighting_factor=" << weighting_factor  
						<< ", ppa=" << ppa << ", x0=" << x0 << ", x1=" << x1 <<  ",w0=" << w0 << ", w1=" << w1 << ", ppamax-ppamin=" << x1-x0 << "\n\n";
					log( outm.str() );
*/			
		}
		its++;

	}	// target tax investor return in target year
	while (!solved && (its < ppa_soln_max_iteations) && (ppa >= 0) );

/***************** end iterative solution *********************************************************************/


		// save outputs

		assign( "cf_length", var_data( (ssc_number_t) nyears+1 ));
		if (ppa_soln_mode==1) aftertax_irr = irr( CF_after_tax_net_equity_cash_flow, nyears, min_irr_target);

/*
					std::stringstream outm;
					outm << "iteration=" << its  << ", irr=" << aftertax_irr  << ", use_target_irr=" << use_target_irr
						//  << ", npvactual=" << itnpv_actual  << ", npvactual_delta=" << itnpv_target_delta  
						<< ", ppa=" << ppa << ", x0=" << x0 << ", x1=" << x1 <<  ",w0=" << w0 << ", w1=" << w1 << ", ppamax-ppamin=" << x1-x0;
					log( outm.str() );
*/
		double x = npv( CF_energy_net, nyears, real_discount_rate );
		if (x == 0.0) throw general_error("lcoe real failed because energy npv is zero");
		double lcoe_real = npv(CF_energy_value, nyears, nom_discount_rate)  * 100 / x;

		x = npv( CF_energy_net, nyears, nom_discount_rate );
		if (x == 0.0) throw general_error("lcoe nom failed because energy npv is zero");
		double lcoe_nom = npv(CF_energy_value, nyears, nom_discount_rate)  * 100 / x;

		double net_present_value = cf.at(CF_after_tax_net_equity_cash_flow,0) + npv(CF_after_tax_net_equity_cash_flow, nyears, nom_discount_rate );

		assign( "lcoe_real", var_data((ssc_number_t)lcoe_real) );
		assign( "lcoe_nom", var_data((ssc_number_t)lcoe_nom) );
		assign( "npv",  var_data((ssc_number_t)net_present_value) );
		assign( "ppa",  var_data((ssc_number_t)ppa) );

		assign( "min_cashflow",  var_data((ssc_number_t)min_after_tax_cash_flow) );
		assign( "irr",  var_data((ssc_number_t)(aftertax_irr*100.0)) );
		assign( "min_dscr",  var_data((ssc_number_t)min_dscr) );

		assign( "depr_basis_fed", var_data((ssc_number_t)federal_depr_basis ));
		assign( "depr_basis_sta", var_data((ssc_number_t)state_depr_basis ));
		assign( "discount_nominal", var_data((ssc_number_t)(nom_discount_rate*100.0) ));
//		assign( "sales_tax_deduction", var_data((ssc_number_t)total_sales_tax ));
		assign( "adj_installed_cost", var_data((ssc_number_t)adjusted_installed_cost ));

		
		save_cf( CF_pretax_dscr, nyears, "cf_pretax_dscr" );
		save_cf( CF_energy_net, nyears, "cf_energy_net" );
		save_cf( CF_energy_value, nyears, "cf_energy_value" );
		save_cf( CF_om_fixed_expense, nyears, "cf_om_fixed_expense" );
		save_cf( CF_om_production_expense, nyears, "cf_om_production_expense" );
		save_cf( CF_om_capacity_expense, nyears, "cf_om_capacity_expense" );
		save_cf( CF_om_fuel_expense, nyears, "cf_om_fuel_expense" );
		save_cf( CF_property_tax_assessed_value, nyears, "cf_property_tax_assessed_value" );
		save_cf( CF_property_tax_expense, nyears, "cf_property_tax_expense" );
		save_cf( CF_insurance_expense, nyears, "cf_insurance_expense" );
		save_cf( CF_operating_expenses, nyears, "cf_operating_expenses" );
		save_cf( CF_operating_income, nyears, "cf_operating_income" );

		save_cf( CF_deductible_expenses, nyears, "cf_deductible_expenses");

		save_cf( CF_debt_balance, nyears, "cf_debt_balance" );
		save_cf( CF_debt_payment_interest, nyears, "cf_debt_payment_interest" );
		save_cf( CF_debt_payment_principal, nyears, "cf_debt_payment_principal" );
		save_cf( CF_debt_payment_total, nyears, "cf_debt_payment_total" );

		assign( "ibi_total", var_data((ssc_number_t) ibi_total));
		assign( "cbi_total", var_data((ssc_number_t) cbi_total));
		save_cf( CF_pbi_total, nyears, "cf_pbi_total" );

		save_cf( CF_ptc_fed, nyears, "cf_ptc_fed" );
		save_cf( CF_ptc_sta, nyears, "cf_ptc_sta" );

		assign( "itc_fed_total", var_data((ssc_number_t) itc_fed_total));
		assign( "itc_sta_total", var_data((ssc_number_t) itc_sta_total));

		save_cf( CF_sta_depr_sched, nyears, "cf_sta_depr_sched" );
		save_cf( CF_sta_depreciation, nyears, "cf_sta_depreciation" );
		save_cf( CF_sta_incentive_income_less_deductions, nyears, "cf_sta_incentive_income_less_deductions" );
		save_cf( CF_sta_taxable_income_less_deductions, nyears, "cf_sta_taxable_income_less_deductions" );
		save_cf( CF_sta_tax_savings, nyears, "cf_sta_tax_savings" );

		save_cf( CF_fed_depr_sched, nyears, "cf_fed_depr_sched" );
		save_cf( CF_fed_depreciation, nyears, "cf_fed_depreciation" );
		save_cf( CF_fed_incentive_income_less_deductions, nyears, "cf_fed_incentive_income_less_deductions" );
		save_cf( CF_fed_taxable_income_less_deductions, nyears, "cf_fed_taxable_income_less_deductions" );
		save_cf( CF_fed_tax_savings, nyears, "cf_fed_tax_savings" );

		save_cf( CF_sta_and_fed_tax_savings, nyears, "cf_sta_and_fed_tax_savings" );
		save_cf( CF_after_tax_net_equity_cash_flow, nyears, "cf_after_tax_net_equity_cash_flow" );
		save_cf( CF_after_tax_cash_flow, nyears, "cf_after_tax_cash_flow" );

		save_cf( CF_ppa_price,nyears,"cf_ppa_price");

		// dispatch
		if (as_integer("system_use_lifetime_output"))
			process_lifetime_dispatch_output(nyears);
		else
			process_dispatch_output(nyears);


		// dispatch energy
		save_cf( CF_TODJanEnergy, nyears, "cf_energy_net_jan");
		save_cf( CF_TODFebEnergy, nyears, "cf_energy_net_feb");
		save_cf( CF_TODMarEnergy, nyears, "cf_energy_net_mar");
		save_cf( CF_TODAprEnergy, nyears, "cf_energy_net_apr");
		save_cf( CF_TODMayEnergy, nyears, "cf_energy_net_may");
		save_cf( CF_TODJunEnergy, nyears, "cf_energy_net_jun");
		save_cf( CF_TODJulEnergy, nyears, "cf_energy_net_jul");
		save_cf( CF_TODAugEnergy, nyears, "cf_energy_net_aug");
		save_cf( CF_TODSepEnergy, nyears, "cf_energy_net_sep");
		save_cf( CF_TODOctEnergy, nyears, "cf_energy_net_oct");
		save_cf( CF_TODNovEnergy, nyears, "cf_energy_net_nov");
		save_cf( CF_TODDecEnergy, nyears, "cf_energy_net_dec");

		save_cf( CF_TOD1Energy, nyears, "cf_energy_net_dispatch1");
		save_cf( CF_TOD2Energy, nyears, "cf_energy_net_dispatch2");
		save_cf( CF_TOD3Energy, nyears, "cf_energy_net_dispatch3");
		save_cf( CF_TOD4Energy, nyears, "cf_energy_net_dispatch4");
		save_cf( CF_TOD5Energy, nyears, "cf_energy_net_dispatch5");
		save_cf( CF_TOD6Energy, nyears, "cf_energy_net_dispatch6");
		save_cf( CF_TOD7Energy, nyears, "cf_energy_net_dispatch7");
		save_cf( CF_TOD8Energy, nyears, "cf_energy_net_dispatch8");
		save_cf( CF_TOD9Energy, nyears, "cf_energy_net_dispatch9");

		// dispatch revenue
		for (i=1;i<=nyears;i++)
		{
			cf.at(CF_TOD1Revenue,i) = cf.at(CF_ppa_price,i)/100.0 * dispatch_factor1 * cf.at(CF_TOD1Energy,i);
			cf.at(CF_TOD2Revenue,i) = cf.at(CF_ppa_price,i)/100.0 * dispatch_factor2 * cf.at(CF_TOD2Energy,i);
			cf.at(CF_TOD3Revenue,i) = cf.at(CF_ppa_price,i)/100.0 * dispatch_factor3 * cf.at(CF_TOD3Energy,i);
			cf.at(CF_TOD4Revenue,i) = cf.at(CF_ppa_price,i)/100.0 * dispatch_factor4 * cf.at(CF_TOD4Energy,i);
			cf.at(CF_TOD5Revenue,i) = cf.at(CF_ppa_price,i)/100.0 * dispatch_factor5 * cf.at(CF_TOD5Energy,i);
			cf.at(CF_TOD6Revenue,i) = cf.at(CF_ppa_price,i)/100.0 * dispatch_factor6 * cf.at(CF_TOD6Energy,i);
			cf.at(CF_TOD7Revenue,i) = cf.at(CF_ppa_price,i)/100.0 * dispatch_factor7 * cf.at(CF_TOD7Energy,i);
			cf.at(CF_TOD8Revenue,i) = cf.at(CF_ppa_price,i)/100.0 * dispatch_factor8 * cf.at(CF_TOD8Energy,i);
			cf.at(CF_TOD9Revenue,i) = cf.at(CF_ppa_price,i)/100.0 * dispatch_factor9 * cf.at(CF_TOD9Energy,i);
		}

		save_cf( CF_TOD1Revenue, nyears, "cf_revenue_dispatch1");
		save_cf( CF_TOD2Revenue, nyears, "cf_revenue_dispatch2");
		save_cf( CF_TOD3Revenue, nyears, "cf_revenue_dispatch3");
		save_cf( CF_TOD4Revenue, nyears, "cf_revenue_dispatch4");
		save_cf( CF_TOD5Revenue, nyears, "cf_revenue_dispatch5");
		save_cf( CF_TOD6Revenue, nyears, "cf_revenue_dispatch6");
		save_cf( CF_TOD7Revenue, nyears, "cf_revenue_dispatch7");
		save_cf( CF_TOD8Revenue, nyears, "cf_revenue_dispatch8");
		save_cf( CF_TOD9Revenue, nyears, "cf_revenue_dispatch9");

		for (i=1;i<=nyears;i++)
		{
			cf.at(CF_TODJanRevenue,i) = cf.at(CF_ppa_price,i)/100.0 * (
				dispatch_factor1 * cf.at(CF_TOD1JanEnergy,i) +
				dispatch_factor2 * cf.at(CF_TOD2JanEnergy,i) +
				dispatch_factor3 * cf.at(CF_TOD3JanEnergy,i) +
				dispatch_factor4 * cf.at(CF_TOD4JanEnergy,i) +
				dispatch_factor5 * cf.at(CF_TOD5JanEnergy,i) +
				dispatch_factor6 * cf.at(CF_TOD6JanEnergy,i) +
				dispatch_factor7 * cf.at(CF_TOD7JanEnergy,i) +
				dispatch_factor8 * cf.at(CF_TOD8JanEnergy,i) +
				dispatch_factor9 * cf.at(CF_TOD9JanEnergy,i) 			);
		}
		save_cf( CF_TODJanRevenue, nyears, "cf_revenue_jan");
		
		for (i=1;i<=nyears;i++)
		{
			cf.at(CF_TODFebRevenue,i) = cf.at(CF_ppa_price,i)/100.0 * (
				dispatch_factor1 * cf.at(CF_TOD1FebEnergy,i) +
				dispatch_factor2 * cf.at(CF_TOD2FebEnergy,i) +
				dispatch_factor3 * cf.at(CF_TOD3FebEnergy,i) +
				dispatch_factor4 * cf.at(CF_TOD4FebEnergy,i) +
				dispatch_factor5 * cf.at(CF_TOD5FebEnergy,i) +
				dispatch_factor6 * cf.at(CF_TOD6FebEnergy,i) +
				dispatch_factor7 * cf.at(CF_TOD7FebEnergy,i) +
				dispatch_factor8 * cf.at(CF_TOD8FebEnergy,i) +
				dispatch_factor9 * cf.at(CF_TOD9FebEnergy,i) 			);
		}
		save_cf( CF_TODFebRevenue, nyears, "cf_revenue_feb");

		for (i=1;i<=nyears;i++)
		{
			cf.at(CF_TODMarRevenue,i) = cf.at(CF_ppa_price,i)/100.0 * (
				dispatch_factor1 * cf.at(CF_TOD1MarEnergy,i) +
				dispatch_factor2 * cf.at(CF_TOD2MarEnergy,i) +
				dispatch_factor3 * cf.at(CF_TOD3MarEnergy,i) +
				dispatch_factor4 * cf.at(CF_TOD4MarEnergy,i) +
				dispatch_factor5 * cf.at(CF_TOD5MarEnergy,i) +
				dispatch_factor6 * cf.at(CF_TOD6MarEnergy,i) +
				dispatch_factor7 * cf.at(CF_TOD7MarEnergy,i) +
				dispatch_factor8 * cf.at(CF_TOD8MarEnergy,i) +
				dispatch_factor9 * cf.at(CF_TOD9MarEnergy,i) 			);
		}
		save_cf( CF_TODMarRevenue, nyears, "cf_revenue_mar");

		for (i=1;i<=nyears;i++)
		{
			cf.at(CF_TODAprRevenue,i) = cf.at(CF_ppa_price,i)/100.0 * (
				dispatch_factor1 * cf.at(CF_TOD1AprEnergy,i) +
				dispatch_factor2 * cf.at(CF_TOD2AprEnergy,i) +
				dispatch_factor3 * cf.at(CF_TOD3AprEnergy,i) +
				dispatch_factor4 * cf.at(CF_TOD4AprEnergy,i) +
				dispatch_factor5 * cf.at(CF_TOD5AprEnergy,i) +
				dispatch_factor6 * cf.at(CF_TOD6AprEnergy,i) +
				dispatch_factor7 * cf.at(CF_TOD7AprEnergy,i) +
				dispatch_factor8 * cf.at(CF_TOD8AprEnergy,i) +
				dispatch_factor9 * cf.at(CF_TOD9AprEnergy,i) 			);
		}
		save_cf( CF_TODAprRevenue, nyears, "cf_revenue_apr");

		for (i=1;i<=nyears;i++)
		{
			cf.at(CF_TODMayRevenue,i) = cf.at(CF_ppa_price,i)/100.0 * (
				dispatch_factor1 * cf.at(CF_TOD1MayEnergy,i) +
				dispatch_factor2 * cf.at(CF_TOD2MayEnergy,i) +
				dispatch_factor3 * cf.at(CF_TOD3MayEnergy,i) +
				dispatch_factor4 * cf.at(CF_TOD4MayEnergy,i) +
				dispatch_factor5 * cf.at(CF_TOD5MayEnergy,i) +
				dispatch_factor6 * cf.at(CF_TOD6MayEnergy,i) +
				dispatch_factor7 * cf.at(CF_TOD7MayEnergy,i) +
				dispatch_factor8 * cf.at(CF_TOD8MayEnergy,i) +
				dispatch_factor9 * cf.at(CF_TOD9MayEnergy,i) 			);
		}
		save_cf( CF_TODMayRevenue, nyears, "cf_revenue_may");

		for (i=1;i<=nyears;i++)
		{
			cf.at(CF_TODJunRevenue,i) = cf.at(CF_ppa_price,i)/100.0 * (
				dispatch_factor1 * cf.at(CF_TOD1JunEnergy,i) +
				dispatch_factor2 * cf.at(CF_TOD2JunEnergy,i) +
				dispatch_factor3 * cf.at(CF_TOD3JunEnergy,i) +
				dispatch_factor4 * cf.at(CF_TOD4JunEnergy,i) +
				dispatch_factor5 * cf.at(CF_TOD5JunEnergy,i) +
				dispatch_factor6 * cf.at(CF_TOD6JunEnergy,i) +
				dispatch_factor7 * cf.at(CF_TOD7JunEnergy,i) +
				dispatch_factor8 * cf.at(CF_TOD8JunEnergy,i) +
				dispatch_factor9 * cf.at(CF_TOD9JunEnergy,i) 			);
		}
		save_cf( CF_TODJunRevenue, nyears, "cf_revenue_jun");

		for (i=1;i<=nyears;i++)
		{
			cf.at(CF_TODJulRevenue,i) = cf.at(CF_ppa_price,i)/100.0 * (
				dispatch_factor1 * cf.at(CF_TOD1JulEnergy,i) +
				dispatch_factor2 * cf.at(CF_TOD2JulEnergy,i) +
				dispatch_factor3 * cf.at(CF_TOD3JulEnergy,i) +
				dispatch_factor4 * cf.at(CF_TOD4JulEnergy,i) +
				dispatch_factor5 * cf.at(CF_TOD5JulEnergy,i) +
				dispatch_factor6 * cf.at(CF_TOD6JulEnergy,i) +
				dispatch_factor7 * cf.at(CF_TOD7JulEnergy,i) +
				dispatch_factor8 * cf.at(CF_TOD8JulEnergy,i) +
				dispatch_factor9 * cf.at(CF_TOD9JulEnergy,i) 			);
		}
		save_cf( CF_TODJulRevenue, nyears, "cf_revenue_jul");

		for (i=1;i<=nyears;i++)
		{
			cf.at(CF_TODAugRevenue,i) = cf.at(CF_ppa_price,i)/100.0 * (
				dispatch_factor1 * cf.at(CF_TOD1AugEnergy,i) +
				dispatch_factor2 * cf.at(CF_TOD2AugEnergy,i) +
				dispatch_factor3 * cf.at(CF_TOD3AugEnergy,i) +
				dispatch_factor4 * cf.at(CF_TOD4AugEnergy,i) +
				dispatch_factor5 * cf.at(CF_TOD5AugEnergy,i) +
				dispatch_factor6 * cf.at(CF_TOD6AugEnergy,i) +
				dispatch_factor7 * cf.at(CF_TOD7AugEnergy,i) +
				dispatch_factor8 * cf.at(CF_TOD8AugEnergy,i) +
				dispatch_factor9 * cf.at(CF_TOD9AugEnergy,i) 			);
		}
		save_cf( CF_TODAugRevenue, nyears, "cf_revenue_aug");

		for (i=1;i<=nyears;i++)
		{
			cf.at(CF_TODSepRevenue,i) = cf.at(CF_ppa_price,i)/100.0 * (
				dispatch_factor1 * cf.at(CF_TOD1SepEnergy,i) +
				dispatch_factor2 * cf.at(CF_TOD2SepEnergy,i) +
				dispatch_factor3 * cf.at(CF_TOD3SepEnergy,i) +
				dispatch_factor4 * cf.at(CF_TOD4SepEnergy,i) +
				dispatch_factor5 * cf.at(CF_TOD5SepEnergy,i) +
				dispatch_factor6 * cf.at(CF_TOD6SepEnergy,i) +
				dispatch_factor7 * cf.at(CF_TOD7SepEnergy,i) +
				dispatch_factor8 * cf.at(CF_TOD8SepEnergy,i) +
				dispatch_factor9 * cf.at(CF_TOD9SepEnergy,i) 			);
		}
		save_cf( CF_TODSepRevenue, nyears, "cf_revenue_sep");

		for (i=1;i<=nyears;i++)
		{
			cf.at(CF_TODOctRevenue,i) = cf.at(CF_ppa_price,i)/100.0 * (
				dispatch_factor1 * cf.at(CF_TOD1OctEnergy,i) +
				dispatch_factor2 * cf.at(CF_TOD2OctEnergy,i) +
				dispatch_factor3 * cf.at(CF_TOD3OctEnergy,i) +
				dispatch_factor4 * cf.at(CF_TOD4OctEnergy,i) +
				dispatch_factor5 * cf.at(CF_TOD5OctEnergy,i) +
				dispatch_factor6 * cf.at(CF_TOD6OctEnergy,i) +
				dispatch_factor7 * cf.at(CF_TOD7OctEnergy,i) +
				dispatch_factor8 * cf.at(CF_TOD8OctEnergy,i) +
				dispatch_factor9 * cf.at(CF_TOD9OctEnergy,i) 			);
		}
		save_cf( CF_TODOctRevenue, nyears, "cf_revenue_oct");

		for (i=1;i<=nyears;i++)
		{
			cf.at(CF_TODNovRevenue,i) = cf.at(CF_ppa_price,i)/100.0 * (
				dispatch_factor1 * cf.at(CF_TOD1NovEnergy,i) +
				dispatch_factor2 * cf.at(CF_TOD2NovEnergy,i) +
				dispatch_factor3 * cf.at(CF_TOD3NovEnergy,i) +
				dispatch_factor4 * cf.at(CF_TOD4NovEnergy,i) +
				dispatch_factor5 * cf.at(CF_TOD5NovEnergy,i) +
				dispatch_factor6 * cf.at(CF_TOD6NovEnergy,i) +
				dispatch_factor7 * cf.at(CF_TOD7NovEnergy,i) +
				dispatch_factor8 * cf.at(CF_TOD8NovEnergy,i) +
				dispatch_factor9 * cf.at(CF_TOD9NovEnergy,i) 			);
		}
		save_cf( CF_TODNovRevenue, nyears, "cf_revenue_nov");

		for (i=1;i<=nyears;i++)
		{
			cf.at(CF_TODDecRevenue,i) = cf.at(CF_ppa_price,i)/100.0 * (
				dispatch_factor1 * cf.at(CF_TOD1DecEnergy,i) +
				dispatch_factor2 * cf.at(CF_TOD2DecEnergy,i) +
				dispatch_factor3 * cf.at(CF_TOD3DecEnergy,i) +
				dispatch_factor4 * cf.at(CF_TOD4DecEnergy,i) +
				dispatch_factor5 * cf.at(CF_TOD5DecEnergy,i) +
				dispatch_factor6 * cf.at(CF_TOD6DecEnergy,i) +
				dispatch_factor7 * cf.at(CF_TOD7DecEnergy,i) +
				dispatch_factor8 * cf.at(CF_TOD8DecEnergy,i) +
				dispatch_factor9 * cf.at(CF_TOD9DecEnergy,i) 			);
		}
		save_cf( CF_TODDecRevenue, nyears, "cf_revenue_Dec");



	}

/* These functions can be placed in common financial library with matrix and constants passed? */

	void save_cf(int cf_line, int nyears, const std::string &name)
	{
		ssc_number_t *arrp = allocate( name, nyears+1 );
		for (int i=0;i<=nyears;i++)
			arrp[i] = (ssc_number_t)cf.at(cf_line, i);
	}

	bool is_positive_cashflow(int cf_line, int nyears)
	{
		bool positive = true;
		for (int i=1;i<=nyears;i++)
			if (cf.at(cf_line, i)<0.0)
			{
				positive=false;
				break;
			}
		return positive;
	}

	double min_cashflow_value(int cf_line, int nyears)
	{
		double min_value = DBL_MAX;
		for (int i=1;i<=nyears;i++)
			if ((cf.at(cf_line, i)<min_value) && (cf.at(cf_line, i)!=0)) min_value = cf.at(cf_line, i);
		return min_value;
	}

	double npv( int cf_line, int nyears, double rate ) throw ( general_error )
	{
		if (rate <= -1.0) throw general_error("cannot calculate NPV with discount rate less or equal to -1.0");

		double rr = 1/(1+rate);
		double result = 0;
		for (int i=nyears;i>0;i--)
			result = rr * result + cf.at(cf_line,i);

		return result*rr;
	}

/* ported from http://code.google.com/p/irr-newtonraphson-calculator/ */
	bool is_valid_iter_bound(double estimated_return_rate)
	{
		return estimated_return_rate != -1 && (estimated_return_rate < INT_MAX) && (estimated_return_rate > INT_MIN);
	}

	double irr_poly_sum(double estimated_return_rate, int cf_line, int count)
	{
		double sum_of_polynomial = 0;
		if (is_valid_iter_bound(estimated_return_rate))
		{
			for (int j = 0; j <= count ; j++)
			{
				double val = (pow((1 + estimated_return_rate), j));
				if (val != 0.0)
					sum_of_polynomial += cf.at(cf_line,j)/val;
				else
					break;
			}
		}
		return sum_of_polynomial;
	}

	double irr_derivative_sum(double estimated_return_rate,int cf_line, int count)
	{
		double sum_of_derivative = 0;
		if (is_valid_iter_bound(estimated_return_rate))
			for (int i = 1; i <= count ; i++)
			{
				sum_of_derivative += cf.at(cf_line,i)*(i)/pow((1 + estimated_return_rate), i+1);
			}
		return sum_of_derivative*-1;
	}

	double irr_scale_factor( int cf_unscaled, int count)
	{
		// scale to max value for better irr convergence
		if (count<1) return 1.0;
		int i=0;
		double max=fabs(cf.at(cf_unscaled,0));
		for (i=0;i<=count;i++) 
			if (fabs(cf.at(cf_unscaled,i))> max) max =fabs(cf.at(cf_unscaled,i));
		return (max>0 ? max:1);
	}

	bool is_valid_irr( int cf_line, int count, double residual, double tolerance, int number_of_iterations, int max_iterations, double calculated_irr, double scale_factor )
	{
		double npv_of_irr = npv(cf_line,count,calculated_irr)+cf.at(cf_line,0);
		double npv_of_irr_plus_delta = npv(cf_line,count,calculated_irr+0.001)+cf.at(cf_line,0);
		bool is_valid = ( (number_of_iterations<max_iterations) && (fabs(residual)<tolerance) && (npv_of_irr>npv_of_irr_plus_delta) && (fabs(npv_of_irr/scale_factor)<tolerance) );
				//if (!is_valid)
				//{
				//std::stringstream outm;
				//outm <<  "cf_line=" << cf_line << "count=" << count << "residual=" << residual << "number_of_iterations=" << number_of_iterations << "calculated_irr=" << calculated_irr
				//	<< "npv of irr=" << npv_of_irr << "npv of irr plus delta=" << npv_of_irr_plus_delta;
				//log( outm.str() );
				//}
		return is_valid;
	}

	double irr( int cf_line, int count, double initial_guess=-2, double tolerance=1e-6, int max_iterations=100 )
	{
		int number_of_iterations=0;
		double calculated_irr=0;


		if (count < 1)
			return calculated_irr;

		// only possible for first value negative
		if ( (cf.at(cf_line,0) <= 0))
		{
			// initial guess from http://zainco.blogspot.com/2008/08/internal-rate-of-return-using-newton.html
			if ((initial_guess < -1) && (count > 1))// second order
			{
				if (cf.at(cf_line,0) !=0) 
				{
					double b = 2.0+ cf.at(cf_line,1)/cf.at(cf_line,0);
					double c = 1.0+cf.at(cf_line,1)/cf.at(cf_line,0)+cf.at(cf_line,2)/cf.at(cf_line,0);
					initial_guess = -0.5*b - 0.5*sqrt(b*b-4.0*c);
					if ((initial_guess <= 0) || (initial_guess >= 1)) initial_guess = -0.5*b + 0.5*sqrt(b*b-4.0*c);
				}
			}
			else if (initial_guess < 0) // first order
			{
				if (cf.at(cf_line,0) !=0) initial_guess = -(1.0 + cf.at(cf_line,1)/cf.at(cf_line,0));
			}

			double scale_factor = irr_scale_factor(cf_line,count);
			double residual=DBL_MAX;

			calculated_irr = irr_calc(cf_line,count,initial_guess,tolerance,max_iterations,scale_factor,number_of_iterations,residual);

			if (!is_valid_irr(cf_line,count,residual,tolerance,number_of_iterations,max_iterations,calculated_irr,scale_factor)) // try 0.1 as initial guess
			{
				initial_guess=0.1;
				number_of_iterations=0;
				residual=0;
				calculated_irr = irr_calc(cf_line,count,initial_guess,tolerance,max_iterations,scale_factor,number_of_iterations,residual);
			}

			if (!is_valid_irr(cf_line,count,residual,tolerance,number_of_iterations,max_iterations,calculated_irr,scale_factor)) // try -0.1 as initial guess
			{
				initial_guess=-0.1;
				number_of_iterations=0;
				residual=0;
				calculated_irr = irr_calc(cf_line,count,initial_guess,tolerance,max_iterations,scale_factor,number_of_iterations,residual);
			}
			if (!is_valid_irr(cf_line,count,residual,tolerance,number_of_iterations,max_iterations,calculated_irr,scale_factor)) // try 0 as initial guess
			{
				initial_guess=0;
				number_of_iterations=0;
				residual=0;
				calculated_irr = irr_calc(cf_line,count,initial_guess,tolerance,max_iterations,scale_factor,number_of_iterations,residual);
			}

			if (!is_valid_irr(cf_line,count,residual,tolerance,number_of_iterations,max_iterations,calculated_irr,scale_factor)) // try 0.1 as initial guess
			{
				calculated_irr = 0.0; // did not converge
			}

		}
		return calculated_irr;
	}


	double irr_calc( int cf_line, int count, double initial_guess, double tolerance, int max_iterations, double scale_factor, int &number_of_iterations, double &residual )
	{
		double calculated_irr=0;
		double deriv_sum = irr_derivative_sum(initial_guess,cf_line,count);
		if (deriv_sum != 0.0)
			calculated_irr = initial_guess - irr_poly_sum(initial_guess,cf_line,count)/deriv_sum;
		else
			return initial_guess;

		number_of_iterations++;


		residual = irr_poly_sum(calculated_irr,cf_line,count) / scale_factor;

		while (!(fabs(residual) <= tolerance) && (number_of_iterations < max_iterations))
		{
			deriv_sum = irr_derivative_sum(initial_guess,cf_line,count);
			if (deriv_sum != 0.0)
				calculated_irr = calculated_irr - irr_poly_sum(calculated_irr,cf_line,count)/deriv_sum;
			else
				break;

			number_of_iterations++;
			residual = irr_poly_sum(calculated_irr,cf_line,count) / scale_factor;
		}
		return calculated_irr;
	}


	void compute_production_incentive( int cf_line, int nyears, const std::string &s_val, const std::string &s_term, const std::string &s_escal )
	{
		size_t len = 0;
		ssc_number_t *parr = as_array(s_val, &len);
		int term = as_integer(s_term);
		double escal = as_double(s_escal)/100.0;

		if (len == 1)
		{
			for (int i=1;i<=nyears;i++)
				cf.at(cf_line, i) = (i <= term) ? parr[0] * cf.at(CF_energy_net,i) * pow(1 + escal, i-1) : 0.0;
		}
		else
		{
			for (int i=1;i<=nyears && i <= (int)len;i++)
				cf.at(cf_line, i) = parr[i-1]*cf.at(CF_energy_net,i);
		}
	}

		void compute_production_incentive_IRS_2010_37( int cf_line, int nyears, const std::string &s_val, const std::string &s_term, const std::string &s_escal )
	{
		// rounding based on IRS document and emails from John and Matt from DHF Financials 2/24/2011 and DHF model v4.4
		size_t len = 0;
		ssc_number_t *parr = as_array(s_val, &len);
		int term = as_integer(s_term);
		double escal = as_double(s_escal)/100.0;

		if (len == 1)
		{
			for (int i=1;i<=nyears;i++)
				cf.at(cf_line, i) = (i <= term) ? cf.at(CF_energy_net,i) / 1000.0 * round_dhf(1000.0 * parr[0] * pow(1 + escal, i-1)) : 0.0;
		}
		else
		{
			for (int i=1;i<=nyears && i <= (int)len;i++)
				cf.at(cf_line, i) = parr[i-1]*cf.at(CF_energy_net,i);
		}
	}


	void single_or_schedule( int cf_line, int nyears, double scale, const std::string &name )
	{
		size_t len = 0;
		ssc_number_t *p = as_array(name, &len);
		for (int i=1;i<=(int)len && i <= nyears;i++)
			cf.at(cf_line, i) = scale*p[i-1];
	}

	void single_or_schedule_check_max( int cf_line, int nyears, double scale, const std::string &name, const std::string &maxvar )
	{
		double max = as_double(maxvar);
		size_t len = 0;
		ssc_number_t *p = as_array(name, &len);
		for (int i=1;i<=(int)len && i <= nyears;i++)
			cf.at(cf_line, i) = min( scale*p[i-1], max );
	}

	double taxable_incentive_income(int year, const std::string &fed_or_sta)
	{
		double ti = 0.0;
		if (year==1)
		{
			if ( as_boolean("ibi_fed_amount_tax_"+fed_or_sta) ) ti += ibi_fed_amount;
			if ( as_boolean("ibi_sta_amount_tax_"+fed_or_sta) ) ti += ibi_sta_amount;
			if ( as_boolean("ibi_uti_amount_tax_"+fed_or_sta) ) ti += ibi_uti_amount;
			if ( as_boolean("ibi_oth_amount_tax_"+fed_or_sta) ) ti += ibi_oth_amount;

			if ( as_boolean("ibi_fed_percent_tax_"+fed_or_sta) ) ti += ibi_fed_per;
			if ( as_boolean("ibi_sta_percent_tax_"+fed_or_sta) ) ti += ibi_sta_per;
			if ( as_boolean("ibi_uti_percent_tax_"+fed_or_sta) ) ti += ibi_uti_per;
			if ( as_boolean("ibi_oth_percent_tax_"+fed_or_sta) ) ti += ibi_oth_per;

			if ( as_boolean("cbi_fed_tax_"+fed_or_sta) ) ti += cbi_fed_amount;
			if ( as_boolean("cbi_sta_tax_"+fed_or_sta) ) ti += cbi_sta_amount;
			if ( as_boolean("cbi_uti_tax_"+fed_or_sta) ) ti += cbi_uti_amount;
			if ( as_boolean("cbi_oth_tax_"+fed_or_sta) ) ti += cbi_oth_amount;
		}

		if ( as_boolean("pbi_fed_tax_"+fed_or_sta) ) ti += cf.at( CF_pbi_fed, year );
		if ( as_boolean("pbi_sta_tax_"+fed_or_sta) ) ti += cf.at( CF_pbi_sta, year );
		if ( as_boolean("pbi_uti_tax_"+fed_or_sta) ) ti += cf.at( CF_pbi_uti, year );
		if ( as_boolean("pbi_oth_tax_"+fed_or_sta) ) ti += cf.at( CF_pbi_oth, year );

		return ti;
	}

	void depreciation_sched_macrs_half_year( int cf_line, int nyears )
	{
		for (int i=1; i<=nyears; i++)
		{
			double factor = 0.0;
			switch(i)
			{
			case 1: factor = 0.2000; break;
			case 2: factor = 0.3200; break;
			case 3: factor = 0.1920; break;
			case 4: factor = 0.1152; break;
			case 5: factor = 0.1152; break;
			case 6: factor = 0.0576; break;
			default: factor = 0.0; break;
			}
			cf.at(cf_line, i) = factor;
		}
	}

	void depreciation_sched_straight_line( int cf_line, int nyears, int slyears )
	{
		double depr_per_year = (slyears!=0) ? 1.0 / ((double)slyears) : 0;
		for (int i=1; i<=nyears; i++)
			cf.at(cf_line, i) = (i<=slyears) ? depr_per_year : 0.0;
	}

	void depreciation_sched_custom( int cf_line, int nyears, ssc_number_t *custp, int custp_len )
	{
		// note - allows for greater than or less than 100% depreciation - warning to user in samsim
		if (custp_len < 2)
		{
			cf.at(cf_line, 1) = custp[0]/100.0;
		}
		else
		{
			for (int i=1;i<=nyears && i-1 < custp_len;i++)
				cf.at(cf_line, i) = custp[i-1]/100.0;
		}
	}

	void escal_or_annual( int cf_line, int nyears, const std::string &variable,
			double inflation_rate, double scale, bool as_rate=true, double escal = 0.0)
	{
		size_t count;
		ssc_number_t *arrp = as_array(variable, &count);

		if (as_rate)
		{
			if (count == 1)
			{
				escal = inflation_rate + scale*arrp[0];
				for (int i=0; i < nyears; i++)
					cf.at(cf_line, i+1) = pow( 1+escal, i );
			}
			else
			{
				for (int i=0; i < nyears && i < (int)count; i++)
					cf.at(cf_line, i+1) = 1 + arrp[i]*scale;
			}
		}
		else
		{
			if (count == 1)
			{
				for (int i=0;i<nyears;i++)
					cf.at(cf_line, i+1) = arrp[0]*scale*pow( 1+escal+inflation_rate, i );
			}
			else
			{
				for (int i=0;i<nyears && i<(int)count;i++)
					cf.at(cf_line, i+1) = arrp[i]*scale;
			}
		}
	}

	double min( double a, double b )
	{
		return (a < b) ? a : b;
	}

	double max( double a, double b )
	{
		return (a > b) ? a : b;
	}

	bool compute_dispatch_output(int nyears)
	{
	//Calculate energy dispatched in each dispatch period 
		ssc_number_t *hourly_dispatch; // tou period 
		ssc_number_t *hourly_enet; // hourly energy output


		int h;
		size_t count;

	// hourly energy
		hourly_enet = as_array("energy_net_hourly", &count );
		if ( count != 8760)
		{
			std::stringstream outm;
			outm <<  "Bad hourly energy output length (" << count << "), should be 8760 value";
			log( outm.str() );
			return false;
		}

	// hourly dispatch
		hourly_dispatch = as_array("dispatch_hourly", &count );
		if ( count != 8760)
		{
			std::stringstream outm;
			outm <<  "Bad hourly dispatch output length (" << count << "), should be 8760 value";
			log( outm.str() );
			return false;
		}
	
		cf.at(CF_TOD1Energy,1) = 0;
		cf.at(CF_TOD2Energy,1) = 0;
		cf.at(CF_TOD3Energy,1) = 0;
		cf.at(CF_TOD4Energy,1) = 0;
		cf.at(CF_TOD5Energy,1) = 0;
		cf.at(CF_TOD6Energy,1) = 0;
		cf.at(CF_TOD7Energy,1) = 0;
		cf.at(CF_TOD8Energy,1) = 0;
		cf.at(CF_TOD9Energy,1) = 0;
		for (h=0;h<8760;h++)
		{
			switch ((int)hourly_dispatch[h])
			{
				case 0:
					cf.at(CF_TOD1Energy,1) += hourly_enet[h];
					break;
				case 1:
					cf.at(CF_TOD2Energy,1) += hourly_enet[h];
					break;
				case 2:
					cf.at(CF_TOD3Energy,1) += hourly_enet[h];
					break;
				case 3:
					cf.at(CF_TOD4Energy,1) += hourly_enet[h];
					break;
				case 4:
					cf.at(CF_TOD5Energy,1) += hourly_enet[h];
					break;
				case 5:
					cf.at(CF_TOD6Energy,1) += hourly_enet[h];
					break;
				case 6:
					cf.at(CF_TOD7Energy,1) += hourly_enet[h];
					break;
				case 7:
					cf.at(CF_TOD8Energy,1) += hourly_enet[h];
					break;
				case 8:
					cf.at(CF_TOD9Energy,1) += hourly_enet[h];
					break;
			}
		}

		for (int y=2;y<=nyears;y++)
		{
	// compute energy dispatched
			cf.at(CF_TOD1Energy,y) = cf.at(CF_TOD1Energy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD2Energy,y) = cf.at(CF_TOD2Energy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD3Energy,y) = cf.at(CF_TOD3Energy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD4Energy,y) = cf.at(CF_TOD4Energy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD5Energy,y) = cf.at(CF_TOD5Energy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD6Energy,y) = cf.at(CF_TOD6Energy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD7Energy,y) = cf.at(CF_TOD7Energy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD8Energy,y) = cf.at(CF_TOD8Energy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD9Energy,y) = cf.at(CF_TOD9Energy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);

		}
		cf.at(CF_TOD1Energy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD2Energy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD3Energy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD4Energy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD5Energy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD6Energy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD7Energy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD8Energy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD9Energy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);

		return true;
	}

	bool process_dispatch_output(int nyears)
	{
	//Calculate energy dispatched in each dispatch period 
		ssc_number_t *hourly_dispatch; // tou period 
		ssc_number_t *hourly_enet; // hourly energy output

		size_t count;

	// hourly energy
		hourly_enet = as_array("energy_net_hourly", &count );
		if ( count != 8760)
		{
			std::stringstream outm;
			outm <<  "Bad hourly energy output length (" << count << "), should be 8760 value";
			log( outm.str() );
			return false;
		}

	// hourly dispatch
		hourly_dispatch = as_array("dispatch_hourly", &count );
		if ( count != 8760)
		{
			std::stringstream outm;
			outm <<  "Bad hourly dispatch output length (" << count << "), should be 8760 value";
			log( outm.str() );
			return false;
		}

		// from TranslateSchedule
		static int nday[12] = {31,28,31,30,31,30,31,31,30,31,30,31};


		cf.at(CF_TODJanEnergy,1) = 0;
		cf.at(CF_TODFebEnergy,1) = 0;
		cf.at(CF_TODMarEnergy,1) = 0;
		cf.at(CF_TODAprEnergy,1) = 0;
		cf.at(CF_TODMayEnergy,1) = 0;
		cf.at(CF_TODJunEnergy,1) = 0;
		cf.at(CF_TODJulEnergy,1) = 0;
		cf.at(CF_TODAugEnergy,1) = 0;
		cf.at(CF_TODSepEnergy,1) = 0;
		cf.at(CF_TODOctEnergy,1) = 0;
		cf.at(CF_TODNovEnergy,1) = 0;
		cf.at(CF_TODDecEnergy,1) = 0;

		cf.at(CF_TOD1JanEnergy,1) = 0;
		cf.at(CF_TOD1FebEnergy,1) = 0;
		cf.at(CF_TOD1MarEnergy,1) = 0;
		cf.at(CF_TOD1AprEnergy,1) = 0;
		cf.at(CF_TOD1MayEnergy,1) = 0;
		cf.at(CF_TOD1JunEnergy,1) = 0;
		cf.at(CF_TOD1JulEnergy,1) = 0;
		cf.at(CF_TOD1AugEnergy,1) = 0;
		cf.at(CF_TOD1SepEnergy,1) = 0;
		cf.at(CF_TOD1OctEnergy,1) = 0;
		cf.at(CF_TOD1NovEnergy,1) = 0;
		cf.at(CF_TOD1DecEnergy,1) = 0;
	
		cf.at(CF_TOD2JanEnergy,1) = 0;
		cf.at(CF_TOD2FebEnergy,1) = 0;
		cf.at(CF_TOD2MarEnergy,1) = 0;
		cf.at(CF_TOD2AprEnergy,1) = 0;
		cf.at(CF_TOD2MayEnergy,1) = 0;
		cf.at(CF_TOD2JunEnergy,1) = 0;
		cf.at(CF_TOD2JulEnergy,1) = 0;
		cf.at(CF_TOD2AugEnergy,1) = 0;
		cf.at(CF_TOD2SepEnergy,1) = 0;
		cf.at(CF_TOD2OctEnergy,1) = 0;
		cf.at(CF_TOD2NovEnergy,1) = 0;
		cf.at(CF_TOD2DecEnergy,1) = 0;
	
		cf.at(CF_TOD3JanEnergy,1) = 0;
		cf.at(CF_TOD3FebEnergy,1) = 0;
		cf.at(CF_TOD3MarEnergy,1) = 0;
		cf.at(CF_TOD3AprEnergy,1) = 0;
		cf.at(CF_TOD3MayEnergy,1) = 0;
		cf.at(CF_TOD3JunEnergy,1) = 0;
		cf.at(CF_TOD3JulEnergy,1) = 0;
		cf.at(CF_TOD3AugEnergy,1) = 0;
		cf.at(CF_TOD3SepEnergy,1) = 0;
		cf.at(CF_TOD3OctEnergy,1) = 0;
		cf.at(CF_TOD3NovEnergy,1) = 0;
		cf.at(CF_TOD3DecEnergy,1) = 0;
	
		cf.at(CF_TOD4JanEnergy,1) = 0;
		cf.at(CF_TOD4FebEnergy,1) = 0;
		cf.at(CF_TOD4MarEnergy,1) = 0;
		cf.at(CF_TOD4AprEnergy,1) = 0;
		cf.at(CF_TOD4MayEnergy,1) = 0;
		cf.at(CF_TOD4JunEnergy,1) = 0;
		cf.at(CF_TOD4JulEnergy,1) = 0;
		cf.at(CF_TOD4AugEnergy,1) = 0;
		cf.at(CF_TOD4SepEnergy,1) = 0;
		cf.at(CF_TOD4OctEnergy,1) = 0;
		cf.at(CF_TOD4NovEnergy,1) = 0;
		cf.at(CF_TOD4DecEnergy,1) = 0;
	
		cf.at(CF_TOD5JanEnergy,1) = 0;
		cf.at(CF_TOD5FebEnergy,1) = 0;
		cf.at(CF_TOD5MarEnergy,1) = 0;
		cf.at(CF_TOD5AprEnergy,1) = 0;
		cf.at(CF_TOD5MayEnergy,1) = 0;
		cf.at(CF_TOD5JunEnergy,1) = 0;
		cf.at(CF_TOD5JulEnergy,1) = 0;
		cf.at(CF_TOD5AugEnergy,1) = 0;
		cf.at(CF_TOD5SepEnergy,1) = 0;
		cf.at(CF_TOD5OctEnergy,1) = 0;
		cf.at(CF_TOD5NovEnergy,1) = 0;
		cf.at(CF_TOD5DecEnergy,1) = 0;
	
		cf.at(CF_TOD6JanEnergy,1) = 0;
		cf.at(CF_TOD6FebEnergy,1) = 0;
		cf.at(CF_TOD6MarEnergy,1) = 0;
		cf.at(CF_TOD6AprEnergy,1) = 0;
		cf.at(CF_TOD6MayEnergy,1) = 0;
		cf.at(CF_TOD6JunEnergy,1) = 0;
		cf.at(CF_TOD6JulEnergy,1) = 0;
		cf.at(CF_TOD6AugEnergy,1) = 0;
		cf.at(CF_TOD6SepEnergy,1) = 0;
		cf.at(CF_TOD6OctEnergy,1) = 0;
		cf.at(CF_TOD6NovEnergy,1) = 0;
		cf.at(CF_TOD6DecEnergy,1) = 0;
	
		cf.at(CF_TOD7JanEnergy,1) = 0;
		cf.at(CF_TOD7FebEnergy,1) = 0;
		cf.at(CF_TOD7MarEnergy,1) = 0;
		cf.at(CF_TOD7AprEnergy,1) = 0;
		cf.at(CF_TOD7MayEnergy,1) = 0;
		cf.at(CF_TOD7JunEnergy,1) = 0;
		cf.at(CF_TOD7JulEnergy,1) = 0;
		cf.at(CF_TOD7AugEnergy,1) = 0;
		cf.at(CF_TOD7SepEnergy,1) = 0;
		cf.at(CF_TOD7OctEnergy,1) = 0;
		cf.at(CF_TOD7NovEnergy,1) = 0;
		cf.at(CF_TOD7DecEnergy,1) = 0;
	
		cf.at(CF_TOD8JanEnergy,1) = 0;
		cf.at(CF_TOD8FebEnergy,1) = 0;
		cf.at(CF_TOD8MarEnergy,1) = 0;
		cf.at(CF_TOD8AprEnergy,1) = 0;
		cf.at(CF_TOD8MayEnergy,1) = 0;
		cf.at(CF_TOD8JunEnergy,1) = 0;
		cf.at(CF_TOD8JulEnergy,1) = 0;
		cf.at(CF_TOD8AugEnergy,1) = 0;
		cf.at(CF_TOD8SepEnergy,1) = 0;
		cf.at(CF_TOD8OctEnergy,1) = 0;
		cf.at(CF_TOD8NovEnergy,1) = 0;
		cf.at(CF_TOD8DecEnergy,1) = 0;
	
		cf.at(CF_TOD9JanEnergy,1) = 0;
		cf.at(CF_TOD9FebEnergy,1) = 0;
		cf.at(CF_TOD9MarEnergy,1) = 0;
		cf.at(CF_TOD9AprEnergy,1) = 0;
		cf.at(CF_TOD9MayEnergy,1) = 0;
		cf.at(CF_TOD9JunEnergy,1) = 0;
		cf.at(CF_TOD9JulEnergy,1) = 0;
		cf.at(CF_TOD9AugEnergy,1) = 0;
		cf.at(CF_TOD9SepEnergy,1) = 0;
		cf.at(CF_TOD9OctEnergy,1) = 0;
		cf.at(CF_TOD9NovEnergy,1) = 0;
		cf.at(CF_TOD9DecEnergy,1) = 0;
	
		int i=0;
		for (int m=0;m<12;m++)
		{
			for (int d=0;d<nday[m];d++)
			{
				for (int h=0;h<24&&i<8760 && m*24+h<288;h++)
				{
					switch (m)
					{
						case 0:
							cf.at(CF_TODJanEnergy,1) +=  hourly_enet[i];
							switch ((int)hourly_dispatch[i])
							{
								case 0:
									cf.at(CF_TOD1JanEnergy,1) +=  hourly_enet[i];
									break;
								case 1:
									cf.at(CF_TOD2JanEnergy,1) +=  hourly_enet[i];
									break;
								case 2:
									cf.at(CF_TOD3JanEnergy,1) +=  hourly_enet[i];
									break;
								case 3:
									cf.at(CF_TOD4JanEnergy,1) +=  hourly_enet[i];
									break;
								case 4:
									cf.at(CF_TOD5JanEnergy,1) +=  hourly_enet[i];
									break;
								case 5:
									cf.at(CF_TOD6JanEnergy,1) +=  hourly_enet[i];
									break;
								case 6:
									cf.at(CF_TOD7JanEnergy,1) +=  hourly_enet[i];
									break;
								case 7:
									cf.at(CF_TOD8JanEnergy,1) +=  hourly_enet[i];
									break;
								case 8:
									cf.at(CF_TOD9JanEnergy,1) +=  hourly_enet[i];
									break;
							}
							break;
						case 1:
							cf.at(CF_TODFebEnergy,1) +=  hourly_enet[i];
							switch ((int)hourly_dispatch[i])
							{
								case 0:
									cf.at(CF_TOD1FebEnergy,1) +=  hourly_enet[i];
									break;
								case 1:
									cf.at(CF_TOD2FebEnergy,1) +=  hourly_enet[i];
									break;
								case 2:
									cf.at(CF_TOD3FebEnergy,1) +=  hourly_enet[i];
									break;
								case 3:
									cf.at(CF_TOD4FebEnergy,1) +=  hourly_enet[i];
									break;
								case 4:
									cf.at(CF_TOD5FebEnergy,1) +=  hourly_enet[i];
									break;
								case 5:
									cf.at(CF_TOD6FebEnergy,1) +=  hourly_enet[i];
									break;
								case 6:
									cf.at(CF_TOD7FebEnergy,1) +=  hourly_enet[i];
									break;
								case 7:
									cf.at(CF_TOD8FebEnergy,1) +=  hourly_enet[i];
									break;
								case 8:
									cf.at(CF_TOD9FebEnergy,1) +=  hourly_enet[i];
									break;
							}
							break;
						case 2:
							cf.at(CF_TODMarEnergy,1) +=  hourly_enet[i];
							switch ((int)hourly_dispatch[i])
							{
								case 0:
									cf.at(CF_TOD1MarEnergy,1) +=  hourly_enet[i];
									break;
								case 1:
									cf.at(CF_TOD2MarEnergy,1) +=  hourly_enet[i];
									break;
								case 2:
									cf.at(CF_TOD3MarEnergy,1) +=  hourly_enet[i];
									break;
								case 3:
									cf.at(CF_TOD4MarEnergy,1) +=  hourly_enet[i];
									break;
								case 4:
									cf.at(CF_TOD5MarEnergy,1) +=  hourly_enet[i];
									break;
								case 5:
									cf.at(CF_TOD6MarEnergy,1) +=  hourly_enet[i];
									break;
								case 6:
									cf.at(CF_TOD7MarEnergy,1) +=  hourly_enet[i];
									break;
								case 7:
									cf.at(CF_TOD8MarEnergy,1) +=  hourly_enet[i];
									break;
								case 8:
									cf.at(CF_TOD9MarEnergy,1) +=  hourly_enet[i];
									break;
							}
							break;
						case 3:
							cf.at(CF_TODAprEnergy,1) +=  hourly_enet[i];
							switch ((int)hourly_dispatch[i])
							{
								case 0:
									cf.at(CF_TOD1AprEnergy,1) +=  hourly_enet[i];
									break;
								case 1:
									cf.at(CF_TOD2AprEnergy,1) +=  hourly_enet[i];
									break;
								case 2:
									cf.at(CF_TOD3AprEnergy,1) +=  hourly_enet[i];
									break;
								case 3:
									cf.at(CF_TOD4AprEnergy,1) +=  hourly_enet[i];
									break;
								case 4:
									cf.at(CF_TOD5AprEnergy,1) +=  hourly_enet[i];
									break;
								case 5:
									cf.at(CF_TOD6AprEnergy,1) +=  hourly_enet[i];
									break;
								case 6:
									cf.at(CF_TOD7AprEnergy,1) +=  hourly_enet[i];
									break;
								case 7:
									cf.at(CF_TOD8AprEnergy,1) +=  hourly_enet[i];
									break;
								case 8:
									cf.at(CF_TOD9AprEnergy,1) +=  hourly_enet[i];
									break;
							}
							break;
						case 4:
							cf.at(CF_TODMayEnergy,1) +=  hourly_enet[i];
							switch ((int)hourly_dispatch[i])
							{
								case 0:
									cf.at(CF_TOD1MayEnergy,1) +=  hourly_enet[i];
									break;
								case 1:
									cf.at(CF_TOD2MayEnergy,1) +=  hourly_enet[i];
									break;
								case 2:
									cf.at(CF_TOD3MayEnergy,1) +=  hourly_enet[i];
									break;
								case 3:
									cf.at(CF_TOD4MayEnergy,1) +=  hourly_enet[i];
									break;
								case 4:
									cf.at(CF_TOD5MayEnergy,1) +=  hourly_enet[i];
									break;
								case 5:
									cf.at(CF_TOD6MayEnergy,1) +=  hourly_enet[i];
									break;
								case 6:
									cf.at(CF_TOD7MayEnergy,1) +=  hourly_enet[i];
									break;
								case 7:
									cf.at(CF_TOD8MayEnergy,1) +=  hourly_enet[i];
									break;
								case 8:
									cf.at(CF_TOD9MayEnergy,1) +=  hourly_enet[i];
									break;
							}
							break;
						case 5:
							cf.at(CF_TODJunEnergy,1) +=  hourly_enet[i];
							switch ((int)hourly_dispatch[i])
							{
								case 0:
									cf.at(CF_TOD1JunEnergy,1) +=  hourly_enet[i];
									break;
								case 1:
									cf.at(CF_TOD2JunEnergy,1) +=  hourly_enet[i];
									break;
								case 2:
									cf.at(CF_TOD3JunEnergy,1) +=  hourly_enet[i];
									break;
								case 3:
									cf.at(CF_TOD4JunEnergy,1) +=  hourly_enet[i];
									break;
								case 4:
									cf.at(CF_TOD5JunEnergy,1) +=  hourly_enet[i];
									break;
								case 5:
									cf.at(CF_TOD6JunEnergy,1) +=  hourly_enet[i];
									break;
								case 6:
									cf.at(CF_TOD7JunEnergy,1) +=  hourly_enet[i];
									break;
								case 7:
									cf.at(CF_TOD8JunEnergy,1) +=  hourly_enet[i];
									break;
								case 8:
									cf.at(CF_TOD9JunEnergy,1) +=  hourly_enet[i];
									break;
							}
							break;
						case 6:
							cf.at(CF_TODJulEnergy,1) +=  hourly_enet[i];
							switch ((int)hourly_dispatch[i])
							{
								case 0:
									cf.at(CF_TOD1JulEnergy,1) +=  hourly_enet[i];
									break;
								case 1:
									cf.at(CF_TOD2JulEnergy,1) +=  hourly_enet[i];
									break;
								case 2:
									cf.at(CF_TOD3JulEnergy,1) +=  hourly_enet[i];
									break;
								case 3:
									cf.at(CF_TOD4JulEnergy,1) +=  hourly_enet[i];
									break;
								case 4:
									cf.at(CF_TOD5JulEnergy,1) +=  hourly_enet[i];
									break;
								case 5:
									cf.at(CF_TOD6JulEnergy,1) +=  hourly_enet[i];
									break;
								case 6:
									cf.at(CF_TOD7JulEnergy,1) +=  hourly_enet[i];
									break;
								case 7:
									cf.at(CF_TOD8JulEnergy,1) +=  hourly_enet[i];
									break;
								case 8:
									cf.at(CF_TOD9JulEnergy,1) +=  hourly_enet[i];
									break;
							}
							break;
						case 7:
							cf.at(CF_TODAugEnergy,1) +=  hourly_enet[i];
							switch ((int)hourly_dispatch[i])
							{
								case 0:
									cf.at(CF_TOD1AugEnergy,1) +=  hourly_enet[i];
									break;
								case 1:
									cf.at(CF_TOD2AugEnergy,1) +=  hourly_enet[i];
									break;
								case 2:
									cf.at(CF_TOD3AugEnergy,1) +=  hourly_enet[i];
									break;
								case 3:
									cf.at(CF_TOD4AugEnergy,1) +=  hourly_enet[i];
									break;
								case 4:
									cf.at(CF_TOD5AugEnergy,1) +=  hourly_enet[i];
									break;
								case 5:
									cf.at(CF_TOD6AugEnergy,1) +=  hourly_enet[i];
									break;
								case 6:
									cf.at(CF_TOD7AugEnergy,1) +=  hourly_enet[i];
									break;
								case 7:
									cf.at(CF_TOD8AugEnergy,1) +=  hourly_enet[i];
									break;
								case 8:
									cf.at(CF_TOD9AugEnergy,1) +=  hourly_enet[i];
									break;
							}
							break;
						case 8:
							cf.at(CF_TODSepEnergy,1) +=  hourly_enet[i];
							switch ((int)hourly_dispatch[i])
							{
								case 0:
									cf.at(CF_TOD1SepEnergy,1) +=  hourly_enet[i];
									break;
								case 1:
									cf.at(CF_TOD2SepEnergy,1) +=  hourly_enet[i];
									break;
								case 2:
									cf.at(CF_TOD3SepEnergy,1) +=  hourly_enet[i];
									break;
								case 3:
									cf.at(CF_TOD4SepEnergy,1) +=  hourly_enet[i];
									break;
								case 4:
									cf.at(CF_TOD5SepEnergy,1) +=  hourly_enet[i];
									break;
								case 5:
									cf.at(CF_TOD6SepEnergy,1) +=  hourly_enet[i];
									break;
								case 6:
									cf.at(CF_TOD7SepEnergy,1) +=  hourly_enet[i];
									break;
								case 7:
									cf.at(CF_TOD8SepEnergy,1) +=  hourly_enet[i];
									break;
								case 8:
									cf.at(CF_TOD9SepEnergy,1) +=  hourly_enet[i];
									break;
							}
							break;
						case 9:
							cf.at(CF_TODOctEnergy,1) +=  hourly_enet[i];
							switch ((int)hourly_dispatch[i])
							{
								case 0:
									cf.at(CF_TOD1OctEnergy,1) +=  hourly_enet[i];
									break;
								case 1:
									cf.at(CF_TOD2OctEnergy,1) +=  hourly_enet[i];
									break;
								case 2:
									cf.at(CF_TOD3OctEnergy,1) +=  hourly_enet[i];
									break;
								case 3:
									cf.at(CF_TOD4OctEnergy,1) +=  hourly_enet[i];
									break;
								case 4:
									cf.at(CF_TOD5OctEnergy,1) +=  hourly_enet[i];
									break;
								case 5:
									cf.at(CF_TOD6OctEnergy,1) +=  hourly_enet[i];
									break;
								case 6:
									cf.at(CF_TOD7OctEnergy,1) +=  hourly_enet[i];
									break;
								case 7:
									cf.at(CF_TOD8OctEnergy,1) +=  hourly_enet[i];
									break;
								case 8:
									cf.at(CF_TOD9OctEnergy,1) +=  hourly_enet[i];
									break;
							}
							break;
						case 10:
							cf.at(CF_TODNovEnergy,1) +=  hourly_enet[i];
							switch ((int)hourly_dispatch[i])
							{
								case 0:
									cf.at(CF_TOD1NovEnergy,1) +=  hourly_enet[i];
									break;
								case 1:
									cf.at(CF_TOD2NovEnergy,1) +=  hourly_enet[i];
									break;
								case 2:
									cf.at(CF_TOD3NovEnergy,1) +=  hourly_enet[i];
									break;
								case 3:
									cf.at(CF_TOD4NovEnergy,1) +=  hourly_enet[i];
									break;
								case 4:
									cf.at(CF_TOD5NovEnergy,1) +=  hourly_enet[i];
									break;
								case 5:
									cf.at(CF_TOD6NovEnergy,1) +=  hourly_enet[i];
									break;
								case 6:
									cf.at(CF_TOD7NovEnergy,1) +=  hourly_enet[i];
									break;
								case 7:
									cf.at(CF_TOD8NovEnergy,1) +=  hourly_enet[i];
									break;
								case 8:
									cf.at(CF_TOD9NovEnergy,1) +=  hourly_enet[i];
									break;
							}
							break;
						case 11:
							cf.at(CF_TODDecEnergy,1) +=  hourly_enet[i];
							switch ((int)hourly_dispatch[i])
							{
								case 0:
									cf.at(CF_TOD1DecEnergy,1) +=  hourly_enet[i];
									break;
								case 1:
									cf.at(CF_TOD2DecEnergy,1) +=  hourly_enet[i];
									break;
								case 2:
									cf.at(CF_TOD3DecEnergy,1) +=  hourly_enet[i];
									break;
								case 3:
									cf.at(CF_TOD4DecEnergy,1) +=  hourly_enet[i];
									break;
								case 4:
									cf.at(CF_TOD5DecEnergy,1) +=  hourly_enet[i];
									break;
								case 5:
									cf.at(CF_TOD6DecEnergy,1) +=  hourly_enet[i];
									break;
								case 6:
									cf.at(CF_TOD7DecEnergy,1) +=  hourly_enet[i];
									break;
								case 7:
									cf.at(CF_TOD8DecEnergy,1) +=  hourly_enet[i];
									break;
								case 8:
									cf.at(CF_TOD9DecEnergy,1) +=  hourly_enet[i];
									break;
							}
							break;
					}
					i++;					
				}
			}
		}

		for (int y=2;y<=nyears;y++)
		{
	// compute energy dispatched
			cf.at(CF_TODJanEnergy,y) = cf.at(CF_TODJanEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TODFebEnergy,y) = cf.at(CF_TODFebEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TODMarEnergy,y) = cf.at(CF_TODMarEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TODAprEnergy,y) = cf.at(CF_TODAprEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TODMayEnergy,y) = cf.at(CF_TODMayEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TODJunEnergy,y) = cf.at(CF_TODJunEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TODJulEnergy,y) = cf.at(CF_TODJulEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TODAugEnergy,y) = cf.at(CF_TODAugEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TODSepEnergy,y) = cf.at(CF_TODSepEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TODOctEnergy,y) = cf.at(CF_TODOctEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TODNovEnergy,y) = cf.at(CF_TODNovEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TODDecEnergy,y) = cf.at(CF_TODDecEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);

			cf.at(CF_TOD1JanEnergy,y) = cf.at(CF_TOD1JanEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD1FebEnergy,y) = cf.at(CF_TOD1FebEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD1MarEnergy,y) = cf.at(CF_TOD1MarEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD1AprEnergy,y) = cf.at(CF_TOD1AprEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD1MayEnergy,y) = cf.at(CF_TOD1MayEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD1JunEnergy,y) = cf.at(CF_TOD1JunEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD1JulEnergy,y) = cf.at(CF_TOD1JulEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD1AugEnergy,y) = cf.at(CF_TOD1AugEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD1SepEnergy,y) = cf.at(CF_TOD1SepEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD1OctEnergy,y) = cf.at(CF_TOD1OctEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD1NovEnergy,y) = cf.at(CF_TOD1NovEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD1DecEnergy,y) = cf.at(CF_TOD1DecEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);

			cf.at(CF_TOD2JanEnergy,y) = cf.at(CF_TOD2JanEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD2FebEnergy,y) = cf.at(CF_TOD2FebEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD2MarEnergy,y) = cf.at(CF_TOD2MarEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD2AprEnergy,y) = cf.at(CF_TOD2AprEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD2MayEnergy,y) = cf.at(CF_TOD2MayEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD2JunEnergy,y) = cf.at(CF_TOD2JunEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD2JulEnergy,y) = cf.at(CF_TOD2JulEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD2AugEnergy,y) = cf.at(CF_TOD2AugEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD2SepEnergy,y) = cf.at(CF_TOD2SepEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD2OctEnergy,y) = cf.at(CF_TOD2OctEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD2NovEnergy,y) = cf.at(CF_TOD2NovEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD2DecEnergy,y) = cf.at(CF_TOD2DecEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);

			cf.at(CF_TOD3JanEnergy,y) = cf.at(CF_TOD3JanEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD3FebEnergy,y) = cf.at(CF_TOD3FebEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD3MarEnergy,y) = cf.at(CF_TOD3MarEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD3AprEnergy,y) = cf.at(CF_TOD3AprEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD3MayEnergy,y) = cf.at(CF_TOD3MayEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD3JunEnergy,y) = cf.at(CF_TOD3JunEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD3JulEnergy,y) = cf.at(CF_TOD3JulEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD3AugEnergy,y) = cf.at(CF_TOD3AugEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD3SepEnergy,y) = cf.at(CF_TOD3SepEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD3OctEnergy,y) = cf.at(CF_TOD3OctEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD3NovEnergy,y) = cf.at(CF_TOD3NovEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD3DecEnergy,y) = cf.at(CF_TOD3DecEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);

			cf.at(CF_TOD4JanEnergy,y) = cf.at(CF_TOD4JanEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD4FebEnergy,y) = cf.at(CF_TOD4FebEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD4MarEnergy,y) = cf.at(CF_TOD4MarEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD4AprEnergy,y) = cf.at(CF_TOD4AprEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD4MayEnergy,y) = cf.at(CF_TOD4MayEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD4JunEnergy,y) = cf.at(CF_TOD4JunEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD4JulEnergy,y) = cf.at(CF_TOD4JulEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD4AugEnergy,y) = cf.at(CF_TOD4AugEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD4SepEnergy,y) = cf.at(CF_TOD4SepEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD4OctEnergy,y) = cf.at(CF_TOD4OctEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD4NovEnergy,y) = cf.at(CF_TOD4NovEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD4DecEnergy,y) = cf.at(CF_TOD4DecEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);

			cf.at(CF_TOD5JanEnergy,y) = cf.at(CF_TOD5JanEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD5FebEnergy,y) = cf.at(CF_TOD5FebEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD5MarEnergy,y) = cf.at(CF_TOD5MarEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD5AprEnergy,y) = cf.at(CF_TOD5AprEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD5MayEnergy,y) = cf.at(CF_TOD5MayEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD5JunEnergy,y) = cf.at(CF_TOD5JunEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD5JulEnergy,y) = cf.at(CF_TOD5JulEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD5AugEnergy,y) = cf.at(CF_TOD5AugEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD5SepEnergy,y) = cf.at(CF_TOD5SepEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD5OctEnergy,y) = cf.at(CF_TOD5OctEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD5NovEnergy,y) = cf.at(CF_TOD5NovEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD5DecEnergy,y) = cf.at(CF_TOD5DecEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);

			cf.at(CF_TOD6JanEnergy,y) = cf.at(CF_TOD6JanEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD6FebEnergy,y) = cf.at(CF_TOD6FebEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD6MarEnergy,y) = cf.at(CF_TOD6MarEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD6AprEnergy,y) = cf.at(CF_TOD6AprEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD6MayEnergy,y) = cf.at(CF_TOD6MayEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD6JunEnergy,y) = cf.at(CF_TOD6JunEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD6JulEnergy,y) = cf.at(CF_TOD6JulEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD6AugEnergy,y) = cf.at(CF_TOD6AugEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD6SepEnergy,y) = cf.at(CF_TOD6SepEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD6OctEnergy,y) = cf.at(CF_TOD6OctEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD6NovEnergy,y) = cf.at(CF_TOD6NovEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD6DecEnergy,y) = cf.at(CF_TOD6DecEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);

			cf.at(CF_TOD7JanEnergy,y) = cf.at(CF_TOD7JanEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD7FebEnergy,y) = cf.at(CF_TOD7FebEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD7MarEnergy,y) = cf.at(CF_TOD7MarEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD7AprEnergy,y) = cf.at(CF_TOD7AprEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD7MayEnergy,y) = cf.at(CF_TOD7MayEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD7JunEnergy,y) = cf.at(CF_TOD7JunEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD7JulEnergy,y) = cf.at(CF_TOD7JulEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD7AugEnergy,y) = cf.at(CF_TOD7AugEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD7SepEnergy,y) = cf.at(CF_TOD7SepEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD7OctEnergy,y) = cf.at(CF_TOD7OctEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD7NovEnergy,y) = cf.at(CF_TOD7NovEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD7DecEnergy,y) = cf.at(CF_TOD7DecEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);

			cf.at(CF_TOD8JanEnergy,y) = cf.at(CF_TOD8JanEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD8FebEnergy,y) = cf.at(CF_TOD8FebEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD8MarEnergy,y) = cf.at(CF_TOD8MarEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD8AprEnergy,y) = cf.at(CF_TOD8AprEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD8MayEnergy,y) = cf.at(CF_TOD8MayEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD8JunEnergy,y) = cf.at(CF_TOD8JunEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD8JulEnergy,y) = cf.at(CF_TOD8JulEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD8AugEnergy,y) = cf.at(CF_TOD8AugEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD8SepEnergy,y) = cf.at(CF_TOD8SepEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD8OctEnergy,y) = cf.at(CF_TOD8OctEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD8NovEnergy,y) = cf.at(CF_TOD8NovEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD8DecEnergy,y) = cf.at(CF_TOD8DecEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);

			cf.at(CF_TOD9JanEnergy,y) = cf.at(CF_TOD9JanEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD9FebEnergy,y) = cf.at(CF_TOD9FebEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD9MarEnergy,y) = cf.at(CF_TOD9MarEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD9AprEnergy,y) = cf.at(CF_TOD9AprEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD9MayEnergy,y) = cf.at(CF_TOD9MayEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD9JunEnergy,y) = cf.at(CF_TOD9JunEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD9JulEnergy,y) = cf.at(CF_TOD9JulEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD9AugEnergy,y) = cf.at(CF_TOD9AugEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD9SepEnergy,y) = cf.at(CF_TOD9SepEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD9OctEnergy,y) = cf.at(CF_TOD9OctEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD9NovEnergy,y) = cf.at(CF_TOD9NovEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
			cf.at(CF_TOD9DecEnergy,y) = cf.at(CF_TOD9DecEnergy,1) * cf.at(CF_Degradation,y) * cf.at(CF_Availability,y);
		}


		cf.at(CF_TODJanEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TODFebEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TODMarEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TODAprEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TODMayEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TODJunEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TODJulEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TODAugEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TODSepEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TODOctEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TODNovEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TODDecEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);

		cf.at(CF_TOD1JanEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD1FebEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD1MarEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD1AprEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD1MayEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD1JunEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD1JulEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD1AugEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD1SepEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD1OctEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD1NovEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD1DecEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);

		cf.at(CF_TOD2JanEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD2FebEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD2MarEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD2AprEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD2MayEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD2JunEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD2JulEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD2AugEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD2SepEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD2OctEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD2NovEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD2DecEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);

		cf.at(CF_TOD3JanEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD3FebEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD3MarEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD3AprEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD3MayEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD3JunEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD3JulEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD3AugEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD3SepEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD3OctEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD3NovEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD3DecEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);

		cf.at(CF_TOD4JanEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD4FebEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD4MarEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD4AprEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD4MayEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD4JunEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD4JulEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD4AugEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD4SepEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD4OctEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD4NovEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD4DecEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);

		cf.at(CF_TOD5JanEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD5FebEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD5MarEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD5AprEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD5MayEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD5JunEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD5JulEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD5AugEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD5SepEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD5OctEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD5NovEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD5DecEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);

		cf.at(CF_TOD6JanEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD6FebEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD6MarEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD6AprEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD6MayEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD6JunEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD6JulEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD6AugEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD6SepEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD6OctEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD6NovEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD6DecEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);

		cf.at(CF_TOD7JanEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD7FebEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD7MarEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD7AprEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD7MayEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD7JunEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD7JulEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD7AugEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD7SepEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD7OctEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD7NovEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD7DecEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);

		cf.at(CF_TOD8JanEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD8FebEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD8MarEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD8AprEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD8MayEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD8JunEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD8JulEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD8AugEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD8SepEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD8OctEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD8NovEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD8DecEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);

		cf.at(CF_TOD9JanEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD9FebEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD9MarEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD9AprEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD9MayEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD9JunEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD9JulEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD9AugEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD9SepEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD9OctEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD9NovEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);
		cf.at(CF_TOD9DecEnergy,1) *= cf.at(CF_Degradation,1) * cf.at(CF_Availability,1);

		return true;
	}


	bool compute_lifetime_dispatch_output(int nyears)
	{
	//Calculate energy dispatched in each dispatch period 
		ssc_number_t *hourly_dispatch; // tou period 
		ssc_number_t *hourly_enet; // hourly energy output


		int h;
		size_t count;

	// hourly energy
		hourly_enet = as_array("energy_net_hourly", &count );
		if ( (int)count != (8760*nyears))
		{
			std::stringstream outm;
			outm <<  "Bad hourly dispatch output length (" << count << "), should be (analysis period-1) * 8760 value (" << 8760*nyears << ")";
			log( outm.str() );
			return false;
		}

	// hourly dispatch
		hourly_dispatch = as_array("dispatch_hourly", &count );
		if ( (int)count != 8760)
		{
			std::stringstream outm;
			outm <<  "Bad hourly dispatch output length (" << count << "), should be 8760";
			log( outm.str() );
			return false;
		}



		for (int y=1;y<=nyears;y++)
		{
			cf.at(CF_TOD1Energy,y) = 0;
			cf.at(CF_TOD2Energy,y) = 0;
			cf.at(CF_TOD3Energy,y) = 0;
			cf.at(CF_TOD4Energy,y) = 0;
			cf.at(CF_TOD5Energy,y) = 0;
			cf.at(CF_TOD6Energy,y) = 0;
			cf.at(CF_TOD7Energy,y) = 0;
			cf.at(CF_TOD8Energy,y) = 0;
			cf.at(CF_TOD9Energy,y) = 0;

			for (h=0;h<8760;h++)
			{
				switch ((int)hourly_dispatch[h])
				{
					case 0:
						cf.at(CF_TOD1Energy,y) += hourly_enet[(y-1)*8760+h] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
						break;
					case 1:
						cf.at(CF_TOD2Energy,y) += hourly_enet[(y-1)*8760+h] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
						break;
					case 2:
						cf.at(CF_TOD3Energy,y) += hourly_enet[(y-1)*8760+h] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
						break;
					case 3:
						cf.at(CF_TOD4Energy,y) += hourly_enet[(y-1)*8760+h] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
						break;
					case 4:
						cf.at(CF_TOD5Energy,y) += hourly_enet[(y-1)*8760+h] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
						break;
					case 5:
						cf.at(CF_TOD6Energy,y) += hourly_enet[(y-1)*8760+h] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
						break;
					case 6:
						cf.at(CF_TOD7Energy,y) += hourly_enet[(y-1)*8760+h] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
						break;
					case 7:
						cf.at(CF_TOD8Energy,y) += hourly_enet[(y-1)*8760+h] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
						break;
					case 8:
						cf.at(CF_TOD9Energy,y) += hourly_enet[(y-1)*8760+h] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
						break;
				}
			}
			cf.at(CF_energy_net,y) = 
				cf.at(CF_TOD1Energy,y) +
				cf.at(CF_TOD2Energy,y) +
				cf.at(CF_TOD3Energy,y) +
				cf.at(CF_TOD4Energy,y) +
				cf.at(CF_TOD5Energy,y) +
				cf.at(CF_TOD6Energy,y) +
				cf.at(CF_TOD7Energy,y) +
				cf.at(CF_TOD8Energy,y) +
				cf.at(CF_TOD9Energy,y) ;

		}


		return true;
	}

	bool process_lifetime_dispatch_output(int nyears)
	{
	//Calculate energy dispatched in each dispatch period 
		ssc_number_t *hourly_dispatch; // tou period 
		ssc_number_t *hourly_enet; // hourly energy output

		size_t count;

	// hourly energy
		hourly_enet = as_array("energy_net_hourly", &count );
		if ( (int)count != (8760*nyears))
		{
			std::stringstream outm;
			outm <<  "Bad hourly dispatch output length (" << count << "), should be (analysis period-1) * 8760 value (" << 8760*nyears << ")";
			log( outm.str() );
			return false;
		}

	// hourly dispatch
		hourly_dispatch = as_array("dispatch_hourly", &count );
		if ( count != 8760)
		{
			std::stringstream outm;
			outm <<  "Bad hourly dispatch output length (" << count << "), should be 8760";
			log( outm.str() );
			return false;
		}

		// from TranslateSchedule
		static int nday[12] = {31,28,31,30,31,30,31,31,30,31,30,31};


		for (int y=1;y<=nyears;y++)
		{
			cf.at(CF_TODJanEnergy,y) = 0;
			cf.at(CF_TODFebEnergy,y) = 0;
			cf.at(CF_TODMarEnergy,y) = 0;
			cf.at(CF_TODAprEnergy,y) = 0;
			cf.at(CF_TODMayEnergy,y) = 0;
			cf.at(CF_TODJunEnergy,y) = 0;
			cf.at(CF_TODJulEnergy,y) = 0;
			cf.at(CF_TODAugEnergy,y) = 0;
			cf.at(CF_TODSepEnergy,y) = 0;
			cf.at(CF_TODOctEnergy,y) = 0;
			cf.at(CF_TODNovEnergy,y) = 0;
			cf.at(CF_TODDecEnergy,y) = 0;

			cf.at(CF_TOD1JanEnergy,y) = 0;
			cf.at(CF_TOD1FebEnergy,y) = 0;
			cf.at(CF_TOD1MarEnergy,y) = 0;
			cf.at(CF_TOD1AprEnergy,y) = 0;
			cf.at(CF_TOD1MayEnergy,y) = 0;
			cf.at(CF_TOD1JunEnergy,y) = 0;
			cf.at(CF_TOD1JulEnergy,y) = 0;
			cf.at(CF_TOD1AugEnergy,y) = 0;
			cf.at(CF_TOD1SepEnergy,y) = 0;
			cf.at(CF_TOD1OctEnergy,y) = 0;
			cf.at(CF_TOD1NovEnergy,y) = 0;
			cf.at(CF_TOD1DecEnergy,y) = 0;
		
			cf.at(CF_TOD2JanEnergy,y) = 0;
			cf.at(CF_TOD2FebEnergy,y) = 0;
			cf.at(CF_TOD2MarEnergy,y) = 0;
			cf.at(CF_TOD2AprEnergy,y) = 0;
			cf.at(CF_TOD2MayEnergy,y) = 0;
			cf.at(CF_TOD2JunEnergy,y) = 0;
			cf.at(CF_TOD2JulEnergy,y) = 0;
			cf.at(CF_TOD2AugEnergy,y) = 0;
			cf.at(CF_TOD2SepEnergy,y) = 0;
			cf.at(CF_TOD2OctEnergy,y) = 0;
			cf.at(CF_TOD2NovEnergy,y) = 0;
			cf.at(CF_TOD2DecEnergy,y) = 0;
		
			cf.at(CF_TOD3JanEnergy,y) = 0;
			cf.at(CF_TOD3FebEnergy,y) = 0;
			cf.at(CF_TOD3MarEnergy,y) = 0;
			cf.at(CF_TOD3AprEnergy,y) = 0;
			cf.at(CF_TOD3MayEnergy,y) = 0;
			cf.at(CF_TOD3JunEnergy,y) = 0;
			cf.at(CF_TOD3JulEnergy,y) = 0;
			cf.at(CF_TOD3AugEnergy,y) = 0;
			cf.at(CF_TOD3SepEnergy,y) = 0;
			cf.at(CF_TOD3OctEnergy,y) = 0;
			cf.at(CF_TOD3NovEnergy,y) = 0;
			cf.at(CF_TOD3DecEnergy,y) = 0;
		
			cf.at(CF_TOD4JanEnergy,y) = 0;
			cf.at(CF_TOD4FebEnergy,y) = 0;
			cf.at(CF_TOD4MarEnergy,y) = 0;
			cf.at(CF_TOD4AprEnergy,y) = 0;
			cf.at(CF_TOD4MayEnergy,y) = 0;
			cf.at(CF_TOD4JunEnergy,y) = 0;
			cf.at(CF_TOD4JulEnergy,y) = 0;
			cf.at(CF_TOD4AugEnergy,y) = 0;
			cf.at(CF_TOD4SepEnergy,y) = 0;
			cf.at(CF_TOD4OctEnergy,y) = 0;
			cf.at(CF_TOD4NovEnergy,y) = 0;
			cf.at(CF_TOD4DecEnergy,y) = 0;
		
			cf.at(CF_TOD5JanEnergy,y) = 0;
			cf.at(CF_TOD5FebEnergy,y) = 0;
			cf.at(CF_TOD5MarEnergy,y) = 0;
			cf.at(CF_TOD5AprEnergy,y) = 0;
			cf.at(CF_TOD5MayEnergy,y) = 0;
			cf.at(CF_TOD5JunEnergy,y) = 0;
			cf.at(CF_TOD5JulEnergy,y) = 0;
			cf.at(CF_TOD5AugEnergy,y) = 0;
			cf.at(CF_TOD5SepEnergy,y) = 0;
			cf.at(CF_TOD5OctEnergy,y) = 0;
			cf.at(CF_TOD5NovEnergy,y) = 0;
			cf.at(CF_TOD5DecEnergy,y) = 0;
		
			cf.at(CF_TOD6JanEnergy,y) = 0;
			cf.at(CF_TOD6FebEnergy,y) = 0;
			cf.at(CF_TOD6MarEnergy,y) = 0;
			cf.at(CF_TOD6AprEnergy,y) = 0;
			cf.at(CF_TOD6MayEnergy,y) = 0;
			cf.at(CF_TOD6JunEnergy,y) = 0;
			cf.at(CF_TOD6JulEnergy,y) = 0;
			cf.at(CF_TOD6AugEnergy,y) = 0;
			cf.at(CF_TOD6SepEnergy,y) = 0;
			cf.at(CF_TOD6OctEnergy,y) = 0;
			cf.at(CF_TOD6NovEnergy,y) = 0;
			cf.at(CF_TOD6DecEnergy,y) = 0;
		
			cf.at(CF_TOD7JanEnergy,y) = 0;
			cf.at(CF_TOD7FebEnergy,y) = 0;
			cf.at(CF_TOD7MarEnergy,y) = 0;
			cf.at(CF_TOD7AprEnergy,y) = 0;
			cf.at(CF_TOD7MayEnergy,y) = 0;
			cf.at(CF_TOD7JunEnergy,y) = 0;
			cf.at(CF_TOD7JulEnergy,y) = 0;
			cf.at(CF_TOD7AugEnergy,y) = 0;
			cf.at(CF_TOD7SepEnergy,y) = 0;
			cf.at(CF_TOD7OctEnergy,y) = 0;
			cf.at(CF_TOD7NovEnergy,y) = 0;
			cf.at(CF_TOD7DecEnergy,y) = 0;
		
			cf.at(CF_TOD8JanEnergy,y) = 0;
			cf.at(CF_TOD8FebEnergy,y) = 0;
			cf.at(CF_TOD8MarEnergy,y) = 0;
			cf.at(CF_TOD8AprEnergy,y) = 0;
			cf.at(CF_TOD8MayEnergy,y) = 0;
			cf.at(CF_TOD8JunEnergy,y) = 0;
			cf.at(CF_TOD8JulEnergy,y) = 0;
			cf.at(CF_TOD8AugEnergy,y) = 0;
			cf.at(CF_TOD8SepEnergy,y) = 0;
			cf.at(CF_TOD8OctEnergy,y) = 0;
			cf.at(CF_TOD8NovEnergy,y) = 0;
			cf.at(CF_TOD8DecEnergy,y) = 0;
		
			cf.at(CF_TOD9JanEnergy,y) = 0;
			cf.at(CF_TOD9FebEnergy,y) = 0;
			cf.at(CF_TOD9MarEnergy,y) = 0;
			cf.at(CF_TOD9AprEnergy,y) = 0;
			cf.at(CF_TOD9MayEnergy,y) = 0;
			cf.at(CF_TOD9JunEnergy,y) = 0;
			cf.at(CF_TOD9JulEnergy,y) = 0;
			cf.at(CF_TOD9AugEnergy,y) = 0;
			cf.at(CF_TOD9SepEnergy,y) = 0;
			cf.at(CF_TOD9OctEnergy,y) = 0;
			cf.at(CF_TOD9NovEnergy,y) = 0;
			cf.at(CF_TOD9DecEnergy,y) = 0;
		
			int i=0;
			for (int m=0;m<12;m++)
			{
				for (int d=0;d<nday[m];d++)
				{
					for (int h=0;h<24&&i<8760 && m*24+h<288;h++)
					{
						switch (m)
						{
							case 0:
								cf.at(CF_TODJanEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
								switch ((int)hourly_dispatch[i])
								{
									case 0:
										cf.at(CF_TOD1JanEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 1:
										cf.at(CF_TOD2JanEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 2:
										cf.at(CF_TOD3JanEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 3:
										cf.at(CF_TOD4JanEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 4:
										cf.at(CF_TOD5JanEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 5:
										cf.at(CF_TOD6JanEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 6:
										cf.at(CF_TOD7JanEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 7:
										cf.at(CF_TOD8JanEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 8:
										cf.at(CF_TOD9JanEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
								}
								break;
							case 1:
								cf.at(CF_TODFebEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
								switch ((int)hourly_dispatch[i])
								{
									case 0:
										cf.at(CF_TOD1FebEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 1:
										cf.at(CF_TOD2FebEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 2:
										cf.at(CF_TOD3FebEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 3:
										cf.at(CF_TOD4FebEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 4:
										cf.at(CF_TOD5FebEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 5:
										cf.at(CF_TOD6FebEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 6:
										cf.at(CF_TOD7FebEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 7:
										cf.at(CF_TOD8FebEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 8:
										cf.at(CF_TOD9FebEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
								}
								break;
							case 2:
								cf.at(CF_TODMarEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
								switch ((int)hourly_dispatch[i])
								{
									case 0:
										cf.at(CF_TOD1MarEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 1:
										cf.at(CF_TOD2MarEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 2:
										cf.at(CF_TOD3MarEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 3:
										cf.at(CF_TOD4MarEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 4:
										cf.at(CF_TOD5MarEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 5:
										cf.at(CF_TOD6MarEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 6:
										cf.at(CF_TOD7MarEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 7:
										cf.at(CF_TOD8MarEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 8:
										cf.at(CF_TOD9MarEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
								}
								break;
							case 3:
								cf.at(CF_TODAprEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
								switch ((int)hourly_dispatch[i])
								{
									case 0:
										cf.at(CF_TOD1AprEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 1:
										cf.at(CF_TOD2AprEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 2:
										cf.at(CF_TOD3AprEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 3:
										cf.at(CF_TOD4AprEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 4:
										cf.at(CF_TOD5AprEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 5:
										cf.at(CF_TOD6AprEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 6:
										cf.at(CF_TOD7AprEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 7:
										cf.at(CF_TOD8AprEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 8:
										cf.at(CF_TOD9AprEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
								}
								break;
							case 4:
								cf.at(CF_TODMayEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
								switch ((int)hourly_dispatch[i])
								{
									case 0:
										cf.at(CF_TOD1MayEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 1:
										cf.at(CF_TOD2MayEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 2:
										cf.at(CF_TOD3MayEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 3:
										cf.at(CF_TOD4MayEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 4:
										cf.at(CF_TOD5MayEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 5:
										cf.at(CF_TOD6MayEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 6:
										cf.at(CF_TOD7MayEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 7:
										cf.at(CF_TOD8MayEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 8:
										cf.at(CF_TOD9MayEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
								}
								break;
							case 5:
								cf.at(CF_TODJunEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
								switch ((int)hourly_dispatch[i])
								{
									case 0:
										cf.at(CF_TOD1JunEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 1:
										cf.at(CF_TOD2JunEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 2:
										cf.at(CF_TOD3JunEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 3:
										cf.at(CF_TOD4JunEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 4:
										cf.at(CF_TOD5JunEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 5:
										cf.at(CF_TOD6JunEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 6:
										cf.at(CF_TOD7JunEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 7:
										cf.at(CF_TOD8JunEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 8:
										cf.at(CF_TOD9JunEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
								}
								break;
							case 6:
								cf.at(CF_TODJulEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
								switch ((int)hourly_dispatch[i])
								{
									case 0:
										cf.at(CF_TOD1JulEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 1:
										cf.at(CF_TOD2JulEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 2:
										cf.at(CF_TOD3JulEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 3:
										cf.at(CF_TOD4JulEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 4:
										cf.at(CF_TOD5JulEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 5:
										cf.at(CF_TOD6JulEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 6:
										cf.at(CF_TOD7JulEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 7:
										cf.at(CF_TOD8JulEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 8:
										cf.at(CF_TOD9JulEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
								}
								break;
							case 7:
								cf.at(CF_TODAugEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
								switch ((int)hourly_dispatch[i])
								{
									case 0:
										cf.at(CF_TOD1AugEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 1:
										cf.at(CF_TOD2AugEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 2:
										cf.at(CF_TOD3AugEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 3:
										cf.at(CF_TOD4AugEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 4:
										cf.at(CF_TOD5AugEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 5:
										cf.at(CF_TOD6AugEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 6:
										cf.at(CF_TOD7AugEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 7:
										cf.at(CF_TOD8AugEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 8:
										cf.at(CF_TOD9AugEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
								}
								break;
							case 8:
								cf.at(CF_TODSepEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
								switch ((int)hourly_dispatch[i])
								{
									case 0:
										cf.at(CF_TOD1SepEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 1:
										cf.at(CF_TOD2SepEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 2:
										cf.at(CF_TOD3SepEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 3:
										cf.at(CF_TOD4SepEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 4:
										cf.at(CF_TOD5SepEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 5:
										cf.at(CF_TOD6SepEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 6:
										cf.at(CF_TOD7SepEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 7:
										cf.at(CF_TOD8SepEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 8:
										cf.at(CF_TOD9SepEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
								}
								break;
							case 9:
								cf.at(CF_TODOctEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
								switch ((int)hourly_dispatch[i])
								{
									case 0:
										cf.at(CF_TOD1OctEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 1:
										cf.at(CF_TOD2OctEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 2:
										cf.at(CF_TOD3OctEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 3:
										cf.at(CF_TOD4OctEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 4:
										cf.at(CF_TOD5OctEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 5:
										cf.at(CF_TOD6OctEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 6:
										cf.at(CF_TOD7OctEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 7:
										cf.at(CF_TOD8OctEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 8:
										cf.at(CF_TOD9OctEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
								}
								break;
							case 10:
								cf.at(CF_TODNovEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
								switch ((int)hourly_dispatch[i])
								{
									case 0:
										cf.at(CF_TOD1NovEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 1:
										cf.at(CF_TOD2NovEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 2:
										cf.at(CF_TOD3NovEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 3:
										cf.at(CF_TOD4NovEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 4:
										cf.at(CF_TOD5NovEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 5:
										cf.at(CF_TOD6NovEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 6:
										cf.at(CF_TOD7NovEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 7:
										cf.at(CF_TOD8NovEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 8:
										cf.at(CF_TOD9NovEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
								}
								break;
							case 11:
								cf.at(CF_TODDecEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
								switch ((int)hourly_dispatch[i])
								{
									case 0:
										cf.at(CF_TOD1DecEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 1:
										cf.at(CF_TOD2DecEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 2:
										cf.at(CF_TOD3DecEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 3:
										cf.at(CF_TOD4DecEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 4:
										cf.at(CF_TOD5DecEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 5:
										cf.at(CF_TOD6DecEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 6:
										cf.at(CF_TOD7DecEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 7:
										cf.at(CF_TOD8DecEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
									case 8:
										cf.at(CF_TOD9DecEnergy,y) += hourly_enet[(y-1)*8760+i] * cf.at(CF_Availability,y) * cf.at(CF_Degradation,y);
										break;
								}
								break;
						}
						i++;					
					}
				}
			}

		}

		return true;
	}



};

DEFINE_MODULE_ENTRY( ippppa, "Utility IPP/Commerical PPA Finance model.", 1 );
