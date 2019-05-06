import argparse

import numpy as np
import librosa

from scipy.io import wavfile
from scipy.signal import resample


DTYPE_RANGES = {
    np.dtype('float32'): (-1.0, 1.0), np.dtype('int32'): (-2147483648, 2147483647), 
    np.dtype('int16'): (-32768, 32767), np.dtype('uint8'): (0, 255)
}
BITS_TO_DTYPE = {
    64: np.dtype('float32'), 32: np.dtype('int32'), 16: np.dtype('int16'), 8: np.dtype('uint8')
}


parser = argparse.ArgumentParser(description='Create a data file for the Choreographed Particle Simulator')
parser.add_argument('input_file', type=str, help='audio file path')
parser.add_argument('output_file', type=str, help='data file path')
parser.add_argument('--sample_rate', '-r', type=int, required=False, default=10)
parser.add_argument('--bits_per_sample', '-b', type=int, required=False, default=8)
args = parser.parse_args()


def load_resampled_audio(input_filename, sample_rate):
    audio, fs = librosa.core.load(input_filename, sr=None)
    # fs, audio = wavfile.read(input_filename)
    return sample_rate, resample(audio, len(audio) * sample_rate // fs).astype(np.dtype('float32'))


def encode_audio_file(input_filename, output_filename, sample_rate=10, bits_per_sample=8):
    # Downsample and save (librosa supports only 16-bits and scipy.resample is slow af)
    # y, sr = librosa.core.load(input_filename, sr=sample_rate, mono=True)
    # Reduce bitrate of audio
    fs, audio = load_resampled_audio(input_filename, sample_rate)
    new_dtype = BITS_TO_DTYPE[bits_per_sample]
    if new_dtype != audio.dtype:
        current_range, new_range = DTYPE_RANGES[audio.dtype], DTYPE_RANGES[new_dtype]
        audio = (
            (audio - current_range[0]) / (current_range[1] - current_range[0]) *
            (new_range[1] - new_range[0]) + new_range[0]
        ).astype(new_dtype)
    # Write file encoded as such:
    # 4B: sample rate
    # 4B: magnitude length = X
    # XB: wav data
    with open(output_filename, 'wb') as fp:
        fp.write(fs.to_bytes(4, "little"))
        fp.write(len(audio).to_bytes(4, "little"))
        fp.write(bytes(audio))


if __name__ == "__main__":
    encode_audio_file(args.input_file, args.output_file, args.sample_rate, args.bits_per_sample)
