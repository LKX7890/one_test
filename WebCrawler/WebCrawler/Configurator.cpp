#include "Configurator.h"
#include "common.h"
#include "StrKit.h"
#include "Log.h"


Configurator::Configurator(void) : m_log_level(Log::LEVEL_DBG), m_max_jobs(-1), m_max_depth(-1),
m_max_rawurls(-1), m_max_dns_urls(-1), m_stat_interval(0)
{

}

void Configurator::LoadCfgFile(string const& cfg_file)
{
	ifstream ifs(cfg_file.c_str());

	if (!ifs)
	{
		g_app->m_log.printf(Log::LEVEL_ERR, __FILE__, __LINE__, "加载配置文件\"%s\"失败", cfg_file.c_str());
	}

	string line;

	for (int line = 0; getline(ifs, line); ++line)
	{
		StrKit::StrTrim(line);		// 单行处理

		if (line[0] == '#' || line[0] == '\0')
		{
			continue;
		}

		vector<string> strv = StrKit::StrSplit(line, "=", 1);
		if (strv.size() == 2)
		{
			if (!strcasecmp(strv[0].c_str(), "LOG_LEVEL"))
			{
				m_log_level = atoi(strv[1].c_str());
			}

			else if (!strcasecmp(strv[0].c_str(), "LOG_FILE"))
			{
				m_log_file = strv[1];
			}

			else if (!strcasecmp(strv[0].c_str(), "MAX_JOBS"))
			{
				m_max_jobs = atoi(strv[1].c_str());
			}

			else if (!strcasecmp([strv[0].c_str(), "MAX_DEPTH"))
			{
				m_max_depth = atoi(strv[1].c_str());
			}

			else if (!strcasecmp(strv[0].c_str(), "MAX_RAW_URLS"))
			{
				m_max_rawurls = atoi(strv[1].c_str());
			}

			else if (!strcasecmp(strv[0].c_str(), "MAX_DNS_URLS"))
			{
				m_max_dns_urls = atoi(strv[1].c_str());
			}

			else if (!strcasecmp(strv[0].c_str(), "STAT_INTERVAL"))
			{
				m_stat_interval = atoi(strv[1].c_str());
			}

			else if (!strcasecmp(strv[0].c_str(), "SEEDS"))
			{
				m_seeds = strv[1];
			}

			else if (!strcasecmp(strv[0].c_str(), "INCLUDE_PREFIXES"))
			{
				m_include_prefixes = strv[1];
			}

			else if (!strcasecmp(strv[0].c_str(), "EXCLUDE_PREFIXES"))
			{
				m_exclude_prefixes = strv[1];
			}

			else if (!strcasecmp(strv[0].c_str(), "PLUGINS_PATH"))
			{
				m_plugins_path = strv[1];
			}

			else if (!strcasecmp(strv[0].c_str(), "LOAD_PLUGIN"))
			{
				m_load_plugins.push_back(strv[1]);
			}

			else if (!strcasecmp(strv[0].c_str(), "ACCEPT_TYPE"))
			{
				m_accept_type.push_back(strv[1]);
			}

			else
				g_app->m_log.printf(Log::LEVEL_ERR, __FILE__, __LINE__, "无效配置项：%s, %d, %s",
				cfg_file.c_str(), line, strv[0].c_str());
		}

		else
			g_app->m_log.printf(Log::LEVEL_ERR, __FILE__, __LINE__, "无效配置项：%s, %d, %s",
			cfg_file.c_str(), line, line.c_str());
	}

	ifs.close();
}
