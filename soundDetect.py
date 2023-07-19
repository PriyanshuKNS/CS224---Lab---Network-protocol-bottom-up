import sounddevice as sd
import numpy as np
import matplotlib.pyplot as plt

fs = 44100  # Sample rate
duration = 5  # Record for 5 seconds

# Initialize an empty list to store the maximum frequencies
max_frequencies = []

# Initialize a time array for the x-axis of the graph
times = []
total_time = 0 
buffer_duration = 512/fs

# Define a callback function to be called every buffer size
def audio_callback(indata, frames, time, status):
    global total_time
    volume_norm = np.linalg.norm(indata)*10
    # FFT of the sound data
    fft_data = np.fft.fft(indata[:, 0])
    # Get the frequencies of the FFT
    frequencies = np.fft.fftfreq(len(fft_data), 1/fs)
    # Get the absolute value of the FFT
    fft_data = np.abs(fft_data)
    # Find the maximum frequency
    max_frequency = frequencies[np.argmax(fft_data)]
    if max_frequency < 0:
        max_frequency = -max_frequency
    max_frequencies.append(max_frequency)
    total_time += buffer_duration
    times.append(total_time)

# Start the sound recording
stream = sd.InputStream(callback=audio_callback)
with stream:
    sd.sleep(int(duration * 1000))

# Plot the maximum frequency versus time
plt.plot(times, max_frequencies)
plt.xlabel('Time (s)')
plt.ylabel('Maximum Frequency (Hz)')
plt.show()
