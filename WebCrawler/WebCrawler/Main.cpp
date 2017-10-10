#include "WebCrawler.h"
#include "BloomFilter.h"

BloomFilter g_filter;
WebCrawler *g_app = new WebCrawler(g_filter);

static void showVersion(void)
{
	cout << endl;

	cout << "WebCrawler 1.0" << endl;
	cout << endl;
}

static void splashScreen(void)
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

static void showUsage(string const &cmd)
{
	cout << endl;
	cout << "�÷���" << cmd << "[-v|-d|-h]" << endl;
	cout << "ѡ� -v - ��Ȩ��Ϣ" << endl;
	cout << "	-d - ��������" << endl;
	cout << "	-h - ������Ϣ" << endl;
	cout << endl;
}

int main(int argc, char *argv[])
{
	bool daemon = false;

	char ch;
	while ((ch = getopt(argc, argv, "vdh")) != -1)
	{
		switch (ch)
		{
		case 'v':
			showVersion();
			return EXIT_SUCCESS;

		case 'd':
			daemon = true;
			break;

		case 'h':
		case '?':
			showUsage(argv[0]);
			return EXIT_SUCCESS;

		default:
			break;
		}
	}

	splashScreen();
	g_app->Init(daemon);
	g_app->exec();

	return EXIT_SUCCESS;

}