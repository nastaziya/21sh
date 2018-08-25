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

void      add_token_to_lexer(t_lexer* lexer, const char* text, int text_size, e_token_type type)
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

t_oplist  type_of_token(const char* s)
{
  const t_oplist *ex_tok;
  t_oplist not_found;
  int i = 0;

  ex_tok = existing_token;
  not_found = (t_oplist){0, 0, 0};
 
  while (ex_tok && ex_tok->op)
  {
    if (ft_strncmp(s, ex_tok->op, ex_tok->size) == 0)
      return *ex_tok;
    ++ex_tok;
  }
  return (not_found);
}

int       string_to_lexer(const char* s, t_lexer* lexer)
{
  t_oplist    current;
  const char* prev;

  prev = s;
  while (s && *s)
  {
    if (*s == '\\')
    {
      ++s;
      continue;
    }
    current = type_of_token(s);
    if ((*s == '>' || *s == '<') && ft_isdigit(*(s - 1))) // ajoute d'IO number dans les tokens (ex : 2>)
    {
      add_token_to_lexer(lexer, prev, s - prev, T_IO_NUMB);
     // ++s;
    }
    else if ((current.op != 0 || *s == '"' || *s == '\'') && prev != s)
      add_token_to_lexer(lexer, prev, s - prev, T_WORD);
    if (current.op != 0)
    {
      s += current.size;
      if (current.type != T_EAT)
        add_token_to_lexer(lexer, current.op, current.size, current.type);
      prev = s;
    }
    else if (*s == '"' || *s == '\'')
    {
      ++s;
      while (*s && *s != '\'' && *s != '"')
        ++s;
      if (!*s || (*s != '\'' && *s != '"'))
      {
        return 0;
      }
      ++s;
    }
    else
      ++s;
  }
  if (prev != s)
    add_token_to_lexer(lexer, prev, s - prev, T_WORD);
  return 1;
}

void      print(const t_lexer* lexer)
{
  int i = 0;

  for (i = 0; i < lexer->used_size; ++i)
    printf("{ %s (%i) } ", lexer->tokens[i].content, lexer->tokens[i].type);
  printf("\n");
}

t_lexer   final_tokens()
{
  char* cmd = " ls 10> hoho a.out 2>> io && hei & uo";

  t_lexer lexer;
  lexer_init(&lexer);

  if (!string_to_lexer(cmd, &lexer))
    printf("error !\n");

  //print(&lexer);
  //free_the_content_array_token(&lexer); free lexer 
  return lexer;
}