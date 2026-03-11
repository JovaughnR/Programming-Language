#ifndef PARSER_API_H
#define PARSER_API_H

#include "../core/lib/list.h"

/* From parser.y */
extern int yyparse(void);
extern List *global_statements;

/* From lex.yy.c — flex buffer API */
typedef struct yy_buffer_state *YY_BUFFER_STATE;

extern YY_BUFFER_STATE yy_scan_string(const char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buf);
extern void yy_switch_to_buffer(YY_BUFFER_STATE buf);
extern YY_BUFFER_STATE get_current_buffer(void);

#endif