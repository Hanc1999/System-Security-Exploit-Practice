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
#ifdef HAVE_SETLOCALE
# include <locale.h>
#endif
#include <pwd.h>
#include <grp.h>
#include <time.h>

#include "sudoers.h"

struct path_escape {
    const char *name;
    size_t (*copy_fn)(char *, size_t);
};

static size_t fill_seq(char *, size_t);
static size_t fill_user(char *, size_t);
static size_t fill_group(char *, size_t);
static size_t fill_runas_user(char *, size_t);
static size_t fill_runas_group(char *, size_t);
static size_t fill_hostname(char *, size_t);
static size_t fill_command(char *, size_t);

static struct path_escape escapes[] = {
    { "seq", fill_seq },
    { "user", fill_user },
    { "group", fill_group },
    { "runas_user", fill_runas_user },
    { "runas_group", fill_runas_group },
    { "hostname", fill_hostname },
    { "command", fill_command },
    { NULL, NULL }
};

static size_t
fill_seq(char *str, size_t strsize)
{
    static char sessid[7];
    int len;

    if (sessid[0] == '\0')
	io_nextid(def_iolog_dir, sessid);

    /* Path is of the form /var/log/sudo-io/00/00/01. */
    len = snprintf(str, strsize, "%c%c/%c%c/%c%c", sessid[0],
	sessid[1], sessid[2], sessid[3], sessid[4], sessid[5]);
    if (len < 0)
	return strsize;	/* handle non-standard snprintf() */
    return (size_t)len;
}

static size_t
fill_user(char *str, size_t strsize)
{
    return strlcpy(str, user_name, strsize);
}

static size_t
fill_group(char *str, size_t strsize)
{
    struct group *grp;
    size_t len;

    if ((grp = sudo_getgrgid(user_gid)) != NULL) {
	len = strlcpy(str, grp->gr_name, strsize);
	gr_delref(grp);
    } else {
	len = strlen(str);
	len = snprintf(str + len, strsize - len, "#%u",
	    (unsigned int) user_gid);
    }
    return len;
}

static size_t
fill_runas_user(char *str, size_t strsize)
{
    return strlcpy(str, runas_pw->pw_name, strsize);
}

static size_t
fill_runas_group(char *str, size_t strsize)
{
    struct group *grp;
    size_t len;

    if (runas_gr != NULL) {
	len = strlcpy(str, runas_gr->gr_name, strsize);
    } else {
	if ((grp = sudo_getgrgid(runas_pw->pw_gid)) != NULL) {
	    len = strlcpy(str, grp->gr_name, strsize);
	    gr_delref(grp);
	} else {
	    len = strlen(str);
	    len = snprintf(str + len, strsize - len, "#%u",
		(unsigned int) runas_pw->pw_gid);
	}
    }
    return len;
}

static size_t
fill_hostname(char *str, size_t strsize)
{
    return strlcpy(str, user_shost, strsize);
}

static size_t
fill_command(char *str, size_t strsize)
{
    return strlcpy(str, user_base, strsize);
}

/*
 * Concatenate dir + file, expanding any escape sequences.
 * Returns the concatenated path and sets slashp point to
 * the path separator between the expanded dir and file.
 */
char *
expand_iolog_path(const char *prefix, const char *dir, const char *file,
    char **slashp)
{
    size_t len, prelen = 0;
    char *dst, *dst0, *path, *pathend, tmpbuf[PATH_MAX];
    const char *endbrace, *src = dir;
    int pass, strfit;

    /* Expanded path must be <= PATH_MAX */
    if (prefix != NULL)
	prelen = strlen(prefix);
    dst = path = emalloc(prelen + PATH_MAX);
    *path = '\0';
    pathend = path + prelen + PATH_MAX;

    /* Copy prefix, if present. */
    if (prefix != NULL) {
	memcpy(path, prefix, prelen);
	dst += prelen;
	*dst = '\0';
    }

    /* Trim leading slashes from file component. */
    while (*file == '/')
	file++;

    for (pass = 0; pass < 3; pass++) {
	strfit = FALSE;
	switch (pass) {
	case 0:
	    src = dir;
	    break;
	case 1:
	    /* Trim trailing slashes from dir component. */
	    while (dst - path - 1 > prelen && dst[-1] == '/')
		dst--;
	    if (slashp)
		*slashp = dst;
	    src = "/";
	    break;
	case 2:
	    src = file;
	    break;
	}
	dst0 = dst;
	for (; *src != '\0'; src++) {
	    if (src[0] == '%') {
		if (src[1] == '{') {
		    endbrace = strchr(src + 2, '}');
		    if (endbrace != NULL) {
			struct path_escape *esc;
			len = (size_t)(endbrace - src - 2);
			for (esc = escapes; esc->name != NULL; esc++) {
			    if (strncmp(src + 2, esc->name, len) == 0 &&
				esc->name[len] == '\0')
				break;
			}
			if (esc->name != NULL) {
			    len = esc->copy_fn(dst, (size_t)(pathend - dst));
			    if (len >= (size_t)(pathend - dst))
				goto bad;
			    dst += len;
			    src = endbrace;
			    continue;
			}
		    }
		} else if (src[1] == '%') {
		    /* Collapse %% -> % */
		    src++;
		} else {
		    /* May need strftime() */
		    strfit = 1;
		}
	    }
	    /* Need at least 2 chars, including the NUL terminator. */
	    if (dst + 1 >= pathend)
		goto bad;
	    *dst++ = *src;
	}
	*dst = '\0';

	/* Expand strftime escapes as needed. */
	if (strfit) {
	    time_t now;
	    struct tm *timeptr;

	    time(&now);
	    timeptr = localtime(&now);

#ifdef HAVE_SETLOCALE
	    if (!setlocale(LC_ALL, def_sudoers_locale)) {
		warningx(_("unable to set locale to \"%s\", using \"C\""),
		    def_sudoers_locale);
		setlocale(LC_ALL, "C");
	    }
#endif
	    /* We only calls strftime() on the current part of the buffer. */
	    tmpbuf[sizeof(tmpbuf) - 1] = '\0';
	    len = strftime(tmpbuf, sizeof(tmpbuf), dst0, timeptr);

#ifdef HAVE_SETLOCALE
	    setlocale(LC_ALL, "");
#endif
	    if (len == 0 || tmpbuf[sizeof(tmpbuf) - 1] != '\0')
		goto bad;		/* strftime() failed, buf too small? */

	    if (len >= (size_t)(pathend - dst0))
		goto bad;		/* expanded buffer too big to fit. */
	    memcpy(dst0, tmpbuf, len);
	    dst = dst0 + len;
	    *dst = '\0';
	}
    }

    return path;
bad:
    efree(path);
    return NULL;
}
