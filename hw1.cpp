#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
const int MAX_SIZE = 10;
class NPuzzle
{
    static const int CNT_POSSIBLE_MOVES = 4;
    const int DX[CNT_POSSIBLE_MOVES] = {0, 1, 0, -1};
    const int DY[CNT_POSSIBLE_MOVES] = {1, 0, -1, 0};
    const char* DIRECTIONS[CNT_POSSIBLE_MOVES] = {"left", "up", "right", "down"};
    int grid[MAX_SIZE][MAX_SIZE], size, zeroX, zeroY, goalPosZero;

    bool isSolvable()
    {
        int cntInversions = 0;
        for(int i = 0; i < size * size - 1; i++)
        {
            if(grid[i / size][i % size] == 0)
            {
                continue;
            }
            for(int j = i + 1; j < size * size; j++)
            {
                if(grid[j / size][j % size] != 0 && grid[i / size][i % size] > grid[j / size][j % size])
                {
                    cntInversions++;
                }
            }
        }
        if(size % 2 == 1)
        {
            return ((cntInversions % 2) == 0);
        }
        return (((cntInversions + zeroX) % 2) == 1);
    }
    int calculateManhattanDistance(int x, int y, int val)
    {
        if(val <= goalPosZero)
        {
            val--;
        }
        return std::abs(x - val / size) + std::abs(y - val % size);
    }
    void makeMove(int move)
    {
        int newX = zeroX + DX[move], newY = zeroY + DY[move];
        std::swap(grid[zeroX][zeroY], grid[newX][newY]);
        zeroX = newX;
        zeroY = newY;
    }
    int dls(int manhattanDistance, int threshold, std::vector<int>& moves)
    {
        if(manhattanDistance == 0)
        {
            return -1;
        }
        if(manhattanDistance + (int)moves.size() > threshold)
        {
            return manhattanDistance + moves.size();
        }
        int possibleMoves[CNT_POSSIBLE_MOVES], l = 0, r = CNT_POSSIBLE_MOVES - 1;
        for(int i = 0; i < CNT_POSSIBLE_MOVES; i++)
        {
            if(moves.size() > 0 && std::abs(i - moves[moves.size() - 1]) == 2)
            {
                continue;
            }
            int newX = zeroX + DX[i], newY = zeroY + DY[i];
            if(newX >= 0 && newX < size && newY >= 0 && newY < size)
            {
                if(calculateManhattanDistance(zeroX, zeroY, grid[newX][newY]) < calculateManhattanDistance(newX, newY, grid[newX][newY]))
                {
                    possibleMoves[l] = i;
                    l++;
                }
                else
                {
                    possibleMoves[r] = i;
                    r--;
                }
            }
        }

        int mini = 1e9;
        for(int i = 0; i < CNT_POSSIBLE_MOVES; i++)
        {
            if(i >= l && i <= r)
            {
                i = r;
                continue;
            }
            makeMove(possibleMoves[i]);
            moves.push_back(possibleMoves[i]);
            int cur = dls(manhattanDistance + (i < l ? -1 : 1), threshold, moves);
            if(cur == -1)
            {
                return -1;
            }
            mini = std::min(mini, cur);
            makeMove((possibleMoves[i] + 2) % CNT_POSSIBLE_MOVES);
            moves.pop_back();
        }
        return mini;
    }
    std::vector<int> idaStar()
    {
        int initialManhattanDistance = 0;
        for(int i = 0; i < size; i++)
        {
            for(int j = 0; j < size; j++)
            {
                if(grid[i][j] != 0)
                {
                    initialManhattanDistance += calculateManhattanDistance(i, j, grid[i][j]);
                }
            }
        }
        int threshold = initialManhattanDistance;
        std::vector<int> moves;
        while(true)
        {
            int cur = dls(initialManhattanDistance, threshold, moves);
            if(cur == -1)
            {
                return moves;
            }
            threshold = cur;
        }
    }

public:
    NPuzzle(int size, int grid[MAX_SIZE][MAX_SIZE], int goalPosZero)
    {
        this->size = size;
        for(int i = 0; i < size; i++)
        {
            for(int j = 0; j < size; j++)
            {
                this->grid[i][j] = grid[i][j];
                if(grid[i][j] == 0)
                {
                    zeroX = i;
                    zeroY = j;
                }
            }
        }
        this->goalPosZero = goalPosZero;
    }
    void solve(bool printTime)
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto stop = std::chrono::high_resolution_clock::now();
        if(isSolvable())
        {
            std::vector<int> moves = idaStar();
            if(printTime)
            {
                stop = std::chrono::high_resolution_clock::now();
            }
            std::cout << moves.size() << "\n";
            for(int move : moves)
            {
                std::cout << DIRECTIONS[move] << "\n";
            }
        }
        else
        {
            stop = std::chrono::high_resolution_clock::now();
            std::cout << "-1\n";
        }
        if(printTime)
        {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            std::cout << "Time taken: " << duration.count() / 1e6 << " seconds\n";
        }
    }
};
int main()
{
    int n, goalPosZero, grid[MAX_SIZE][MAX_SIZE];
    std::cin >> n >> goalPosZero;
    if(goalPosZero == -1)
    {
        goalPosZero = n;
    }
    int size = std::sqrt(n + 1);
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            std::cin >> grid[i][j];
        }
    }
    NPuzzle puzzle(size, grid, goalPosZero);
    puzzle.solve(false);
    return 0;
}

