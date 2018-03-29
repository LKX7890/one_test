#ifndef __MESSAGEHANDLER_HPP_
#define __MESSAGEHANDLER_HPP_
#include <cstdlib>
#include "protocol.hpp"


class MessageHandler
{
public:
	MessageHandler();
	~MessageHandler(){}

	static MessageHandler* GetMessageHandlerInstall();

	void OnMessageHandler(int msg_type, void* msg_data);
	void OnHeartMsg(void* msg_data);
	void OnFiveChessPlayingMsg(void* msg_data);

	typedef void(MessageHandler::*func)(void*msg_data);
	func msg_handler_list[protocol::MSG_TYPE_MAX];
private:


	static MessageHandler* m_messagehandler;
};

#endif
