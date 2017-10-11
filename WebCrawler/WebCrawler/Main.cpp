#include "WebCrawler.h"
#include "BloomFilter.h"

BloomFilter g_filter;
WebCrawler *g_app = new WebCrawler(g_filter);

static void ShowVersion(void)
{
	cout << endl;

	cout << "WebCrawler 1.0" << endl;
	cout << endl;
}

static void SplashScreen(void)
{
	cout << endl;

	ifstream ifs("WebCrawler.scr");
	if (ifs)
	{
		string line;
		while (getline(ifs, line))
		{
			cout << line << endl;
		}

		ifs.close();
	}

	cout << endl;
}

static void ShowUsage(string const &cmd)
{
	cout << endl;
	cout << "用法：" << cmd << "[-v|-d|-h]" << endl;
	cout << "选项： -v - 版权信息" << endl;
	cout << "	-d - 启动精灵" << endl;
	cout << "	-h - 帮助信息" << endl;
	cout << endl;
}

int main(int argc, char *argv[])
{
	bool daemon = false;

	char ch;
	while ((ch = getopt(argc, argv, "vdh")) != -1)	// 分析命令行参数
	{
		switch (ch)
		{
		case 'v':
			ShowVersion();
			return EXIT_SUCCESS;

		case 'd':
			daemon = true;
			break;

		case 'h':
		case '?':
			ShowUsage(argv[0]);
			return EXIT_SUCCESS;

		default:
			break;
		}
	}

	SplashScreen();
	g_app->Init(daemon);
	g_app->exec();

	return EXIT_SUCCESS;

}