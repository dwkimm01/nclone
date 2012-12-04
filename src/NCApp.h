/*
 * NCApp.h
 *
 *  Created on: Oct 23, 2012
 *      Author: dkimmel
 */

#ifndef NCAPP_H_
#define NCAPP_H_

#include "NCObject.h"

namespace ncpp
{

class NCApp : public NCObject
{
public:
	/**
	 * <b>Purpose:</b> CTOR
	 */
	NCApp();

	/**
	 * <b>Purpose:</b> DTOR
	 */
	~NCApp();

	/**
	 * <b>Purpose:</b> Refresh
	 */
	virtual void refresh();

	/**
	 * <b>Purpose:</b> Output a (null terminated) string
	 */
	NCApp& operator<<(const char* str);

	/**
	 * <b>Purpose:</b> Get a single char as input
	 */
	NCApp& operator>>(int &c);

	// getters
	int maxHeight() const;
	int maxWidth() const;

};

} // namespace ncpp

#endif // NCAPP_H_
