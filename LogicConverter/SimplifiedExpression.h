#ifndef __SIMPLIFIEDEXPRESSION_H__
#define __SIMPLIFIEDEXPRESSION_H__

#include "LogicExpression.h"
#include "MinSum.h"
#include "SingleItem.h"
#include <list>

class SimplifiedExpression;
typedef std::shared_ptr<SimplifiedExpression> SimpPtr;

class SimplifiedExpression:public LogicExpression
{
public:
	SimplifiedExpression(const MinSum &minsum);
	std::string Tostring() const override;
	ExpressionPtr Clone() const override;
private:
	SimplifiedExpression(const SimplifiedExpression &expression);
	SimplifiedExpression(const SimplifiedExpression &&expression);
	void CombineItems();
	void FindFinalItems();
	bool IsCombineEnd();

	std::vector<std::list<std::pair<SingleItemPtr,bool>>> nowItems;
	std::vector<SingleItemPtr> uniqueItems;
};


#endif