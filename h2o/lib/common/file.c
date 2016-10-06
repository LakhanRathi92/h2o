/*
 * Copyright (c) 2015 DeNA Co., Ltd., Kazuho Oku
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "h2o/file.h"

#ifndef _MSC_VER
#include <sys/uio.h>
#include <unistd.h>
#else
#include<io.h>
#if !defined(_SSIZE_T_) && !defined(_SSIZE_T_DEFINED)
typedef intptr_t ssize_t;
# define _SSIZE_T_
# define _SSIZE_T_DEFINED
#endif
#endif

h2o_iovec_t h2o_file_read(const char *fn)
{
#ifndef _MSC_VER
    int fd;
	struct stat st;
	h2o_iovec_t ret = { NULL };
#else
	int fd = -1;
	FILE *filePoint;
	struct _stat st;
	h2o_iovec_t ret = { 0 };
#endif

	

    /* open */
#ifndef _MSC_VER
    if ((fd = open(fn, O_RDONLY | O_CLOEXEC)) == -1)
		goto Error;
#else
	if ((filePoint = fopen(fn, "rbN")) == NULL)
		goto Error;
#endif

#ifndef _MSC_VER
    fstat(fd, &st);
#else
	_stat(fn, &st);
#endif

    /* allocate memory */
    if (st.st_size > SIZE_MAX) {
        errno = ENOMEM;
        goto Error;
    }
    if ((ret.base = malloc((size_t)st.st_size)) == NULL)
        goto Error;
    /* read */
    while (ret.len != (size_t)st.st_size) {
        ssize_t r;
#ifndef _MSC_VER
        while ((r = read(fd, ret.base + ret.len, (size_t)st.st_size - ret.len)) == -1 && errno == EINTR)
            ;
#else
		fd = fileno(filePoint);
		while ((r = _read(fd, ret.base + ret.len, (size_t)st.st_size - ret.len)) == -1 && errno == EINTR)
			;
#endif
        if (r <= 0)
            goto Error;
        ret.len += r;
    }
    /* close */
#ifndef _MSC_VER
    close(fd);
#else
	fclose(filePoint);
//	_close(fd);
#endif

    return ret;

Error:
	if (fd != -1)
#ifndef _MSC_VER
		close(fd);
#else
		_close(fd);
#endif
    free(ret.base);
#ifndef _MSC_VER
    return (h2o_iovec_t){NULL};
#else
	return (h2o_iovec_t) { 0 };
#endif
}
