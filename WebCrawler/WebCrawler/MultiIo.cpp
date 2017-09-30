#include "MultiIo.h"



MultiIo::MultiIo(void)
{
	g_app->m_log.printf(Log::LEVEL_ERR, __FILE__, __LINE__, "epoll_create1: %s", strerror(errno));
}

MultiIo::~MultiIo(void)
{
	close(m_epoll);
}

bool MultiIo::add(int fd, epoll_event &event) const
{
	if (epoll_crl(m_epoll, EPOLL_CTL_ADD, fd, &event) == -1)
	{
		g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "epoll_ctl: %s", strerror(errno));

		return false;
	}

	return true;
}

bool MultiIo::del(int fd, epoll_event &event) const
{
	if (epoll_ctl(m_epoll, EPOLL_CTL_DEL, fd, &event) == -1)
	{
		g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "epoll_ctl: %s", strerror(errno));
		return false;
	}

	return true;
}

int MultiIo::wait(epoll_event events[], int max, int timeout) const
{
	int fds = epoll_wait(m_epoll, events, max, timeout);
	if (fds == -1 && errno != EINTR)
	{
		g_app->m_log.printf(Log::LEVEL_WAR, __FILE__, __LINE__, "epoll_wait: %s", strerror(errno));
	}

	return fds;
}
