/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_trace.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wxuerui <wxuerui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 12:18:48 by wxuerui           #+#    #+#             */
/*   Updated: 2022/10/24 17:42:17 by wxuerui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_TRACE_H
# define MALLOC_TRACE_H

# include <stdlib.h>
# include <stdio.h>
# include <dlfcn.h>

# define _GNU_SOURCE

# ifdef __cplusplus
	extern "C"
# endif

/* Colors */
# define BOLD		"\x1b[1m"
# define RED		"\x1b[31m"
# define GREEN 		"\x1b[32m"
# define YELLOW		"\x1b[33m"
# define BLUE		"\x1b[34m"
# define MAGENTA	"\x1b[35m"
# define CYAN		"\x1b[36m"
# define RESET		"\x1b[0m"

# define ERROR_EXIT 1

typedef char	BYTE;
typedef char *	BYTE_PTR;

/* function pointer types to the original malloc and free function */
typedef void *	(*malloc_ptr)(size_t);
typedef void *	(*free_ptr)(void *);

void	malloc_trace_lock(void);
void	malloc_trace_unlock(void);

#endif /* malloc_trace.h */
