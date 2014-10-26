#include "LogicExpression.h"

using namespace std;

map<char,int> LogicExpression::variableToInt;
map<int,char> LogicExpression::intToVariable;
std::set<char> LogicExpression::variables;
int LogicExpression::numVariable=0;

void LogicExpression::SubmitVariable(char c)
{
	if(variables.find(c)==variables.end())
	{
		numVariable++;
		variables.insert(c);
	}
}

void LogicExpression::SetVariableIDs()
{
	int id=0;
	for(auto variable:variables)
	{
		variableToInt[variable]=id;
		intToVariable[id]=variable;
		id++;
	}
}

void LogicExpression::ClearVariable()
{
	numVariable=0;
	variables.clear();
	variableToInt.clear();
	intToVariable.clear();
}