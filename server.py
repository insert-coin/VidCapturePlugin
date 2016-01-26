import win32pipe, win32file

p = win32pipe.CreateNamedPipe(r'\\.\pipe\test_pipe',
    win32pipe.PIPE_ACCESS_DUPLEX,
    win32pipe.PIPE_TYPE_MESSAGE | win32pipe.PIPE_WAIT,
    1, 65536, 65536,300,None)

win32pipe.ConnectNamedPipe(p, None)

count = 0
try:
    while (1):
        data = win32file.ReadFile(p, 4096)
        if count%10000 == 0:
            print data
            print '\n'

        count = count + 1
        
except:
    print 'ended'
