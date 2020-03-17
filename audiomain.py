import pyaudio
import numpy as np
from numpy import interp
import matplotlib.pyplot as pl
import matplotlib.animation as animation
import serial
import struct
import time
WIDTH=1280
HEIGHT=720
FPS=30.0
nFFT=512
FORMAT=pyaudio.paInt16
RATE=44100
y2 = [0,0,0,0,0,0,0]
x2 = [0, 1, 2, 3, 4, 5, 6]
ser=serial.Serial('COM4',9600)
time.sleep(3)
def init(line):
    line.set_ydata(np.zeros(nFFT-1))
    return line,
def barlist(n):
    return [1/float(n*k) for k in range(1,6)]
def frequencySpectrum(spect):
    subBass=(spect[255]+spect[256])/2
    subBass=round(interp(subBass,[0,10],[0,255]),2)
    bass=(spect[257]+spect[258]+spect[259])/3
    bass=round(interp(bass,[0,10],[0,255]),2)
    lowerMid=(spect[260]+spect[261]+spect[262])
    lowerMid=round(interp(lowerMid,[0,10],[0,255]),2)
    mid=highMid=presense=brilliance=0
    for i in range(1,20):
        mid=mid+spect[262+i]
        mid=mid/19
        mid=round(interp(mid,[0,11],[0,255]),2)
    for i in range(1,26):
        highMid=highMid+spect[281+i]
        highMid=round(highMid/25,6)
    for i in range(1, 26):
        presense = presense + spect[306 + i]
        presense = round(presense / 25,6)
    for i in range(1,175):
        brilliance=brilliance+spect[331+i]
        brilliance=round(brilliance/175,6)
    #print('[',subBass,']',' [',bass,']',' [',lowerMid,']',' [',mid,']'' [',highMid,']',' [',presense,']',
         # ' [',brilliance,']')
    if subBass>70:
        ser.write(chr(int(mid)))
        print 'yes'
    else:
        ser.write('')
        print "no"


    return [subBass,bass,lowerMid,mid,highMid,presense,brilliance]

def animate(i,line,bar,stream,wf,maxY):
    N=max(stream.get_read_available()/nFFT,1)*nFFT
    data=stream.read(int(N))
    y = np.array(struct.unpack("%dh" % (N*2), data)) / maxY
    y_L = y[::2]
    y_R = y[1::2]
    Y_L = np.fft.fft(y_L, nFFT)
    Y_R = np.fft.fft(y_R, nFFT)

    # Sewing FFT of two channels together, DC part uses right channel's
    Y = abs(np.hstack((Y_L[int(-nFFT / 2):-1], Y_R[:int(nFFT / 2)])))
    newY=frequencySpectrum(Y)
    line.set_ydata(Y)

    for bar, h in zip(bar,x2):
        bar.set_height(newY[h])

    #bar.set_height(newY)
    return line,bar,
def main():
    fig=pl.figure()
    xf=1.0*np.arange(-nFFT/2+1,nFFT/2)/nFFT*RATE
    ax=fig.add_subplot(121,title='audio spectrum',xlim=(xf[0],xf[-1]),
                       ylim=(0,2*np.pi*nFFT**2/RATE))
    ax2=fig.add_subplot(122,title='7 spectrum values',xlim=(0,6),
                        ylim=(0,2*np.pi*nFFT**2/RATE))
    ax.set_yscale('symlog',linthreshy=nFFT**.1)
    line,=ax.plot(xf,np.zeros(nFFT-1))
    #ax2.axvline(1,0,.25, color='black')
    #bar,=ax2.stem(x2,y2)
    bar=ax2.bar(x2,y2)
    p=pyaudio.PyAudio()
    maxY=2.0**(p.get_sample_size(FORMAT)*8-1)
    stream=p.open(format= pyaudio.paInt16,
                  channels=2,
                  rate=44100,
                  input=True,
                  frames_per_buffer=2048,
                  input_device_index=2)
    frames=None
    wf=None
    ani=animation.FuncAnimation(fig, animate, frames,
    init_func=lambda: init(line), fargs=(line,bar, stream, wf, maxY),
    interval=1000.0/FPS, blit=True)
    pl.show()
if __name__ == '__main__':
  main()










