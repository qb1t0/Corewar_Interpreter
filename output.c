#include "asm.h"

/*
** Final command write here:
*/

void cw_write_code(t_cmnd *tmp, int fd)
{
    int i;
    int type;

    while (tmp)
    {
        i = 1;
        char k = tmp->cipher << 4;
        write(fd, &g_tab[tmp->cmd].num, 1);
        g_tab[tmp->cmd].id1 == 1 ? write(fd, &(k), 1) : 0;
        while (i <= g_tab[tmp->cmd].arg)
        {
            type = tmp->cipher >> (g_tab[tmp->cmd].arg - i) * 2;
            type &= 3;
            if (type == 1)                                                     //if T_REG
                write(fd, &(tmp->i_arg[i - 1]), 1);
            else if (type == 2 && !g_tab[tmp->cmd].id2)
                write(fd, &(tmp->i_arg[i - 1]), 4);
            else if (type == 3 || type == 2)
                write(fd, &(tmp->i_arg[i - 1]), 2);
            i++;
        }
        tmp = tmp->prev;
    }
}

int cw_write_all(char *name, t_cmnd *tmp)
{
    char *new;
    int fd;

    g_head->prog_size = cw_endian(g_bytes);                                  //write file size
    g_head->magic = cw_endian(COREWAR_EXEC_MAGIC);                           //write magic number
    if (g_file->cmnd == NULL)                                                //no instruction error
        return (cw_e(25));
    new = ft_strsub(name, 0, ft_strlen(name) - 2);
    new = ft_strjoin(new, ".cor");
    if ((fd = open(new, O_RDWR | O_CREAT, 00666)) < 0)
        return (cw_e(26));                                                   //cant create .cor file error
    write(fd, g_head, sizeof(t_header));
    while (tmp->next)
        tmp = tmp->next;
    cw_write_code(tmp, fd);
    close(fd);
    //ft_putstr_fd(g_head->comment, fd);
    return (1);
}