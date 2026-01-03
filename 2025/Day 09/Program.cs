var redTiles = new List<(long,long)>();

using (var fileStream = File.OpenRead("input.txt"))
using (var streamReader = new StreamReader(fileStream))
{
    while (!streamReader.EndOfStream)
    {
        string line = streamReader.ReadLine();
        var coords = line.Split(",");
        redTiles.Add((long.Parse(coords[0]),long.Parse(coords[1])));
    }
}

long area((long,long) tile1, (long,long) tile2)
{
    return Math.Abs(tile1.Item1-tile2.Item1+1) * Math.Abs(tile1.Item2-tile2.Item2+1);
}

bool rectIntersectsLine((long, long) rect1, (long,long) rect2, (long,long) line1, (long,long) line2)
{
    // check if area tile1, tile2 intersects with ab line
    long lineMinX = Math.Min(line1.Item1, line2.Item1);
    long lineMaxX = Math.Max(line1.Item1, line2.Item1);
    long lineMinY = Math.Min(line1.Item2, line2.Item2);
    long lineMaxY = Math.Min(line1.Item2, line2.Item2);
    long rectMinX = Math.Min(rect1.Item1, rect2.Item1);
    long rectMaxX = Math.Max(rect1.Item1, rect2.Item1);
    long rectMinY = Math.Min(rect1.Item2, rect2.Item2);
    long rectMaxY = Math.Max(rect1.Item2, rect2.Item2);

    bool withinRectX = isLineWithinRect(rectMinX, rectMaxX, lineMinX, lineMaxX);
    bool withinRectY = isLineWithinRect(rectMinY, rectMaxY, lineMinY, lineMaxY);
    return withinRectX && withinRectY;
    
    // do this with x and y
    bool isLineWithinRect(long rectMin, long rectMax, long lineMin, long lineMax)
    {
        if (lineMin < rectMin && lineMax > rectMax)
            return true;

        if (lineMax <= rectMin)
            return false;
        if (lineMin >= rectMax)
            return false;

        return true;
    }
}

//TESTING
Console.WriteLine(rectIntersectsLine((10,10), (20,20), (5, 15), (7,15))); // false
Console.WriteLine(rectIntersectsLine((10,10), (20,20), (5, 30), (15,30))); // false
Console.WriteLine(rectIntersectsLine((10,10), (20,20), (30, 15), (40,15))); // false
Console.WriteLine(rectIntersectsLine((10,10), (20,20), (5, 15), (30,15))); // true
Console.WriteLine(rectIntersectsLine((10,10), (20,20), (5, 15), (15,15))); // true
Console.WriteLine(rectIntersectsLine((10,10), (20,20), (13, 15), (17,15))); // true
Console.WriteLine(rectIntersectsLine((10,10), (20,20), (15, 15), (30,15))); // true
Console.WriteLine(rectIntersectsLine((10,10), (20,20), (15, 5), (30,5))); // false

bool isValid(int tile1Idx, int tile2Idx)
{
    (long, long) tile1 = redTiles[tile1Idx];
    (long, long) tile2 = redTiles[tile2Idx];
    (long, long) a = redTiles[redTiles.Count - 1];
    (long, long) b = redTiles[0];
    int i = 0;
    do
    {
        if (rectIntersectsLine(tile1, tile2, a, b))
            return false;

        i++;
        a = redTiles[i];
        b = redTiles[i-1];
    }
    while (i < redTiles.Count - 1);

    return true;
}

long max = -1;
long maxPartTwo = -1;
for (int i = 0; i < redTiles.Count; i++)
{
    for (int j = 0; j < redTiles.Count; j++)
    {
        max = Math.Max(max, area(redTiles[i], redTiles[j]));
        if (isValid(i, j))
        {
            maxPartTwo = Math.Max(maxPartTwo, area(redTiles[i], redTiles[j]));
        }
    }
}

Console.WriteLine($"Biggest area is {max}");
Console.WriteLine($"Biggest area with only green and red tiles is {maxPartTwo}");
