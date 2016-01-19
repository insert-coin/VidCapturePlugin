// Some copyright should be here...

#include "VidCapturePluginPrivatePCH.h"
#include "VidCapturePluginCommands.h"

#define LOCTEXT_NAMESPACE "FVidCapturePluginModule"

void FVidCapturePluginCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "VidCapturePlugin", "Execute VidCapturePlugin action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
