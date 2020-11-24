%code requires {
 /*
 * @file spef_parser.y
 * @date 2020-11-02
 * @brief
 *
 * Copyright (C) 2020 NIIC EDA
 *
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 *
 * of the BSD license.  See the LICENSE file for details.
 */

#include <string.h>
#include "util/message.h"
#include "db/core/object.h"
#include "db/timing/spef/spef_reader.h"

typedef void* yyscan_t;

}

%code provides {
#undef  YY_DECL
#define YY_DECL int spef_lex(SPEF_STYPE *yylval_param, yyscan_t yyscanner, SpefReader::SpefReader *spefReader)
YY_DECL;

void spef_error(yyscan_t scanner, SpefReader::SpefReader *spefReader, const char *str);
}

%union {
  char ch;
  char *string;
  int integer;
  float value;
  SpefReader::StringVec *stringVec;
  open_edi::db::Net *net;
  open_edi::db::Pin *pin;
  ObjectId id;
}

%define api.pure full
%define api.prefix {spef_}
%parse-param {yyscan_t yyscanner}
%parse-param { SpefReader::SpefReader* spefReader }
%lex-param   {yyscan_t yyscanner }
%lex-param   { SpefReader::SpefReader* spefReader }

%token SPEF DESIGN DATE PVERSION VENDOR PROGRAM DESIGN_FLOW DIVIDER DELIMITER BUS_DELIMITER 
%token T_UNIT C_UNIT R_UNIT L_UNIT
%token POWER_NETS GROUND_NETS NAME_MAP PORTS PHYSICAL_PORTS COORDINATE CAPLOAD SLEW CELL
%token DEFINE PDEFINE VARI_PARA D_NET RCONF CONN PPORT PINT NINT CAP SC RES INDUC R_NET
%token DRVPIN DRVCELL C2_R1_C1 LOADS RC QPOLE RESIDUE D_PNET R_PNET END

%token INTEGER FLOAT IDENTIFIER NAME INDEX QSTRING

%type <value> FLOAT par_value
%type <value> number pos_number threshold
%type <value> real_component imaginary_component pole poles
%type <value> residue residues complex_par_value cnumber

%type <integer> INTEGER
%type <integer> conf cap_id res_id induc_id cap_elem cap_elems
%type <integer> res_elem res_elems induc_elem induc_elems
%type <integer> pos_integer

%type <string> IDENTIFIER NAME INDEX QSTRING
%type <string> name_or_index net_name net_names inst_name
%type <string> name_map_entries name_map_entry mapped_item
%type <string> physical_inst port_name pport_name port_entry pport_entry
%type <string> port_entries pport_entries pport direction
%type <string> entity cell_type driver_cell pnet_ref
%type <string> pexternal_connection internal_pdspf_node
%type <string> parasitic_node internal_parasitic_node
%type <string> pin_name driver_pair internal_connection external_connection

%type <ch> hchar suffix_bus_delim prefix_bus_delim

%type <stringVec> qstrings

%type<net> net

%start file

%%

/****************************************************************/
prefix_bus_delim:
        '['
        { $$ = '['; }
|       '{'
        { $$ = '}'; }
|       '('
        { $$ = ')'; }
|       '<'
        { $$ = '<'; }
|       ':'
        { $$ = ':'; }
|       '.'
        { $$ = '.'; }
;

suffix_bus_delim:
        ']'
        { $$ = ']'; }
|       '}'
        { $$ = '}'; }
|       ')'
        { $$ = ')'; }
|       '>'
        { $$ = '>'; }
;

hchar:
        '.'
        { $$ = '.'; }
|       '/'
        { $$ = '/'; }
|       '|'
        { $$ = '|'; }
|       ':'
        { $$ = ':'; }
;
/****************************************************************/

file:
	header_def
	name_map
	power_def
	external_def
	define_def
	internal_def
;

/*variation_def*/
/****************************************************************/

header_def:
	spef_version
	design_name
	date
	vendor
	program_name
	program_version
	design_flow
	hierarchy_div_def
	pin_delim_def
	bus_delim_def
        time_scale
        cap_scale
        res_scale
        induc_scale
   
