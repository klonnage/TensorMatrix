import pandas as pd
import seaborn as sns
from sys import argv
import matplotlib.pyplot as plt

def main():
    data = pd.read_csv(argv[1])
    data["Gflops"] = data["time"].apply(lambda time: 1024*8*8*1024*8*8 / (time * 1e9))
    #data["speedup"] = data[data["optimization"]=="BASIC OMP2"]["time"].apply(lambda time: data[data["optimization"]=="BASIC"]["time"][0] / time)
    bar = sns.lineplot(x = "nthread", y = "Gflops", hue= "optimization", data = data)
    #data["Gflops"] = data["time"].apply(lambda time: 
    bar.set_ylabel("Gflops/s")
    bar.set_xlabel("Thread OpenMP")
    #bar.set_ylim((data["time"].min() - 1, data["time"].max() + 1))
    bar.set_title('\n'.join(argv[2].split("|")))
    plt.savefig(argv[3])

main()
