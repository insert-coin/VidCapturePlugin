import cv2
import subprocess
import sys

f = open('outbuffer4p.txt', 'rb') # input file frame - binary
frameWidth = 428
frameHeight = 179
FPS = 1 # 1 frame per second, just for testing
frameCount = 4

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
framedata = f.read(bytes_per_frame*frameCount)
pipe = subprocess.Popen(cmdString, stdin=subprocess.PIPE, shell=False)
pipe.communicate( framedata )
#pipe.stdin.write('test')

pipe.stdin.close()
f.close()
    
    

