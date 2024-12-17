Computer3Bit computer = new Computer3Bit();
string program_instructions;
ulong b, c;

// Read input
using (var fileStream = File.OpenRead("input.txt"))
    using (var streamReader = new StreamReader(fileStream))
    {
        string line;
        for (int i = 0; i < 3; i++)
        {
            line = streamReader.ReadLine();
            computer.registers[i] = uint.Parse(line.Split(" ")[2]);
        }
        b = computer.b_reg;
        c = computer.c_reg;
        line = streamReader.ReadLine(); // empy line
        line = streamReader.ReadLine();
        //Console.WriteLine(line);
        program_instructions = line.Split(" ")[1];
        //Console.WriteLine(program_instructions);
        var commaSplit = line.Split(",");
        computer.instructions = new byte[commaSplit.Length];
        computer.instructions[0] = byte.Parse(commaSplit[0].Split(" ")[1]);
        for (int i = 1; i < commaSplit.Length; i++)
        {
            computer.instructions[i] = byte.Parse(commaSplit[i]);
        }
        
    }

// Part 1
while (computer.Step());
Console.WriteLine(computer.output);

// Part 2
ulong a = 1;
while (true)
{
    computer.Reset(a, b, c);
    while (computer.Step()/* && program_instructions.Substring(0, computer.output.Length) == computer.output*/);
    if (computer.output == program_instructions)
    {
        Console.WriteLine($"Found register A: {a}");
        break;
    }
    string partial = program_instructions.Substring(program_instructions.Length - computer.output.Length);
    if (partial == computer.output)
    {
        a = a << 3;
    }
    else
        a++;
    //Console.WriteLine($"{a}: {computer.output} #==# {partial}");
}

class Computer3Bit
{
    public ulong[] registers = new ulong[3];
    public ulong a_reg { get { return registers[0];} set {registers[0] = value;}}
    public ulong b_reg { get { return registers[1];} set {registers[1] = value;}}
    public ulong c_reg { get { return registers[2];} set {registers[2] = value;}}
    public byte[] instructions;
    public string output = "";
    private uint instruction_ptr;

    public event Action OnOutput;

    public void Reset(ulong a, ulong b, ulong c)
    {
        instruction_ptr = 0;
        a_reg = a;
        b_reg = b;
        c_reg = c;
        output = "";
    }

    public bool Halted()
    {
        return instruction_ptr >= instructions.Length;
    }

    // Returns false if halting
    public bool Step()
    {
        if (instruction_ptr >= instructions.Length)
            return false;
        byte instruction = instructions[instruction_ptr];
        byte op = instructions[instruction_ptr + 1];
        switch (instruction)
        {
            case 0: // adv
                DivInstruction(0);
            break;
            case 1: // bxl
                b_reg = b_reg ^ op;
            break;
            case 2: // bst
                b_reg = GetComboOperand(op) % 8;
            break;
            case 3: // jnz
                if (a_reg != 0)
                {
                    instruction_ptr = op;
                    return true; // we're jumping, so don't increment instruction ptr
                }
            break;
            case 4: // bxc
                b_reg = b_reg ^ c_reg;
            break;
            case 5: // out
                if (output.Length > 0) output += ",";
                output += $"{GetComboOperand(op) % 8}";
            break;
            case 6: // bdv
                DivInstruction(1);
            break;
            case 7: // cdv
                DivInstruction(2);
            break;
        }
        instruction_ptr += 2;
        return true;

        void DivInstruction(int target_reg_idx)
        {
            int combo = (int)GetComboOperand(op);
            registers[target_reg_idx] = a_reg >> combo;
        }
    }

    private ulong GetComboOperand(byte op)
    {
        if (op <= 3)
            return op;
        return registers[op - 4];
    }
}

