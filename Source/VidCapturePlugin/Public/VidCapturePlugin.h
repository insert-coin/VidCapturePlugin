// Some copyright should be here...

#pragma once

#include "ModuleManager.h"



DECLARE_LOG_CATEGORY_EXTERN(ModuleLog, Log, All)

class FToolBarBuilder;
class FMenuBuilder;

class FVidCapturePluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();

	void Split2Player(TArray<FColor> OutBuffer);
	void Split4Player(TArray<FColor> OutBuffer, int sizeX);
private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};