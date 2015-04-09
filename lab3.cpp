#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdio>
#include <algorithm>
#include <map>

using namespace std;

struct card {

        string name;
        int marketPrice;
        float salePrice;
        float profit;
	float ratio;
};
struct node{
	int profit;
	int weight;
	int upperBound;
};
void greedyOne(int moneyToSpend, vector<card> * cards, std::ofstream& of);
void greedyTwo(int moneyToSpend, vector<card> * cards, std::ofstream& of);
void backTracking(int moneyToSpend, vector<card> * cards, std::ofstream& of);
void knapSack(int i, int profit, int weight, int moneyToSpend);
bool promising(int i, node * v, int moneyToSpend);
long long KWF(int i, int weight, int profit, int moneyToSpend);
node startingNode;
//Sorting struct for high-low ratios
struct CompRatio
{
	bool operator()(const card& c1, const card& c2)
	{
		return (c1.ratio > c2.ratio);
	}
};
//Sorting struct for low-high weights(sale prices)
struct WeightComp
{
	bool operator()(const card& c1, const card& c2)
	{
		if(c1.salePrice < c2.salePrice)
			return true;
		else if(c1.salePrice > c2.salePrice)
			return false;
		else if(c1.salePrice == c2.salePrice)
		{
			return(c1.profit > c2.profit);
		}
	}

};
/*
struct NameComp
{
	bool operator()(const card& c1, const card& c2)
	{
		return(c1.name < c2.name);
	}
};
*/
void printMarket(vector<card> * list);

