// Some copyright should be here...

#include "VidCapturePluginPrivatePCH.h"

#include "SlateBasics.h"
#include "SlateExtras.h"

#include "VidCapturePluginStyle.h"
#include "VidCapturePluginCommands.h"

#include "LevelEditor.h"

#include "AllowWindowsPlatformTypes.h"
#include <windows.h>
#include "HideWindowsPlatformTypes.h"

#include <conio.h>
#include <tchar.h>
#include <iostream> // can delete these for io later
#include <fstream>
#include <stdio.h>

#include <string>
#include <sstream>


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
			TArray<FColor> OutBuffer, OutBuffer1, OutBuffer2, OutBuffer3, OutBuffer4; // 4 buffers for 4 player screens

			ReadingViewport->ReadPixels(OutBuffer);
			
			ULocalPlayer* FirstPlayer = GEngine->FindFirstLocalPlayerFromControllerId(0);
			TArray<FSplitscreenData> SplitscreenInfo = FirstPlayer->ViewportClient->SplitscreenInfo;

			int SplitscreenType = FirstPlayer->ViewportClient->GetCurrentSplitscreenConfiguration();
			UE_LOG(ModuleLog, Warning, TEXT("splitscreen %d"), SplitscreenType);
			
			
			/*********************************************************************************/
			/* This part is for split screen. Currently only writes to file
			/*********************************************************************************/
			/*
			int sizeX = ReadingViewport->GetSizeXY().X;
			int sizeY = ReadingViewport->GetSizeXY().Y;
			UE_LOG(ModuleLog, Warning, TEXT("Width: %d Height: %d"), sizeX, sizeY);

			// split screen for 2 players horizontal
			if (SplitscreenType == ESplitScreenType::TwoPlayer_Horizontal) {
				Split2Player(OutBuffer);
			}

			// split screen for 4 players
			if (SplitscreenType == ESplitScreenType::FourPlayer) {
				Split4Player(OutBuffer, sizeX);
			}
			*/

			//ReadingViewport->TakeHighResScreenShot(); // for testing. works.


			/*********************************************************************************/
			/* This part is for writing buffer to named pipe. Is independent of split screen.
			/*********************************************************************************/

			
			// write outbuffer to windows named pipe			
			int sizeX = ReadingViewport->GetSizeXY().X;
			int sizeY = ReadingViewport->GetSizeXY().Y;
			UE_LOG(ModuleLog, Warning, TEXT("Width: %d Height: %d"), sizeX, sizeY);

			HANDLE hPipe;
			unsigned long dwWritten;

			hPipe = CreateFile(TEXT("\\\\.\\pipe\\test_pipe"),
				GENERIC_READ | GENERIC_WRITE,
				0,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);
			int count = 0;
			if (hPipe != INVALID_HANDLE_VALUE)
			{
				std::ofstream myfile1("testingx.txt");
				BOOL success = false;
				
				if (myfile1.is_open()) {
					/*
					for (uint32 i = 0; i < 256; i++) {
						PixelBuffer = i + i*256 + i*256*256 + i*256*256*256;
						myfile1 << i << i << i << i << '\n';
						success = WriteFile(hPipe, (LPCVOID)&PixelBuffer, 4, &dwWritten, NULL);
					}
					*/
					

					uint32 PixelBuffer;
					for (auto& Pixel : OutBuffer) {
						std::ostringstream PixelStream;
						PixelBuffer = Pixel.A * 256 * 256 * 256 + Pixel.B * 256 * 256 + Pixel.G * 256 + Pixel.R;
						myfile1 << Pixel.R << Pixel.G << Pixel.B << Pixel.A << '\n';

						// convert stream to lpcvoid to write, 4 colour bytes + string terminating char
						success = WriteFile(hPipe, (LPCVOID)&PixelBuffer, 4, &dwWritten, NULL);
						if (success) {
							count++;
						}
						
					}
					
				}
				myfile1.close();
				
			}

			CloseHandle(hPipe);
			UE_LOG(ModuleLog, Warning, TEXT("Finished writing. Total %d pixels."), count)
			
		}
	}
}


// write to file (will change to pipe later)
void FVidCapturePluginModule::Split2Player(TArray<FColor> OutBuffer) {


	// Player 1
	std::ofstream myfile1("outbuffer2p1.txt", std::ios::out | std::ios::app | std::ios::binary);
	if (myfile1.is_open()) {
		for (int i = 0; i < OutBuffer.Num() / 2; i++) {
			FColor Pixel = OutBuffer[i];
			myfile1 << Pixel.R << Pixel.G << Pixel.B << Pixel.A;		
		}
		myfile1.close();
	
	}

	// Player 2
	std::ofstream myfile2("outbuffer2p2.txt", std::ios::out | std::ios::app | std::ios::binary);
	if (myfile2.is_open()) {
		for (int i = OutBuffer.Num() / 2; i < OutBuffer.Num(); i++) {
			FColor Pixel = OutBuffer[i];
			myfile2 << Pixel.R << Pixel.G << Pixel.B << Pixel.A;
			
		}

		myfile2.close();
	
	}
}

// write to file (will change to pipe later)
void FVidCapturePluginModule::Split4Player(TArray<FColor> OutBuffer, int sizeX) {
	
	// Player 1
	std::ofstream myfile1("outbuffer4p1.txt", std::ios::out | std::ios::app | std::ios::binary);
	if (myfile1.is_open()) {
		for (int i = 0; i < OutBuffer.Num() / 2; i++) {
			if (i % sizeX < sizeX / 2) {
				FColor Pixel = OutBuffer[i];
				myfile1 << Pixel.R << Pixel.G << Pixel.B << Pixel.A;
			}
		}
		myfile1.close();
	}
	
	// Player 2
	std::ofstream myfile2("outbuffer4p2.txt", std::ios::out | std::ios::app | std::ios::binary);
	if (myfile2.is_open()) {
		for (int i = 0; i < OutBuffer.Num() / 2; i++) {
			if (i % sizeX >= sizeX / 2) {
				FColor Pixel = OutBuffer[i];
				myfile2 << Pixel.R << Pixel.G << Pixel.B << Pixel.A;
			}
		}
		myfile2.close();
	}

	// Player 3
	std::ofstream myfile3("outbuffer4p3.txt", std::ios::out | std::ios::app | std::ios::binary);
	if (myfile3.is_open()) {
		for (int i = OutBuffer.Num() / 2; i < OutBuffer.Num(); i++) {
			if (i % sizeX < sizeX / 2) {
				FColor Pixel = OutBuffer[i];
				myfile3 << Pixel.R << Pixel.G << Pixel.B << Pixel.A;
			}
		}
		myfile3.close();
	}

	// Player 4
	std::ofstream myfile4("outbuffer4p4.txt", std::ios::out | std::ios::app | std::ios::binary);
	if (myfile4.is_open()) {
		for (int i = OutBuffer.Num() / 2 + 1; i < OutBuffer.Num(); i++) {
			if (i % sizeX >= sizeX / 2) {
				FColor Pixel = OutBuffer[i];
				myfile4 << Pixel.R << Pixel.G << Pixel.B << Pixel.A;
			}
		}
		myfile4.close();
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