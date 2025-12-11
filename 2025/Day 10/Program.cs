using (var fileStream = File.OpenRead("input.txt"))
using (var streamReader = new StreamReader(fileStream))
{
    long sum1 = 0;
    long sum2 = 0;
    while (!streamReader.EndOfStream)
    {
        string line = streamReader.ReadLine();
        var machine = new Machine(line);
        sum1 += machine.ComputePartOne();
        sum2 += machine.ComputePartTwo();
        Console.WriteLine($"Machine {line} needs btn presses: {sum1} & {sum2}");
    }
    Console.WriteLine($"Fewest total button presses for configuring lights = {sum1}");
    Console.WriteLine($"Fewest total button presses for configuring joltage = {sum2}");
}

class Machine
{
    public string lights;
    public List<List<int>> buttons;
    public int[] joltage;

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
        joltage = joltageStr.Split(",").Select(x => int.Parse(x)).ToArray();
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
                //Console.Write($"btn combo {btnCombo}: press btns = ");
                for (int i = 0; i < buttons.Count; i++) // press all buttons in combo
                {
                    if ((btnCombo & (1<<i)) != 0)
                    {
                        //Console.Write($"press {i}, lights = ");
                        foreach (int light in buttons[i])
                        {
                            //Console.Write($"{light}, ");
                            mask ^= 1 << light;
                        }
                    }
                }
                //Console.Write($"mask = {mask}");
                //Console.WriteLine();
                if (mask == targetMask)
                {
                    //Console.WriteLine($"FOUND BTN COMBO WITH {pressCount} PRESSES");
                    return pressCount;
                }
            }
            //Console.WriteLine();
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

    public int ComputePartTwo()
    {
        var currJoltage = new int[joltage.Length];
        int presses = ComputePartTwo(0, currJoltage, 0);
        Console.WriteLine();
        return presses;
    }

    // TODO: this is ok for example, but too slow for actual input
    int ComputePartTwo(int currentBtn, int[] currentJoltage, int btnPresses, int best = int.MaxValue)
    {
        if (btnPresses >= best)
        {
            return best; // pruning
        }
        if (CompareJoltage(currentJoltage) == 0)
        {
            Console.WriteLine($"Found matching joltage with {btnPresses} btn presses: ");
            return btnPresses; // this is a solution
        }
        if (CompareJoltage(currentJoltage) < 0)
        {
            //Console.Write('.');
            return best; // this exceeds joltage
        }
        if (currentBtn >= buttons.Count)
        {
            return best; // this is the last button
        }

        int ret = ComputePartTwo(currentBtn + 1, currentJoltage, btnPresses, best); // no presses on current btn
        best = Math.Min(best, ret);
        PressJoltageButton(currentBtn, currentJoltage);
        ret = ComputePartTwo(currentBtn, currentJoltage, btnPresses + 1, best); // +1 press on current btn
        best = Math.Min(best, ret);
        PressJoltageButton(currentBtn, currentJoltage, -1); // undo btn press
        return best;
    }

    void PressJoltageButton(int btnIndex, int[] targetJoltage, int value=1)
    {
        foreach(int jolt in buttons[btnIndex])
        {
            targetJoltage[jolt] += value;
        }
    }

    // -1 = other joltage exceeds target joltage
    //  0 = joltages are equal
    //  1 = other joltage is under target joltage
    int CompareJoltage(int[] otherJoltage)
    {
        bool isEqual = true;
        for (int i = 0; i < joltage.Length; i++)
        {
            int diff = joltage[i] - otherJoltage[i];
            if (diff < 0)
            {
                return -1;
            }
            if (diff != 0)
            {
                isEqual = false;
            }
        }
        return isEqual ? 0 : 1;
    }
}