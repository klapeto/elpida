//
// Created by klapeto on 13/9/20.
//

// https://stackoverflow.com/questions/53536076/towards-understanding-availability-of-xdg-open
#include <Elpida/Config.hpp>

#ifdef ELPIDA_UNIX
#define  _POSIX_C_SOURCE  200809L
#define  _GNU_SOURCE
//
// SPDX-License-Identifier: CC0-1.0
//
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

/* Number of bits in an unsigned long. */
#define  ULONG_BITS  (CHAR_BIT * sizeof (unsigned long))

/* Helper function to open /dev/null to a specific descriptor.
*/
static inline int devnullfd(const int fd)
{
	int tempfd;

	/* Sanity check. */
	if (fd == -1)
		return errno = EINVAL;

	do
	{
		tempfd = open("/dev/null", O_RDWR | O_NOCTTY);
	} while (tempfd == -1 && errno == EINTR);
	if (tempfd == -1)
		return errno;

	if (tempfd != fd)
	{
		if (dup2(tempfd, fd) == -1)
		{
			const int saved_errno = errno;
			close(tempfd);
			return errno = saved_errno;
		}
		if (close(tempfd) == -1)
			return errno;
	}

	return 0;
}

/* Helper function to close all except small descriptors
   specified in the mask. For obvious reasons, this is not
   thread safe, and is only intended to be used in recently
   forked child processes. */
static void closeAll(const unsigned long mask)
{
	DIR* dir;
	struct dirent* ent;
	ulong dfd;

	dir = opendir("/proc/self/fd/");
	if (!dir)
	{
		/* Cannot list open descriptors.  Just try and close all. */
		const ulong fd_max = sysconf(_SC_OPEN_MAX);
		ulong fd;

		for (fd = 0; fd < ULONG_BITS; fd++)
			if (!(mask & (1uL << fd)))
				close(fd);

		for (fd = ULONG_BITS; fd <= fd_max; fd++)
			close(fd);

		return;
	}

	dfd = dirfd(dir);

	while ((ent = readdir(dir)))
		if (ent->d_name[0] >= '0' && ent->d_name[0] <= '9')
		{
			const char* p = &ent->d_name[1];
			ulong fd = ent->d_name[0] - '0';

			while (*p >= '0' && *p <= '9')
				fd = (10 * fd) + *(p++) - '0';

			if (*p)
				continue;

			if (fd == dfd)
				continue;

			if (fd < ULONG_MAX && (mask & (1uL << fd)))
				continue;

			close(fd);
		}

	closedir(dir);
}
/* Launch the user-preferred application to open a file or URL.
   Returns 0 if success, an errno error code otherwise.
*/

int xdg_open(const char* file_or_url)
{
	pid_t child, p;
	int status;

	/* Sanity check. */
	if (!file_or_url || !*file_or_url)
		return errno = EINVAL;

	/* Fork the child process. */
	child = fork();
	if (child == -1)
		return errno;
	else if (!child)
	{
		/* Child process. */

		uid_t uid = getuid();  /* Real, not effective, user. */
		gid_t gid = getgid();  /* Real, not effective, group. */

		/* Close all open file descriptors. */
		closeAll(0);

		/* Redirect standard streams, if possible. */
		devnullfd(STDIN_FILENO);
		devnullfd(STDOUT_FILENO);
		devnullfd(STDERR_FILENO);

		/* Drop elevated privileges, if any. */
		if (setresgid(gid, gid, gid) == -1 ||
			setresuid(uid, uid, uid) == -1)
			_Exit(98);

		/* Have the child process execute in a new process group. */
		setsid();

		/* Execute xdg-open. */
		execlp("xdg-open", "xdg-open", file_or_url, (char*)0);

		/* Failed. xdg-open uses 0-5, we return 99. */
		_Exit(99);
	}

	/* Reap the child. */
	do
	{
		status = 0;
		p = waitpid(child, &status, 0);
	} while (p == -1 && errno == EINTR);
	if (p == -1)
		return errno;

	if (!WIFEXITED(status))
	{
		/* Killed by a signal. Best we can do is I/O error, I think. */
		return errno = EIO;
	}

	switch (WEXITSTATUS(status))
	{
	case 0: /* No error. */
		return errno = 0; /* It is unusual, but robust to explicitly clear errno. */
	case 1: /* Error in command line syntax. */
		return errno = EINVAL;      /* Invalid argument */
	case 2: /* File does not exist. */
		return errno = ENOENT;      /* No such file or directory */
	case 3: /* A required tool could not be found. */
		return errno = ENOSYS;      /* Not implemented */
	case 4: /* Action failed. */
		return errno = EPROTO;      /* Protocol error */
	case 98: /* Identity shenanigans. */
		return errno = EACCES;      /* Permission denied */
	case 99: /* xdg-open does not exist. */
		return errno = ENOPKG;      /* Package not installed */
	default:
		/* None of the other values should occur. */
		return errno = ENOSYS;      /* Not implemented */
	}
}

#endif