/*
 * NCClientDummy.cpp
 *
 *  Created on: Aug 25, 2013
 *      Author: dwkimm01
 */

#include "NCClientDummy.h"

namespace ncpp
{
namespace ncclientdummy
{

NCClientDummy::NCClientDummy(const std::string &name)
	: p_name(name)
{
}

NCClientDummy::~NCClientDummy() {}

ncclientif::NCClientIf::String NCClientDummy::getName() { return p_name; }

void NCClientDummy::connect() {}
void NCClientDummy::disconnect() {}

void NCClientDummy::sendTyping(const String &who, const String &msg, bool done) {}
void NCClientDummy::msgSend(const String &who, const String &msg) {}
void NCClientDummy::questionAnswerBool(const int id, const bool response) {}
void NCClientDummy::addBuddy(const String &who, const String &group) {}
void NCClientDummy::removeBuddy(const String &who, const String &group) {}
void NCClientDummy::setAway(const String &state, const String &msg) {}


void NCClientDummy::connectionProgress(const String &text, const int step, const int count) {}
void NCClientDummy::connected() {}
void NCClientDummy::disconnected() {}
void NCClientDummy::connectionNotice(const String &text) {}
void NCClientDummy::disconnectNotice(const String &text, const int reason) {}
void NCClientDummy::contactTyping() {}

void NCClientDummy::msgReceived(const String &who, const String &msg) {}

void NCClientDummy::questionAskedBool(const int id, const String &question) {}

void NCClientDummy::debugLog(const String &level, const String &logLine) {}

} // namespace ncclientdummy
} // namespace ncpp
