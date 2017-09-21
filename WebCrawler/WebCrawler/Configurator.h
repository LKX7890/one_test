#ifndef  __CONFIGURATOR_H__
#define  __CONFIGURATOR_H__

class Configurator
{
public:

	Configurator(void);

	void LoadCfgFile(string const& cfg_file);

	int m_log_level;				// 日志等级
	string m_log_file;				// 日志路径
	int m_max_jobs;					// 抓取最大任务数
	int m_max_depth;				// 递归最大深度
	int m_max_rawurls;				// 原始URL队列最大容量
	int m_max_dns_urls;				// 解析URL队列最大容量
	int m_stat_interval;			// 定时间隔

	string m_seeds;					// 种子连接
	string m_include_prefixes;		// 只抓前缀
	string m_exclude_prefixes;		// 排除前缀
	string m_plugins_path;			// 插件路径
	
	vector<string> m_load_plugins;	// 插件列表
	vector<string> m_accept_type;	// 接受类型
};
#endif