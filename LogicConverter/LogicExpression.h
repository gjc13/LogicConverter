#ifndef __LOGICEXPRESSION_H__
#define __LOGICEXPRESSION_H__

#include "common.h"
#include <set>

class LogicExpression;
typedef std::shared_ptr<LogicExpression> ExpressionPtr;

class MinSum;
typedef std::shared_ptr<MinSum> MinSumPtr;

class LogicExpression
{
public:
	LogicExpression(){}
	virtual std::string Tostring() const=0;
	virtual ExpressionPtr Clone() const=0;
	static int GetNumVariable() { return numVariable; }
	static void SubmitVariable(char c);
	static void ClearVariable();
	static void SetVariableIDs();
	virtual ~LogicExpression(){}
protected:
	static std::map<char,int> variableToInt;
	static std::map<int,char> intToVariable;
	static std::set<char> variables;
	static int numVariable;
private:
	LogicExpression(const LogicExpression &item){}
};

#endif