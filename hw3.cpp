#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>
const int GENERATIONS = 300;
struct City
{
	std::string name;
	double x, y;
};
struct Map
{
	std::vector<City> cities;
	double distance(int posCity1, int posCity2) const
	{
		double dx = cities[posCity1].x - cities[posCity2].x, dy = cities[posCity1].y - cities[posCity2].y;
		return std::sqrt(dx * dx + dy * dy);
	}
};
struct Route
{
	Map* map;
	std::vector<int> path;
	double length;
	void calculateLength()
	{
		length = 0;
		for(int i = 0; i < (int)path.size() - 1; i++)
		{
			length += map->distance(path[i], path[i + 1]);
		}
	}
	bool operator<(const Route& other) const
	{
		return length < other.length;
	}
};

class Solution
{
	const int POPULATION_SIZE = 500;
	int n;
	Map* map;
	std::vector<Route> routes;
	double crossOverProbability, mutationInsertProbability, mutationSwapProbability;

	Route createRandomRoute() const
	{
		std::vector<int> path;
		for(int i = 0; i < n; i++)
		{
			path.push_back(i);
		}
		std::random_shuffle(path.begin(), path.end());
		Route route{map, path, 0};
		route.calculateLength();
		return route;
	}
	Route createMSTRoute(int root) const
	{
		std::vector<int> path;
		std::vector<double> dist(n, 1e9);
		int cur = root;
		while(true)
		{
			path.push_back(cur);
			int closest = -1;
			for(int i = 0; i < n; i++)
			{
				dist[i] = std::min(dist[i], map->distance(i, cur));
				if(dist[i] > 0 && (closest == -1 || dist[i] < dist[closest]))
				{
					closest = i;
				}
			}
			if(closest == -1)
			{
				break;
			}
			cur = closest;
		}
		double length = 0;
		for(int i = 0; i < n - 1; i++)
		{
			length += map->distance(path[i], path[i + 1]);
		}
		return Route{map, path, length};
	}
	Route createNearestCityRoute(int start) const
	{
		std::vector<int> vis(n, false);
		std::vector<int> path;
		double length = 0;
		int cur = start;
		while(true)
		{
			path.push_back(cur);
			vis[cur] = true;
			double mini = 1e9;
			int pos = -1;
			for(int i = 0; i < n; i++)
			{
				if(!vis[i])
				{
					double dist = map->distance(i, cur);
					if(dist < mini)
					{
						mini = dist;
						pos = i;
					}
				}
			}
			if(pos == -1)
			{
				break;
			}
			length += mini;
			cur = pos;
		}
		return Route{map, path, length};
	}

	Route crossOver(const Route& route1, const Route& route2, int i, int j)
	{
		std::vector<int> vis(n, false);
		std::vector<int> path(n);
		for(int k = i; k <= j; k++)
		{
			path[k] = route1.path[k];
			vis[path[k]] = true;
		}
		int pos = (j + 1) % n;
		for(int k = j + 1; k < n; k++)
		{
			if(!vis[route2.path[k]])
			{
				path[pos] = route2.path[k];
				vis[path[pos]] = true;
				pos++;
				pos %= n;
			}
		}
		for(int k = 0; k <= j; k++)
		{
			if(!vis[route2.path[k]])
			{
				path[pos] = route2.path[k];
				vis[path[pos]] = true;
				pos++;
				pos %= n;
			}
		}
		Route route{map, path, 0};
		route.calculateLength();
		return route;
	}
	void mutateSwap(Route& route, int i, int j)
	{
		//std::swap(route.path[i], route.path[j]);
		//route.calculateLength();
		if(i > 0)
		{
			route.length -= map->distance(route.path[i - 1], route.path[i]);
		}
		if(i < n - 1)
		{
			route.length -= map->distance(route.path[i], route.path[i + 1]);
		}
		if(j > 0)
		{
			route.length -= map->distance(route.path[j - 1], route.path[j]);
		}
		if(j < n - 1)
		{
			route.length -= map->distance(route.path[j], route.path[j + 1]);
		}

		std::swap(route.path[i], route.path[j]);

		if(i > 0)
		{
			route.length += map->distance(route.path[i - 1], route.path[i]);
		}
		if(i < n - 1)
		{
			route.length += map->distance(route.path[i], route.path[i + 1]);
		}
		if(j > 0)
		{
			route.length += map->distance(route.path[j - 1], route.path[j]);
		}
		if(j < n - 1)
		{
			route.length += map->distance(route.path[j], route.path[j + 1]);
		}

	}
	void mutateInsert(Route& route, int i, int j)
	{
		if(i == j)
		{
			return;
		}
		if(i < j)
		{
			for(int k = i; k < j; k++)
			{
				std::swap(route.path[k], route.path[k + 1]);
			}
		}
		else
		{
			for(int k = i + 1; k <= j; k++)
			{
				std::swap(route.path[k], route.path[k - 1]);
			}
		}
		route.calculateLength();
	}
public:
	Solution(Map* map, double crossOverProbability, double mutationInsertProbability, double mutationSwapProbability)
	{
		this->map = map;
		n = map->cities.size();

		for(int i = 0; i < n; i++)
		{
			//routes.push_back(createMSTRoute(i));
		}
		for(int i = 0; i < n; i++)
		{
			//routes.push_back(createNearestCityRoute(i));
		}

		while((int)routes.size() < POPULATION_SIZE)
		{
			routes.push_back(createRandomRoute());
		}
		std::sort(routes.begin(), routes.end());
		this->crossOverProbability = crossOverProbability;
		this->mutationInsertProbability = mutationInsertProbability;
		this->mutationSwapProbability = mutationSwapProbability;
	}

