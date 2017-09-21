#ifndef  __LOG_H__
#define  __LOG_H__

class Log
{
public:
	static int const LEVEL_DBG = 0;
	static int const LEVEL_INF = 1;
	static int const LEVEL_WAR = 2;
	static int const LEVEL_ERR = 3;
	static int const LEVLE_CRT = 4;

	void printf(int level, char const *file, int *line, char const *format, ...)const;

private:

	static char const* m_levels[];
};
#endif