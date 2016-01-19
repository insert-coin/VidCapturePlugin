// Some copyright should be here...

#include "VidCapturePluginPrivatePCH.h"

#include "SlateBasics.h"
#include "SlateExtras.h"

#include "VidCapturePluginStyle.h"
#include "VidCapturePluginCommands.h"

#include "LevelEditor.h"

#include <iostream>
#include <fstream>

static const FName VidCapturePluginTabName("VidCapturePlugin");

#define LOCTEXT_NAMESPACE "FVidCapturePluginModule"

DEFINE_LOG_CATEGORY(ModuleLog)

void FVidCapturePluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FVidCapturePluginStyle::Initialize();
	FVidCapturePluginStyle::ReloadTextures();

	FVidCapturePluginCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FVidCapturePluginCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FVidCapturePluginModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FVidCapturePluginModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FVidCapturePluginModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	
	
}

void FVidCapturePluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
	
	
	FVidCapturePluginStyle::Shutdown();

	FVidCapturePluginCommands::Unregister();
}

void FVidCapturePluginModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FVidCapturePluginModule::PluginButtonClicked()")),
							FText::FromString(TEXT("VidCapturePlugin.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);

	// ensure engine is running before getting viewport
	if (GEngine) {
		if (GEngine->GameViewport != nullptr && GIsRunning && IsInGameThread())
		{
			FViewport* ReadingViewport = GEngine->GameViewport->Viewport;
			TArray<FColor> OutBuffer;

			ReadingViewport->ReadPixels(OutBuffer);
			
			//UE_LOG(ModuleLog, Warning, TEXT("Get! %d"), OutBuffer.Num());
			
			int count = 0;
			for (auto& x : OutBuffer) {
				if (count%1000 == 0)
					UE_LOG(ModuleLog, Warning, TEXT("Get! %d %d %d"), x.A, x.B, x.R, x.G);
				count++;
			}
			
			//ReadingViewport->TakeHighResScreenShot(); // for testing. works.

			// write outbuffer to file
			std::ofstream myfile("outbuffer.txt");
			if (myfile.is_open()) {
				for (auto& Pixel : OutBuffer) {
					myfile << Pixel.R << " " << Pixel.G << " " << Pixel.B << " " << Pixel.A << '\n';
				}
				myfile.close();
				
			}

			UE_LOG(ModuleLog, Warning, TEXT("Finished writing"))
		}
	}
}

void FVidCapturePluginModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FVidCapturePluginCommands::Get().PluginAction);
}

void FVidCapturePluginModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FVidCapturePluginCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FVidCapturePluginModule, VidCapturePlugin)