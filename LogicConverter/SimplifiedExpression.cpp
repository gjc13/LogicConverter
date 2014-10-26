#include "SimplifiedExpression.h"
#include "MinSingle.h"
#include <algorithm>
#include <iostream>
#include <map>

using namespace std;

bool hasIntersection(const set<int> &lhs,const set<int> &rhs)
{
	for(int val:rhs)
	{
		if(rhs.find(val)!=rhs.end())
		{
			return true;
		}
	}
	return false;
}

SimplifiedExpression::SimplifiedExpression(const MinSum &minsum)
{
	nowItems.resize(numVariable+1);
	for(const auto &item:minsum.minItems)
	{
		nowItems[item.second->GetNumOne()].push_back(pair<SingleItemPtr,bool>(item.second,false));
	}
	CombineItems();
	int nowcount=uniqueItems.size();
	FindFinalItems();
}

SimplifiedExpression::SimplifiedExpression(const SimplifiedExpression &expression)
	:nowItems(expression.nowItems),uniqueItems(expression.uniqueItems)
{
}

SimplifiedExpression::SimplifiedExpression(const SimplifiedExpression &&expression)
	:nowItems(expression.nowItems),uniqueItems(expression.uniqueItems)
{
}

void SimplifiedExpression::CombineItems()
{
	//pair<SingleItemPtr,bool>:SingleItem在这一轮化简中是否已经用到
	while(!IsCombineEnd())
	{
		std::vector<std::list<pair<SingleItemPtr,bool>>> newItems;
		newItems.resize(numVariable+1);
		for(int i=0; i!=(nowItems.size()-1); i++)
		{
			auto iterSource=nowItems[i].begin();
			while(iterSource!=nowItems[i].end())
			{
				SingleItemPtr source=iterSource->first;
				bool hasCombined=false;
				auto iterTarget=nowItems[i+1].begin();
				while(iterTarget!=nowItems[i+1].end())
				{
					SingleItemPtr target=iterTarget->first;
					if(SingleItemPtr result=source->UninonToNewSingle(target))
					{
						newItems[result->GetNumOne()].push_back(pair<SingleItemPtr,bool>(result,false));
						if(!hasCombined)
						{
							iterSource=nowItems[i].erase(iterSource);
						}
						iterTarget->second=true;
						hasCombined=true;
					}
					iterTarget++;
				}
				if(!hasCombined)
				{
					iterSource++;
				}
			}
		}
		for(const auto &lst:nowItems)
		{
			for(const auto &item:lst)
			{
				if(item.second==false)
				{
					bool hasAppeared=false;
					for(auto nowitem:uniqueItems)
					{
						if(nowitem->Equals(item.first))
						{
							hasAppeared=true;
						}
					}
					if(!hasAppeared)
					{
						uniqueItems.push_back(item.first);
					}
				}
			}
		}
		nowItems=std::move(newItems);
	}
}

bool SimplifiedExpression::IsCombineEnd()
{
	for(const auto &lst:nowItems)
	{
		if(!lst.empty())
		{
			return false;
		}
	}
	return true;
}

//void SimplifiedExpression::FindFinalItems()
//{
//	vector<SingleItemPtr> finalItems;
//	vector<pair<int,set<int>>> itemIDs;
//	itemIDs.resize(uniqueItems.size());
//	for(int i=0; i!=uniqueItems.size(); i++)
//	{
//		itemIDs[i]=pair<int,set<int>>(i,uniqueItems[i]->GetMinItemIDs());
//	}
//	sort(itemIDs.begin(),itemIDs.end(),[](const pair<int,set<int>> &a,const pair<int,set<int>> &b)->bool{return a.second.size()<b.second.size();});
//	while(!itemIDs.empty() && itemIDs.back().second.size()>0)
//	{
//		finalItems.push_back(uniqueItems[itemIDs.back().first]);
//		for(int id:itemIDs.back().second)
//		{
//			for(int i=0; i<itemIDs.size()-1; i++)
//			{
//				itemIDs[i].second.erase(id);
//			}
//		}
//		itemIDs.pop_back();
//		sort(itemIDs.begin(),itemIDs.end(),[](const pair<int,set<int>> &a,const pair<int,set<int>> &b)->bool{return a.second.size()<b.second.size();});
//	}
//	uniqueItems=std::move(finalItems);
//}

