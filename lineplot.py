import pandas as pd
import seaborn as sns
from sys import argv
import matplotlib.pyplot as plt

def main():
    data = pd.read_csv(argv[1])
    bar = sns.lineplot(x = "nthread", y = "time", hue= "compiler", data = data)
    bar.set_ylabel("Temps (s)")
    bar.set_xlabel("Thread OpenMP")
    #bar.set_ylim((data["time"].min() - 1, data["time"].max() + 1))
    bar.set_title('\n'.join(argv[2].split("|")))
    plt.savefig(argv[3])

main()
