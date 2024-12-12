List<string> farmStrings = new List<string>();



using (var fileStream = File.OpenRead("input.txt"))
    using (var streamReader = new StreamReader(fileStream))
    {
        string line;
        while ((line = streamReader.ReadLine()) != null)
        {
            farmStrings.Add(line);
        }
    }

Cell[][] farm = new Cell[farmStrings.Count][];
for (int y = 0; y < farmStrings.Count; y++)
{
    farm[y] = new Cell[farmStrings[y].Length];
    for (int x = 0; x < farmStrings[y].Length; x++)
    {
        farm[y][x] = new Cell(farmStrings[y][x]);
    }
}

(int,int)[] directions = new[] {(1,0),(-1,0),(0,1),(0,-1)};

// Returns (area, perimeter)
(UInt64, UInt64) MeasureRegion(int x, int y)
{
    farm[y][x].visited = true;
    char plant = farm[y][x].plant;
    UInt64 area = 1;
    UInt64 perimeter = 0;

    foreach (var direction in directions)
    {
        int newX = x + direction.Item1;
        int newY = y + direction.Item2;
        try
        {
            if (farm[newY][newX].plant == plant)
            {
                if (farm[newY][newX].visited)
                    continue;
                var result = MeasureRegion(newX,newY);
                area += result.Item1;
                perimeter += result.Item2;
            }
            else
            {
                perimeter += 1;
            }
        }
        catch // we're at the edge of the 2d array
        {
            perimeter++;
        }
    }

    return (area, perimeter);
}

UInt64 totalPrice = 0;
for (int y = 0; y < farm.Length; y++)
{
    for (int x = 0; x < farm[y].Length; x++)
    {
        if (farm[y][x].visited)
            continue;
        var result = MeasureRegion(x,y);
        UInt64 price = result.Item1 * result.Item2;
        totalPrice += price;
        Console.WriteLine($"A region of {farm[y][x].plant} plants with price {result.Item1} * {result.Item2} = {price}");
    }
}
Console.WriteLine($"Total price of {totalPrice}");


public class Cell
{
    public Cell(char plant)
    {
        this.plant = plant;
        visited = false;
    }

    public char plant;
    public bool visited;
}


