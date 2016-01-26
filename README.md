## Description

Temporary plugin for video capture from Unreal. Outputs the current screen as a stream of bytes into a file. Can now also read buffer to Windows Named Pipe, but not formatted well.

## Setup

In your game folder, create a folder named 'Plugins' if it doesn't exist. Put VidCapturePlugin in your Plugins folder. Build and run your game. The plugin should show up in Menu > Edit > Plugins.

## Usage
Split screen function is completed, for 2 and 4 players. Streaming is halfway done. Comment out the irrelevant section to test.
To capture the current frame, run the game, and press the plugin's button in the toolbar on the top of the screen.

Split screen - After running split screen, you need to take note of the width and height in the log. The .txt files of all player frames can be found in your Engine\Binaries\Win64 folder (at least for me). Run buffertoimg.py to convert to image. You will need to change the height and width accordingly. For 2 player, divide height by 2. For 4 player, divide both height and width by 2.

Streaming - The file server.py needs to be run to open a pipe.  Currently the pixels are directly converted to string by Unreal and read as a tuple by python. This is very rough. File size is big, so only a few points are printed.