#include "../includes/asm.h"

//j for number of reading argument
int cw_check_value(t_cmnd *cmnd, int j, char *label)                         //if (islabel) ? checking if those label is already exist
{                                                                            //else if(!islabel) check number string
    int     i;

    cmnd->cipher |= (g_tab[cmnd->cmd].kind[j] & T_DIR) ? DIR_CODE : IND_CODE;//adding cipher for DIR | IND CODE
    cmnd->size += \
        ((g_tab[cmnd->cmd].kind[j] & T_DIR) && !g_tab[cmnd->cmd].id2) ? 4 : 2;// adding +4 if (!idk1 && T_DIR), else +4
    i = 0;
    if (j + 1 == g_tab[cmnd->cmd].arg)
    {
        if (!cw_write_cmd(cmnd, j))
            return (cw_e(27));                                                    //shit after line error
    }
    if (IS_LBL(label[i]))
        return (0);
    cmnd->args[j][i] == '%' ? i++ : 0;
    cmnd->args[j][i] == '-' ? i++ : 0;
    if (!(ft_isaldigit(cmnd->args[j] + i)))                                          //NaN(not a number), error == 1
        return (cw_e(21));
    cmnd->i_arg[j] = (UI)ft_atoi(label);                                          //getting value
    if ((g_tab[cmnd->cmd].kind[j] & T_DIR) && !g_tab[cmnd->cmd].id2)
    {
        cmnd->i_arg[j] = ((cmnd->i_arg[j] << 8) & 0xFF00FF00) |\
         ((cmnd->i_arg[j] >> 8) & 0xFF00FF);
        cmnd->i_arg[j] = SWAP_UI(cmnd->i_arg[j]);
    }
    else
        cmnd->i_arg[j] = SWAP_USI(cmnd->i_arg[j]);
    return (0);
}

// calculating byte size of command
int cw_write_cmd(t_cmnd *cmnd, int type)
{
    char *s;
    int i;
    int j;

    if (type >= 0)
    {
        i = 0;
        s = cmnd->args[type];
        if (s[i] == '%' ? ++i : 0)
            s[i] == ':' ? i++ : 0;
        s[i] == '-' ? i++ : 0;
        while (s[i] && !SPACE(s[i]))
            i++;
        j = i;
        if (s[i])
        {
            while (s[i] && SPACE(s[i]))
                i++;
            if (s[i] && !IS_COM(s[i]))
                return (0);
        }
        cmnd->args[type] = ft_strsub(s, 0, (size_t)j);
        return (1);
    }
    cmnd->size++;                                                           //for command id(t_cmnd->cmd)
    g_tab[cmnd->cmd].id1 ? cmnd->size++ : 0;                                //if id1 == 1 than add 1 cipher size(1 byte)
    return (1);
}

/*
** parse all(i) args(arg) in command line(cmnd)
** checking if all arguments matched their required types
*/

