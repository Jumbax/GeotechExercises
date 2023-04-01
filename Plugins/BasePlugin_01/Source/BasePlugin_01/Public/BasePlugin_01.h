// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class BASEPLUGIN_01_API FBasePlugin_01Module : public IModuleInterface, public FSelfRegisteringExec
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	void HelloWorld();
	TSharedRef<SDockTab> SpawnNomadTab(const FSpawnTabArgs& TabSpawnArgs);
	bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
	void TextCommited(const FText& InText, ETextCommit::Type InCommitType);
	FReply OnButtonClicked();
	FText ClassName;
};
