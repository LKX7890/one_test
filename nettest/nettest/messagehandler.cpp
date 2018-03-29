#include <iostream>
#include "messagehandler.hpp"
#include "network.hpp"
#include "fivechess.hpp"

MessageHandler* MessageHandler::m_messagehandler = NULL;

MessageHandler* MessageHandler::GetMessageHandlerInstall()
{
	if (NULL != m_messagehandler)
	{
		return m_messagehandler;
	}
	else
	{
		m_messagehandler = new MessageHandler;
		return m_messagehandler;
	}
}

MessageHandler::MessageHandler()
{
	msg_handler_list[protocol::MSG_TYPE_HEART] = &MessageHandler::OnHeartMsg;
	msg_handler_list[protocol::MSG_TYPE_FIVECHESS_POS_CS] = &MessageHandler::OnFiveChessPlayingMsg;
}

void MessageHandler::OnMessageHandler(int msg_type, void* msg_data)
{
	(this->*msg_handler_list[msg_type])(msg_data);
}

void MessageHandler::OnHeartMsg(void* msg_data)
{
	protocol::Heart* heart = (protocol::Heart*)msg_data;
	std::cout << heart->m_h << ":" << heart->m_m << ":" << heart->m_s << std::endl;

	heart->m_h += 1;
	NetWork::GetNetWorkInstall()->NetWorkSendMsg(heart);
}

void MessageHandler::OnFiveChessPlayingMsg(void* msg_data)
{
	protocol::CSFiveChessPos* chess_pieces_msg = static_cast<protocol::CSFiveChessPos*>(msg_data);
	FiveChess::GetFiveChessInstall().OnPlaying(chess_pieces_msg->x, chess_pieces_msg->y, chess_pieces_msg->player_type);
}

