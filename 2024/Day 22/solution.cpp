#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <map>

using namespace std;

#define INT uint64_t

inline INT mix(INT a, INT b)
{
    return a ^ b;
}

inline INT prune(INT in)
{
    return in % 16777216;
}

inline INT next(INT secret)
{
    secret = prune(mix(secret, secret << 6));
    secret = prune(mix(secret, secret >> 5));
    secret = prune(mix(secret, secret << 11));
    return secret;
}

inline int price(INT secret)
{
    return secret % 10;
}

void print_tuple(tuple<int,int,int,int> x)
{
    cout   << std::get<0>(x) << ","
           << std::get<1>(x) << ","
           << std::get<2>(x) << ","
           << std::get<3>(x);
}

int main()
{
    ifstream fin("input.txt");
    vector<vector<int>> prices;
    INT secret_number, sum = 0;
    while (fin >> secret_number)
    {
        //cout << secret_number << ": ";
        vector<int> prices_for_this_number{price(secret_number)};
        for (int i = 0; i < 2000; i++)
        {
            secret_number = next(secret_number);
            prices_for_this_number.push_back(price(secret_number));
        }
        prices.push_back(std::move(prices_for_this_number));
        sum += secret_number;
        //cout << secret_number << "\n";
    }
    cout << "Part 1: " << sum << "\n";
    
    map<tuple<int,int,int,int>, int> bananas;
    for (int i = 0; i < prices.size(); i++)
    {
        map<tuple<int,int,int,int>, int> bananas_for_this_buyer;
        for (int j = 4; j < prices[i].size(); j++)
        {
            int a = prices[i][j-3] - prices[i][j-4];
            int b = prices[i][j-2] - prices[i][j-3];
            int c = prices[i][j-1] - prices[i][j-2];
            int d = prices[i][j  ] - prices[i][j-1];
            auto key = make_tuple(a,b,c,d);
            if (!bananas_for_this_buyer.contains(key))
                bananas_for_this_buyer[key] = prices[i][j];
        }
        for (auto& [key,val] : bananas_for_this_buyer)
        {
            if (bananas.contains(key))
                bananas[key] += val;
            else
                bananas[key] = val;
        }
    }

    int max_bs = -1;
    tuple<int,int,int,int> best_seq;
    for (auto& [key,val] : bananas)
    {
        if (val > max_bs)
        {
            max_bs = val;
            best_seq = key;
        }
    }
    cout << "Part 2: " << max_bs << " with sequence " ;
    print_tuple(best_seq);
}