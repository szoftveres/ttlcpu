#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lex.h"


static int     last_char;

static char*  pointer;

char     lexeme[MAX_TOKEN_SIZE];
int      token;

void pushchar (int c) {
    if (last_char == EOF) {
        last_char = c;
    } else {
        fprintf(stderr, "[%s:%d]: error\n", __FUNCTION__, __LINE__);
        exit (1);
    }
    return;
}

int popchar (void) {
    int c;
    if (last_char != EOF) {
        c = last_char;
        last_char = EOF;
    } else {
        c = getchar();
    }
    return c;
}

/**
*/
int wspace (char c) {
    switch (c) {
      case ' ' : case '\t' : case '\n' : case '\r' :
        return 1;
    }
    return 0;
}

int num (char c) {
    switch (c) {
      case '0' : case '1' : case '2' : case '3' : case '4' :
      case '5' : case '6' : case '7' : case '8' : case '9' :
        return 1;
    }
    return 0;
}

int alpha (char c) {
    switch (c) {
      case 'a' : case 'b' : case 'c' : case 'd' : case 'e' :
      case 'f' : case 'g' : case 'h' : case 'i' : case 'j' :
      case 'k' : case 'l' : case 'm' : case 'n' : case 'o' :
      case 'p' : case 'q' : case 'r' : case 's' : case 't' :
      case 'u' : case 'v' : case 'w' : case 'x' : case 'y' :
      case 'z' :
      case 'A' : case 'B' : case 'C' : case 'D' : case 'E' :
      case 'F' : case 'G' : case 'H' : case 'I' : case 'J' :
      case 'K' : case 'L' : case 'M' : case 'N' : case 'O' :
      case 'P' : case 'Q' : case 'R' : case 'S' : case 'T' :
      case 'U' : case 'V' : case 'W' : case 'X' : case 'Y' :
      case 'Z' :
      case '_' :
        return 1;
    }
    return 0;
}


