from scipy.fftpack import fft
import numpy as np

# x = np.array([1, 2, 3, 4])
x = np.array([1, 2, 3, 4, 5, 6, 7, 8])
y = fft(x)
print(y)
