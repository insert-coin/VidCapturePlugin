## Description

(Temporary?) Plugin for video capture from Unreal. Currently it only outputs the current screen in a random format.

## Setup

In your game folder, create a folder named 'Plugins' if it doesn't exist. Put VidCapturePlugin in your Plugins folder. Build and run your game. The plugin should show up in Menu > Edit > Plugins.

## Usage
To capture the current frame, run the game, and press the plugin's button in the toolbar on the top of the screen. Currently the file is written in a r g b a format, in your UnrealEngine\Engine\Binaries\Win64 folder. You can change the file path as you like in the VidCapturePlugin.cpp. You may need Notepad++ or anything but Notepad to open the file. It is large.

You can also test out the screen capture into png format. Just uncomment the take high res line.