## displaying the pixels read from the viewport

import cv2
import numpy
import os

thisfile = "outbuffer4p4.txt"

# change the dimensions of the image here
width = 428
height = 179

test = open(thisfile, "rb") # read binary file

temp_arr = []

# pre-process: read rgba as int into array
try:
    byte = test.read(1)
    while byte != "":
        rgba = []
        for i in range (0, 4):
            try:
                rgba.append(ord(byte))
            
            except TypeError:
                print 'here!' + byte

            byte = test.read(1)
        
        temp_arr.append(rgba)

finally:
    test.close()

# fill up empty pixels
while len(temp_arr) < width*height:
    temp_arr.append([0,0,0,0])

q = numpy.array(temp_arr)

# img array is [ht, wd, rgb]
w = numpy.empty([height, width,3], dtype='uint8')

for a in range(0, height):
    for b in range(0, width):
        pixel_rgba = q[width*a + b]
        w[a, b, 0] = pixel_rgba[2]
        w[a, b, 1] = pixel_rgba[1]
        w[a, b, 2] = pixel_rgba[0]
        if (a==121 and b>65 and b<80):
            print pixel_rgba

cv2.imshow(str(thisfile), w)

cv2.waitKey(0)
cv2.destroyAllWindows()
