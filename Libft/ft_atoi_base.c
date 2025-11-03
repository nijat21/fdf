/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 18:09:23 by nismayil          #+#    #+#             */
/*   Updated: 2025/11/03 18:39:32 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	char_to_val(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	return (-1);
}

static int	skip_prefix(const char *str, int base)
{
	int	i;

	i = 0;
	while (is_space(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (base == 16 && str[i] == '0')
		if (str[i + 1] == 'x' || str[i + 1] == 'X')
			i += 2;
	return (i);
}

int	ft_atoi_base(const char *str, int base)
{
	int		i;
	int		sign;
	long	result;
	int		val;

	if (base < 2 || base > 16)
		return (0);
	i = 0;
	sign = 1;
	result = 0;
	while (is_space(str[i]))
		i++;
	if (str[i] == '-')
		sign = -1;
	i = skip_prefix(str, base);
	while (1)
	{
		val = char_to_val(str[i++]);
		if (val < 0 || val >= base)
			break ;
		result = result * base + val;
	}
	if (result * sign > 2147483647 || result * sign < -2147483648)
		return (0);
	return ((int)(result * sign));
}

// int	ft_atoi_base(const char *str, int base)
// {
// 	int		i;
// 	int		sign;
// 	long	result;
// 	int		val;

// 	if (base < 2 || base > 16)
// 		return (0);
// 	i = 0;
// 	sign = 1;
// 	result = 0;
// 	while (is_space(str[i]))
// 		i++;
// 	if (str[i] == '-' || str[i] == '+')
// 	{
// 		if (str[i] == '-')
// 			sign = -1;
// 		i++;
// 	}
// 	if (base == 16 && str[i] == '0' && (str[i + 1] == 'x' || str[i + 1] == 'X'))
// 		i += 2;
// 	while ((val = char_to_val(str[i])) >= 0 && val < base)
// 	{
// 		result = result * base + val;
// 		i++;
// 	}
// 	result *= sign;
// 	if (!(result <= 2147483647 && result >= -2147483648))
// 		return (0);
// 	return ((int)result);
// }
