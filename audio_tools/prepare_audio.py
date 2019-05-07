import argparse

import numpy as np
import librosa

from scipy.io import wavfile
import scipy.signal


DTYPE_RANGES = {
    np.dtype('float32'): (0.0, 1.0), np.dtype('uint8'): (0, 255)
    # np.dtype('float32'): (-1.0, 1.0), np.dtype('int32'): (-2147483648, 2147483647),
    # np.dtype('int16'): (-32768, 32767), np.dtype('uint8'): (0, 255)
}
BITS_TO_DTYPE = {
    64: np.dtype('float32'), 32: np.dtype('int32'), 16: np.dtype('int16'), 8: np.dtype('uint8')
}

# Example usage:
# python3 ../../audio_tools/prepare_audio.py test_song.mp3 test_song_8b_5Hz -r 5

parser = argparse.ArgumentParser(description='Create a data file for the Choreographed Particle Simulator')
parser.add_argument('input_file', type=str, help='audio file path')
parser.add_argument('output_file', type=str, help='data file path')
parser.add_argument('--sample_rate', '-r', type=int, required=False, default=100)
# parser.add_argument('--bits_per_sample', '-b', type=int, required=False, default=8)
args = parser.parse_args()


def load_audio(input_filename):
    print("Loading audio file")
    return librosa.core.load(input_filename, sr=None)


def resample(audio_og, sample_rate_og, sample_rate):
    print("Subsampling")
    magnitude = abs(audio_og)
    audio_len = len(audio_og) * sample_rate // sample_rate_og
    # Warning: this function will take a very long time if len(audio_og) is prime
    # Zero-pad magnitude to a power of two to improve runtime
    resampled_audio = scipy.signal.resample(magnitude, audio_len).astype(np.dtype('float32'))
    return abs(resampled_audio) / resampled_audio.max()


def reduce_bitrate(audio, bits_per_sample):
    print("Reducing bits per sample")
    new_dtype = BITS_TO_DTYPE[bits_per_sample]
    if new_dtype != audio.dtype:
        current_range, new_range = DTYPE_RANGES[audio.dtype], DTYPE_RANGES[new_dtype]
        audio = (
                (audio - current_range[0]) / (current_range[1] - current_range[0]) *
                (new_range[1] - new_range[0]) + new_range[0]
        ).astype(new_dtype)
    return audio


def get_onsets(audio_og, sample_rate_og, sample_rate):
    print("Computing onsets")
    audio_len = len(audio_og) * sample_rate // sample_rate_og
    onsets = librosa.onset.onset_detect(y=audio_og, sr=sample_rate_og, units='samples')
    onsets_indicator = np.zeros((audio_len, ), dtype=np.uint8)
    if len(onsets) * 10 < audio_len:
        print("Found", len(onsets), "onsets for audio length", audio_len)
        onsets_indicator[onsets * sample_rate // sample_rate_og] = 1
    return onsets_indicator


def get_beats(audio_og, sample_rate_og, sample_rate):
    print("Computing beats")
    audio_len = len(audio_og) * sample_rate // sample_rate_og
    tempo, beats = librosa.beat.beat_track(y=audio_og, sr=sample_rate_og, units='samples')
    beat_indicator = np.zeros((audio_len, ), dtype=np.uint8)
    print("Found", len(beats), "beats for audio length", audio_len)
    if len(beats) * 10 < audio_len:
        beat_indicator[beats * sample_rate // sample_rate_og] = 1
    return beat_indicator


def write_file(output_filename, sample_rate, magnitudes, onsets, beats):
    print("Writing file")
    with open(output_filename, 'wb') as fp:
        fp.write(sample_rate.to_bytes(4, "little"))
        fp.write(len(magnitudes).to_bytes(4, "little"))
        assert len(magnitudes) == len(onsets) == len(beats)
        fp.write(bytes(magnitudes))
        fp.write(bytes(onsets))
        fp.write(bytes(beats))


def encode_audio_file(input_filename, output_filename, sample_rate):
    audio_og, sample_rate_og = load_audio(input_filename)
    magnitudes = reduce_bitrate(resample(audio_og, sample_rate_og, sample_rate), 8)
    onsets = get_onsets(audio_og, sample_rate_og, sample_rate)
    beats = get_beats(audio_og, sample_rate_og, sample_rate)
    write_file(output_filename, sample_rate, magnitudes, onsets, beats)


if __name__ == "__main__":
    encode_audio_file(args.input_file, args.output_file, args.sample_rate)
