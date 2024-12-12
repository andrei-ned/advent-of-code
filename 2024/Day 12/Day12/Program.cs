using System.Data;

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

Cell[][] farm = new Cell[farmStrings.Count+2][];
farm[0] = new Cell[farmStrings[0].Length + 2];
farm[farmStrings.Count+1] = new Cell[farmStrings[0].Length + 2];
for (int x = 0; x < farmStrings[0].Length+2; x++)
{
    farm[0][x] = new Cell('.');
    farm[farmStrings.Count+1][x] = new Cell('.');
}
for (int y = 0; y < farmStrings.Count ; y++)
{
    int farmY = y+1;
    farm[farmY] = new Cell[farmStrings[y].Length + 2];
    farm[farmY][0] = new Cell('.');
    farm[farmY][farmStrings[y].Length+1] = new Cell('.');
    for (int x = 0; x < farmStrings[y].Length; x++)
    {
        int farmX = x+1;
        farm[farmY][farmX] = new Cell(farmStrings[y][x]);
    }
}

(int,int)[] directions = new[] {(1,0),(0,1),(-1,0),(0,-1)};
(int,int)[] corners = new[] {(-1,-1),(-1,1),(1,-1),(1,1)};

// Returns (area, perimeter, sides)
(int, int, int) MeasureRegion(int x, int y)
{
    farm[y][x].visited = true;
    char plant = farm[y][x].plant;
    int area = 1;
    int perimeter = 0;
    int sides = 0;

    for (int i = 0; i < directions.Length; i++)
    {
        var direction = directions[i];
        int newX = x + direction.Item1;
        int newY = y + direction.Item2;

        if (farm[newY][newX].plant == plant)
        {
            if (farm[newY][newX].visited)
                continue;
            var result = MeasureRegion(newX,newY);
            area      += result.Item1;
            perimeter += result.Item2;
            sides     += result.Item3;
        }
        else
        {
            perimeter++;
        }
    }

    // Corner?
    for (int dirIdx = 0; dirIdx < directions.Length; dirIdx++)
    {
        var dir1 = directions[dirIdx];
        var dir2 = directions[(dirIdx + 1) % directions.Length];
        var neighbor1 = farm[y + dir1.Item2][x + dir1.Item1];
        var neighbor2 = farm[y + dir2.Item2][x + dir2.Item1];
        var neighbor3 = farm[y + dir1.Item2 + dir2.Item2][x + dir1.Item1 + dir2.Item1];
        if (neighbor1.plant != plant && neighbor2.plant != plant)
            sides++;
        if (neighbor1.plant == plant && neighbor2.plant == plant && neighbor3.plant != plant)
            sides++;
    }

    // if (sides > 0)
    //     Console.WriteLine($"Cell {x},{y} has {sides} corners");

    return (area, perimeter, sides);
}

int totalPrice = 0;
int totalDiscountedPrice = 0;
for (int y = 0; y < farm.Length; y++)
{
    for (int x = 0; x < farm[y].Length; x++)
    {
        if (farm[y][x].visited || farm[y][x].plant == '.')
            continue;
        var result = MeasureRegion(x,y);
        int price = result.Item1 * result.Item2;
        totalPrice += price;
        int discountPrice = result.Item1 * result.Item3;
        totalDiscountedPrice += discountPrice;
        //Console.WriteLine($"{farm[y][x].plant} region with price {result.Item1} * {result.Item2} = {price}");
        Console.WriteLine($"{farm[y][x].plant} region with price {result.Item1} * {result.Item3} = {discountPrice} (discounted)");
    }
}
Console.WriteLine($"Total price of {totalPrice}");
Console.WriteLine($"Total discounted price of {totalDiscountedPrice}");



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


