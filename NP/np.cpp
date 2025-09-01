#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

class KnapSack
{
private:
    vector<vector<long long int>> dpTable;
    vector<int> weights;
    vector<int> values;
    vector<int> selected;
    vector<int> originalval;
    int n, W;
    int oritcost;

    void initializeDPTable(int m, int n)
    {
        dpTable.assign(m + 1, vector<long long int>(n + 1, -1));
    }

    int knapsack(int i, int v)
    {
        if (v < 0)
            return 0;
        if (v == 0)
            dpTable[i][v]= 0;
        if (i == 0 && v > 0)
            dpTable[i][v]= 9999;
        if (dpTable[i][v] != -1)
            return dpTable[i][v];

        int notPicked = knapsack(i - 1, v);
        int nextRecur = knapsack(i - 1, v - values[i - 1]);
        int picked = weights[i - 1] + nextRecur;

        if (notPicked < picked)
        {
            dpTable[i][v] = notPicked;
            return notPicked;
        }
        else
        {
            dpTable[i][v] = picked;
            return picked;
        }
    }




    int getselected(int s,int n,int W)
    {
        int usedW = knapsack(n, s);
        while(usedW>W)
        {
            usedW=knapsack(n,--s);
        }
        int j = s, findW = usedW;
        for (int i = n; i > 0; i--)
        {
            if (dpTable[i][j] != dpTable[i - 1][j])
            {
                selected.push_back(i);
                findW -= weights[i - 1];
                if(findW<=0) break;
                j = dpTable[i - 1].size() - 1 - distance(rbegin(dpTable[i - 1]), find(rbegin(dpTable[i - 1]), rend(dpTable[i - 1]), findW));
            }
        }
        return usedW;
    }

    void printOriginalInstance(int usedW)
    {
        int totalCost = 0;
        for (int item : selected) totalCost += values[item-1];
        oritcost=totalCost;

        cout << "\nOriginal Instance:\n";
        cout << "Answer: " << totalCost << endl;
        cout << "Used Weight: " << usedW << endl;
        cout << "Indices: ";
        for (int item : selected) cout << item << " ";
        cout << endl;
    }



public:
    KnapSack() {}

    void solve()
    {
        cin >> n >> W;
        weights.clear();
        values.clear();
        int Vmax = 0, Vmin = 99999;
        int originalVmax, originalVmin;
        int tempV, tempW, maxValSum = 0;
        for (int i = 0; i < n; i++)
        {
            cin >> tempV >> tempW;
            maxValSum += tempV;
            values.push_back(tempV);
            originalval.push_back(tempV);
            weights.push_back(tempW);
            if (tempV > Vmax)
            {
                originalVmax=Vmax = tempV;
            }
            if (tempV < Vmin)
            {
                originalVmin=Vmin = tempV;
            }
        }
        for(auto i : values) cout<<i<<endl;
        initializeDPTable(n, maxValSum);

        int s=maxValSum;

        int usedW=getselected(s,n,W);
        printOriginalInstance(usedW);


        double epsilons[] = {0.5, 0.2, 0.1, 0.05};
        for (double epsilon : epsilons)
        {
            int realCost = 0;
            maxValSum = 0;
            int totalCost = 0;
            long double theta = epsilon * Vmax / (2 * n);
            values.clear();

            for (int value : originalval)
            {
                int tempV = (int)ceil(1.0 * value / theta)+(value==Vmax);
                maxValSum += tempV;
                values.push_back(tempV);
            }

            initializeDPTable(n, maxValSum);
            s=maxValSum;

            usedW = knapsack(n, s);

            while(usedW>W)
            {
                usedW=knapsack(n,--s);
            }
            int j = s, findW = usedW;
            selected.clear();

            for (int i = n; i > 0; i--)
            {
                if (dpTable[i][j] != dpTable[i - 1][j])
                {
                    selected.push_back(i);
                    findW -= weights[i - 1];
                    if(findW<=0) break;
                    j = dpTable[i - 1].size() - 1 - distance(rbegin(dpTable[i - 1]), find(rbegin(dpTable[i - 1]), rend(dpTable[i - 1]), findW));
                }
            }

            for (int item : selected)
            {
                realCost += originalval[item-1];
            }

            cout << "\n\nRounded Instance with Eps: " << epsilon << endl;
            cout << "Theta: " << setprecision(16) << theta << endl;
            cout << "Answer of reduced instance: " << totalCost << endl;
            cout << "Answer of reduced instance multiplied by theta: " << setprecision(16) << totalCost * theta << endl;
            cout << "Indices: ";
            for (int item : selected) cout << item << " ";
            cout << endl;
            cout << "Answer of original instance (rounded up): " << realCost << endl;
            cout << "Used Weight: " << usedW << endl;
            cout << "Ratio: " << (long double)oritcost  / (long double)realCost << endl;
        }
    }
};

int main()
{
    KnapSack solver;
    solver.solve();
    return 0;
}

