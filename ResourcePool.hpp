#pragma once
struct ResourceCost{
	ResourceCost() {};
	ResourceCost(int _wood, int _stone, int _money)
	: wood(_wood), stone(_stone), money(_money){
	}
	int wood = 0;
	int stone = 0;
	int money = 0;
};

struct ResourceIncome{
	ResourceIncome(){};
	ResourceIncome(int _food, int _wood, int _stone, int _money, int _population)
	: food(_food), wood(_wood), stone(_stone), money(_money), population(_population){
	}
	int food = 0;
	int wood = 0;
	int stone = 0;
	int money = 0;
	int population = 0;
};

class ResourcePool
{
public:
	ResourcePool();
	void Spend(const ResourceCost& cost);
	void ProcessIncome();
	void GainIncome(const ResourceIncome& income);
	void LoseIncome(const ResourceIncome& income);
	bool CanAfford(const ResourceCost& cost);
	void GainMoney(int);

	int food = 20;
	int wood = 200;
	int stone = 150;
	int money = 600;
	int population = 0;

	int foodIncome = 0;
	int woodIncome = 0;
	int stoneIncome = 0;
	int moneyIncome = 0;
};