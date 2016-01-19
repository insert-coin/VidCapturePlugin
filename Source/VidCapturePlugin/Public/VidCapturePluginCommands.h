// Some copyright should be here...

#pragma once

#include "SlateBasics.h"
#include "VidCapturePluginStyle.h"

class FVidCapturePluginCommands : public TCommands<FVidCapturePluginCommands>
{
public:

	FVidCapturePluginCommands()
		: TCommands<FVidCapturePluginCommands>(TEXT("VidCapturePlugin"), NSLOCTEXT("Contexts", "VidCapturePlugin", "VidCapturePlugin Plugin"), NAME_None, FVidCapturePluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
