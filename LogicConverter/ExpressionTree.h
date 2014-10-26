#ifndef __EXPRESSIONTREE_H__
#define __EXPRESSIONTREE_H__

#include "SimplifiedExpression.h"
#include "MinSum.h"
#include "MinSingle.h"
#include <stack>

class ExpressionTree;
typedef std::shared_ptr<ExpressionTree> ExpressionTreePtr;

class ExpressionTree
{
private:
	struct TreeNode;
	typedef std::shared_ptr<TreeNode> TreeNodePtr;
	struct TreeNode
	{
		TreeNode(const std::string &sval,TreeNodePtr pleft=nullptr,TreeNodePtr pright=nullptr)
			:val(sval),left(pleft),right(pright)
		{
		}
		std::string val;
		TreeNodePtr left;
		TreeNodePtr right;
	};
public:
	void SetExpression(const std::string &expressionStr);

	MinSumPtr Evaluate();

	static ExpressionTreePtr GetExpressionTree();

private:
	MinSumPtr Evaluate(TreeNodePtr node);

	ExpressionTree();

	void SimplifyInverses();

	TreeNodePtr BuildExpressionTree();

	void AddOperators();

	bool IsStartWithLeftBracket();

	bool IsStartWithRightBracket();

	void EvaluateOperator(std::stack<char> &operators,std::stack<TreeNodePtr> &operands);

	void PushOperator(std::stack<char> &operators,std::stack<TreeNodePtr> &operands,char oper);

	char GetOperator();

	std::string GetOperand();

	int GetOperatorPiriority(char oper);

	TreeNodePtr EvaluateFullStack(std::stack<char> &operators,std::stack<TreeNodePtr> &operands);

	void CheckExpression();

	std::string expression;
	TreeNodePtr root;

	static ExpressionTreePtr nowExpression;
};

#endif