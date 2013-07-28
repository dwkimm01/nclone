/*
 * NCUtils.h
 *
 *  Created on: Nov 10, 2012
 *      Author: dwkimm01
 */

#ifndef NCUTILS_H_
#define NCUTILS_H_

namespace ncpp
{
namespace ncutils
{

/**
 * <b>Purpose:</b> Prevent compilation complaints
 * int var;
 * UNUSED(var);
 */
#define UNUSED(x) ((void)(x))

} // namespace ncutils
} // namespace ncpp

#endif // NCUTILS_H_
