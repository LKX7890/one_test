#ifndef __MULTIIO_H__
#define __MULTIIO_H__

class MultiIo
{
public:

	MultiIo(void);
	~MultiIo(void);

	bool add(int fd, epoll_event &event) const;
	bool del(int fd, epoll_event &event) const;
	int wait(epoll_event events[], int max, int timeout) const;

private:

	int m_epoll;
};
#endif
