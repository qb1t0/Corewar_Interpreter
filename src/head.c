#include "../includes/asm.h"

int cw_check_end(int i)
{
    char c;

    lseek(g_fd, -1, SEEK_END);
    read(g_fd, &c, 1);
    if (c == '\n')
        return (1);
    while (c != '\n')
    {
        if (lseek(g_fd, -i, SEEK_END) == -1L)
            return (0);
        read(g_fd, &c, 1);
        i++;
    }
    while (!IS_COM(c) && SPACE(c))
        read(g_fd, &c, 1);
    return (IS_COM(c) ? 1 : 0);
}

/*
** Cleaning trash function(after .name && .comment)
*/

int cw_head_clean(int type) //type - 1 ? name : comment
{
    char *s;
    int i;
    int j;

    i = 0;
    s = type ? g_file->name : g_file->comm;
    while (s[i] && s[i] != '\"')
        i++;
    j = i;
    while(s[++j])
        if (!SPACE(s[j]))
            return (type ? cw_e(23) : cw_e(24));
    if (type)
    {
        g_file->name[i] = '\0';
        if (ft_strlen(g_file->name) > PROG_NAME_LENGTH)
            return (cw_e(12));                                               // length name/command error
        else
            ft_bzero(g_head->prog_name, PROG_NAME_LENGTH + 1);
        ft_strncpy(g_head->prog_name, g_file->name, ft_strlen(g_file->name));
        return (cw_head_clean(0));
    }
    g_file->comm[i] = '\0';
    if (ft_strlen(g_file->comm) > COMMENT_LENGTH)                        // length name/command error
        return (cw_e(13));
    ft_bzero(g_head->comment,  COMMENT_LENGTH + 1);
    ft_strncpy(g_head->comment, g_file->comm, ft_strlen(g_file->comm));
    return (1);
}

/*
** Parsing HEADER(.comment && .name)
*/

int cw_head_parse(int i, char **dst, char *str)
{
    while (str[i] && SPACE(str[i]))                                          //skip spaces/tabs
        i++;
    if (!str[i])
        return (cw_e(8));                                                    // empty line error
    if (str[i++] != '\"')                                                    // strlen for len inside brackets (strlen -= i)
        return (cw_e(9));                                                    // error no opening brackets in comment/name
    *dst = ft_strdup(str + i);
    while (!ft_strchr(str, '\"') && ft_strjoin(*dst, "\n"))                  // if line doesn't consist \" than dst + '\n' && gnl()
    {
        g_file->lines++;
        ft_strjoin(*dst, str) ? ft_strdel(&str) : 0;
        if (get_next_line(g_fd, &str) < 1)
            return (cw_e(10));                                               // get_next_line error
    }
    while (str[i] != '\"')                                                   // 2 whiles for check shit after 2nd \"
        i++;
    while (str[++i])                                                         // !tabs/spaces == shit after line error
        if (str[i] && !SPACE(str[i]))
            return (cw_e(11));
    return (1);                                                              // all ok
}

/*
** Reading HEADER(.comment && .name)
*/

int cw_read_head(int error)
{
    if (!ft_strncmp(g_s + g_i, NAME, ft_strlen(NAME)))                       //if its .name
    {
        if (g_file->name != NULL)                                            //repeat name error
            return (cw_e(4));
        g_i += ft_strlen(NAME);
        error = cw_head_parse(0, &g_file->name, g_s + g_i);
    }
    else if (!ft_strncmp(g_s + g_i, COMMENT, ft_strlen(COMMENT)))            //if its .comment
    {
        if (g_file->comm != NULL)                                            //repeat comment error
            return (cw_e(5));
        g_i += ft_strlen(COMMENT);
        error = cw_head_parse(0, &g_file->comm, g_s + g_i);
    }
    else
        return (!g_file->name ? cw_e(6) : cw_e(7));                          //no name(-8) || comment(-9) error
    if (error < 0)
        return (-42);
    return (!g_file->name || !g_file->comm ? 0 : cw_head_clean(1));          //all ok, 0 - still reading, 1 - head end
}