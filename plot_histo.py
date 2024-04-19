import pandas as pd
import matplotlib.pyplot as plt
import statistics

def plot_histogram_from_csv(csv_file, max_duration):
    # Read the CSV file into a DataFrame
    df = pd.read_csv(csv_file)
    
    # Calculate and print statistics
    max_time = round(df['Measured time [us]'].max())
    min_time = round(df['Measured time [us]'].min())
    median_time = round(statistics.median(df['Measured time [us]']))
    print(f"Max Time: {max_time} µs.")
    print(f"Min Time: {min_time} µs.")
    print(f"Median Time: {median_time} µs.")
    
    df.loc[df['Measured time [us]'] > max_duration, 'Measured time [us]'] = max_duration
    
    # Plot the histogram
    plt.hist(df['Measured time [us]'], bins=200, edgecolor='black')
    plt.xlabel('Duration (µs)')
    plt.ylabel('Frequency')
    plt.title('Histogram of Time Intervals')
    plt.grid(True)
    plt.show()

# Replace 'your_csv_file.csv' with the path to your CSV file
plot_histogram_from_csv('./data/data.csv', max_duration=200)

