#ifndef __MINSUM_H__
#define __MINSUM_H__

#include "LogicItem.h"
#include <set>

class MinSingle;
typedef std::shared_ptr<MinSingle> MinSinglePtr;

class MinSum:public LogicItem
{
	friend class SimplifiedExpression;
public:
	MinSum();
	MinSum(const std::vector<int> &minItemIDs);
	ExpressionPtr Simplify();
	void IntersectWith(const LogicItemPtr rhs);
	void UnionWith(const LogicItemPtr rhs);
	void InverseItem();

	LogicItemPtr IntersectToNewItem(const LogicItemPtr rhs) const override;
	LogicItemPtr UnionToNewItem(const LogicItemPtr rhs) const override;
	LogicItemPtr InverseToNewItem() const override;


	std::string Tostring() const override;
	MinSumPtr CloneToMinSum() const override;
	ExpressionPtr Clone() const override;
private:
	MinSum(const MinSum &item);
	MinSum(const MinSum &&item);
	std::map<int,MinSinglePtr> minItems;
};

#endif