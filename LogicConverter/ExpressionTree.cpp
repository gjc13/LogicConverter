#include "ExpressionTree.h"
#include <cctype>

using namespace std;

ExpressionTreePtr ExpressionTree::nowExpression=nullptr;

ExpressionTree::ExpressionTree()
	:root(nullptr)
{
}

ExpressionTreePtr ExpressionTree::GetExpressionTree()
{
	if(nowExpression==nullptr)
	{
		nowExpression=ExpressionTreePtr(new ExpressionTree());
	}
	return nowExpression;
}

void ExpressionTree::SetExpression(const string &expressionStr)
{
	expression=expressionStr;
	CheckExpression();
	SimplifyInverses();
	AddOperators();
	root=BuildExpressionTree();
}

void ExpressionTree::CheckExpression()
{
	LogicExpression::ClearVariable();
	auto iter=expression.begin();
	while(iter!=expression.end())
	{
		if(*iter==' ')
		{
			iter=expression.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	for(char c:expression)
	{
		if(isalpha(c))
		{
			LogicExpression::SubmitVariable(c);
			continue;
		}
		else if(c=='\'' || c=='*' || c=='+' || c=='(' || c==')')
		{
			continue;
		}
		throw Expression_Exception();
	}
	LogicExpression::SetVariableIDs();
}

void ExpressionTree::SimplifyInverses()
{
	unsigned pos=0;
	while((pos=expression.find('\'',pos))<expression.size())
	{
		if(pos==0)
		{
			throw Expression_Exception();
		}
		unsigned numinv=0;
		while(pos+numinv!=expression.size() && expression[pos+numinv]=='\'')
		{
			numinv++;
		}
		if(numinv%2==0)
		{
			expression.erase(pos,numinv);
		}
		else
		{
			expression.erase(pos,numinv-1);
			if(isalpha(expression[pos-1]))
			{
				expression[pos]='`';
			}
			pos++;
		}
		if(pos>=expression.size())
		{
			break;
		}
	}
}

void ExpressionTree::AddOperators()	
{
	unsigned pos=0;
	while((pos=expression.find('(',pos))<expression.size())
	{
		if(pos>0 && (expression[pos-1]==')'|| expression[pos-1]=='\''|| expression[pos-1]=='`' || isalpha(expression[pos-1])))
		{
			expression.insert(pos,"*");
			pos++;
		}
		pos++;
	}
	pos=0;
	while((pos=expression.find(')',pos))<expression.size())
	{
		if(pos!=expression.size()-1 && isalpha(expression[pos+1]))
		{
			expression.insert(pos+1,"*");
			pos++;
		}
		pos++;
	}
}

ExpressionTree::TreeNodePtr ExpressionTree::BuildExpressionTree()
{
	static int depth=0;
	stack<char> operators;
	stack<TreeNodePtr> operands;
	bool canReadOperand=true;
	bool canReadOperator=false;
	try
	{
		while(!expression.empty())
		{
			if(canReadOperand)
			{
				if(IsStartWithLeftBracket())
				{
					depth++;
					operands.push(BuildExpressionTree());
					canReadOperand=false;
					canReadOperator=true;
				}
				else
				{
					TreeNodePtr newNode;
					string str=GetOperand();
					if(!str.empty())
					{
						operands.push(TreeNodePtr(new TreeNode(str)));
						canReadOperand=false;
						canReadOperator=true;
					}
					else
					{
						throw Expression_Exception();
					}
				}
			}
			else if(canReadOperator)
			{
				if(IsStartWithRightBracket())
				{
					depth--;
					if(depth<0)
					{
						depth=0;
						throw Expression_Exception();
					}
					else
					{
						return EvaluateFullStack(operators,operands);
					}
				}
				else
				{
					if(char oper=GetOperator())
					{
						PushOperator(operators,operands,oper);
						if(oper!='\'')
						{
							canReadOperand=true;
							canReadOperator=false;
						}
					}
					else
					{
						throw Expression_Exception();
					}
				}
			}
		}
		if(depth==0)
		{
			return EvaluateFullStack(operators,operands);
		}
		else
		{
			throw Expression_Exception();
		}
	}
	catch(const Expression_Exception &ex)
	{
		depth=0;
		throw ex;
	}
}

char ExpressionTree::GetOperator()
{
	if(expression.find_first_of("+*'")==0)
	{
		char oper=expression[0];
		expression.erase(expression.begin());
		return oper;
	}
	return 0;
}

string ExpressionTree::GetOperand()
{
	unsigned pos=expression.find_first_of("+*'()");
	if(pos>=expression.size())
	{
		string str=expression;
		expression.clear();
		return str;
	}
	else
	{
		string str=expression.substr(0,pos);
		expression.erase(0,pos);
		return str;
	}
}

ExpressionTree::TreeNodePtr ExpressionTree::EvaluateFullStack(stack<char> &operators,stack<TreeNodePtr> &operands)
{
	while(!operators.empty())
	{
		EvaluateOperator(operators,operands);
	}
	if(operands.size()!=1)
	{
		throw Expression_Exception();
	}
	else
	{
		return operands.top();
	}
}

void ExpressionTree::EvaluateOperator(stack<char> &operators,stack<TreeNodePtr> &operands)
{
	if(operators.top()=='\'')
	{
		if(operands.empty())
		{
			throw Expression_Exception();
		}
		TreeNodePtr newNode=TreeNodePtr(new TreeNode("'"));
		newNode->left=operands.top();
		operands.pop();
		operands.push(newNode);
	}
	else
	{
		if(operands.size()<2)
		{
			throw Expression_Exception();
		}
		TreeNodePtr rhs=operands.top();
		operands.pop();
		TreeNodePtr lhs=operands.top();
		operands.pop();
		TreeNodePtr newNode=nullptr;
		if(operators.top()=='*')
		{
			newNode=TreeNodePtr(new TreeNode("*"));
		}
		else if(operators.top()=='+')
		{
			newNode=TreeNodePtr(new TreeNode("+"));
		}
		newNode->left=lhs;
		newNode->right=rhs;
		operands.push(newNode);
	}
	operators.pop();
}

void ExpressionTree::PushOperator(stack<char> &operators,stack<TreeNodePtr> &operands,char oper)
{
	while(!operators.empty() && GetOperatorPiriority(operators.top())>GetOperatorPiriority(oper))
	{
		EvaluateOperator(operators,operands);
	}
	operators.push(oper);
}

int ExpressionTree::GetOperatorPiriority(char oper)
{
	switch (oper)
	{
	case '+':
		return 0;
	case '*':
		return 1;
	case '\'':
		return 2;
	default:
		break;
	}
	throw Expression_Exception();
}

bool ExpressionTree::IsStartWithLeftBracket()
{
	if(expression[0]=='(')
	{
		expression.erase(expression.begin());
		return true;
	}
	return false;
}

bool ExpressionTree::IsStartWithRightBracket()
{
	if(expression[0]==')')
	{
		expression.erase(expression.begin());
		return true;
	}
	return false;
}

MinSumPtr ExpressionTree::Evaluate()
{
	return Evaluate(root);
}

MinSumPtr ExpressionTree::Evaluate(TreeNodePtr node)
{
	if(node==nullptr)
	{
		return nullptr;
	}
	if(node->left==nullptr && node->right==nullptr)
	{
		SingleItemPtr item(new SingleItem(node->val));
		return item->CloneToMinSum();
	}
	if(node->val=="'")
	{
		MinSumPtr lhs=Evaluate(node->left);
		return dynamic_pointer_cast<MinSum>(lhs->InverseToNewItem());
	}
	if(node->val=="+")
	{
		MinSumPtr lhs=Evaluate(node->left);
		MinSumPtr rhs=Evaluate(node->right);
		lhs->UnionWith(rhs);
		return lhs;
	}
	if(node->val=="*")
	{
		MinSumPtr lhs=Evaluate(node->left);
		MinSumPtr rhs=Evaluate(node->right);
		lhs->IntersectWith(rhs);
		return lhs;
	}
}
