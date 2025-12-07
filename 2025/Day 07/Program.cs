long noOfSplits = 0;

using (var fileStream = File.OpenRead("input.txt"))
using (var streamReader = new StreamReader(fileStream))
{
    string line = streamReader.ReadLine();
    HashSet<int> beams = [line.IndexOf('S')];
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
            }
        }
    }
}

Console.WriteLine($"Total beam splits: {noOfSplits}");

