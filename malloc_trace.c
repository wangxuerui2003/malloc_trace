/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_trace.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wxuerui <wxuerui@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 12:20:53 by wxuerui           #+#    #+#             */
/*   Updated: 2022/10/20 13:33:25 by wxuerui          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "malloc_trace.h"

static long long	total = 0;
static long long	current = 0;

static int	locked = 0;

/* Function pointers of original malloc and free */
static malloc_ptr	malloc_original;
static free_ptr		free_original;

static void	increace_count(size_t size)
{
	total += size;
	current += size;
}

static void	decreace_count(size_t size)
{
	current -= size;
}

void	malloc_trace_lock(void)
{
	locked = 1;
}

void	malloc_trace_unlock(void)
{
	locked = 0;
}

/* Rewrite original malloc function */
extern void	*malloc(size_t size)
{
	void	*return_ptr;

	if (size == 0)
		return (NULL);
	if (malloc_original != NULL)
	{
		return_ptr = (*malloc_original)(sizeof(size_t) + size); /* One more size_t for tracing the size of malloc */
		if (locked == 0)
			increace_count(size);
		((size_t *)return_ptr)[0] = size;
		return ((BYTE_PTR)return_ptr + sizeof(size_t));
	}
	return (NULL);
}

/* Rewrite original free function */
extern void	free(void *ptr)
{
	size_t	size; /* The size traced since the ptr was malloced */

	if (free_original != NULL)
	{
		ptr = (BYTE_PTR)ptr - sizeof(size_t);
		size = ((size_t *)ptr)[0];
		if (locked == 0)
			decreace_count(size);
		(*free_original)(ptr);
	}
}

/* Program constructor */
static __attribute__((constructor)) void	init(void)
{
	char	*error;

	malloc_original = (malloc_ptr)dlsym(RTLD_NEXT, "malloc");
	error = dlerror();
	if (error != NULL)
	{
		fprintf(stderr, "malloc_trace error: %s\n", error);
		exit(ERROR_EXIT);
	}

	free_original = (free_ptr)dlsym(RTLD_NEXT, "free");
	error = dlerror();
	if (error != NULL)
	{
		fprintf(stderr, "malloc_trace error: %s\n", error);
		exit(ERROR_EXIT);
	}
}

/* Program destructor */
static __attribute__((destructor)) void	output(void)
{
	fprintf(stderr, "malloc_trace exited, total: %lld, current: %lld\n", total, current);
	if (current == 0)
	{
		fprintf(stderr, GREEN"0 Leaks found\n"RESET);
	}
	else
	{
		fprintf(stderr, RED"Leaks: %lld bytes\n"RESET, current);
	}
}