//Global variables for backtracking
vector<int> include, bestSet;
int money[256];
int maxProfit;
long long numConsidered;
long long numNotConsidered;
clock_t start;
double diff;
vector<card> globalCards;
int main(int argc, char ** argv)
{
	diff = 0;
	map<string, int> theMap;
	int i = 0, j = 0, algFlag = 0;
	int value = 0;
	maxProfit = 0;
	numConsidered = 0;
	numNotConsidered = 0;
        string marketPrices, gPrices, outFile, temp, temp2, word, word2;
        //Parse the command line arguments
        if(argc != 8)
        {
                cerr << "Usage:" << endl;
                cerr << "./lab3 -m <market-price-file> -p <price-list-file -o output-file-name [0|1|2]\n>" <<endl;
                exit(0);
        }
	//Ensure correct order
        if((string)argv[1] == "-m")
        {
                marketPrices = (string)argv[2];
                gPrices = (string)argv[4];
		outFile = (string)argv[6];
		algFlag = atoi(argv[7]);
        }

	//start process of inputting prices
        stringstream ss, dd;
        ifstream in, in2;
        vector< vector<card> > gCards;
        for(i = 0; i < 256; i++)
        money[i] = 0;
        //Read the price list file first
        i = -1;
        in.open(gPrices.c_str(), ifstream::in);
	in2.open(marketPrices.c_str(), ifstream::in);
 	while(in.good() && getline(in, temp))
        {
                ss << temp;
                while(ss >> word)
                {
                        if(isdigit(word.c_str()[0]))
                        {
                                i++;

                                vector<card> vec;
                                vec.reserve(atoi(word.c_str()));
				//w.reserve(atoi(word.c_str()));
				//p.reserve(atoi(word.c_str()));
				include.resize(atoi(word.c_str()) + 1, 0);
				bestSet.resize(atoi(word.c_str()) + 1, 0);
                                ss >> word;
                                money[i] = atoi(word.c_str());
                                gCards.push_back(vec);
				card a;
				a.name = "";
				a.salePrice = 0;
				a.marketPrice = 0;
				a.profit = 0;
				a.ratio = 0;
					
				gCards[i].push_back(a);

                        }
                        else
                        {
                                card a;
                                a.name = word;
                                ss >> word;
                                a.salePrice = atoi(word.c_str());
                                a.marketPrice = 0;
                                gCards.at(i).push_back(a);
                        	//theMap.insert(pair<string, card>(a.name, a));
			}
                }
                ss.clear();
        }
 
	i = 0;
        in.close();
        //Read the market price list file second
        in.open(marketPrices.c_str(), ifstream::in);
        //vector<card> mCards;
	getline(in, temp);
	while(in.good() && getline(in, temp))
	{
		ss << temp;
		while(ss >> word)
		{
			temp = word;
			ss >> word;
			value = atoi(word.c_str());
			theMap.insert(pair<string, int>(temp, value));


		}
		ss.clear();

	}

	map<string, int>::iterator finder;
	//Match the market prices to each card now
	//Also set profit and ratio values
	for(i = 0; i < gCards.size(); i++)
	{
		for(j = 1; j < gCards.at(i).size(); j++)
		{	
			finder = theMap.find(gCards[i][j].name);
			if(finder != theMap.end())
			{		
				gCards[i][j].marketPrice = finder->second;
				gCards[i][j].profit = finder->second - gCards[i][j].salePrice;
				gCards[i][j].ratio = (gCards[i][j].profit) / (gCards[i][j].salePrice);
			}
			else
				cerr << "Could not find card with name: " << gCards[i][j].name << endl;
		}
	}
	ofstream out;
	out.open(argv[6], ofstream::out);	
	if(algFlag == 0)
	{	
		for(i = 0; i < gCards.size(); i++)
		{
			greedyOne(money[i], &gCards[i], out);
		}
	}
	else if(algFlag == 1)
	{
		for(i = 0; i < gCards.size(); i++)
		{
			greedyTwo(money[i], &gCards[i], out);
		}

	}
	else if(algFlag == 2)
	{
		for(i = 0; i < gCards.size(); i++)
		{
			for(int k = 0; k < include.size(); k++)
			{
				include[k] = 0;
				bestSet[k] = 0;
			}
			maxProfit = 0;
			globalCards.assign(gCards[i].begin(), gCards[i].end());
			backTracking(money[i], &gCards[i], out);
		}

	}




	return 0;
}
void backTracking(int moneyToSpend, vector<card> * cards, std::ofstream& of)
{
	start = clock();
	//Initialize variables that will be needed for backTracking
	vector<card> bestSet1, bestSet2;
	int n = cards->size() - 1, j = 0;
	int totalMoney = moneyToSpend;
	map<string, int> includes;
	//Calculate the money earned through greedy one algorithm
	vector<card>::iterator rover;
	int greedyOneProfit = 0, cardCount = 0;
	const int moneyAvailable = moneyToSpend;
	//First sort the vector in order of High-low ratios
	rover = cards->begin();
	rover++;
	sort(rover, cards->end(), CompRatio());
	for(rover = cards->begin(); rover != cards->end(); rover++)
	{
		if(moneyToSpend >= rover->salePrice)
		{
			greedyOneProfit += rover->profit;
			moneyToSpend -= rover->salePrice;
			cardCount++;
			bestSet1.push_back(*rover);
			includes.insert(pair<string, int>(rover->name, 1));	
		}
	}
	//cout << "Greedy two: " << cards->size() << " " << greedyOneProfit << " " << cardCount << endl;
	//Find the highest profit of any single card
	int pMax = -10000;
	for(rover = cards->begin(); rover != cards->end(); rover++)
	{
		if(rover->profit > pMax && rover->salePrice <= totalMoney)
		{	pMax = rover->profit;
			//bestSet2.pop_back();
			bestSet2.push_back(*rover);
		}

	}	
	rover = globalCards.begin();
	rover++;
	sort(rover, globalCards.end(), CompRatio());
	//vector<card> bestSet;
	if(greedyOneProfit > pMax)
	{
		//Need to set bestSet to have 1's in all the spots greedy one decided were included.
		sort(bestSet1.begin(), bestSet1.end(), WeightComp());
		//Set bestSet based on names of cards in map
		for(int i = 1; i <= n; i++)
		{
			if(includes.find(globalCards.at(i).name) != includes.end())	
			{
				bestSet[i] = 1;
			}
		}

		pMax = greedyOneProfit;
	}
	else
	{
		string lookingFor = bestSet2.back().name;
		for(int i = 1; i <= n; i++)
		{
			if(!lookingFor.compare(globalCards.at(i).name))
			{
				bestSet[i] = 1;
				break;
			}
		}
	}
	knapSack(0, 0, 0, totalMoney);
	cardCount = 0;
	for(int i = 0; i < bestSet.size(); i++)
	{
		if(bestSet[i])
			cardCount++;
	}	
	diff = (clock() - start) / (double)CLOCKS_PER_SEC;
	of << "Backtracking: " << globalCards.size()-1 << " " << maxProfit << " " << cardCount << " " << diff <<  endl;
	of << numConsidered << " subsets considered : " << numNotConsidered << " avoided via backtracking" << endl;
}

void knapSack(int i, int profit, int weight, int moneyToSpend)
{
	//cout << "In knapsack with i = " << i << endl;
	//Obtained a new best solution; save to appropriate arrays.
	if(i == globalCards.size() -1)
		numConsidered++;
	if(weight <= moneyToSpend && profit > maxProfit)
	{
		maxProfit = profit;
		numConsidered = i;
		bestSet.assign(include.begin(), include.end());	
	}
	node passThis;
	passThis.profit = profit;
	passThis.weight = weight;
	//If node is promising, call procedure on children
	if(promising(i+1, &passThis, moneyToSpend))
	{
		//Include the next card
		include[i + 1] = 1;
		knapSack(i+1, profit + globalCards[i+1].profit, weight + globalCards[i+1].salePrice, moneyToSpend);
		//Dont include the next card
		include[i+1] = 0;
		knapSack(i+1, profit, weight, moneyToSpend);
	}
	else
	{
		//At this point, numCards-i levels are being saved.
		int levelsSaved = globalCards.size() - 1 -i;
		numNotConsidered += pow(2, levelsSaved) - 1;


	}
	


}