;

spef_version:
	SPEF QSTRING
	{ spefReader->stringDelete($2); }
;

design_name:
	DESIGN QSTRING
	{ spefReader->setCell($2); }
;

date:
	DATE QSTRING
	{ spefReader->stringDelete($2); }
;

program_name:
	PROGRAM QSTRING
	{ spefReader->stringDelete($2); }
;

program_version:
	PVERSION QSTRING
	{ spefReader->stringDelete($2); }
;

vendor:
	VENDOR QSTRING
	{ spefReader->stringDelete($2); }
;

design_flow:
	DESIGN_FLOW qstrings
	{ spefReader->setDesignFlow($2); }
;

qstrings:
	QSTRING
	{ $$ = new SpefReader::StringVec;
	  $$->push_back($1);
	}
|	qstrings QSTRING
	{ $$->push_back($2); }
;

hierarchy_div_def:
	DIVIDER hchar
	{ spefReader->setDivider($2); }
;

pin_delim_def:
	DELIMITER hchar
	{ spefReader->setDelimiter($2); }
;

bus_delim_def:
	BUS_DELIMITER prefix_bus_delim
	{ spefReader->setPreBusDel($2); }
|	BUS_DELIMITER prefix_bus_delim suffix_bus_delim
	{ spefReader->setPreBusDel($2); spefReader->setSufBusDel($3); }
;

time_scale:
	T_UNIT pos_number IDENTIFIER
	{ spefReader->setTimeScale($2, $3); }
;

cap_scale:
	C_UNIT pos_number IDENTIFIER
	{ spefReader->setCapScale($2, $3); }
;

res_scale:
	R_UNIT pos_number IDENTIFIER
	{ spefReader->setResScale($2, $3); }
;

induc_scale:
	L_UNIT pos_number IDENTIFIER
	{ spefReader->setInductScale($2, $3); }
;

/****************************************************************/

name_map:
	/* empty */
|	NAME_MAP name_map_entries
;

name_map_entries:
	name_map_entry
|	name_map_entries name_map_entry
;

name_map_entry:
	INDEX mapped_item
	{ spefReader->addNameMap($1, $2); }
;

mapped_item:
	IDENTIFIER
|	NAME
|	QSTRING
;

/****************************************************************/

power_def:
	/* empty */
|	power_net_def
|	ground_net_def
|	power_net_def ground_net_def
;

power_net_def:
	POWER_NETS net_names
;

ground_net_def:
	GROUND_NETS net_names
;

net_names:
	net_name
|	net_names net_name
;

net_name:
	name_or_index
        { spefReader->stringDelete($1); }
;

/****************************************************************/

external_def:
	/* empty */
|	port_def
|	physical_port_def
|	port_def physical_port_def
;

port_def:
	PORTS port_entries
;

port_entries:
	port_entry
|	port_entries port_entry
;

port_entry:
	port_name direction conn_attrs
	{ spefReader->addPort($1); }
;

direction:
	IDENTIFIER
	{ spefReader->stringDelete($1); }
;

port_name:
	name_or_index
;

inst_name:
	name_or_index
;

physical_port_def:
	PHYSICAL_PORTS pport_entries
;

pport_entries:
	pport_entry
|	pport_entries pport_entry
;

pport_entry:
	pport_name IDENTIFIER conn_attrs
;

pport_name:
	name_or_index
	{ spefReader->stringDelete($1); }
|	physical_inst ':' pport
	{ spefReader->stringDelete($1);
	  spefReader->stringDelete($3);
	}
;

pport:
	name_or_index
;

physical_inst:
	name_or_index
;

/****************************************************************/

conn_attrs:
	/* empty */
|	conn_attrs conn_attr
;

conn_attr:
	coordinates
|	cap_load
|       par_value
|	slews
|	driving_cell
;

coordinates:
	COORDINATE number number
;

cap_load:
	CAPLOAD par_value
;

par_value:
	number
        { $$ = spefReader->addParValue($1); }
