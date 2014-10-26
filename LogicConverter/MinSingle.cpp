#include "MinSingle.h"

MinSingle::MinSingle(int id)
	:itemID(id)
{
	for(int i=0; i!=numVariable; i++)
	{
		itemBits[numVariable-i-1]=BitStaus((id>>i)&0x01);
	}
}

bool MinSingle::UnionSingle(const SingleItemPtr rhs)
{
	if(!SingleItem::Equals(rhs))
	{
		return false;
	}
	return true;
}

void MinSingle::IntersectSingle(const SingleItemPtr rhs)
{
	if(!SingleItem::Equals(rhs))
	{
		itemBits.clear();
	}
}

ExpressionPtr MinSingle::Clone() const
{
	return MinSinglePtr(new MinSingle(*this)); 
}

MinSingle::MinSingle(const MinSingle &item)
	:SingleItem(item),itemID(item.itemID)
{
}

MinSingle::MinSingle(const MinSingle &&item)
	:SingleItem(item),itemID(item.itemID)
{
}