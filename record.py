#!/bin/python3
import sys
import pyaudio
import struct
import math
import numpy as np
from scipy import signal
import matplotlib.pyplot as plt
import importlib

anim = importlib.import_module("create_animation")

THRESHOLD = 50 # dB
RATE = 48000
INPUT_BLOCK_TIME = 0.1 # 30 ms
INPUT_FRAMES_PER_BLOCK = int(RATE * INPUT_BLOCK_TIME)

def get_rms(block):
    return np.sqrt(np.mean(np.square(block)))

audio = None

class AudioHandler(object):
    def __init__(self):
        global audio
        self.pa = pyaudio.PyAudio()
        self.stream = self.open_mic_stream()
        self.threshold = THRESHOLD
        self.plot_counter = 0
        self.t = []
        self.f = []
        self.Sxx = []
        audio = self

    def stop(self):
        self.stream.close()

    def find_input_device(self):
        device_index = None
        for i in range( self.pa.get_device_count() ):
            devinfo = self.pa.get_device_info_by_index(i)
            print('Device %{}: %{}'.format(i, devinfo['name']), file=sys.stderr)

            for keyword in ['mic','input']:
                if keyword in devinfo['name'].lower():
                    print('Found an input: device {} - {}'.format(i, devinfo['name']), file=sys.stderr)
                    device_index = i
                    return device_index

        if device_index == None:
            print('No preferred input found; using default input device.', file=sys.stderr)

        return device_index

    def open_mic_stream( self ):
        device_index = self.find_input_device()

        stream = self.pa.open(  format = pyaudio.paInt16,
                                channels = 1,
                                rate = RATE,
                                input = True,
                                input_device_index = device_index,
                                frames_per_buffer = INPUT_FRAMES_PER_BLOCK)

        return stream

    def make_spectrogram(self, framenum, x, y, z):
        global audio
        tlen = len(audio.t)
        flen = len(audio.f)
        x_sam = audio.t[int((x / 6) * tlen)]
        y_sam = audio.f[int((y / 6) * flen)]
        #z_sam = audio.Sxx[int((x / 6) * tlen)][int((y / 6) * flen)]
        z_sam = audio.Sxx[int((y/6) * flen)][int((x/6) * tlen)]
        #print(audio.Sxx, file=sys.stderr)
        #print("x: {}, y: {}, z: {}".format(x_sam, y_sam, z_sam), file=sys.stderr)
        #print("t: {}, f: {}".format(audio.t, audio.f), file=sys.stderr)
        #print(z_sam, file=sys.stderr)
        #sys.stderr.write("z_sam: {}, ".format(z_sam))
        if z_sam > pow(10, -z):
            anim.set_led(int(x), int(y), 5-int(z), True)


    def processBlock(self, snd_block):
        self.f, self.t, self.Sxx = signal.spectrogram(snd_block, RATE)
        #plt.pcolormesh(t, f, Sxx)
        #plt.ylabel('Frequency [Hz]')
        #plt.xlabel('Time [sec]')
        #plt.savefig('data/spec{}.png'.format(self.plot_counter), bbox_inches='tight')
        #self.plot_counter += 1
        anim.clear_frame()
        anim.supply_3d(0, self.make_spectrogram)
        anim.print_frame(100)

    def listen(self):
        try:
            raw_block = self.stream.read(INPUT_FRAMES_PER_BLOCK, exception_on_overflow = False)
            count = len(raw_block) / 2
            format = '%dh' % (count)
            snd_block = np.array(struct.unpack(format, raw_block))
        except Exception as e:
            print('Error recording: {}'.format(e), file=sys.stderr)
            return

        amplitude = get_rms(snd_block)
        if amplitude > self.threshold:
            self.processBlock(snd_block)
        else:
            pass

if __name__ == '__main__':
    audio = AudioHandler()
    for i in range(0,10000):
        audio.listen()