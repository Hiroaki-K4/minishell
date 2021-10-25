#include "minishell.h"


char	*delete_space(char *command)
{
	char	*trimed1;
	char	*trimed2;

	trimed1 = ft_strtrim(command, " ");
	trimed2 = ft_strtrim(trimed1, "\f");
	free(trimed1);
	trimed1 = ft_strtrim(trimed2, "\n");
	free(trimed2);
	trimed2 = ft_strtrim(trimed1, "\r");
	free(trimed1);
	trimed1 = ft_strtrim(trimed2, "\t");
	free(trimed2);
	trimed2 = ft_strtrim(trimed1, "\v");
	return (trimed2);
}

int	lexer(char *line)
{
	char *trimed;

	trimed = delete_space(line);
	free(line);
	printf("lexer: %s\n", trimed);
	return (0);
}