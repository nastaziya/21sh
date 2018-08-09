#include "../inc/sh.h"


void      free_the_content_array_token(t_lexer* lexer)
{
  int i;

  i = -1;
  while (++i < lexer->used_size)
    free(lexer->tokens[i].content);
  free(lexer->tokens);
}

void lexer_init(t_lexer *lexer) {
  lexer->used_size = 0;
  lexer->capacity = LEXER_INITIAL_CAPACITY;
  lexer->tokens = malloc(sizeof(t_lexer_token) * lexer->capacity);
}

void      addToLexer(t_lexer* lexer, const char* text, int text_size, e_token_type type)
{
  t_lexer_token item;
  t_lexer_token *temp;
  int i;

  i = -1;
  item.content = ft_strndup(text, text_size);
  item.content[text_size] = 0;
  item.size = text_size;
  item.type = type;
  if (lexer->used_size == lexer->capacity)
  {
      temp = lexer->tokens;
      lexer->capacity = (lexer->used_size * 3) / 2 + 1;
      lexer->tokens = malloc(sizeof(t_lexer_token) * lexer->capacity + 1);
      if (lexer->tokens == NULL)
        exit(EXIT_FAILURE);
      while(++i < lexer->used_size)
        lexer->tokens[i] = temp[i];
      free(temp);
  }
  lexer->tokens[lexer->used_size] = item;
  ++lexer->used_size;
}

t_oplist  searchTokenType(const char* s)
{
  const t_oplist* ex_tok = existing_token;
  t_oplist not_found = {0, 0, 0};

  while (ex_tok && ex_tok->op)
  {
    if (ft_strncmp(s, ex_tok->op, ex_tok->size) == 0)
      return *ex_tok;
    ++ex_tok;
  }

  return not_found;
}

int       fillLexerFromString(const char* s, t_lexer* lexer)
{
  t_oplist    current;
  const char* prev = s; /* Previous time we encountered a known token. */

  while (s && *s)
  {
    /* Handle \\ escaping */
    if (*s == '\\')
    {
      ++s;
      continue;
    }

    /* Search any know token */
    current = searchTokenType(s);
    /*
       if current.type == T_OP_*
       add +1 a counter !
       if current.type == T_CL_*
       dec -1 a counter !
       At the end, check if the counter is equal to 0.
       If not, there is a syntax error!
    */

    /* Find another token ? let's see if we have something inbetween */
    if ((current.op != 0 || *s == '"' || *s == '\'') && prev != s)
      addToLexer(lexer, prev, s - prev, T_NAME);
    /* Token found */
    if (current.op != 0)
    {
      s += current.size;
      if (current.type != T_EAT)
        addToLexer(lexer, current.op, current.size, current.type);
      prev = s;
    }
    /* Token not found, but could be an escaped string */
    else if (*s == '"' || *s == '\'')
    {
      /* handleEscaping(); Handle it for real */
      ++s;
      while (*s && *s != '\'' && *s != '"') /* <- dumb function */
        ++s;
      if (!*s || (*s != '\'' && *s != '"'))
      {
        /* string not finished! */
        return 0;
      }
      ++s;
    }
    else
      ++s;
  }

  /* Handle what's remain at the end of the input */
  if (prev != s)
    addToLexer(lexer, prev, s - prev, T_NAME);

  return 1;
}

void      print(const t_lexer* lexer)
{
  int i = 0;

  for (i = 0; i < lexer->used_size; ++i)
    printf("<%s (%i)> ", lexer->tokens[i].content, lexer->tokens[i].type);
  printf("\n");
}

int       main()
{
  const char* cmd = "ls ; echo hei";

  t_lexer lexer;
  lexer_init(&lexer);

  if (!fillLexerFromString(cmd, &lexer))
    printf("Syntax error !\n");

  print(&lexer);
  free_the_content_array_token(&lexer);
  return 0;
}
