%{
#include <stdio.h>
int yylex(void);
void yyerror(const char *s) { (void)s; }
int bison_dyck_result = 0;
%}

%token TOK_LP TOK_RP TOK_END

%%

input: D TOK_END  { bison_dyck_result = 1; }
     ;

D: /* empty */
 | TOK_LP D TOK_RP D
 ;

%%

static const char * _src2 = NULL;
static int          _pos2 = 0;
static int          _len2 = 0;

void bison_dyck_init(const char * s, int n) {
    _src2 = s; _pos2 = 0; _len2 = n;
    bison_dyck_result = 0;
}

int yylex(void) {
    if (_pos2 >= _len2) return TOK_END;
    char c = _src2[_pos2++];
    if (c == '(') return TOK_LP;
    if (c == ')') return TOK_RP;
    return -1;
}
