#include "Log.h"
#include "common.h"


void Log::printf(int level, char const *file, int *line, char const *format, ...) const
{
	if (level >= g_app->m_cfg.m_logLevel)
	{
		char dateTime[32];
		time_t now = time(NULL);
		strftime(dateTime, sizeof(dateTime), "%Y-%m-%d %H:%M:%S", localtime(&now));

		fprintf(stdout, "[%s][%s][pid=%d tid=%lu][%s:%d]\n", 
			dateTime, m_levels[level], getpid(), this_thread::get_id(), file, line);

		va_list ap;
		va_start(ap, format);
		vfprintf(stdout, format, ap);
		va_end(ap);

		fprintf(stdout, "\n\n");
	}
	
	if (level >= LEVEL_ERR)
	{
		exit(EXIT_FAILURE);
	}
}

// 类静态数组初始化
char const *Log::m_levels[] = { "dbg", "inf", "war", "err", "crt" };
