/*
 * Copyright (c) 2011 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <config.h>

#include <sys/types.h>
#include <stdio.h>
#ifdef STDC_HEADERS
# include <stdlib.h>
# include <stddef.h>
#else
# ifdef HAVE_STDLIB_H
#  include <stdlib.h>
# endif
#endif /* STDC_HEADERS */
#ifdef HAVE_STRING_H
# if defined(HAVE_MEMORY_H) && !defined(STDC_HEADERS)
#  include <memory.h>
# endif
# include <string.h>
#endif /* HAVE_STRING_H */
#ifdef HAVE_STRINGS_H
# include <strings.h>
#endif /* HAVE_STRINGS_H */

#include "missing.h"
#include "error.h"

extern void writeln_wrap(FILE *fp, char *line, size_t len, size_t maxlen);

static void
usage(void)
{
    fprintf(stderr, "usage: check_wrap inputfile\n");
    exit(1);
}

int
main(int argc, char *argv[])
{
    size_t len;
    FILE *fp;
    char *cp, *dash, *line, lines[2][2048];
    int which = 0;

#if !defined(HAVE_GETPROGNAME) && !defined(HAVE___PROGNAME)
    setprogname(argc > 0 ? argv[0] : "check_wrap");
#endif

    if (argc != 2)
	usage();

    fp = fopen(argv[1], "r");
    if (fp == NULL)
	errorx(1, "unable to open %s", argv[1]);

    /*
     * Each test record consists of a log entry on one line and a list of
     * line lengths to test it with on the next.  E.g.
     *
     * Jun 30 14:49:51 : millert : TTY=ttypn ; PWD=/usr/src/local/millert/hg/sudo/trunk/plugins/sudoers ; USER=root ; TSID=0004LD ; COMMAND=/usr/local/sbin/visudo
     * 60-80,40
     */
    while ((line = fgets(lines[which], sizeof(lines[which]), fp)) != NULL) {
	len = strcspn(line, "\n");
	line[len] = '\0';

	/* If we read the 2nd line, parse list of line lengths and check. */
	if (which) {
	    for (cp = strtok(lines[1], ","); cp != NULL; cp = strtok(NULL, ",")) {
		size_t maxlen;
		/* May be either a number or a range. */
		len = maxlen = atoi(cp);
		dash = strchr(cp, '-');
		if (dash)
		    maxlen = atoi(dash + 1);
		while (len <= maxlen) {
		    printf("# word wrap at %d characters\n", (int)len);
		    writeln_wrap(stdout, lines[0], strlen(lines[0]), len);
		    len++;
		}
	    }
	}
	which = !which;
    }

    exit(0);
}

void
cleanup(int gotsig)
{
    return;
}
