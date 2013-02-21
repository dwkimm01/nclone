/*
 * NCObject.cpp
 *
 *  Created on: Oct 24, 2012
 *      Author: dkimmel
 */


/*
 * NCObject.h
 *
 *  Created on: Oct 24, 2012
 *      Author: dkimmel
 */

#include <algorithm>
#include <functional>
#include "NCObject.h"
#include "NCStlUtils.h"

namespace ncpp
{
namespace ncobject
{

NCObject::NCObject(NCObject* parent) : p_parent(parent)
{
	if(p_parent)
	{
		p_parent->add(this);
	}
}

NCObject::~NCObject()
{
	if(p_parent)
	{
		p_parent->remove(this);
		p_parent = 0;
	}
}

void NCObject::refresh()
{
	// Refresh all children in order, last will be top (most visible) on screen
	std::for_each
		( p_objVec.begin()
		, p_objVec.end()
		, std::mem_fun(&NCObject::refresh)
	);
}


void NCObject::add(NCObject* ncobject)
{
	// Add, if it is not already added
	if( (std::find(p_objVec.begin(), p_objVec.end(), ncobject) == p_objVec.end()) 
	&& (p_parent != ncobject) )
	{
		p_objVec.push_back(ncobject);
	}
}


void NCObject::remove(NCObject* ncobject)
{
	// Erase, if found in vector
	p_objVec.erase(std::find(p_objVec.begin(), p_objVec.end(), ncobject));
}


void NCObject::bringToFront(NCObject* ncobject)
{
	// Use gather: begin range, end range, point to drag to, fn to determine which object to drag
	ncstlutils::gather
		( p_objVec.begin()
		, p_objVec.end()
		, p_objVec.end()
		, std::function<bool(NCObject*)>([&](NCObject* o)
			{
				return o == ncobject;
			})
		);
}


void NCObject::bringToBack(NCObject* ncobject)
{
	// Use gather: begin range, end range, point to drag to, fn to determine which object to drag
	ncstlutils::gather
		( p_objVec.begin()
		, p_objVec.end()
		, p_objVec.begin()
		, std::function<bool(NCObject*)>([&](NCObject* o)
			{
				return o == ncobject;
			})
		);
}

bool NCObject::isOnTopOf(NCObject* a, NCObject* b)
{
	// Determine which is on top, winKeys or the chat windows
	int aPos = 0;
	int bPos = 0;
	int counter = 0;
	forEachChild([&](ncpp::ncobject::NCObject* obj)
	{
		++counter;
		if(obj == a)
		{
			aPos = counter;
		}
		else if(obj == b)
		{
			bPos = counter;
		}
		if(0 != aPos && 0 != bPos)
			return false;
		return true;
	});

	return aPos > bPos;
}



void NCObject::rotate()
{
	// Rotate, much like a Windows ALT-TAB
	std::rotate(p_objVec.begin(), p_objVec.begin() + 1, p_objVec.end());
}


void NCObject::rotateBack()
{
	// Rotate, much like a Windows ALT-TAB, but backwards
	std::rotate(p_objVec.begin(), p_objVec.end() - 1, p_objVec.end());
}


NCObject* NCObject::getTop()
{
	return (p_objVec.size())?(p_objVec.back()):(NULL);
}

void NCObject::forEachChild(std::function<bool(NCObject*)> func)
{
	if(func)
	{
		for(auto x : p_objVec)
		{
			// Call method on child
			if(!func(x))
			{
				return;
			}

			// Recurse through child
			x->forEachChild(func);
		}
	}
}

} // namespace ncobject
} // namespace ncpp
