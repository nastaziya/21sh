#include "../../inc/sh.h"


void      free_the_content_array_token(t_lexer* lexer)
{
  int i;

  i = -1;
  while (++i < lexer->used_size)
    free(lexer->tokens[i].content);
  free(lexer->tokens);
}

void      lexer_init(t_lexer *lexer)
{
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
  int         quote_done;
  const char* prev;
  char        type_quote;

  quote_done = 0;
  prev = s;
  while (s && *s)
  {
    current = type_of_token(s);
    // Lex the redirection
    if ((*s == '>' || *s == '<') && ft_isdigit(*(s - 1)))
      add_token_to_lexer(lexer, prev, s - prev, T_IO_NUMB);
    // Tokenize properly the quotes
    else if ((*s == '"' || *s == '\'') && *(s - 1) != '\\')
    {
      prev = s;
      type_quote = *s;
      while (*s && ++s)
        if ((*s == type_quote && *(s - 1) != '\\' && type_quote != '\'') || (type_quote == '\'' && *s == '\''))
              break;
      //if there are words after the closing quote, continue
      // printf("<char if qui bug : %c >", *s);
      if (!(*(s+1) >= 8 && *(s+1) <= 13) && *(s+1) != 32)
      {
        ++s;
        //loops until the end of the word, and if encounters '\n' after \, skip it 
        // which is only when the user presses enter and continues the string afterward
        // => (*s == '\n' && *(s-1) == '\\' ? ++s : s) part
        while ((*s == '\n' && *(s-1) == '\\' ? ++s : s) && !(*s >= 8 && *s <= 13) && *s != 32 && *s)
          ++s;
        // fix bug : oui oui " oui " --> dubble tokenization
        s = (!*s ? --s : s);
      }
      // What is under, important to avoid tokenizing too far in the string (when i go too far)
      // if (prev != s)//&& (*s || *(s - 1)) //&& *prev
      if (prev != s)//&& *(s-1) != '"'
      {
            // printf("je suis passe |%s - %s|", prev, s);
            // printf("je suis passe |%s|", prev);
            ++s;
            add_token_to_lexer(lexer, prev, s - prev, T_WORD);
                  // prev = s;
                  // ++s;
      }
      quote_done = 1;
    }
    // Management of the \ at the end of the string, when not inside the word sticking the quotes : " bla"oui\ <-
    else if (*s == '\n' && *(s-1) == '\\')
    {
      // printf(" oups la gaffe ");
      ++s;
      continue ;
    }
    // Tokenize the normal word
    else if (current.op != 0 && prev != s)
    {
            // printf(" je suis passe merde");

        //  printf("je suis passe |%s - %s|", prev, s);
            add_token_to_lexer(lexer, prev, s - prev, T_WORD);
    }
    // Allows to advance and tokenize everything that is not a word.
    // when quotes are present, I use it to advance too because it won't lex
    if (current.op != 0 || quote_done == 1)
    {
      // printf(" je suis la-, current.size: %d", current.size);
      s += current.size;
      if (current.type != T_EAT)
      {
        // printf("Fait chier");
        add_token_to_lexer(lexer, current.op, current.size, current.type);
      }
      prev = s;
      // printf("youhou: |%s| ---", prev);
      quote_done = 0;
    }
    else
    {
      // printf("++s final");
    // If nothing has been found, advance to the next char
      ++s;
    }
  }
  // Last tokenization if there are things left
  if (prev != s && *(s-1) != '"')
  {
    // printf(" jamais ici");
    add_token_to_lexer(lexer, prev, s - prev, T_WORD);
  }
  return (1);
}

void      print(const t_lexer* lexer)
{
  int i = 0;

  for (i = 0; i < lexer->used_size; ++i)
    printf("{ %s (%i) } ", lexer->tokens[i].content, lexer->tokens[i].type);
  printf("\n");
}

/*
*** - Aim of the function :
*** - The aim is to check if impair number of quotes :
*** -  " ' (impair or pair number of dquotes or squotes)
*** - if everything's allright, returns 0
*** - Otherwise, returns the corresponding char
*** - And checks if the string ends by \ -> if yes, returns \
*/
char         ft_count_quote(char *str)
{
  char    type_quote;

  type_quote = 0;
  while (str && *str)
  {
    if ((*str == '"' || *str == '\'' || *str == '`') && *(str - 1) != '\\')
    {
      type_quote = *str;
      while (++str && *str)
        if ((*str == type_quote && *(str - 1) != '\\' && type_quote != '\'')
          || (type_quote == '\'' && *str == '\''))
          break;
      if (!*str)
        return (type_quote);
      else
        ++str;
    }
    else
      ++str;
  }
  if (!*str && *(str - 1) == '\\')
    return ('\\');
  return (0);
}