void SimplifiedExpression::FindFinalItems()
{
	if(uniqueItems.empty() || uniqueItems.size()==1)
	{
		return;
	}
	//所有最小项ID对于的项
	multimap<int,SingleItemPtr> idToItems;

	//结果
	vector<SingleItemPtr> finalItems;
	
	//每一项对于的最小项ID
	map<SingleItemPtr,set<int>> itemIDs;

	//生成数据
	for(int i=0; i!=uniqueItems.size(); i++)
	{
		set<int> minIDs=uniqueItems[i]->GetMinItemIDs();
		for(int ID:minIDs)
		{
			idToItems.insert(make_pair(ID,uniqueItems[i]));
		}
		itemIDs[uniqueItems[i]]=minIDs;
	}

	int nowID=idToItems.begin()->first;
	int noWIDcount=1;
	auto prev=idToItems.begin();
	auto iter=(++prev);
	prev--;
	do
	{
		if(iter->first==nowID)
		{
			noWIDcount++;
		}
		else
		{
			//如果这个最小项ID只有一个对应项
			if(noWIDcount==1)
			{
				//对应项一定是结果的一部分
				SingleItemPtr item=prev->second;
				bool hasAppeared=false;
				for(const auto &exsistingItem:finalItems)
				{
					if(exsistingItem->Equals(item))
					{
						hasAppeared=true;
					}
				}
				if(!hasAppeared)
				{
					finalItems.push_back(item);
				}

				//从每一项的最小项中删除这一项已经包含的最小项
				set<int> itemID=itemIDs[item];
				auto iterID=itemIDs.begin();
				while(iterID!=itemIDs.end())
				{
					for(int ID:itemID)
					{
						iterID->second.erase(ID);
					}

					//对于被删空的最小项，它一定已经被完全包含
					if(iterID->second.empty())
					{
						iterID=itemIDs.erase(iterID);
					}
					else
					{
						iterID++;
					}
				}
			}
			else
			{
				noWIDcount=1;
			}
		}
		if(iter!=idToItems.end())
		{
			nowID=iter->first;
			iter++;prev++;
		}
	}
	while(iter!=idToItems.end());
	
	//现在ItemIDs中一定包含几个互不相交的项集合，在每个项集合中一定有一项的最小项包含了其他所有最小项
	if(itemIDs.empty())
	{
		uniqueItems=std::move(finalItems);
		return;
	}
	vector<pair<SingleItemPtr,set<int>>> possibleItems;
	for(auto &item:itemIDs)
	{
		possibleItems.push_back(item);
	}
	sort(possibleItems.begin(),possibleItems.end(),[](const pair<SingleItemPtr,set<int>> &a,const pair<SingleItemPtr,set<int>> &b){return a.second.size()>b.second.size();});
	/*
	auto iterID=itemIDs.begin();
	possibleItems.push_back(*iterID);
	iterID++;
	while(iterID!=itemIDs.end())
	{
		bool isNew=true;
		for(int i=0; i!=possibleItems.size(); i++)
		{
			if(hasIntersection(iterID->second,possibleItems[i].second))
			{
				if(possibleItems[i].second.size()<iterID->second.size())
				{
					possibleItems[i]=*iterID;
				}
				isNew=false;
				break;
			}
			if(isNew)
			{
				possibleItems.push_back(*iterID);
			}
		}
		iterID++;
	}*/
	for(int i=0; i<possibleItems.size()-1; i++)
	{
		if(possibleItems[i].second.size()>possibleItems[i+1].second.size())
		{
			auto iter=possibleItems.begin()+i+1;
			while(iter!=possibleItems.end())
			{
				if(hasIntersection(possibleItems[i].second,iter->second))
				{
					iter=possibleItems.erase(iter);
				}
				else
				{
					iter++;
				}
			}
		}
	}
	for(auto possibleItem:possibleItems)
	{
		finalItems.push_back(possibleItem.first);
	}
	uniqueItems=std::move(finalItems);
}

string SimplifiedExpression::Tostring() const
{
	if(uniqueItems.empty())
	{
		return string("0");
	}
	string str;
	str.append(uniqueItems[0]->Tostring());
	for(int i=1; i<uniqueItems.size(); i++)
	{
		str.append(" + ");
		str.append(uniqueItems[i]->Tostring());
	}
	return str;
}

ExpressionPtr SimplifiedExpression::Clone() const
{
	return SimpPtr(new SimplifiedExpression(*this));
}
