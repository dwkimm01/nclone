#include "NCBuddy.h"

namespace ncpp
{
namespace ncbuddy
{

NCBuddy::NCBuddy() : p_chatUpdated(false) {}

NCBuddy::NCBuddy(const std::string &connection, const std::string &fullName, const std::string &nickName)
	: p_connnection(connection), p_fullName(fullName), p_nickName(nickName), p_dispName(nickName), p_status("Available"), p_chatUpdated(false)
{
}

const std::string& NCBuddy::connection() const { return p_connnection; }
const std::string& NCBuddy::full() const { return p_fullName; }
const std::string& NCBuddy::nick() const { return p_nickName; }
const std::string& NCBuddy::display() const { return p_nickName; }
void NCBuddy::appendChat(const std::string &msg)
{
	p_chatText.push_back(msg);
	p_chatUpdated = true;
}

std::vector<std::string>& NCBuddy::getChat() { return p_chatText; }
std::string NCBuddy::getStatus() const { return p_status; }

void NCBuddy::setStatus(const std::string &status) { p_status = status; }
bool NCBuddy::getChatUpdated() const { return p_chatUpdated; }

} // namespace ncbuddy
} // namespace ncpp