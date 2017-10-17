//
// Created by FlapJackasS on 7/31/17.
//

#include <fcntl.h>
#include "lem_in.h"

#define DEF(x, y, z, q) *x = 0; *y = 0; *z = 0; *q = 1;
#define IS_COM(a) ((*a == '#') && *(a + 1) != '#' ? (1) : (0))

int g_fd; //FILE DESCRIPTOR
int g_j;
int g_o;
int g_f;
int g_is;
int g_end;
int g_isrt;
int g_iend;
int g_start;
int g_stop;
int g_err; // error ? 1 : 0;
int g_e; // 3 etaps of valid: 1 - N(ants); 2 - Vertexes; 3 - Edges
int *g_al;
int g_h;
t_r *g_head;
t_r *g_back;
t_l *g_arr;
t_l *g_ways;
char *g_s;

int		ft_strfcmp(const char *s1, const char *s2, size_t n)
{
    if (n == 0)
        return (0);
    while ((*s1 && *s2) && (n > 1) && (*s1 == *s2))
    {
        s1++;
        s2++;
        n--;
    }
    return (*(unsigned char *)(s1) - *(unsigned char *)(s2));
}

int     algo(int k, int i, int v)
{
    t_l *wid;
    t_r *bro;
    t_r *u;

    wid = g_arr;
    g_al = (int *)malloc(sizeof(int) * v); //int mass for stack
    while ((g_o = -1) && ++k < v && (bro = wid[k].room)) //loop for vertices marking
        while((++g_o + 1) && bro)
        {
            bro->alg = k;
            if (!bro->was && bro->alg == -1) //mark here
                g_al[k + g_o] = bro->i;
            if (bro->i == g_iend) //stop if end
                break;
            bro = bro->next;
        }
    if (wid[k].room->i != g_iend)
        return (0);
//    while (k != 0 && (bro = wid[k].room))
//        while (bro)
////            if ((g_ways[0] = (t_l *)malloc(sizeof(t_l) * k)))
////                ;
//            if (bro->i == k - 1) {
//                g_back = (t_r *)malloc(sizeof(t_r)); //key list
//                g_back =
//            }
//            else
//                bro = bro->next;
}

int    build(t_r *buf, int v, int time, int i)
{
    static int src;
    static int dst;

    while(g_s[i] && time && g_err && g_s[i] != '-')
        i++;
    i = time == 2 ? 0 : i + 1;
    while(g_s && time && buf != NULL)
        if (!(g_is = (ft_strfcmp(buf->name, g_s + i, (size_t)i + 1))))
        {
            time == 2 ? src = buf->i : 0;
            time == 1 ? dst = buf->i : 0;
            return(build(g_head, v, --time, 0));
        }
        else
            buf = buf->dali;
    g_arr[dst].room->next = g_arr[src].room;
    g_arr[src].room = g_arr[dst].room;
    g_stop = (g_is || !g_s) ? 0 : 1;
    return (1);
//    while(g_arr[src].room && ft_printf("ROOM[%d] :%s\n"))
//        ft_printf("");
}

int l_cmp(t_r *src, int v)
{
    t_r *temp;
    t_l *g_arr;

    temp = g_head;
    if (!src && (g_arr = (t_l *)malloc(sizeof(t_l) * v)))
        while(temp)
            (g_arr[temp->i].room = temp) ? temp = temp->dali : 0;
    if (!src && !(g_start && g_end ? build(g_head, v, 2, 0) : 0))
        exit(ft_printf("ERROR\n"));
    else if (!src)
        return(1);
    while (temp)
        if (!ft_strcmp(src->name, temp->name) || \
        (src->x_cord == temp->x_cord && src->y_cord == temp->y_cord))
            return ((g_err = 0));
        else if (temp->dali == NULL)
            break;
        else
            temp = temp->dali;
    g_head->stend == 1 ? g_isrt = v : 0;
    g_head->stend == 2 ? g_iend = v : 0;
    g_head->alg = -1;
    g_head->was = 0;
    g_head->i = v;
    g_head = src;
    return (v);
}


t_r *i_write(int i, int j, int stend)
{
    t_r *temp;

    if (!g_start && !ft_strcmp("##start", g_s) && ++g_start)
        return (i_write(0, 2, 1));
    else if (!g_end && !ft_strcmp("##end", g_s) && ++g_end)
        return (i_write(0, 2, 2));
    while (stend && ft_strclr(g_s) && get_next_line(g_fd, &g_s))
        if (!IS_COM(g_s))
            break;
    temp = (t_r *)malloc(sizeof(t_r));
    while(ft_isdigit(g_s[i]) || ft_isalpha(g_s[i]))
        g_e = (g_s[++i] == ' ') ? 2 : 3;
    temp->name = ft_strnew(i) ? ft_strncpy(temp->name, g_s, (size_t)i) : 0;
    while(g_e == 2 && (g_j = 1) && j--)
    {
        while (ft_isdigit(g_s[++i]) && g_err)
            g_j++;
        if ((j && g_s[i] != ' ') || (!j && g_s[i] != '\0'))
            g_e = 3;
        j ? temp->x_cord = ft_atoi(g_s + i - 1) : 0;
        !j ? temp->y_cord = ft_atoi(g_s + i - 1) : 0;
    }
    temp->dali = (temp->stend = stend) ? g_head : g_head;
    temp->next = NULL;
    g_e == 3 ? free(temp) : 0;
    return(g_e == 2 ? temp : 0);
}

int main(void)
{
    t_info		*t;

    g_fd = open("../test", O_RDONLY);
    t = (t_info *)malloc(sizeof(t_info));
    g_err = (int)read(g_fd, g_s, 0) >= 0 ? 1 : 0;
    DEF(&g_start, &g_end, &t->v, &g_e);
    while (ft_strclr(g_s) && get_next_line(g_fd, &g_s) && g_err)
        if (IS_COM(g_s))
            ft_printf("%s\n",g_s);
        else if (g_e == 1)
            ft_isaldigit(g_s) && ++g_e ? t->ant = ft_atoi(g_s) : --g_err;
        else if (g_e == 2)
            l_cmp(i_write(0, 2, 0), t->v++) ? 0 : 1;
        else if (g_e == 3)
            ;
    if (g_f && (g_ways = (t_l *)malloc(sizeof(t_l) * t->v)))
        g_al[0] = g_isrt ? algo(-1, 0, t->v) : 0;
    g_err ? 0 : write(2, "ERROR\n", 6);
}