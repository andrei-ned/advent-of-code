long noOfSplits = 0;

using (var fileStream = File.OpenRead("input.txt"))
using (var streamReader = new StreamReader(fileStream))
{
    string line = streamReader.ReadLine();
    int start = line.IndexOf('S');
    HashSet<int> beams = [start];
    long[] timelines = new long[line.Length];
    timelines[start] = 1;
    while (!streamReader.EndOfStream)
    {
        line = streamReader.ReadLine();
        //Console.WriteLine($"{line}\nbeams: {string.Join(",",beams)}");
        for (int i = 0; i < line.Length; i++)
        {
            if (line[i] == '^')
            {
                if (beams.Contains(i))
                {
                    noOfSplits++;
                    beams.Add(i-1);
                    beams.Add(i+1);
                    beams.Remove(i);
                }

                if (timelines[i]> 0)
                {
                    timelines[i + 1] += timelines[i];
                    timelines[i - 1] += timelines[i];
                    timelines[i] = 0;
                }
            }
        }
    }

    Console.WriteLine($"No of timelines: {timelines.Sum()}");
}

Console.WriteLine($"Total beam splits: {noOfSplits}");

