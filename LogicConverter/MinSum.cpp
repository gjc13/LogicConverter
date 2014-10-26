#include "MinSum.h"
#include "MinSingle.h"
#include "SimplifiedExpression.h"
#include <algorithm>
#include <sstream>

using namespace std;

MinSum::MinSum()
{
}

MinSum::MinSum(const std::vector<int> &minItemIDs)
{
	for(int id:minItemIDs)
	{
		minItems[id]=(MinSinglePtr(new MinSingle(id)));
	}
}

MinSum::MinSum(const MinSum &item)
	:minItems(item.minItems)
{
}

MinSum::MinSum(const MinSum &&item)
	:minItems(std::move(item.minItems))
{
}

ExpressionPtr MinSum::Simplify()
{
	return SimpPtr(new SimplifiedExpression(*this));
}

LogicItemPtr MinSum::IntersectToNewItem(const LogicItemPtr rhs) const
{
	MinSumPtr result(new MinSum(*this));
	result->IntersectWith(rhs);
	return result;
}

void MinSum::IntersectWith(const LogicItemPtr rhs)
{
	map<int,MinSinglePtr> result;
	MinSumPtr minSumRhs=dynamic_pointer_cast<MinSum>(rhs);
	if(minSumRhs==nullptr)
	{
		minSumRhs=rhs->CloneToMinSum();
	}
	for(auto & minitem:minSumRhs->minItems)
	{
		if(minItems.find(minitem.second->GetID())!=minItems.end())
		{
			result[minitem.second->GetID()]=minitem.second;
		}
	}
	minItems=std::move(result);
}

LogicItemPtr MinSum::UnionToNewItem(const LogicItemPtr rhs) const
{
	MinSumPtr result(new MinSum(*this));
	result->UnionWith(rhs);
	return result;
}

void MinSum::UnionWith(const LogicItemPtr rhs)
{
	MinSumPtr minSumRhs=dynamic_pointer_cast<MinSum>(rhs);
	if(minSumRhs==nullptr)
	{
		minSumRhs=rhs->CloneToMinSum();
	}
	for(auto & minitem:minSumRhs->minItems)
	{
		minItems.insert(minitem);
	}
}

LogicItemPtr MinSum::InverseToNewItem() const
{
	MinSumPtr result(new MinSum(*this));
	result->InverseItem();
	return result;
}

void MinSum::InverseItem()
{
	map<int,MinSinglePtr> result;
	for(int i=0; i<pow(2,numVariable); i++)
	{
		if(minItems.find(i)==minItems.end())
		{
			result[i]=MinSinglePtr(new MinSingle(i)); 
		}
	}
	minItems=std::move(result);
}

string MinSum::Tostring() const
{
	ostringstream sout;
	if(minItems.empty())
	{
		return string("0");
	}
	sout<<"MinTerm with Order:";
	for(char c:variables)
	{
		sout<<c;
	}
	sout<<endl;
	sout<<"Sum m(";

	auto iter=minItems.begin();
	sout<<iter->second->GetID();
	iter++;
	while(iter!=minItems.end())
	{
		sout<<","<<iter->second->GetID();
		iter++;
	}
	sout<<")";
	return sout.str();
}

MinSumPtr MinSum::CloneToMinSum() const
{
	return MinSumPtr(new MinSum(*this));
}

ExpressionPtr MinSum::Clone() const
{
	return CloneToMinSum();
}


