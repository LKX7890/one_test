#ifndef __FIVECHESS_HPP_
#define __FIVECHESS_HPP_

static const int CHESS_BOARD_X_MAX = 16;
static const int CHESS_BOARD_Y_MAX = 16;

class FiveChess
{
public:
	enum CHESS_PIECES_TYPE
	{
		CHESS_PIECES_TYPE_INVALID = 0,
		CHESS_PIECES_TYPE_WHITE,
		CHESS_PIECES_TYPE_BLACK,
	};

	enum CHESS_PLAYER_TYPE
	{
		CHESS_PLAYER_TYPE_INVALID = 0,
		CHESS_PLAYER_TYPE_WHITE,
		CHESS_PLAYER_TYPE_BLACK,
		CHESS_PLAYER_TYPE_MAX,
	};

	enum DIRECTION_TYPE
	{
		DIRECTION_TYPE_XIANG_YOU,			// 左向右 
		DIRECTION_TYPE_XIANG_SHANG,			// 下向上
		DIRECTION_TYPE_XIANG_YOUXIAJIAO,	// 左上角向右下角
		DIRECTION_TYPE_XIANG_YOUSHANGJIAO,	// 左下角向右上角
	};

	static FiveChess& GetFiveChessInstall();

	bool OnPlaying(const int pos_x, const int pos_y, const char player_type);
	bool SetChessPiecesPos(const int pos_x, const int pos_y, const char player_type);
	bool CheckWin(const int cur_pos_x, const int cur_pos_y, const char cur_player);
	int CalChessPiecesCount(const int cur_pos_x, const int cur_pos_y,
		const int begin_x, const int begin_y, const int x_add, const int y_add, const char play_type);

	bool ChessAIPlay();

	void SendChessPiecesInfo();
private:
	FiveChess();
	~FiveChess();

	char m_cur_player_type;
	char m_chess_board_list[CHESS_BOARD_X_MAX][CHESS_BOARD_Y_MAX];
};
#endif