|	number ':' number ':' number
        { $$ = spefReader->addParValue($1, $3, $5); }
;

slews:
	SLEW par_value par_value
|	SLEW par_value par_value threshold threshold
;

threshold:
	pos_number
|	pos_number ':' pos_number ':' pos_number
;

driving_cell:
	CELL cell_type
        { spefReader->stringDelete($2); }
;

cell_type:
	IDENTIFIER
|	INDEX
;

/****************************************************************/

define_def:
	/* empty */
|	define_def define_entry
;

define_entry:
	DEFINE inst_name entity
	{ spefReader->stringDelete($2);
	  spefReader->stringDelete($3);
	}
|	DEFINE inst_name inst_name entity
	{ spefReader->stringDelete($2);
	  spefReader->stringDelete($3);
	  spefReader->stringDelete($4);
	}
|	PDEFINE physical_inst entity
	{ spefReader->stringDelete($2);
	  spefReader->stringDelete($3);
	}
;

entity:
	QSTRING
;

/****************************************************************/

/*variation_def :*/
        /* empty */
/*|       define_def define_entry*/
/*;*/

/****************************************************************/
internal_def:
	nets
|	internal_def nets
;

nets:
	d_net
|	r_net
|	d_pnet
|	r_pnet
;

/****************************************************************/

d_net:
	D_NET net total_cap
	{ spefReader->addDNetBegin($2); }
	routing_conf conn_sec cap_sec res_sec induc_sec END
	{ spefReader->addDNetEnd(); }
;

net:
	name_or_index
	{ $$ = spefReader->findNet($1); }
;

total_cap:
	par_value
;

routing_conf:
	/* empty */
|	RCONF conf
;

conf:
	pos_integer
;

/****************************************************************/

conn_sec:
	/* empty */
|	CONN conn_defs internal_node_coords
;

conn_defs:
	/* empty */
|	conn_defs conn_def
;

conn_def:
	PPORT external_connection direction conn_attrs
        { spefReader->addPinNode($2); }
|	PINT internal_connection direction conn_attrs
        { spefReader->addPinNode($2); }
;

external_connection:
	name_or_index
	{ $$ = $1; }
|	physical_inst ':' pport
	{ $$ = $3;
	  spefReader->stringDelete($1);
	}
;

internal_connection:
	pin_name
;

pin_name:
	name_or_index
	{ $$ = $1; }
;

internal_node_coords:
	/* empty */
|	internal_node_coords internal_node_coord
;

internal_node_coord:
	NINT internal_parasitic_node coordinates
;

internal_parasitic_node:
	name_or_index
	{ spefReader->stringDelete($1); }
;

/****************************************************************/

cap_sec:
	/* empty */
|	CAP cap_elems
;

cap_elems:
	/* empty */
	{ $$ = 0; }
|	cap_elems cap_elem
;

cap_elem:
	cap_id parasitic_node par_value
	{ spefReader->addGroundCap($2); }
|	cap_id parasitic_node parasitic_node par_value
	{ spefReader->addCouplingCap($2, $3); }
;

cap_id:
	pos_integer
;

parasitic_node:
	name_or_index
;

/****************************************************************/

res_sec:
	/* empty */
|	RES res_elems
;

res_elems:
	/* empty */
	{ $$ = 0; }
|	res_elems res_elem
;

res_elem:
	res_id parasitic_node parasitic_node par_value
	{ spefReader->addResistor($2, $3); }
;

res_id:
	pos_integer
;

/****************************************************************/

induc_sec:
	/* empty */
|	INDUC induc_elems
;

induc_elems:
	/* empty */
	{ $$ = 0; }
|	induc_elems induc_elem
;

induc_elem:
	induc_id parasitic_node parasitic_node par_value
;

induc_id:
	pos_integer
;

/****************************************************************/

r_net:
	R_NET net total_cap
	{ spefReader->addRNetBegin($2); }
	routing_conf driver_reducs END
	{ spefReader->addRNetEnd(); }
;

driver_reducs:
	/* empty */
