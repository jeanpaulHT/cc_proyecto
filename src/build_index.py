import pandas as pd
import os
import timeit

if __name__ == "__main__":
    start = timeit.default_timer()

    # These files were actually removed after indexation.
    file_dir = sorted(os.listdir("data/"))

    # Collecting dimension data.
    for i in range(6):
        raw_csv = file_dir[i]
        print(f"Processing: {raw_csv}")
        dimensions = ["Store_and_fwd_flag", "RateCodeID"]
        df = pd.read_csv(f"data/{raw_csv}", skiprows=[1], usecols=dimensions)
        df = df[df["Store_and_fwd_flag"] != '0.0']
        df = df[df["RateCodeID"] != '0.0']  
        df.to_csv(f"idx/idx_green_{i+1}.csv", index=False, encoding="utf-8")
        print(f"**Filtered {len(df)} rows.")
    for i in range(6,12):
        raw_csv = file_dir[i]
        print(f"Processing: {raw_csv}")
        dimensions = ["Pickup_longitude", "Pickup_latitude"]
        df = pd.read_csv(f"data/{raw_csv}", skiprows=[1], usecols=dimensions)
        df = df[df["Pickup_longitude"] != '0.0']
        df = df[df["Pickup_latitude"] != '0.0']  
        df.to_csv(f"idx/idx_green_{i+1}.csv", index=False, encoding="utf-8")
        print(f"**Filtered {len(df)} rows.")
    print(f"**Built all indexes in {timeit.default_timer() - start}s.")