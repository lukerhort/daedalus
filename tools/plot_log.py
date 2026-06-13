#!/usr/bin/env python3
"""Plot a Daedalus CSV log (Phase 1 exit artifact).

Usage: python tools/plot_log.py LOG.CSV
Columns: t_us, state, pressure_pa, temp_c, ax, ay, az, sats
"""
import sys
import matplotlib.pyplot as plt
import pandas as pd

COLS = ["t_us", "state", "pressure_pa", "temp_c", "ax", "ay", "az", "sats"]

def main(path: str) -> None:
    df = pd.read_csv(path, names=COLS)
    t = (df.t_us - df.t_us.iloc[0]) / 1e6
    fig, axes = plt.subplots(2, 1, sharex=True, figsize=(10, 7))
    axes[0].plot(t, df.pressure_pa)
    axes[0].set_ylabel("pressure (Pa)")
    for axis in ("ax", "ay", "az"):
        axes[1].plot(t, df[axis], label=axis)
    axes[1].set_ylabel("accel (m/s^2)")
    axes[1].set_xlabel("t (s)")
    axes[1].legend()
    fig.suptitle(path)
    plt.show()

if __name__ == "__main__":
    main(sys.argv[1])
