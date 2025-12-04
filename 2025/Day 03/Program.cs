using System.Linq;

string[] banks;
using (var fileStream = File.OpenRead("input.txt"))
    using (var streamReader = new StreamReader(fileStream))
    {
        banks = streamReader.ReadToEnd().Split("\r\n");
    }

long LargestJoltage(string bank, int numBatteries)
{
    var digits = bank.Select(t=>int.Parse(t.ToString())).ToArray();

    long joltage = 0;
    int minIndex = 0;
    for (int i = 0; i < numBatteries; i++)
    {
        int remainingBatteries = numBatteries - i;
        int maxValue = -1;
        for (int j = minIndex; j < bank.Length - remainingBatteries + 1; j++)
        {
            if (digits[j] > maxValue)
            {
                maxValue = digits[j];
                minIndex = j+1;
            }
        }
        joltage = joltage * 10 + maxValue;
    }
    return joltage;
}

// foreach(var bank in banks)
//     Console.WriteLine($"Joltage for bank {bank}={LargestJoltage(bank,2)}");

int[] testCases = {2, 12};
foreach(int test in testCases)
    Console.WriteLine($"Sum of largest Joltages for {test} batteries = {banks.Select(bank => LargestJoltage(bank, test)).Sum()}");
