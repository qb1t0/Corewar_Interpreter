/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalata <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/28 16:36:12 by ysalata           #+#    #+#             */
/*   Updated: 2016/12/05 15:46:17 by ysalata          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char			*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;

	if (s1 == NULL && s2 == NULL)
		return (ft_strnew(0));
	else if (s1 == NULL)
		return (ft_strdup(s2));
	else if (s2 == NULL)
		return (ft_strdup(s1));
	if (!(res = ft_strnew(ft_strlen(s1) + ft_strlen(s2))))
		return (NULL);
	res = ft_strcat(ft_strcpy(res, s1), s2);
	return (ft_strclr(s1) ? res : 0);
}
