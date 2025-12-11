using (var fileStream = File.OpenRead("input.txt"))
using (var streamReader = new StreamReader(fileStream))
{
    long sum = 0;
    while (!streamReader.EndOfStream)
    {
        var machine = new Machine(streamReader.ReadLine());
        sum += machine.ComputePartOne();
    }
    Console.WriteLine($"Fewest total button presses = {sum}");
}

class Machine
{
    public Machine(string line)
    {
        var lineSplit = line.Split(" ");
        lights = lineSplit[0].Substring(1, lineSplit[0].Length - 2);
        //Console.WriteLine(lights);
        buttons = new List<List<int>>();
        for (int i = 1; i < lineSplit.Length - 1; i++)
        {
            string numsStr = lineSplit[i].Substring(1, lineSplit[i].Length - 2);
            buttons.Add(numsStr.Split(",").Select(x => int.Parse(x)).ToList());
        }
        int lastIdx = lineSplit.Length - 1;
        var joltageStr = lineSplit[lastIdx].Substring(1, lineSplit[lastIdx].Length - 2);
        joltage = joltageStr.Split(",").Select(x => int.Parse(x)).ToList();
    }

    public int ComputePartOne()
    {
        int targetMask = 0;
        for (int i = 0; i < lights.Length; i++)
        {
            if (lights[i] == '#')
            {
                targetMask |= 1 << i;
            }
        }
        for (int pressCount = 1; pressCount < buttons.Count; pressCount++)
        {
            foreach(int btnCombo in ChooseBtns(pressCount))
            {
                int mask = 0;
                Console.Write($"btn combo {btnCombo}: press btns = ");
                for (int i = 0; i < buttons.Count; i++) // press all buttons in combo
                {
                    if ((btnCombo & (1<<i)) != 0)
                    {
                        Console.Write($"press {i}, lights = ");
                        foreach (int light in buttons[i])
                        {
                            Console.Write($"{light}, ");
                            mask ^= 1 << light;
                        }
                    }
                }
                Console.Write($"mask = {mask}");
                //Console.Write($"{btnCombo},");
                Console.WriteLine();
                if (mask == targetMask)
                {
                    Console.WriteLine($"FOUND BTN COMBO WITH {pressCount} PRESSES");
                    return pressCount;
                }
            }
            Console.WriteLine();
        }
        return -1;
    }

    IEnumerable<int> ChooseBtns(int btnCount)
    {
        return ChooseBtns(btnCount, 0, 0, 0);
    }

    IEnumerable<int> ChooseBtns(int btnCount, int start, int maskSoFar, int countSoFar)
    {
        if (countSoFar == btnCount)
        {
            yield return maskSoFar;
            yield break;
        }

        for (int i = start; i < buttons.Count - btnCount + 1 + countSoFar; i++)
        {
            maskSoFar |= 1 << i;
            foreach (var combo in ChooseBtns(btnCount, i+1, maskSoFar, countSoFar+1))
            {
                yield return combo;
            }
            maskSoFar &= ~(1 << i);
        }
    }

    public string lights;
    public List<List<int>> buttons;
    public List<int> joltage;
}