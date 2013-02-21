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
namespace ncwin
{

class NCWin : public ncobject::NCObject
{
public:
	// Typedefs
	typedef std::function<int(NCWin*)> ResizeFuncs;

	/**
	 * <b>Purpose:</b> CTOR
	 */
	NCWin
		( NCObject* parent
		, NCWinCfg cfg = NCWinCfg()
		, ResizeFuncs resizeWidth = ResizeFuncs()
		, ResizeFuncs resizeHeight = ResizeFuncs()
		, ResizeFuncs resizeX = ResizeFuncs()
		, ResizeFuncs resizeY = ResizeFuncs() );

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
	// TODO, this should be cleaned up along with the refresh, rRefresh, clear, etc...
	virtual void updateSize();

	/**
	 * <b>Purpose:</b> Configuration getter
	 * @return const reference NCWinCfg
	 */
	const NCWinCfg& getConfig() const;

	/**
	 * <b>Purpose:</b> Scroll window up one level
	 */
//	void scrollUp(const int n);
//	void scrolldown(const int n);

	void print(const char* str);
	void print(const char* str, const int x, const int y);
	void print(const NCString &ncStr);

	// Put cursor at the top left position
	void cursorReset();
	void cursorNextLine();
	void clearTillEnd();
	void cursorSet(const int x, const int y);

	void putChar(const char c, const int x, const int y);

	int winId() const;

	/**
	 * <b>Purpose:</b> Print a const char* string and paint it
	 * based on the color const char* string.  Both must be
	 * NULL terminated.  The color string probably has to be
	 * the exact same length
	 * @param const char* string to print to the window
	 * @param const char* string representing the color to print
	 */
	void printColor(const char* str, const char* color);

	/**
	 * <b>Purpose:</b> Get a single char as input
	 */
	NCWin& operator>>(int &c);


private:
	class NCWinData;
	std::auto_ptr<NCWinData> p_data;

};

} // namespace ncwin
} // namespace ncpp

#endif // NCWIN_H_
