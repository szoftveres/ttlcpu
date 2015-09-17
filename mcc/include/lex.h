#ifndef _LEX_H_
#define _LEX_H_

#define MAX_TOKEN_SIZE          128


enum {
    T_NONE,
    T_EOF,
    T_IDENTIFIER,               /* [a-zA-Z_][a-zA-Z0-9_] */
    T_INTEGER,                  /* [0-9] */

    T_CHAR_START,               /* ' */
    T_CHAR_CONTENT,             /* '. */
    T_CHAR_SPECIAL,           /* \n \t */
    T_CHAR,                     /* '.' */

    T_STRING_START,             /* " */
    T_STRING_CONTENT,           /* ".* */
    T_STRING_SPECIAL,           /* \n \t */
    T_STRING,                   /* ".*" */

    T_AT,                       /* @ */
    T_SEMICOLON,                /* ; */
    T_COLON,                    /* : */
    T_QUESTIONMARK,             /* ? */
    T_LEFT_PARENTH,             /* ( */
    T_RIGHT_PARENTH,            /* ) */
    T_LEFT_SQUARE_BRACKET,      /* [ */
    T_RIGHT_SQUARE_BRACKET,     /* ] */
    T_LEFT_BRACE,               /* { */
    T_RIGHT_BRACE,              /* } */
    T_COMMA,                    /* , */
    T_FULLSTOP,                 /* . */
    T_ASSIGN,                   /* = */
    T_NEG,                      /* ! */
    T_LESS,                     /* < */
    T_GREATER,                  /* > */
    T_PLUS,                     /* + */
    T_MINUS,                    /* - */
    T_MUL,                      /* * */
    T_DIV,                      /* / */
    T_MOD,                      /* % */
    T_EQ,                       /* == */
    T_NEQ,                      /* != */
    T_LEQ,                      /* <= */
    T_GREQ,                     /* >= */
    
    T_SLEFT,                    /* << */
    T_SRIGHT,                   /* >> */

	T_ARROW,                    /* -> */

    T_RECURADD,                 /* += */
    T_RECURSUB,                 /* -= */
    T_RECURMUL,                 /* *= */
    T_RECURDIV,                 /* /= */
    T_RECURBWOR,                /* |= */
    T_RECURBWAND,               /* &= */
    T_RECURBWXOR,               /* ^= */

    T_BWXOR,                    /* ^ */
    T_BWNEG,                    /* ~ */
    T_BWAND,                    /* & */
    T_LAND,                     /* && */
    T_BWOR,                     /* | */
    T_LOR,                      /* || */

};


extern char     lexeme[];
extern int      token;

void lex_init (void);
void get_token (void);
void str_process (void);

#endif
