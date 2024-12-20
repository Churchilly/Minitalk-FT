#include <stdlib.h>
#include <unistd.h>
#include "minitalk_bonus.h"

t_list	*list_new(char c)
{
    t_list *list = malloc(sizeof(t_list));
    if (!list)
        exit(EXIT_FAILURE);
    list->content = c;
    list->next = NULL;
    return list;
}

void list_add_back(t_list **head, t_list *new_node)
{
    if (!head || !new_node)
        return;
    if (!*head) {
        *head = new_node;
    } else {
        t_list *temp = *head;
        while (temp->next)
            temp = temp->next;
        temp->next = new_node;
    }
}

void write_list(t_list *head)
{
    while (head) {
        write(1, &head->content, 1);
        head = head->next;
    }
	write(1, "\n", 1);
}

void clear_list(t_list **head)
{
    t_list *temp;
    while (*head) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}