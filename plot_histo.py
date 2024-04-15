import pandas as pd
import matplotlib.pyplot as plt

def plot_histogram_from_csv(csv_file, max_duration):
    # Read the CSV file into a DataFrame
    df = pd.read_csv(csv_file)

    df.loc[df['Measured time [ns]'] > max_duration, 'Measured time [ns]'] = max_duration
    
    # Plot the histogram
    plt.hist(df['Measured time [ns]'], bins=200, edgecolor='black')
    plt.xlabel('Duration (nanoseconds)')
    plt.ylabel('Frequency')
    plt.title('Histogram of Time Intervals')
    plt.grid(True)
    plt.show()

# Replace 'your_csv_file.csv' with the path to your CSV file
plot_histogram_from_csv('./data/data.csv', max_duration=200000)

