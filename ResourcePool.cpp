#include "ResourcePool.hpp"

ResourcePool::ResourcePool()
{

}

void ResourcePool::Spend(const ResourceCost& cost){
	stone -= cost.stone;
	money -= cost.money;
	wood -= cost.wood;
}

void ResourcePool::ProcessIncome(){
	money += moneyIncome;
	food += foodIncome;
	wood += woodIncome;
	stone += stoneIncome;
	//people die with no food
	if(food < 0){
		population -= 2;
	}
}

void ResourcePool::GainIncome(const ResourceIncome& income){
	moneyIncome += income.money;
	foodIncome += income.food;
	woodIncome += income.wood;
	stoneIncome += income.stone;
	population += income.population;
}

void ResourcePool::LoseIncome(const ResourceIncome& income){
	moneyIncome -= income.money;
	foodIncome -= income.food;
	woodIncome -= income.wood;
	stoneIncome -= income.stone;
	population -= income.population;
}

bool ResourcePool::CanAfford(const ResourceCost& cost)
{
	return money >= cost.money && stone >= cost.stone && wood >= cost.wood;
}

void ResourcePool::SellObject(const ResourceCost& cost)
{
	stone -= cost.stone*0.25;
	money -= cost.money*0.5;
	wood -= cost.wood*0.25;
}