char[][] grid;
using (var fileStream = File.OpenRead("input.txt"))
    using (var streamReader = new StreamReader(fileStream))
    {
        grid = streamReader.ReadToEnd().Split("\r\n").Select(row => row.ToArray()).ToArray();
    }

void PrintGrid()
{
    for (int y = 0; y < grid.Length; y++)
    {
        for (int x = 0; x < grid[y].Length; x++)
        {
            Console.Write(grid[y][x]);
        }
        Console.WriteLine();
    }
}

int MarkPaper()
{
    int sum = 0;

    for (int y = 0; y < grid.Length; y++)
    {
        for (int x = 0; x < grid[y].Length; x++)
        {
            if (grid[y][x] != '@')
                continue;
            int adjacentCount = 0;

            for (int j = -1; j <=1; j++)
                for (int i = -1; i <=1; i++)
                {
                    if (i == 0 && j == 0)
                        continue;
                    try
                    {
                        if (grid[y+j][x+i] != '.')
                            adjacentCount++;
                    }
                    catch (Exception) {}
                }

            if (adjacentCount < 4)
            {
                grid[y][x] = 'x';
                sum++;
            }
        }
    }

    return sum;
}

void RemoveMarkedPaper()
{
    for (int y = 0; y < grid.Length; y++)
    {
        for (int x = 0; x < grid[y].Length; x++)
        {
            if (grid[y][x] == 'x')
                grid[y][x] = '.';
        }
    }
}

int total = 0;
while (true)
{
    int removedCount = MarkPaper();
    total += removedCount;
    if (removedCount == 0)
    {
        Console.WriteLine($"Total rolls of paper removed = {total}");
        break;
    }
    //PrintGrid();
    RemoveMarkedPaper();
}
