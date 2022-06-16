/*
 * Copyright (c) 2007-2011 Todd C. Miller <Todd.Miller@courtesan.com>
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

#ifndef _SUDO_NSS_H
#define _SUDO_NSS_H

struct lbuf;
struct passwd;

struct sudo_nss {
    struct sudo_nss *prev;
    struct sudo_nss *next;
    int (*open)(struct sudo_nss *nss);
    int (*close)(struct sudo_nss *nss);
    int (*parse)(struct sudo_nss *nss);
    int (*setdefs)(struct sudo_nss *nss);
    int (*lookup)(struct sudo_nss *nss, int, int);
    int (*display_cmnd)(struct sudo_nss *nss, struct passwd *);
    int (*display_defaults)(struct sudo_nss *nss, struct passwd *, struct lbuf *);
    int (*display_bound_defaults)(struct sudo_nss *nss, struct passwd *, struct lbuf *);
    int (*display_privs)(struct sudo_nss *nss, struct passwd *, struct lbuf *);
    void *handle;
    short ret_if_found;
    short ret_if_notfound;
};

TQ_DECLARE(sudo_nss)

struct sudo_nss_list *sudo_read_nss(void);

#endif /* _SUDO_NSS_H */