|	driver_reducs driver_reduc
;

driver_reduc:
	driver_pair driver_cell pi_model
	{ spefReader->addRNetDrvr($1);
	  spefReader->stringDelete($2);
	}
	load_desc
;

driver_pair:
	DRVCELL pin_name
	{ $$ = $2; }
;

driver_cell:
	CELL cell_type
	{ $$ = $2; }
;

pi_model:
	C2_R1_C1 par_value par_value par_value
	{ spefReader->addPiModel($2, $3, $4); }
;

/****************************************************************/

load_desc:
	LOADS rc_descs
;

rc_descs:
	rc_desc
|	rc_descs rc_desc
;

rc_desc:
	RC pin_name par_value
|	RC pin_name par_value pole_residue_desc
;

pole_residue_desc:
	pole_desc residue_desc
;

pole_desc:
	QPOLE pos_integer poles
;

poles:
	pole
|	poles pole
;

pole:
	complex_par_value
;

complex_par_value:
	cnumber
|	number
|	cnumber ':' cnumber ':' cnumber
|	number ':' number ':' number
;

cnumber:
	'(' real_component imaginary_component ')'
	{ $$ = $2; }
;

real_component:
	number
;

imaginary_component:
	number
;

residue_desc:
	RESIDUE pos_integer residues
;

residues:
	residue
|	residues residue
;

residue:
	complex_par_value
;

/****************************************************************/

d_pnet:
	D_PNET pnet_ref total_cap routing_conf pconn_sec cap_sec res_sec
               induc_sec END
	{ spefReader->stringDelete($2); }
;

pnet_ref:
	name_or_index
;

pconn_sec:
	CONN pconn_defs internal_pnode_coords
;

pconn_defs:
	pconn_def
|	pconn_defs pconn_def
;

pconn_def:
	PPORT pexternal_connection direction conn_attr
|	PINT internal_connection direction conn_attr
;

pexternal_connection:
	pport_name
;

internal_pnode_coords:
	/* empty */
|	internal_pnode_coords internal_pnode_coord
;

internal_pnode_coord:
	NINT internal_pdspf_node coordinates
;

internal_pdspf_node:
	name_or_index
	{
	  spefReader->stringDelete($1);
	  $$ = 0;
	}
;

name_or_index:
	IDENTIFIER
|	NAME
|	INDEX
;

/****************************************************************/

r_pnet:
	R_PNET pnet_ref total_cap routing_conf END
	{ spefReader->stringDelete($2); }
|	R_PNET pnet_ref total_cap routing_conf pdriver_reduc END
	{ spefReader->stringDelete($2); }
;

pdriver_reduc:
	pdriver_pair driver_cell pi_model load_desc
;

pdriver_pair:
	DRVCELL internal_connection
;

/****************************************************************/

number:
	INTEGER
	{ float value = static_cast<float>($1);
          $$ = value; 
        }
|	FLOAT
	{ float value = static_cast<float>($1);
          $$ = value; 
        }
;

pos_integer:
	INTEGER
	{ int value = $1;
	  if (value < 0)
	      open_edi::util::message->issueMsg(
                              open_edi::util::kWarn, "Number is not positive.");
	  $$ = value;
	}
;

pos_number:
	INTEGER
	{ float value = static_cast<float>($1);
	  if (value < 0)
	      open_edi::util::message->issueMsg(
                              open_edi::util::kWarn, "Number is not positive.");
	  $$ = value;
	}
|	FLOAT
	{ float value = static_cast<float>($1);
	  if (value < 0)
	      open_edi::util::message->issueMsg(
                              open_edi::util::kWarn, "Number is not positive.");
	  $$ = value;
	}
;

%%

void spef_error(yyscan_t scanner, SpefReader::SpefReader *spefReader, const char *str)
{
    char errMsg[4096];
    sprintf(errMsg, "Error found in line %lu in SPEF file %s\n", spefReader->getLineNo(), spefReader->getSpefFile().c_str()); 
    open_edi::util::message->issueMsg(open_edi::util::kError, errMsg);
}
