import simpl
import numpy as np
from scipy.io.wavfile import write

input_file = '../../tests/audio/flute.wav'
output_file = 'resynth.wav'

audio = simpl.read_wav(input_file)[0]

pd = simpl.SMSPeakDetection()
pd.max_peaks = 40
pd.hop_size = 512
peaks = pd.find_peaks(audio)
pt = simpl.MQPartialTracking()
pt.max_partials = 20
partials = pt.find_partials(peaks)
synth = simpl.SndObjSynthesis()
audio_out = synth.synth(partials)
audio_out = np.asarray(audio_out * 32768, np.int16)
write(output_file, 44100, audio_out)
