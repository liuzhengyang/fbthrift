/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef T_GLOBALS_H
#define T_GLOBALS_H

#include <set>
#include <queue>
#include <stack>
#include <vector>
#include <string>

/**
 * This module contains all the global variables (slap on the wrist) that are
 * shared throughout the program. The reason for this is to facilitate simple
 * interaction between the parser and the rest of the program. Before calling
 * yyparse(), the main.cc program will make necessary adjustments to these
 * global variables such that the parser does the right thing and puts entries
 * into the right containers, etc.
 *
 */

/**
 * Hooray for forward declaration of types!
 */

class t_program;
class t_scope;
class t_base_type;

/**
 * Parsing mode, two passes up in this gin rummy!
 */

enum PARSE_MODE {
  INCLUDES = 1,
  PROGRAM = 2
};

/**
 * Strictness level
 */
extern int g_strict;

/**
 * The master program parse tree. This is accessed from within the parser code
 * to build up the program elements.
 */
extern t_program* g_program;

/**
 * Global types for the parser to be able to reference
 */

extern t_base_type* g_type_void;
extern t_base_type* g_type_string;
extern t_base_type* g_type_binary;
extern t_base_type* g_type_slist;
extern t_base_type* g_type_bool;
extern t_base_type* g_type_byte;
extern t_base_type* g_type_i16;
extern t_base_type* g_type_i32;
extern t_base_type* g_type_i64;
extern t_base_type* g_type_double;
extern t_base_type* g_type_float;

/**
 * The scope that we are currently parsing into
 */
extern t_scope* g_scope;

/**
 * The parent scope to also load symbols into
 */
extern t_scope* g_parent_scope;

/**
 * The prefix for the parent scope entries
 */
extern std::string g_parent_prefix;

/**
 * The parsing pass that we are on. We do different things on each pass.
 */
extern PARSE_MODE g_parse_mode;

/**
 * Global time string, used in formatting error messages etc.
 */
extern char* g_time_str;

/**
 * The last parsed doctext comment.
 */
extern char* g_doctext;

/**
 * The location of the last parsed doctext comment.
 */
extern int g_doctext_lineno;

/**
 * Whether or not negative field keys are accepted.
 *
 * When a field does not have a user-specified key, thrift automatically
 * assigns a negative value.  However, this is fragile since changes to the
 * file may unintentionally change the key numbering, resulting in a new
 * protocol that is not backwards compatible.
 *
 * When g_allow_neg_field_keys is enabled, users can explicitly specify
 * negative keys.  This way they can write a .thrift file with explicitly
 * specified keys that is still backwards compatible with older .thrift files
 * that did not specify key values.
 */
extern int g_allow_neg_field_keys;

/**
 * Whether or not negative enum values.
 */
extern int g_allow_neg_enum_vals;


/**
 * Whether or not 64-bit constants will generate a warning.
 *
 * Some languages don't support 64-bit constants, but many do, so we can
 * suppress this warning for projects that don't use any non-64-bit-safe
 * languages.
 */
extern int g_allow_64bit_consts;

#endif
