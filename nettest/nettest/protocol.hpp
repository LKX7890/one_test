#ifndef __MESSAGE_HPP_
#define __MESSAGE_HPP_

#pragma pack(push, 4)
namespace protocol
{
	enum MSG_TYPE
	{
		MSG_TYPE_HEART = 1,
		MSG_TYPE_FIVECHESS_POS_CS = 2,
		MSG_TYPE_FIVECHESS_RET_SC = 3,
		MSG_TYPE_MAX,
	};

	struct MsgHeader
	{
		MsgHeader(int type, int size) :m_msg_type(type){}

		int GetHeaderType() { return m_msg_type; }
		int GetMsgSize() { return m_msg_size; }

		int m_msg_type;
		int m_msg_size;
	};

	struct Heart
	{
		Heart():header(1, sizeof(Heart)), m_h(0), m_m(0), m_s(0){}

		MsgHeader header;

		int m_h;
		int m_m;
		int m_s;
	};

	struct CSFiveChessPos
	{
		CSFiveChessPos() :header(MSG_TYPE_FIVECHESS_POS_CS, sizeof(CSFiveChessPos)), x(0), y(0), player_type(0), reserve_ch(0) {}
		MsgHeader header;

		char x;
		char y;
		char player_type;
		char reserve_ch;
	};

	struct SCFiveChessRet
	{
		SCFiveChessRet() :header(MSG_TYPE_FIVECHESS_RET_SC, sizeof(SCFiveChessRet)), x(0), y(0), winner_type(0), reserve_ch(0){}

		MsgHeader header;

		char x;
		char y;
		char winner_type;
		char reserve_ch;
	};
}

#pragma pack(pop)
#endif // !__MESSAGE_HPP_


