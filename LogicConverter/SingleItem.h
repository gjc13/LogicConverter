#ifndef __SINGLEITEM_H__
#define __SINGLEITEM_H__

#include "LogicItem.h"
#include <set>

class SingleItem;
typedef std::shared_ptr<SingleItem> SingleItemPtr;

class SingleItem:public LogicItem
{
public:
	SingleItem(std::string itemStr);
	virtual bool Equals(const SingleItemPtr rhs) const;
	virtual SingleItemPtr UninonToNewSingle(const SingleItemPtr rhs) const;
	virtual SingleItemPtr IntersectToNewSingle(const SingleItemPtr rhs) const;
	virtual bool UnionSingle(const SingleItemPtr rhs) ;
	virtual void IntersectSingle(const SingleItemPtr rhs) ;
	virtual bool empty() const { return itemBits.empty();}
	virtual int GetNumOne() ;
	BitStaus operator[](int i) const {return itemBits[i];}
	BitStaus &operator[](int i) {return itemBits[i];}
	std::set<int> GetMinItemIDs() const;


	LogicItemPtr IntersectToNewItem(const LogicItemPtr rhs) const override;
	LogicItemPtr UnionToNewItem(const LogicItemPtr rhs) const override;
	LogicItemPtr InverseToNewItem() const override;

	std::string Tostring() const override;
	MinSumPtr CloneToMinSum() const override;
	ExpressionPtr Clone() const override;
protected:
	SingleItem();
	std::vector<BitStaus> itemBits;
	SingleItem(const SingleItem &item);
	SingleItem(const SingleItem &&item);
};

#endif