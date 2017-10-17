#include "asm.h"

/*
** error output function
*/

int cw_e(int type)
{
    if (type == 0)
    {
        ft_printf("Usage: ./asm [-m] <sourcefile.s>\f");
        ft_printf("-m: Instead of creating 1 .cor, flag ./asm can processing "
                          "into the .cor all your files, those was"
                          "transferred to the ./asm argument\n");
        return (0);
    }
    ft_putstr_fd(COL_RED"error #", 2);
    ft_putnbr_fd(type, 2);
    ft_putstr_fd(g_er[type - 1], 2);
    if ((type >= 4  && type < 10) || type == 11 || (type >= 14 && type < 23) || \
     type == 24 || type == 25)
    {
        ft_putstr_fd("\non line ", 2);
        ft_putnbr_fd(g_file->lines, 2);
        ft_putstr_fd(": \"", 2);
        ft_putstr_fd(g_s, 2);
        ft_putstr_fd(": \"", 2);
    }
    ft_putstr_fd("\n"COL_EOC, 2);
    return (-1);
}

unsigned int		cw_endian(unsigned int i)
{
    unsigned char	*us;

    us = (unsigned char*)&i;
    return (0 | (unsigned int)us[0] << 24 | (unsigned int)us[1] << 16 |
          (unsigned int)us[2] << 8 | (unsigned int)us[3]);
}

int cw_file_analyze(int lines, char *file, int len, int error)               //error: error(-1) || head reading(0) || code reading (>0)
{
    g_head = (t_header *)malloc(sizeof(t_header));
    if (len < 3 || file[len - 1] != 's' || file[len - 2] != '.')
        return (cw_e(0));                                                    //not .s error
    if (!cw_check_end(2))                                                    //for lseek() -2, SEEK_END start
        return (cw_e(22));
    lseek(g_fd, 0, SEEK_SET);                                                //return SEEK 2 file start
    g_file->lines = lines;                                                   //lines == 0
    while (error >= 0 && get_next_line(g_fd, &g_s) > 0)                      //reading lines
    {
        ++g_file->lines;                                                     //calculate every line
        while (SPACE(g_s[g_i]))                                              //skip spaces from start
            g_i++;
        if ((!g_s[g_i] || (IS_COM(g_s[g_i]))) && ft_strdel(&g_s))            //if line empty || comment here == skip
            continue;
        if (error > 0)                                                       //if head reading end
            error = (ft_strchr(g_s, LABEL) ? cw_label(g_i, 0) : cw_cmd(0, -1, -1, 0));
                                                                             //str consist ':' ? create label : command
        !error ? error = cw_read_head(0) : 0;                                //while error == 0, we r reading header
        ft_strdel(&g_s) ? g_i = 0 : 0;                                       //str flag resetting
    }
    if (error <= 0 || cw_check_label(0) < 0)
        return (error == 0 ? cw_e(3) : -1);                                  //empty file error
    cw_write_all(file, g_file->cmnd);
    return (1);
}

int main(int ac, char **av)
{
    int     arg;
    int     bonus;


    g_bytes = 0;
    ac < 2 || (ac == 2 && !ft_strcmp(av[1], "-m"))? exit(cw_e(0)) : 0;       //if no args OR just -m == USAGE
    bonus = (ac >= 2 && !ft_strcmp(av[1], "-m")) ? 1 : ac - 1;               //if av[1] == -m => MULTI
    arg = ac;
    while (arg-- > bonus && (g_file = (t_asm *)malloc(sizeof(t_asm))))
    {
        g_i = 0;
        g_s = NULL;
        g_file->name = NULL;
        g_file->comm = NULL;
        g_file->cmnd = NULL;
        g_file->labl = NULL;
        ft_printf(COL_BLUE"Start analyzing file %s\n"COL_EOC, av[arg]);
        if ((g_fd = open(av[arg], O_RDONLY)) == -1)
            return (cw_e(1));                                            //can't open() error
        if (read(g_fd, g_s, 0) == -1)
            return (cw_e(2));                                            //can't read() error
        if (cw_file_analyze(0, av[arg], (int) ft_strlen(av[arg]), 0) > 0)
            ft_printf(COL_GREEN".cor file was successful created!\n"COL_EOC);
        else
            ft_printf(COL_RED"Failed to create .cor file\n"COL_EOC);
    }
}
