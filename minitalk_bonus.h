#ifndef MINITALK_BONUS_H
# define MINITALK_BONUS_H

typedef struct	s_list
{
	char			content;
	struct s_list	*next;
}	t_list;

t_list	*list_new(char c);
void	list_add_back(t_list **head, t_list *new_node);
void	write_list(t_list *head);
void	clear_list(t_list **head);

#endif