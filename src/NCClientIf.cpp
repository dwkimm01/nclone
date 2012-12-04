/*
 * NCClientIf.cpp
 *
 *  Created on: Nov 10, 2012
 *      Author: dwkimm01
 */

#include "NCClientIf.h"

namespace ncpp
{

namespace ncclientif
{

NCClientIf::~NCClientIf() {}


void NCClientIf::connectionProgress(const String &text, const int step, const int count) {}
void NCClientIf::connected() {}
void NCClientIf::disconnected() {}
void NCClientIf::connectionNotice(const String &text) {}
void NCClientIf::disconnectNotice(const String &text, const int reason) {}
void NCClientIf::contactTyping() {}

void NCClientIf::msgReceived(const String &who, const String &msg) {}
void NCClientIf::questionAskedBool(const int id, const String &question) {}
void NCClientIf::debugLog(const String &level, const String &logLine) {}


} // namespace ncclientif

} // namespace ncpp

