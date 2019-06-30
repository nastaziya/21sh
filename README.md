# 21sh
<b>Version 2.0 of my bash implementation.</b>

The third project of the system branch, and the first one to be team-based.

The purpose ? To have learned from the mistakes made during the Minishell’s development and do it again, from scratch !

This time, I had to learn good practices about lexing, parsing, expansion and execution in order to build an AST.

Execution has also become more difficult with the obligation to manage fd redirections : <, <&, >&, >, <>&, pipes and heredocs.

Furthemore, I had to take control of the terminal capabilities (termcaps). The shell doesn’t rely on a readline function : every key touched is being catched by my program and processed accordingly.

From this point on, the shell fully supports signal management.
