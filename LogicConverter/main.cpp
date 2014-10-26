#include "ExpressionTree.h"
#include <iostream>

using namespace std;

int main()
{
	ExpressionTreePtr expressionTree=ExpressionTree::GetExpressionTree();
    cout<<"Logic Converter by gjc,press q to quit"<<endl;
	while(true)
	{
		cout<<"input : ";
		string str;
		getline(cin,str);
        if(str=="q")
        {
            break;
        }
		try
		{
			expressionTree->SetExpression(str);
			MinSumPtr result=expressionTree->Evaluate();
			string minStr=result->Tostring();
			cout<<minStr<<endl;
			//string simpleStr=result->Simplify()->Tostring();
			//cout<<"result: "<<simpleStr<<endl;
			cout<<endl;
		}
		catch(const exception &ex)
		{
			cout<<"Not A Valid Expression"<<endl;
		}
	}
}
