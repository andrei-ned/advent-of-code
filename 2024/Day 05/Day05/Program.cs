using System.Collections.Generic;

Dictionary<int,HashSet<int>> rules = new Dictionary<int, HashSet<int>>();

using (var fileStream = File.OpenRead("input.txt"))
    using (var streamReader = new StreamReader(fileStream))
    {
        string? line;
        while ((line = streamReader.ReadLine()) != "")
        {
            
            var numbersString = line?.Split('|');
            int numSmaller = int.Parse(numbersString[0]);
            int numBigger = int.Parse(numbersString[1]);
            if (!rules.ContainsKey(numSmaller))
                rules.Add(numSmaller, new HashSet<int>());
            rules[numSmaller].Add(numBigger);
        }

        int sumOfMidPages = 0;
        int sumPart2 = 0;
        while ((line = streamReader.ReadLine()) != null)
        {
            bool areRulesOk = true;
            var numbers = line?.Split(',')?.Select(Int32.Parse)?.ToList();
            for (int i = 0; i < numbers.Count && areRulesOk; i++)
            {
                for (int j = i + 1; j < numbers.Count && areRulesOk; j++)
                {
                    if (rules.ContainsKey(numbers[j]) && rules[numbers[j]].Contains(numbers[i]))
                        areRulesOk = false;
                }
            }
            if (areRulesOk)
            {
                sumOfMidPages += numbers[numbers.Count/2];
                //Console.WriteLine(line);
            }
            else // Part 2
            {
                var correctedList = new List<int>();
                foreach (var num in numbers)
                {
                    int targetIndex = 0;
                    for (int i = 0; i < correctedList.Count; i++)
                    {
                        if (rules.ContainsKey(correctedList[i]) && rules[correctedList[i]].Contains(num))
                        {
                            targetIndex = i+1;
                        }
                    }
                    correctedList.Insert(targetIndex, num);
                }
                sumPart2 += correctedList[correctedList.Count/2];
            }
        }
        Console.WriteLine($"Part 1: {sumOfMidPages}");
        Console.WriteLine($"Part 2: {sumPart2}");
    }