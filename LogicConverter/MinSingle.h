#ifndef __MINSINGLE_H__
#define __MINSINGLE_H__

#include "SingleItem.h"

class MinSingle;
typedef std::shared_ptr<MinSingle> MinSinglePtr;

class MinSingle:public SingleItem
{
public:
	MinSingle(int id);
	bool UnionSingle(const SingleItemPtr rhs) override;
	void IntersectSingle(const SingleItemPtr rhs) override;
	ExpressionPtr Clone() const override;

	int GetID(){return itemID;}

	bool Equals(const MinSinglePtr rhs)
	{
		return itemID==rhs->itemID && !empty() && !rhs->empty();
	}

private:
	int itemID;
	MinSingle(const MinSingle &item);
	MinSingle(const MinSingle &&item);
};

#endif