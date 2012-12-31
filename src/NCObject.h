/*
 * NCObject.h
 *
 *  Created on: Oct 24, 2012
 *      Author: dkimmel
 */

#ifndef NCOBJECT_H_
#define NCOBJECT_H_

#include <vector>
#include <functional>

namespace ncpp
{
namespace ncobject
{

// Base type that specs: refresh
class NCObject
{
public:
	/**
	 * <b>Purpose:</b> CTOR
	 */
	NCObject(NCObject* parent);

	/**
	 * <b>Purpose:</b> DTOR
	 */
	virtual ~NCObject();

	/**
	 * <b>Purpose:</b> Refresh the state of this object (and children)
	 */
	virtual void refresh();

	/**
	 * <b>Purpose:</b> Bring child object to front
	 */
	void bringToFront(NCObject* ncobject);

	/**
	 * <b>Purpose:</b> Bring child object to back
	 */
	void bringToBack(NCObject* ncobject);

	/**
	 * <b>Purpose:</b> Rotate window list
	 */
	void rotate();

	/**
	 * <b>Purpose:</b> Rotate window list back
	 */
	void rotateBack();

	/**
	 * <b>Purpose:</b> Get top (last) entry in list
	 */
	NCObject* getTop();

	/**
	 * <b>Purpose:</b> Provide way to iterator over all child objects
	 * TODO, deprecate in favor of begin/end?
	 */
	void forEachChild(std::function<bool(NCObject*)> func);

protected:
	// In the future might be nice to pull these out
	void add(NCObject* ncobject);
	void remove(NCObject* ncobject);


private:
	NCObject* p_parent;
	std::vector<NCObject*> p_objVec;
};

} // namespace ncobject
} // namespace ncpp

#endif // NCOBJECT_H_
