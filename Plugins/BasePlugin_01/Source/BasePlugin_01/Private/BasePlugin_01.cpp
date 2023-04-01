// Copyright Epic Games, Inc. All Rights Reserved.

#include "BasePlugin_01.h"
#include "Editor.h"
#include "Framework/Docking/TabManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"

#define LOCTEXT_NAMESPACE "FBasePlugin_01Module"

static const FName DummyTab = TEXT("DummyTab");

extern "C"
{
	typedef const char* (*aiv_get_string_t)();
	typedef int (*aiv_adder_t)(const int a, const int b);
}

void FBasePlugin_01Module::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogTemp, Warning, TEXT("Startup from Plugin001"));
	void* Handle = FPlatformProcess::GetDllHandle(TEXT("D:/aivlib.dll"));
	if (!Handle)
	{
		return;
	}
	aiv_get_string_t SymbolAddressGetString = reinterpret_cast<aiv_get_string_t>(FPlatformProcess::GetDllExport(Handle, TEXT("aiv_get_string")));
	if (!SymbolAddressGetString)
	{
		FPlatformProcess::FreeDllHandle(Handle);
		return;
	}
	aiv_adder_t SymbolAddressAdder = reinterpret_cast<aiv_adder_t>(FPlatformProcess::GetDllExport(Handle, TEXT("aiv_adder")));

	UE_LOG(LogTemp, Warning, TEXT("GetString and Adder: %s %d"), UTF8_TO_TCHAR(SymbolAddressGetString()), SymbolAddressAdder(10, 20));
}

void FBasePlugin_01Module::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	const TSharedRef<FGlobalTabmanager> TabManager = FGlobalTabmanager::Get();
	TabManager->UnregisterNomadTabSpawner(DummyTab);
}

void FBasePlugin_01Module::HelloWorld()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UE_LOG(LogTemp, Warning, TEXT("Hello World from Plugin001"));
}

//Excercise Spawn an actor in the Editor by pressing the SButton using the class specified in the SEditableTextBox
bool FBasePlugin_01Module::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	if (FParse::Command(&Cmd, TEXT("Show Dummy Window")))
	{
		const TSharedRef<FGlobalTabmanager> TabManager = FGlobalTabmanager::Get();
		FTabSpawnerEntry SpawnerEntry = TabManager->RegisterNomadTabSpawner(DummyTab,
			FOnSpawnTab::CreateRaw(this, &FBasePlugin_01Module::SpawnNomadTab));
		TabManager->TryInvokeTab(DummyTab);
		return true;
	}
	return false;
}

TSharedRef<SDockTab> FBasePlugin_01Module::SpawnNomadTab(const FSpawnTabArgs& TabSpawnArgs)
{
	return SNew(SDockTab).TabRole(ETabRole::NomadTab)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot().AutoHeight()
			[
				SNew(SEditableTextBox).Text(FText::FromString("DummyEditableTextBlock"))
				.OnTextCommitted_Raw(this, &FBasePlugin_01Module::TextCommited)
			]
			+SVerticalBox::Slot().AutoHeight()
			[
				SNew(SButton).Text(FText::FromString("DummyButton"))
				.OnClicked_Raw(this, &FBasePlugin_01Module::OnButtonClicked)
			]
	];
}

void FBasePlugin_01Module::TextCommited(const FText& InText, ETextCommit::Type InCommitType)
{
	ClassName = InText;
}

FReply FBasePlugin_01Module::OnButtonClicked()
{
	UClass* FoundClass = FindObject<UClass>(ANY_PACKAGE, *ClassName.ToString());
	UWorld* World = GEditor->GetEditorWorldContext().World();
	World->SpawnActor(FoundClass);
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBasePlugin_01Module, BasePlugin_01)