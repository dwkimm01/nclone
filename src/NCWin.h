/*
 * NCWin.h
 *
 *  Created on: Oct 23, 2012
 *      Author: dkimmel
 */

#ifndef NCWIN_H_
#define NCWIN_H_

#include <memory>
#include "NCObject.h"
#include "NCWinCfg.h"
#include "NCString.h"

namespace ncpp
{

class NCWin : public NCObject
{
public:
	/**
	 * <b>Purpose:</b> CTOR
	 */
	NCWin(NCObject* parent, NCWinCfg cfg = NCWinCfg());

	/**
	 * <b>Purpose:</b> DTOR
	 */
	virtual ~NCWin();

	/**
	 * <b>Purpose:</b> Refresh
	 */
	virtual void refresh();
	virtual void rRefresh(); // TODO, redo how this is being done
	virtual void clear();

	/**
	 * <b>Purpose:</b> Config get
	 */
	const NCWinCfg& getConfig() const;

	/**
	 * <b>Purpose:</b> Scroll window up one level
	 */
	void scrollup1(const int n);
//	void scrolldown(const int n);

	void print(const char* str);
	void print(const char* str, const int x, const int y);
	void print(const NCString &ncStr);

	// Put cursor at the top left position
	void cursorReset();
	void cursorNextLine();
	void clearTillEnd();

	void putChar(const char c, const int x, const int y);

	int winId() const;

	void printColor(const char* str, const char* color);

	// Resizing automatically functions
	typedef std::function<int(NCWin*)> SFunc;
	SFunc resizeHeight;
	SFunc resizeWidth;



private:
	class NCWinData;
	std::auto_ptr<NCWinData> p_data;

};

} // namespace ncpp

#endif // NCWIN_H_