/**
*/
int lex (char c) {

    if (c == '\\') {
        switch (token) {
          case T_CHAR_START :
          case T_CHAR_CONTENT :
            token = T_CHAR_SPECIAL;
            return 1;
          case T_STRING_START :
          case T_STRING_CONTENT :
            token = T_STRING_SPECIAL;
            return 1;
        }
        fprintf(stderr, "syntax error : unexpected '\\' \n");
        exit (1);
    }

    if (c == '\'') {
        switch (token) {
          case T_NONE :
            token = T_CHAR_START;
            return 1;   /* ok, continue */
          case T_CHAR_CONTENT :
            token = T_CHAR;
            return 1;   /* seems like finished */
        }
    }
    if (c == '\"') {
        switch (token) {
          case T_NONE :
            token = T_STRING_START;
            return 1;   /* ok, continue */
          case T_STRING_SPECIAL :
            token = T_STRING_CONTENT;
            return 1;   /* ok, continue */
          case T_STRING_CONTENT :
            token = T_STRING;
            return 1;   /* seems like finished */
        }
    }
    /* wspace */
    if (wspace(c)) {
        switch (token) {
          case T_CHAR_START :
            token = T_CHAR_CONTENT;
            return 1;   /* ok, continue */
          case T_STRING_START :
          case T_STRING_SPECIAL :
          case T_STRING_CONTENT :
            token = T_STRING_CONTENT;
            return 1;   /* ok, continue */
        }
        return 0;
    }
    /* num : integer or identifier */
    if (num(c)) {
        switch (token) {
          case T_INTEGER :
          case T_IDENTIFIER :
            return 1; /* ok, continue */
          case T_NONE :
            token = T_INTEGER;
            return 1;
          case T_CHAR_START :
            token = T_CHAR_CONTENT;
            return 1;   /* ok, continue */
          case T_STRING_START :
          case T_STRING_SPECIAL :
          case T_STRING_CONTENT :
            token = T_STRING_CONTENT;
            return 1;   /* ok, continue */
          default :
            return 0; /* other token begins */
        }
    }
    /* alpha : identifier */
    if (alpha(c)) {
        switch (token) {
          case T_IDENTIFIER:
            return 1; /* ok, continue */
          case T_NONE :
            token = T_IDENTIFIER;
            return 1;
          case T_INTEGER:
            fprintf(stderr, "error : syntax error\n");
            exit (1);
          case T_CHAR_START :
            token = T_CHAR_CONTENT;
            return 1;   /* ok, continue */
          case T_STRING_START :
          case T_STRING_SPECIAL :
          case T_STRING_CONTENT :
            token = T_STRING_CONTENT;
            return 1;   /* ok, continue */
          default:
            return 0;  /* other token begins, return */
        }
    }
    /* two-char operators */
    switch (c) {
      case '=' :
        switch (token) {
          case  T_ASSIGN :
            token = T_EQ;          /* == */
            return 1;
          case T_NEG :
            token = T_NEQ;         /* != */
            return 1;
          case T_LESS :
            token = T_LEQ;         /* <= */
            return 1;
          case T_GREATER :
            token = T_GREQ;        /* >= */
            return 1;
          case T_PLUS :
            token = T_RECURADD;    /* += */
            return 1;
          case T_MINUS :
            token = T_RECURSUB;    /* -= */
            return 1;
          case T_MUL :
            token = T_RECURMUL;    /* *= */
            return 1;
          case T_DIV :
            token = T_RECURDIV;    /* /= */
            return 1;
          case T_BWAND :
            token = T_RECURBWAND;  /* &= */
            return 1;
          case T_BWOR :
            token = T_RECURBWOR;   /* |= */
            return 1;
          case T_BWXOR :
            token = T_RECURBWXOR;  /* ^= */
            return 1;
        }
        break;
      case '&' :
        switch (token) {
          case T_BWAND :
            token = T_LAND;        /* && */
            return 1;
        }
        break;
      case '|' :
        switch (token) {
          case T_BWOR :
            token = T_LOR;         /* || */
            return 1;
        }
        break;
      case '<' :
        switch (token) {
          case T_LESS :                 /* << */
            token = T_SLEFT;
            return 1;
        }
        break;
      case '>' :
        switch (token) {
          case T_GREATER :              /* >> */
            token = T_SRIGHT;
            return 1;
		  case T_MINUS :
            token = T_ARROW;            /* -> */
            return 1;
        }
        break;
    }

    switch (token) {
      case T_CHAR_START :
        token = T_CHAR_CONTENT;
        return 1;   /* ok, continue */
      case T_STRING_START :
      case T_STRING_SPECIAL :
      case T_STRING_CONTENT :
        token = T_STRING_CONTENT;
        return 1;   /* ok, continue */
    }

    if (token != T_NONE) {
        return 0;                       /* Other token begins, return */
    }

    switch (c) {
      case ';' : token = T_SEMICOLON; return 1;
      case ':' : token = T_COLON; return 1;
      case '?' : token = T_QUESTIONMARK; return 1;
      case '(' : token = T_LEFT_PARENTH; return 1;
      case ')' : token = T_RIGHT_PARENTH; return 1;
      case '[' : token = T_LEFT_SQUARE_BRACKET; return 1;
      case ']' : token = T_RIGHT_SQUARE_BRACKET; return 1;
      case '{' : token = T_LEFT_BRACE; return 1;
      case '}' : token = T_RIGHT_BRACE; return 1;
      case ',' : token = T_COMMA; return 1;
      case '.' : token = T_FULLSTOP; return 1;
      case '+' : token = T_PLUS; return 1;
      case '-' : token = T_MINUS; return 1;
      case '*' : token = T_MUL; return 1;
      case '/' : token = T_DIV; return 1;
      case '%' : token = T_MOD; return 1;
      case '=' : token = T_ASSIGN; return 1;
      case '<' : token = T_LESS; return 1;
      case '>' : token = T_GREATER; return 1;
      case '!' : token = T_NEG; return 1;
      case '@' : token = T_AT; return 1;
      case '&' : token = T_BWAND; return 1;
      case '^' : token = T_BWXOR; return 1;
      case '~' : token = T_BWNEG; return 1;
      case '|' : token = T_BWOR; return 1;
      case EOF : token = T_EOF; return 1;
    }

    fprintf(stderr, "error : illegal character: %c\n", c);
    exit(1);
}

/**
*/
void get_token (void) {
    int c;

    token = T_NONE;
    pointer = lexeme;
    while (1) {
        c = popchar();
        if (!lex((char)c)) {
            if (token != T_NONE) {
                pushchar(c);
                *pointer = '\0';
                /* fprintf(stdout, ";;      %s    \n", token); */
                return;
            }
            continue; /* It was a trailing whitespace */
        }
        *pointer++ = (char)c;
    }
}


void lex_init (void) {
    last_char = EOF;
    get_token();
}


void str_process(void) {
    int tp;
    int bp;
    char buf[MAX_TOKEN_SIZE];
    if (token != T_STRING) {
        return;
    }
    tp = 0;
    bp = 0;
    while ((lexeme[tp]) && (tp < MAX_TOKEN_SIZE)) {
        switch (lexeme[tp]) {
          case '\"' :
            break;
          case '\\' :
            tp++;
            switch (lexeme[tp]) {
              case 'n' :
                buf[bp] = '\n';
                break;
              case 'r' :
                buf[bp] = '\r';
                break;
              case 't' :
                buf[bp] = '\t';
                break;
              case '\\' :
                buf[bp] = '\\';
                break;
              case '\"' :
                buf[bp] = '\"';
                break;
              case '\'' :
                buf[bp] = '\'';
                break;
              default :
                fprintf(stderr, "error : unknown esc seq: \\%c\n", lexeme[tp]);
                exit(1);
                break;
            }
            bp++;
            break;
          default :
            buf[bp] = lexeme[tp];
            bp++;
            break;
        }
        tp++;
    }
    buf[bp] = '\0';
    strcpy(lexeme, buf);
}

