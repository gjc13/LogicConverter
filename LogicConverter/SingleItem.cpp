#include "SingleItem.h"
#include "MinSum.h"
#include <utility>
#include <sstream>
#include <cmath>

using namespace std;

SingleItem::SingleItem(string str)
	:itemBits(numVariable,BitStaus::ALL)
{
	int i=0;
	while(i!=str.size())
	{
		if(i!=str.size()-1 && str[i+1]=='`')
		{
			itemBits[variableToInt[str[i]]]=BitStaus::ZERO;
			i+=2;
		}
		else
		{
			itemBits[variableToInt[str[i]]]=BitStaus::ONE;
			i++;
		}
	}
}

SingleItem::SingleItem()
{
	itemBits.resize(numVariable);
}

bool SingleItem::Equals(const SingleItemPtr rhs) const
{
	if(empty())
	{
		return rhs->empty();
	}
	for(int i=0; i!=numVariable; i++)
	{
		if((*this)[i]!=(*rhs)[i])
		{
			return false;
		}
	}
	return true;
}

SingleItemPtr SingleItem::UninonToNewSingle(const SingleItemPtr rhs) const
{
	if(empty())
	{
		return dynamic_pointer_cast<SingleItem>(rhs->Clone());
	}
	SingleItemPtr newSingle(new SingleItem(*this));
	if(newSingle->UnionSingle(rhs))
	{
		return newSingle;
	}
	else
	{
		return nullptr;
	}
}

bool SingleItem::UnionSingle(const SingleItemPtr rhs)
{
	if(empty())
	{
		itemBits=rhs->itemBits;
		return true;
	}
	bool unioned=false;
	vector<BitStaus> bufBits=itemBits;
	for(int i=0; i<numVariable; i++)
	{
		if(bufBits[i]!=(*rhs)[i] && !unioned)
		{
			bufBits[i]=BitStaus::ALL;
			unioned=true;
		}
		else if(bufBits[i]!=(*rhs)[i])
		{
			return false;
		}
	}
	itemBits=std::move(bufBits);
	return true;
}

LogicItemPtr SingleItem::UnionToNewItem(const LogicItemPtr item) const
{
	if(empty())
	{
		return item->CloneToMinSum();
	}
	if(MinSumPtr sumItem=dynamic_pointer_cast<MinSum>(item))
	{
		MinSumPtr thisSum=this->CloneToMinSum();
		thisSum->UnionWith(sumItem);
		return thisSum;
	}
	MinSumPtr thisSum=this->CloneToMinSum();
	thisSum->UnionWith(item->CloneToMinSum());
	return thisSum;
}


void SingleItem::IntersectSingle(const SingleItemPtr rhs)
{
	if(empty())
	{
		return;
	}
	for(int i=0; i!=numVariable; i++)
	{
		if(itemBits[i]==rhs->itemBits[i] || rhs->itemBits[i]==BitStaus::ALL)
		{
			continue;
		}
		else if(itemBits[i]==BitStaus::ALL)
		{
			itemBits[i]=(*rhs)[i];
			continue;
		}
		else if(itemBits[i]!=(*rhs)[i])
		{
			itemBits.clear();
			break;
		}
	}
}

SingleItemPtr SingleItem::IntersectToNewSingle(const SingleItemPtr rhs) const
{
	if(empty())
	{
		return dynamic_pointer_cast<SingleItem>(this->Clone());
	}
	SingleItemPtr newItem(new SingleItem(*this));
	newItem->IntersectSingle(rhs);
	return newItem;
}

LogicItemPtr SingleItem::IntersectToNewItem(const LogicItemPtr rhs) const
{
	if(empty())
	{
		return MinSumPtr(new MinSum());
	}
	if(MinSumPtr sumItem=dynamic_pointer_cast<MinSum>(rhs))
	{
		MinSumPtr thisSum=this->CloneToMinSum();
		thisSum->IntersectWith(sumItem);
		return thisSum;
	}
	MinSumPtr thisSum=this->CloneToMinSum();
	thisSum->IntersectWith(rhs->CloneToMinSum());
	return thisSum;	
}

LogicItemPtr SingleItem::InverseToNewItem() const
{
	MinSumPtr thisSum=this->CloneToMinSum();
	thisSum->InverseItem();
	return thisSum;
}

int SingleItem::GetNumOne()
{
	if(empty())
	{
		return 0;
	}
	int numOne=0;
	for(int i=0; i!=numVariable; i++)
	{
		if(itemBits[i]==BitStaus::ONE)
		{
			numOne++;
		}
	}
	return numOne;
}

set<int> SingleItem::GetMinItemIDs() const
{
	int fixedID=0;
	vector<int> variedIDs;
	variedIDs.reserve(128);
	variedIDs.push_back(0);
	for(int i=0; i!=numVariable; i++)
	{
		if(itemBits[i]==BitStaus::ONE)
		{
			fixedID+=pow(2,numVariable-i-1);
		}
		else if(itemBits[i]==BitStaus::ALL)
		{
			int num=variedIDs.size();
			for(int j=0; j!=num; j++)
			{
				variedIDs.push_back(variedIDs[j]+pow(2,numVariable-i-1));
			}
		}
	}
	set<int> minItemIDs;
	for(auto val:variedIDs)
	{
		minItemIDs.insert(val+fixedID);
	}
	return minItemIDs;
}

string SingleItem::Tostring() const
{
	ostringstream sout;
	for(int i=0; i!=numVariable; i++)
	{
		if(itemBits[i]==BitStaus::ONE)
		{
			sout<<intToVariable[i];
		}
		else if(itemBits[i]==BitStaus::ZERO)
		{
			sout<<intToVariable[i]<<"'";
		}
	}
	return sout.str();
}

MinSumPtr SingleItem::CloneToMinSum() const
{
	int fixedID=0;
	vector<int> variedIDs;
	variedIDs.reserve(128);
	variedIDs.push_back(0);
	for(int i=0; i!=numVariable; i++)
	{
		if(itemBits[i]==BitStaus::ONE)
		{
			fixedID+=pow(2,numVariable-i-1);
		}
		else if(itemBits[i]==BitStaus::ALL)
		{
			int num=variedIDs.size();
			for(int j=0; j!=num; j++)
			{
				variedIDs.push_back(variedIDs[j]+pow(2,numVariable-i-1));
			}
		}
	}
	for(auto &id:variedIDs)
	{
		id+=fixedID;
	}
	return MinSumPtr(new MinSum(variedIDs));
}

ExpressionPtr SingleItem::Clone() const
{
	return SingleItemPtr(new SingleItem(*this));
}

SingleItem::SingleItem(const SingleItem &item)
	:itemBits(item.itemBits)
{
}

SingleItem::SingleItem(const SingleItem &&item)
	:itemBits(std::move(item.itemBits))
{
}