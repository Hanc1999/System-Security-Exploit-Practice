/*
 * Major sudoers grammar changes are documented here.
 * Note that minor changes such as added Defaults options are not listed here.
 * This file placed in the public domain by Todd C. Miller on Apr 5, 2011.
 *
 * 1	sudo 1.1
 * 2	sudo 1.3, adds support specifying a directory instead of a command.
 * 3	sudo 1.3.2, new parser, Aliases have to be upper case
 * 4	sudo 1.3.2, adds User_Alias
 * 5	sudo 1.3.4, netgroup support
 * 6	sudo 1.3.5, support for escaping special chars
 * 7	sudo 1.3.7, unix group support
 * 8	sudo 1.4.1, wildcard support
 * 9	sudo 1.4.2, double quote support in sudoers command line args
 * 10	sudo 1.4.3, added NOPASSWD tag
 * 11	sudo 1.4.3, added Runas_Spec
 * 12	sudo 1.4.3, wildcards may be used in the pathname
 * 13	sudo 1.4.3, command args of "" means no args allowed
 * 14	sudo 1.4.4, '(' in command args no longer are a syntax error.
 * 15	sudo 1.4.4, '!command' works in the presence of runas user or NOPASSWD.
 * 16	sudo 1.4.4, all-caps user and host names are now handled properly.
 * 17	sudo 1.5.0, usernames may now begin with a digit
 * 18	sudo 1.5.3, adds Runas_Alias
 * 19	sudo 1.5.7, %group may be used in a Runas_List
 * 20	sudo 1.6.0, The runas user and NOPASSWD tags are now persistent across entries in a command list.  A PASSWD tag has been added to reverse NOPASSWD
 * 21	sudo 1.6.0, The '!' operator can be used in a Runas_Spec or an *_Alias
 * 22	sudo 1.6.0, a list of hosts may be used in a Host_Spec
 * 23	sudo 1.6.0, a list of users may be used in a User_Spec
 * 24	sudo 1.6.0, It is now possible to escape "special" characters in usernames, hostnames, etc with a backslash.
 * 25	sudo 1.6.0, Added Defaults run-time settings in sudoers.
 * 26	sudo 1.6.0, relaxed the regexp for matching user, host, group names.
 * 27	sudo 1.6.1, #uid is now allowed in a Runas_Alias.
 * 28	sudo 1.6.2, Wildcards are now allowed in hostnames.
 * 29	sudo 1.6.3p7, escaped special characters may be included in pathnames.
 * 30	sudo 1.6.8, added NOEXEC and EXEC tags.
 * 31	sudo 1.6.9, added SETENV and NOSETENV tags.
 * 32	sudo 1.6.9p4, support for IPv6 address matching.
 * 33	sudo 1.7.0, #include support.
 * 34	sudo 1.7.0, Runas_Group support.
 * 35	sudo 1.7.0, uid may now be used anywhere a username is valid.
 * 36	sudo 1.7.2, #includedir support.
 * 37	sudo 1.7.4, per-command Defaults support.
 * 38	sudo 1.7.4, added LOG_INPUT/LOG_OUTPUT and NOLOG_INPUT/NOLOG_OUTPUT tags
 * 39	sudo 1.7.6/1.8.1, White space is now permitted within a User_List in a per-user Defaults definition.
 * 40	sudo 1.7.6/1.8.1, A group ID is now allowed in a User_List or Runas_List.
*/

#ifndef _SUDOERS_VERSION_H
#define	_SUDOERS_VERSION_H

#define SUDOERS_GRAMMAR_VERSION	40

#endif /* _SUDOERS_VERSION_H */
