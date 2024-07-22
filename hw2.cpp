#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
class Board
{
    static const int MAX_MOVES = 50;
    int n, totalConflicts;
    std::vector<int> row, queensOnRow, queensOnDiagonal, queensOnReverseDiagonal, candidates;

    int colMaxConflicts()
    {
        int maxi = -1, size = 0;
        for(int i = 0; i < n; i++)
        {
            int cur = queensOnRow[row[i]] + queensOnDiagonal[(row[i] - i) + n - 1] + queensOnReverseDiagonal[row[i] + i] - 3;
            if(cur > maxi)
            {
                size = 0;
                maxi = cur;
            }
            if(cur == maxi)
            {
                candidates[size] = i;
                size++;
            }
        }
        return candidates[std::rand() % size];
    }
    int rowMinConflicts(int col)
    {
        int mini = 1e9, size = 0;
        for(int i = 0; i < n; i++)
        {
            int cur = queensOnRow[i] + queensOnDiagonal[(i - col) + n - 1] + queensOnReverseDiagonal[i + col];
            if(i == row[col])
            {
                cur -= 3;
            }
            if(cur < mini)
            {
                size = 0;
                mini = cur;
            }
            if(cur == mini)
            {
                candidates[size] = i;
                size++;
            }
        }
        return candidates[std::rand() % size];
    }
    int init1()
    {
        for(int i = 0; i < n; i++)
        {
            row[i] = -1;
            queensOnRow[i] = 0;
        }
        for(int i = 0; i < 2 * n - 1; i++)
        {
            queensOnDiagonal[i] = 0;
            queensOnReverseDiagonal[i] = 0;
        }
        totalConflicts = 0;

        int rr = 1, s = 0;
        for(int i = 0; i < n; i++)
        {
            int r = rr;
            rr += 2;
            if(rr >= n)
            {
                rr = s;
                s++;
            }
            row[i] = r;
            queensOnRow[r]++;
            queensOnDiagonal[(r - i) + n - 1]++;
            queensOnReverseDiagonal[r + i]++;
            totalConflicts += queensOnRow[r] + queensOnDiagonal[(r - i) + n - 1] + queensOnReverseDiagonal[r + i] - 3;
        }
        return totalConflicts;
    }
    int init2()
    {
        for(int i = 0; i < n; i++)
        {
            row[i] = -1;
            queensOnRow[i] = 0;
        }
        for(int i = 0; i < 2 * n - 1; i++)
        {
            queensOnDiagonal[i] = 0;
            queensOnReverseDiagonal[i] = 0;
        }
        totalConflicts = 0;

        int rr = 0, s = 1;
        for(int i = 0; i < n; i++)
        {
            int r = rr;
            rr += 2;
            if(rr >= n - 1)
            {
                rr = s;
                s++;
            }
            row[i] = r;
            queensOnRow[r]++;
            queensOnDiagonal[(r - i) + n - 1]++;
            queensOnReverseDiagonal[r + i]++;
            totalConflicts += queensOnRow[r] + queensOnDiagonal[(r - i) + n - 1] + queensOnReverseDiagonal[r + i] - 3;
        }
        return totalConflicts;
    }
    void restart()
    {
        if(n == 1 || init2() < init1())
        {
            init2();
        }
    }
    void print()const
    {
        std::cout << "[";
        for(int i = 0; i < n; i++)
        {
            std::cout << row[i];
            if(i < n - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << "]";
    }
public:
    Board(int n): n(n), row(n, -1), queensOnRow(n, 0), queensOnDiagonal(2 * n - 1, 0), queensOnReverseDiagonal(2 * n - 1, 0), candidates(n)
    {
    }
    void solve()
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto stop = std::chrono::high_resolution_clock::now();
        if(n == 2 || n == 3)
        {
            std::cout << -1;
            return ;
        }
        while(true)
        {
            restart();
            for(int i = 0; i < MAX_MOVES; i++)
            {
                if(totalConflicts == 0)
                {

                    if(n <= 100)
                    {
                        print();
                    }
                    else
                    {
                        stop = std::chrono::high_resolution_clock::now();
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
                        std::cout << std::fixed;
                        std::cout << std::setprecision(2);
                        std::cout << duration.count() / 1e6 << "\n";
                    }
                    return ;
                }
                int col = colMaxConflicts();
                int newRow = rowMinConflicts(col);
                int oldRow = row[col];
                totalConflicts -= queensOnRow[oldRow] + queensOnDiagonal[(oldRow - col) + n - 1] + queensOnReverseDiagonal[oldRow + col] - 3;
                row[col] = newRow;
                queensOnRow[oldRow]--;
                queensOnRow[newRow]++;
                queensOnDiagonal[(oldRow - col) + n - 1]--;
                queensOnDiagonal[(newRow - col) + n - 1]++;
                queensOnReverseDiagonal[oldRow + col]--;
                queensOnReverseDiagonal[newRow + col]++;
                totalConflicts += queensOnRow[newRow] + queensOnDiagonal[(newRow - col) + n - 1] + queensOnReverseDiagonal[newRow + col] - 3;
            }
        }

    }
};
int main()
{
    std::srand(std::time(nullptr));
    int n;
    std::cin >> n;
    Board b(n);
    b.solve();
    return 0;
}

