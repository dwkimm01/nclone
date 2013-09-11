/*
 * NCChats.h
 *
 *  Created on: Sep 7, 2013
 *      Author: dwkimm01
 */

#ifndef NCCHATS_H_
#define NCCHATS_H_

#include <string>
#include <map>
#include "NCString.h"
#include "NCTextBuffer.h"

namespace ncpp
{
namespace ncchats
{

class NCChats
{
public:
	class Chat
	{
	public:
		std::string p_connection;
		std::string p_buddy;
		NCTextBuffer p_textBuffer;
	};


	NCChats();
	~NCChats();

	void Add(const std::string &connection, const std::string &buddy, const NCString &msg);
	void Remove(const std::string &id);

	std::string GetId(const std::string &connection, const std::string &buddy);

	NCTextBuffer* GetChat(const std::string &id);

private:

	std::map<std::string, Chat> p_chatMap;

};

} // namespace ncchats
} // namespace ncpp

#endif // NCCHATS_H_
