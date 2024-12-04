List<string> letters = new List<string>();

using (var fileStream = File.OpenRead("input.txt"))
    using (var streamReader = new StreamReader(fileStream))
    {
        string line;
        while ((line = streamReader.ReadLine()) != null)
        {
            letters.Add(line);
        }
    }

int countWord(string word, int xDirection, int yDirection)
{
    int count = 0;

    for (int y = 0; y < letters.Count; y++)
    {
        for (int x = 0; x < letters[y].Length; x++)
        {
            try
            {
                int X = x;
                int Y = y;
                int i = 0;
                for (; i < word.Length; i++)
                {
                    if (letters[Y][X] != word[i])
                        break;
                    Y += yDirection;
                    X += xDirection;
                }
                if (i == word.Length)
                {
                    count++;
                }
            }
            catch
            {
                continue;
            }
        }
    }
    return count;
}

int totalCount = 0;
for (int i = -1; i <= 1; i++)
{
    for (int j = -1; j <= 1; j++)
    {
        if (i == 0 && j == 0)
            continue;
        totalCount += countWord("XMAS", i, j);
    }
}
Console.WriteLine($"XMAS count: {totalCount}");


int xCount = 0;
for (int y = 1; y < letters.Count - 1; y++)
{
    for (int x = 1; x < letters[y].Length - 1; x++)
    {
        if (letters[y][x] == 'A')
        {
            if (((letters[y-1][x-1] == 'M' && letters[y+1][x+1] == 'S') || (letters[y-1][x-1] == 'S' && letters[y+1][x+1] == 'M'))
              &&((letters[y+1][x-1] == 'M' && letters[y-1][x+1] == 'S') || (letters[y+1][x-1] == 'S' && letters[y-1][x+1] == 'M')))
            {
                xCount++;
            }
        }
    }
}
Console.WriteLine($"X-MAS count: {xCount}");