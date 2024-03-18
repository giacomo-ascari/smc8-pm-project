

def index_to_note(i):
    # i=0 -> A0
    clas = ["A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"][i%12]
    if i < 0:
        order = str(int((i-2)/12))
    else:
        order = str(int((i+9)/12))
    return clas + order

for i in range(-15, 16):
    print(i, index_to_note(i))


'''print("printing")
print(yTotal)
# header
fig, ax = plt.subplots(nrows=2, sharex=True)
# original signal
librosa.display.waveshow(yTotal, sr=sr, ax=ax[0], color="blue")
ax[0].set(title='Envelope view, mono')
ax[0].label_outer()
# aaaa
D = librosa.amplitude_to_db(np.abs(librosa.stft(y)), ref=np.max)
img = librosa.display.specshow(D, x_axis='time', y_axis='log', ax=ax[1])
ax[1].set(title='pYIN fundamental frequency estimation')
fig.colorbar(img, ax=ax[1], format="%+2.f dB")
ax[1].plot(times, f0, label='f0', color='cyan', linewidth=3)
ax[1].legend(loc='upper right')

plt.show()

Audio(data=yTotal, rate=sr)'''
