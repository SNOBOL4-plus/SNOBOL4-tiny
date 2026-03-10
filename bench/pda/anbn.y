%{
#include <stdio.h>
#include <string.h>
int yylex(void);
void yyerror(const char *s) { (void)s; }
int bison_anbn_result = 0;
%}

%token TOK_A TOK_B TOK_END

%%

input: S TOK_END  { bison_anbn_result = 1; }
     ;

S: TOK_A S TOK_B
 | TOK_A TOK_B
 ;

%%

/* Token stream fed from a string */
static const char * _src = NULL;
static int          _pos = 0;
static int          _len = 0;

void bison_anbn_init(const char * s, int n) {
    _src = s; _pos = 0; _len = n;
    bison_anbn_result = 0;
}

int yylex(void) {
    if (_pos >= _len) return TOK_END;
    char c = _src[_pos++];
    if (c == 'a') return TOK_A;
    if (c == 'b') return TOK_B;
    return -1;
}
