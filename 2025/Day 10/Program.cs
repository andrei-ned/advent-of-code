

using (var fileStream = File.OpenRead("input.txt"))
using (var streamReader = new StreamReader(fileStream))
{
    while (!streamReader.EndOfStream)
    {
        var machine = new Machine(streamReader.ReadLine());
    }
}

class Machine
{
    public Machine(string line)
    {
        var lineSplit = line.Split(" ");
        lights = lineSplit[0].Substring(1, lineSplit[0].Length - 2);
        Console.WriteLine(lights);
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

    int ComputePartOne()
    {
        return 0;
    }

    public string lights;
    public List<List<int>> buttons;
    public List<int> joltage;
}