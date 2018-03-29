#include "fivechess.hpp"
#include <cstring>
#include <cstdlib>

FiveChess::FiveChess() : m_cur_player_type(0)
{
	memset(m_chess_board_list, 0, sizeof(m_chess_board_list));
}

FiveChess::~FiveChess()
{

}

FiveChess& FiveChess::GetFiveChessInstall()
{
	static FiveChess singl_fivechess;
	return singl_fivechess;
}

bool FiveChess::OnPlaying(const int pos_x, const int pos_y, const char player_type)
{
	if (this->SetChessPiecesPos(pos_x, pos_y, player_type))
	{
		if (this->CheckWin(pos_x, pos_y, player_type))
		{
			this->SendChessPiecesInfo();
			return true;
		}
	}
}

bool FiveChess::SetChessPiecesPos(const int pos_x, const int pos_y, const char player_type)
{
	if (pos_x < 0 || pos_x >= CHESS_BOARD_X_MAX || pos_y < 0 || pos_y >= CHESS_BOARD_Y_MAX) return false;
	if (player_type <= CHESS_PLAYER_TYPE_INVALID || player_type >= CHESS_PLAYER_TYPE_MAX) return false;

	if (CHESS_PLAYER_TYPE_INVALID != m_chess_board_list[pos_x][pos_y]) return false;

	m_chess_board_list[pos_x][pos_y] = static_cast<char>(player_type);
	return true;
}

bool FiveChess::CheckWin(const int cur_pos_x, const int cur_pos_y, const char cur_player)
{
	if (cur_pos_x < 0 || cur_pos_x >= CHESS_BOARD_X_MAX || cur_pos_x < 0 || cur_pos_x >= CHESS_BOARD_Y_MAX) return false;
	if (cur_player <= CHESS_PLAYER_TYPE_INVALID || cur_player >= CHESS_PLAYER_TYPE_MAX) return false;

	for (int direction_cout = 0; direction_cout < 4; ++direction_cout)
	{
		int begin_x = 0;
		int begin_y = 0;
		int x_add = 0;
		int y_add = 0;
		int count = 0;
		switch (direction_cout)
		{
			case DIRECTION_TYPE_XIANG_YOU:
			{
				begin_x = -4;
				begin_y = 0;
				x_add = 1;
				y_add = 0;
				count = CalChessPiecesCount(cur_pos_x, cur_pos_y, begin_x, begin_y, x_add, y_add, cur_player);
			}
			break;

			case DIRECTION_TYPE_XIANG_SHANG:
			{
				begin_x = 0;
				begin_y = -4;
				x_add = 0;
				y_add = 1;
				count = CalChessPiecesCount(cur_pos_x, cur_pos_y, begin_x, begin_y, x_add, y_add, cur_player);
			}
			break;

			case DIRECTION_TYPE_XIANG_YOUXIAJIAO:
			{
				begin_x = -4;
				begin_y = -4;
				x_add = 1;
				y_add = 1;
				count = CalChessPiecesCount(cur_pos_x, cur_pos_y, begin_x, begin_y, x_add, y_add, cur_player);
			}
			break;

			case DIRECTION_TYPE_XIANG_YOUSHANGJIAO:
			{
				begin_x = -4;
				begin_y = 4;
				x_add = 1;
				y_add = -1;
				count = CalChessPiecesCount(cur_pos_x, cur_pos_y, begin_x, begin_y, x_add, y_add, cur_player);
			}
			break;

		}

		if (5 <= count) return true;
	}

	return false;
}

int FiveChess::CalChessPiecesCount(const int cur_pos_x, const int cur_pos_y, const int begin_x, const int begin_y, const int x_add, const int y_add, const char play_type)
{
	int count = 0;
	int begin_pos_x = cur_pos_x + begin_x;
	int begin_pos_y = cur_pos_y + begin_y;
	for (int direction_chess_pieces_count = 0; direction_chess_pieces_count < 9; ++direction_chess_pieces_count)
	{
		if ((begin_pos_x > 0 && begin_pos_x < 15) && (begin_pos_y > 0 && begin_pos_y < 15))
		{
			if (m_chess_board_list[begin_pos_x][begin_pos_y] == play_type)
			{
				++count;
			}

			begin_pos_x += x_add;
			begin_pos_y += y_add;
		}
		else
		{
			begin_pos_x += x_add;
			begin_pos_y += y_add;
			continue;
		}

		if (5 <= count)
		{
			return count;
		}
	}

	return count;
}

bool FiveChess::ChessAIPlay()
{
	// 扫描自己有连续四个的先下
	// 扫描对方有连续四个的
	// 扫描有三个交界的地方先下
	// 扫描对方刚刚下的位置的地方，如果有连续三个以上的交界或者单个的
	// 扫描有两个交界的地方先下
	// 扫描对方有两个交界的地方先下
	// 扫描对方有两个的地方先下
	// 扫描能成两个的先下
}
