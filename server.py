import win32pipe, win32file

p = win32pipe.CreateNamedPipe(r'\\.\pipe\test_pipe',
    win32pipe.PIPE_ACCESS_DUPLEX,
    win32pipe.PIPE_TYPE_BYTE | win32pipe.PIPE_READMODE_BYTE | win32pipe.PIPE_WAIT,
    1, 65536, 65536,3000,None)

win32pipe.ConnectNamedPipe(p, None)

f = open('testing.txt', 'wb')
count = 0
try:
    while (1):
        data = win32file.ReadFile(p, 4)
        f.write(data[1][0:4])
        f.write('\n')

        count = count + 1
        
except:
    print 'ended'

print count
f.close()