/*
*** - Aim of the function :
*** - Print the corresponding prompt according the corresponding error
*/
void   ft_manage_prompt(char type_quote)
{
  if (type_quote == '"')
    ft_putstr_fd("dquote > ", 1);
  else if (type_quote == '\'')
    ft_putstr_fd("squote > ", 1);
  else if (type_quote == '\\')
    ft_putstr_fd("> ", 1);
}

/*
*** - Aim of the function :
*** - Print new promt and collect from standard entry
*** - Then reallocs everything to get the new finished string
*** - And checks if the closing matching quote is found
*** - if yes, end, otherwise, the loop continues
*/
void    ft_new_prompt(char **cmd, char type_quote)
{
  int   ret;
  char  *line;
  char  *tmp;

  line = NULL;
  while (42)
  {
    ft_manage_prompt(type_quote);
    ret = get_next_line(0, &line);
    if (line && ft_strlen(line) > 0)
    {
      tmp = *cmd;
      *cmd = ft_strjoin(tmp, "\n");
      free(tmp);
      tmp = *cmd;
      *cmd = ft_strjoin(tmp, line);
      free(tmp);
    }
    // => First part of the condition (if) checks if the quotes are now closed
    // => And also checks if the \ is present again at the end of the line
    if (!(type_quote = ft_count_quote(*cmd)) || (ft_strlen(line) == 0 && type_quote == '\\'))
      break;
    free(line);
  }
}

/*
*** - Aim of the function :
*** - First GNL that collects the line on the standard entry
*** - then checks if dquote or squote is needed (ft_manage_dquote)
*** - then checks if the line ends with \
*** - If yes for any of the above, print the next then parse the quotes
*/
void    ft_get_entire_line(char **cmd, char *str)
// void    ft_get_entire_line(char **cmd)
{
  int   ret;
  char  type_quote;

  ft_putstr_fd(str, 1);
  ret = get_next_line(0, cmd);
  if (ret == 0)
  {
    free(*cmd);
    exit(1);
  }
  else if (*cmd && ft_strlen(*cmd) > 0)
      if ((type_quote = ft_count_quote(*cmd)))
          ft_new_prompt(cmd, type_quote);
}


/*
*** - Aim of the function :
*** - Function that manages if the lexing ends up with && || |
*** - if that's the case, we need to show a new prompt, collect a new string
*** -  and lex it
*/
int     ft_manage_string_to_lexer(const char* s, t_lexer* lexer)
{
  char    *cmd;

  if (!string_to_lexer(s, lexer))
    return (0);
  while (42)
  {
  //   printf("debug- manage string: |%s|\n", lexer->tokens[lexer->used_size - 1].content);
    if (!ft_strcmp(lexer->tokens[lexer->used_size - 1].content, "|")
      || !ft_strcmp(lexer->tokens[lexer->used_size - 1].content, "&&")
        || !ft_strcmp(lexer->tokens[lexer->used_size - 1].content, "||"))
      {
        ft_get_entire_line(&cmd, "debug > ");
        if (cmd && ft_strlen(cmd) > 0)
          if (!string_to_lexer(cmd, lexer))
            printf("error !\n");
        free(cmd);
      }
    else
      break;
  }
  // printf("debug- manage string: |%s|\n", lexer->tokens[lexer->used_size - 1].content);
  return (1);
}


/*
*** - Aim of the function :
*** - Function that manages first the collect of the line : ft_get_entire_line
*** - then initialize the lexer : lexer_init
*** - and fills it : string_to_lexer
*/
t_lexer   final_tokens(void)
// void   final_tokens(t_lexer *lexer)
{
  //ls -a -l > | blabla (a reparer)
  // char* cmd = "ls -a -l | uouo"; 
  char    *cmd;
  t_lexer lexer;

  // ft_get_entire_line(&cmd);
  ft_get_entire_line(&cmd, "bash > ");
  lexer_init(&lexer);
  if (cmd && ft_strlen(cmd) > 0)
    if (!ft_manage_string_to_lexer(cmd, &lexer))
      printf("error !\n");
    // if (!string_to_lexer(cmd, &lexer))
    //   printf("error !\n");
  
  free(cmd);
  return lexer;
}