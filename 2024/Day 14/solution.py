import pandas as pd
import matplotlib.pyplot as plt

# Part one was done in spreadsheet
# For part two, generating images to find the easter egg
# input is taken from spreadsheet so it can be loaded as a csv of coordinates and velocities

df = pd.read_csv('Day 14/input.txt', sep='\t')

print(df)
def step():
    df['p_x'] = (df['p_x'] + df['v_x']) % 101
    df['p_y'] = (df['p_y'] + df['v_y']) % 103
    plt.close()
    plt.scatter(df['p_x'],df['p_y'])
    plt.savefig(f"Day 14/images/{(i+1):05d}")

for i in range(10000):
    step()
    if i%100 == 0:
        print(f"Step {i}")



