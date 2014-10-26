#ifndef __LOGICITEM_H__
#define __LOGICITEM_H__
#include "common.h"
#include "LogicExpression.h"

class LogicItem;
typedef std::shared_ptr<LogicItem> LogicItemPtr;

class LogicItem:public LogicExpression
{
public:
	LogicItem(){}
	virtual MinSumPtr CloneToMinSum() const=0;
	virtual LogicItemPtr IntersectToNewItem(const LogicItemPtr rhs) const=0;
	virtual LogicItemPtr UnionToNewItem(const LogicItemPtr rhs) const=0;
	virtual LogicItemPtr InverseToNewItem() const=0;
private:
	LogicItem(const LogicItem &item){}
};

#endif