bool promising(int i, node * v, int moneyToSpend)
{
	//If weight is already over, false
	if(v->weight > moneyToSpend)
	{
		return false;
	}
	//Otherwise compute upper bound
	long long bound = KWF(i, v->weight, v->profit, moneyToSpend);

	return(bound > maxProfit);

}
long long KWF(int i, int weight, int profit, int moneyToSpend)
{
	long long bound = profit;
	float x[globalCards.size()];
	//loop through cards
	while(weight < moneyToSpend && i < globalCards.size())
	{
		if(weight + globalCards[i].salePrice <= moneyToSpend)
		{
//	cout << "Adding card with weight " << globalCards[i].salePrice << " and profit " << globalCards[i].profit << endl;
			x[i] = 1;	
			weight += globalCards[i].salePrice;
			bound += globalCards[i].profit;
		}
		else
		{
			x[i] = (moneyToSpend - weight)/(globalCards[i].salePrice);
			weight = moneyToSpend;
			bound += (globalCards[i].profit * x[i]);
		}

		i++;
	}
	return bound;

}

//Still Needs time code inserted
void greedyOne(int moneyToSpend, vector<card> * cards, std::ofstream& of)
{
	start = clock();	
	for(int i = 0; i < bestSet.size(); i++)
		bestSet[i] = 0;
	vector<card>::iterator rover;
	int totalProfit = 0, cardCount = 0, i = 1;
	//Firstsort the vector in order of High-low ratios
	rover = cards->begin();
	rover++;
	
	sort(rover, cards->end(), CompRatio());
	for(rover = rover; rover != cards->end(); rover++)
	{
	/*	cout << "Card Name: " << rover->name << endl;
		cout << "Card Profit: " << rover->profit << endl;
		cout << "Card price: " << rover->salePrice << endl;
		cout << "Card MP: " << rover->marketPrice << endl;
	*/	if(moneyToSpend >= rover->salePrice)
		{
			totalProfit += rover->profit;
			moneyToSpend -= rover->salePrice;
			cardCount++;
			bestSet[i] = 1;
		}	
		else
			bestSet[i] = 0;

		i++;
	}
	diff = (clock() - start) / (double)CLOCKS_PER_SEC;

	of << "Greedy One: " << cards->size()-1 << " " << totalProfit << " " << cardCount << " " << diff <<  endl;
/*	for(int i = 1; i < bestSet.size(); i++)
	{
		if(bestSet[i])
			cout << cards->at(i).name << endl;
	}
*/
}

void greedyTwo(int moneyToSpend, vector<card> * cards, std::ofstream& of)
{
	start = clock();
	//Calculate the money earned through greedy one algorithm
	vector<card>::iterator rover;
	int greedyOneProfit = 0, cardCount = 0;
	const int money = moneyToSpend;
	//First sort the vector in order of High-low ratios
	sort(cards->begin(), cards->end(), CompRatio());
	rover = cards->begin();
	rover++;
	for(rover = rover; rover != cards->end(); rover++)
	{
		if(moneyToSpend > rover->salePrice)
		{
			greedyOneProfit += rover->profit;
			moneyToSpend -= rover->salePrice;
			cardCount++;
		}
	}
	//cout << "Greedy two: " << cards->size() << " " << greedyOneProfit << " " << cardCount << endl;
	//Find the highest profit of any single card
	int pMax = -10000;
	for(rover = cards->begin(); rover != cards->end(); rover++)
	{
		if(rover->profit > pMax && rover->salePrice <= money)
			pMax = rover->profit;
	}
	diff = (clock() - start) / (double)CLOCKS_PER_SEC;
	if(greedyOneProfit > pMax)
		of << "Greedy two: " << cards->size()-1 << " " << greedyOneProfit << " " << cardCount << diff << endl;
	else
		of << "Greedy two: " << cards->size()-1 << " " << pMax << " 1 " << diff << endl;
}
void printMarket(vector<card> * list)
{

        vector<card>::iterator rover;
        for(rover = list->begin(); rover != list->end(); rover++)
        {

                cout << "Card name: " << rover->name << endl;
                cout << "Sale price: " << rover->salePrice << endl;
                cout << "Market price: " << rover->marketPrice << endl;
                cout << endl;
        }




}
/*	ofstream outstream;
	outstream.open(outFile.c_str(), ofstream::out);
	for(i = 0; i < gCards[0].size(); i++)
	{
                outstream << "Card name: " << gCards[0][i].name << endl;
                outstream << "Card profit: " << gCards[0][i].profit << endl;
                outstream << "Card ratio: " << gCards[0][i].ratio << endl;
                outstream << "Sale price: " << gCards[0][i].salePrice << endl;
                outstream << "Market price: " << gCards[0][i].marketPrice << endl;
                outstream << endl;
	}
*/
