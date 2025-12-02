bool IsValidPartOne(long num)
{
    var numStr = num.ToString();
    if (numStr.Length % 2 == 1)
    {
        return true;
    }
    var digits = numStr.Select(t=>int.Parse(t.ToString())).ToArray();

    var half = digits.Length / 2;
    for (int i = 0; i < half; i++)
    {
        if (digits[i] != digits[half+i])
            return true;
    }
    Console.WriteLine($"Num {num} is invalid");
    return false;
}

bool IsValidPartTwo(long num)
{
    var numStr = num.ToString();
    var digits = numStr.Select(t=>int.Parse(t.ToString())).ToArray();

    for (int seqLen = 1; seqLen <= digits.Length / 2; seqLen++)
    {
        if (digits.Length % seqLen != 0)
            continue;
        bool isValid = false;
        for (int i = 0; i < seqLen; i++)
        {
            var target = digits[i];
            for (int j = i+seqLen; j < digits.Length; j += seqLen)
            {
                if (digits[j] != target)
                {
                    isValid = true;
                    break;
                }
            }
        }
        if (!isValid)
        {
            Console.WriteLine($"Num {num} is invalid");
            return false;
        }
    }

    return true;
}

string[] codes;
using (var fileStream = File.OpenRead("input.txt"))
    using (var streamReader = new StreamReader(fileStream))
    {
        string line = streamReader.ReadLine();
        codes = line.Split(",");
    }

long sum = 0;
foreach (var code in codes)
{
    var l_h = code.Split("-");
    long lower = long.Parse(l_h[0]);
    long higher = long.Parse(l_h[1]);
    Console.WriteLine($"Code: {lower}-{higher}");

    for (var i = lower; i <= higher; i++)
    {
        //if (!IsValidPartOne(i))
        if (!IsValidPartTwo(i))
            sum += i;
    }
}

Console.WriteLine($"Sum of all invalid IDs is: {sum}");