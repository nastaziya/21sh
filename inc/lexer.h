#ifndef LEXER_H
# define LEXER_H

# include <stdio.h>
# include <string.h>
#define LEXER_INITIAL_CAPACITY 100

    typedef enum
      {
        //space etc
        T_EAT,
        //T_DQ,
        //T_SQ,
        T_DBL_SEMI,
        // separator
        T_NEG,
        T_DBLAND,
        T_DBLOR,
        T_PIPE,
        T_SEMI,
        T_AND,
        T_OP_ARITHM,
        T_OP_BRA,
        T_CL_BRA,
        T_OP_PAR,
        T_CL_PAR,
        //redirections
        T_GREAT,
        T_DBL_GREAT,
        T_LESS,
        T_DBL_LESS,
        T_REDIR,
        T_ASS_WORD,
        T_IO_NUMB,
        T_OP_CMD,
        T_OP_CURLY,
        T_CL_CURLY,
        T_EQUAL,
        T_STRING,
        T_WORD,
        T_FOR,
        T_WHILE,
        T_NULL,
        T_T
      } e_token_type;

    typedef struct s_oplist
    {
      const char*   op;
      int           size;
      e_token_type  type;
    }              t_oplist;

    typedef struct s_lexer_token
    {
      char          *content;
      int           size;
      e_token_type  type;
    }              t_lexer_token;

    typedef struct
    {
      t_lexer_token *tokens;
      int used_size;
      int capacity;
    } t_lexer;

    static const t_oplist existing_token[] =
      {
       // {"while", 5, T_WHILE},
        {"$((", 3, T_OP_ARITHM},
        {">>-", 3, T_REDIR},
       // {"for", 3, T_FOR},
        {"$(", 2, T_OP_CMD},
        {"${", 2, T_OP_CURLY},
        {">>", 2, T_DBL_GREAT},
        {"<<", 2, T_DBL_LESS},
        {"||", 2, T_DBLOR},
        {">|", 2, T_REDIR},
        {"<>", 2, T_REDIR},
        {"<&", 2, T_REDIR},
        {">&", 2, T_REDIR},
        {"&&", 2, T_DBLAND},
        {"{ ", 2, T_OP_BRA},
        {"{\n", 2, T_OP_BRA},
        {"{\v", 2, T_OP_BRA},
        {"{\t", 2, T_OP_BRA},
        {"{\r", 2, T_OP_BRA},
        {"{\f", 2, T_OP_BRA},
        {" {", 2, T_CL_BRA},
        {"\n{", 2, T_CL_BRA},
        {"\v{", 2, T_CL_BRA},
        {"\t{", 2, T_CL_BRA},
        {"\r{", 2, T_CL_BRA},
        {"\f{", 2, T_CL_BRA},
        {"))", 2, T_T},
        {";;", 2, T_DBL_SEMI},
        {"! ", 2, T_NEG},
        {"}", 1, T_CL_BRA},
        {"|", 1, T_PIPE},
        {"&", 1, T_AND},
        {"(", 1, T_OP_PAR},
        {")", 1, T_CL_PAR},
        {">", 1, T_GREAT},
        {"<", 1, T_LESS},
        {"(", 1, T_OP_PAR},
        {")", 1, T_CL_PAR},
        {"{", 1, T_OP_CURLY},
        {"}", 1, T_CL_CURLY},
        {";", 1, T_SEMI},
        {" ", 1, T_EAT},
        {"\n", 1, T_EAT},
        {"\v", 1, T_EAT},
        {"\t", 1, T_EAT},
        {"\r", 1, T_EAT},
        {"\f", 1, T_EAT},
        {"=", 1, T_EQUAL},
        {NULL, 1, 0}
      };


  int       string_to_lexer(const char* s, t_lexer* lexer);
  t_lexer   final_tokens();
  void      print(const t_lexer* lexer);
  void      free_the_content_array_token(t_lexer* lexer);

  #endif