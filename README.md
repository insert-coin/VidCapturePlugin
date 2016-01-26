## Description

(Temporary?) Plugin for video capture from Unreal. Currently it only outputs the current screen in a random format. Can now also read buffer to Windows Named Pipe. Also not formatted well.

## Setup

In your game folder, create a folder named 'Plugins' if it doesn't exist. Put VidCapturePlugin in your Plugins folder. Build and run your game. The plugin should show up in Menu > Edit > Plugins.

## Usage
The file server.py needs to be run to open a pipe. To capture the current frame, run the game, and press the plugin's button in the toolbar on the top of the screen. Currently the pixels are directly converted to string by Unreal and read as a tuple by python. This is very rough. File size is big, so only a few points are printed.