	void nextGeneration()
	{
		double mini = routes[0].length;
		for(int i = 1; i < POPULATION_SIZE; i++)
		{
			mini = std::min(mini, routes[i].length);
		}
		mini /= 10;
		std::vector<double> freq;
		for(Route& r : routes)
		{
			freq.push_back(r.length / mini);
		}
		std::random_device rd;
		std::mt19937 gen(rd());
		std::discrete_distribution<> d(freq.begin(), freq.end());

		for(int k = 0; k < POPULATION_SIZE; k++)
		{
			int i = d(gen);
			Route r = routes[i];
			int percent = std::rand() % 100;
			if(percent < crossOverProbability * 100)
			{
				int j = d(gen);
				while(j == i)
				{
					j = d(gen);
				}
				int pos1 = std::rand() % n, pos2 = std::rand() % n;
				r = crossOver(routes[i], routes[j], pos1, pos2);
			}
			percent = std::rand() % 100;
			if(percent < mutationInsertProbability * 100)
			{
				int pos1 = std::rand() % n, pos2 = std::rand() % n;
				mutateInsert(r, pos1, pos2);
			}
			percent = std::rand() % 100;
			if(percent < mutationSwapProbability * 100)
			{
				int pos1 = std::rand() % n, pos2 = std::rand() % n;
				mutateSwap(r, pos1, pos2);
			}
			routes.push_back(r);
			//double old = routes[i].length;
			//mutateSwap(routes[i], pos1, pos2);
			//if(old < routes[i].length)mutateSwap(routes[i], pos1, pos2);
		}

		std::sort(routes.begin(), routes.end());
		while((int)routes.size() > POPULATION_SIZE)
		{
			routes.pop_back();
		}
	}

	Route bestRoute() const
	{
		return routes[0];
	}

	double bruteForce()const
	{
		std::vector<int> path;
		for(int i = 0; i < n; i++)
		{
			path.push_back(i);
		}
		double mini = 1e9;
		do
		{
			Route route{map, path, 0};
			route.calculateLength();
			mini = std::min(mini, route.length);
		}
		while(std::next_permutation(path.begin(), path.end()));
		return mini;
	}

};
void testUK()
{
	std::string names[12] =
	{
		"Aberystwyth",
		"Brighton",
		"Edinburgh",
		"Exeter",
		"Glasgow",
		"Inverness",
		"Liverpool",
		"London",
		"Newcastle",
		"Nottingham",
		"Oxford",
		"Stratford"
	};
	double coordinates[12][2] =
	{
		{0.000190032,	-0.000285946},
		{383.458,	-0.000608756},
		{-27.0206,	-282.758},
		{335.751,	-269.577},
		{69.4331,	-246.78},
		{168.521,	31.4012},
		{320.35,	-160.9},
		{179.933,	-318.031},
		{492.671,	-131.563},
		{112.198,	-110.561},
		{306.32,	-108.09},
		{217.343,	-447.089}

	};
	std::vector<City> cities;
	for(int i = 0; i < 12; i++)
	{
		City city;
		city.name = names[i];
		city.x = coordinates[i][0];
		city.y = coordinates[i][1];
		cities.push_back(city);
	}
	Map map{cities};
	Solution solution(&map, 1, 1, 1);
	for(int i = 0; i < GENERATIONS; i++)
	{
		if(i % (GENERATIONS / 10) == 0)
		{
			std::cout << solution.bestRoute().length << "\n";
		}
		solution.nextGeneration();
	}
	Route bestRoute = solution.bestRoute();
	std::cout << bestRoute.length << "\n";
	for(int city : bestRoute.path)
	{
		std::cout << names[city] << " ";
	}
	std::cout << "\n";
}
int main()
{
	std::srand(std::time(nullptr));
	int n;
	std::cin >> n;
	std::vector<City> cities;
	for(int i = 0; i < n; i++)
	{
		City city;
		city.x = std::rand() % 1000;
		city.y = std::rand() % 1000;
		//std::cin >> city.x >> city.y;
		cities.push_back(city);
	}
	Map map{cities};
	Solution solution(&map, 0, 0, 1);
	for(int i = 0; i < GENERATIONS; i++)
	{
		if(i % (GENERATIONS / 10) == 0)
		{
			std::cout << solution.bestRoute().length << "\n";
		}
		solution.nextGeneration();
	}
	std::cout << solution.bestRoute().length << "\n";

	testUK();
	return 0;
}
