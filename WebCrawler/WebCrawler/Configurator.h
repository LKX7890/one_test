#ifndef  __CONFIGURATOR_H__
#define  __CONFIGURATOR_H__

class Configurator
{
public:

	Configurator(void);

	void LoadCfgFile(string const& cfg_file);

	int m_log_level;				// ��־�ȼ�
	string m_log_file;				// ��־·��
	int m_max_jobs;					// ץȡ���������
	int m_max_depth;				// �ݹ�������
	int m_max_rawurls;				// ԭʼURL�����������
	int m_max_dns_urls;				// ����URL�����������
	int m_stat_interval;			// ��ʱ���

	string m_seeds;					// ��������
	string m_include_prefixes;		// ֻץǰ׺
	string m_exclude_prefixes;		// �ų�ǰ׺
	string m_plugins_path;			// ���·��
	
	vector<string> m_load_plugins;	// ����б�
	vector<string> m_accept_type;	// ��������
};
#endif