int cw_args_parse(int i, char *arg, t_cmnd *cmnd)                            //TODO: try to write recursive argument checker
{
    int     error;
    //I wrote those staff in cw_cmd function
//    while (cmnd->args[i])                                                  //calculates how much args operation have
//        i++;
//    if (i != g_tab[cmnd->cmd].arg)                                         //if n args != g_tab[cmd].arg
//        return (-11);                                              _____   //number of args error
// I DON'T NEED THOSE BIG WHILE ANYMORE^ CAUSE IM DAMN RECURSIVE MAN \ R /
//                                                                    \_/
    error = 0;
    if (!arg[0])
        return(cw_e(18));                                                    //empty argument error
// after each read argument i need 2 write argument code(REG_CODE | DIR_CODE | IND_CODE) in cipher variable
    cmnd->cipher <<= 2;                                              //EXAMPLE: 11 01 10 == command(IND(CODE: 3), REG(CODE: 1), DIR(CODE: 2))
    if ((g_tab[cmnd->cmd].kind[i] & T_REG) && arg[0] == 'r' && arg[1])       //if T_REG (should start with 'r')
    {
        g_file->cmnd->cipher |= REG_CODE;                                    //write REG_CODE(cipher |(OR) 01)
        if ((g_file->cmnd->i_arg[i] = (UI)ft_atoi(arg + 1)) > REG_NUMBER)    //wrote register number in g_file->cmnd->r
            return (cw_e(19));                                           //if it higher than max -> invalid register number error
        cmnd->size += T_REG;                                                 //+1 byte for REGISTER TYPE variable
    }
    else if ((g_tab[cmnd->cmd].kind[i] & T_DIR) && IS_DIR(arg[0]))           //if T_DIR
        error = cw_check_value(cmnd, i, (arg + 1));                          //check is_aldigit && write value
    else if ((g_tab[cmnd->cmd].kind[i] & T_IND))                             //if T_IND
        error = cw_check_value(cmnd, i, arg);                                //check is_aldigit && write value
    else
        return(cw_e(20));                                                    //non-existing / invalid argument type
    if (!error && ++i)                                                       // !error means all ok
        return (i != g_tab[cmnd->cmd].arg ? \
            cw_args_parse(i, g_file->cmnd->args[i], g_file->cmnd) : 1);      //recursive call till exist args(i) of command
    else
        return (-13);                                                         //non-existing/invalid argument type error
}

/*
** Initialize new command variables
*/

t_cmnd    *cw_cmnd_init()
{
    t_cmnd *c;

    c = (t_cmnd *)malloc(sizeof(t_cmnd));                                    //malloc() for new operation list
    c->line = g_file->lines;                                                 //saving number of line in our struct
    c->start = g_bytes;                                                      //save start position
    c->size = 0;
    c->cipher = 0;
    return (c);
}
/*
 * adding new command
 * TODO: add skip comment after command line
 */

int cw_cmd(int i, int j, int cmd, int k) //types: command(0) || label(1); i,j - just 2 iteration flags
{
    t_cmnd      *c;

    k ? g_i = 0 : 0;
    if (g_i == 0)
        while (SPACE(g_s[g_i]))
         g_i++;
    while (!SPACE(g_s[g_i + i]))                                            //while operation name(!space)
        i++;
    c = cw_cmnd_init();                                                     //new command initialization
    c->name = ft_strsub(g_s + g_i, 0, (size_t)i);                           //saving operation name
    while (++cmd < 16)                                                      //trying 2 find command in g_tab
        if (!ft_strcmp(g_tab[cmd].op, c->name))                             //comparing names with g_tab(table)
        {
            c->cmd = cmd;
            break ;
        }
    if (cmd == 16)                                                           //if command wasn't founded
        return (cw_e(16));                                                   //non-existing command error
    c->args = ft_strsplit(g_s + g_i + i, SEPARATOR_CHAR);                    //split args by comma(",")
    while (c->args[++j])                                                     //re-write && delete spaces before && after each argument
        c->args[j] = ft_strtrim(c->args[j]);                                 //TODO: leaks here maybe, need 2 fix
    if (j != g_tab[c->cmd].arg)                                              //if n args != g_tab[cmd].arg
        return (cw_e(17));                                                   //  number of args error
    c->next = g_file->cmnd ? g_file->cmnd : NULL;                            //adding from start new cmnd to our operations list
    g_file->cmnd ? g_file->cmnd->prev = c : NULL;                            //creating prev list
    g_file->cmnd = c;                                                        //redefine global head to the start of the list
    if (cw_args_parse(0, g_file->cmnd->args[0], g_file->cmnd) > 0)           //call parsing arguments function
        cw_write_cmd(g_file->cmnd, -1);
    else
        return (-1);                                                         //parsing args error
    return (g_bytes += g_file->cmnd->size);                                  //add command size 2 the total
}