import cv2
import subprocess
import sys
import win32pipe, win32file

# grab frame from pipe
p = win32pipe.CreateNamedPipe(r'\\.\pipe\test_pipe',
    win32pipe.PIPE_ACCESS_DUPLEX,
    win32pipe.PIPE_TYPE_BYTE | win32pipe.PIPE_READMODE_BYTE | win32pipe.PIPE_WAIT,
    1, 65536, 65536,3000,None)

win32pipe.ConnectNamedPipe(p, None)

framedata = ""
try:
    while (1):
        data = win32file.ReadFile(p, 4)
        framedata = framedata + data[1][0:4]
        
        
except:
    print 'ended'

#f = open('outbuffer4p.txt', 'rb') # input file frame - binary
frameWidth = 856
frameHeight = 358
FPS = 1 # 1 frame per second, just for testing
frameCount = 1

bytes_per_frame = 4 * frameWidth * frameHeight # rgba is 4 bytes

print ("Frame width =", frameWidth)
print ("Frame height =", frameHeight)
print ("FPS =", FPS)
print ("Frame count =", frameCount)
    
# ffmpeg command: -f rawvideo -pix_fmt bgr24 -s 640x480 -r 30 -i - -an -f avi -r 30 foo.avi
cmdString = ('ffmpeg.exe',
         '-y',          # Overwrite output file if it already exists
             '-f', 'rawvideo',      # Expect raw video input (RGB)
             '-pix_fmt', 'rgba',   # Set input pixel format to rgba
             '-s', '%dx%d' %(frameWidth, frameHeight),  # Set input video size
             '-r', '%d' %FPS,       # Set frame rate of input 
             '-i', '-',         # Expect input from pipe
             '-an',         # Output has no audio
             '-f', 'avi',       # Set output format to avi
             '-r', '%d' %FPS,       # Set frame rate of output
             'output.avi'       # Output video name
             )
#pipe = subprocess.Popen(cmdString, stdin=subprocess.PIPE, shell=False)
#write frame to file

#for fr in range(0, frameCount):
    #_,img = cap.read() # get video frame
#print(fr)
#framedata = f.read(bytes_per_frame*frameCount)
pipe = subprocess.Popen(cmdString, stdin=subprocess.PIPE, shell=False)
pipe.communicate( framedata )
#pipe.stdin.write('test')

pipe.stdin.close()
#f.close()
    
    

