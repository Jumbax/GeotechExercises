// Copyright Epic Games, Inc. All Rights Reserved.

#include "GeotechExercise.h"
#include "Modules/ModuleManager.h"
#include "Dummy003.h"
#include "BasePlugin_01.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Editor.h"
#include "DummyActor.h"

DEFINE_LOG_CATEGORY(LogGeotech)

bool operator<(const FModuleStatus& ModuleA, const FModuleStatus& ModuleB)
{
	return ModuleA.Name < ModuleB.Name;
}

//Exercise_000: Customizing The Game Module
class FGeotechGameModule : public FDefaultGameModuleImpl, FSelfRegisteringExec
{
	void StartupModule() override
	{
		UE_LOG(LogTemp, Warning, TEXT("Hello from the new Game Module"));

		//Exercise_002: Query the known modules
		TArray<FModuleStatus> Modules;
		FModuleManager::Get().QueryModules(Modules);
		Modules.Sort();
		for (FModuleStatus& Module : Modules)
		{
			UE_LOG(LogTemp, Warning, TEXT("Module Name: %s %s"), *Module.FilePath, *Module.Name);
		}

		//Exercise_004: Playing with allocators
		//TArray<int, TFixedAllocator<5>> InlineArrayTest;
		TArray<int, TInlineAllocator<5>> InlineArrayTest;
		InlineArrayTest.Add(1);
		InlineArrayTest.Add(1);
		InlineArrayTest.Add(1);
		InlineArrayTest.Add(1);
		InlineArrayTest.Add(1);
		InlineArrayTest.Add(1);

		//Exercise_005: Create a UTF16 string with some weird character and store it in a file
		FString Message = TEXT("Water Sign 水");
		//FFileHelper::SaveStringToFile(Message, TEXT("D:/WaterSignExample.txt"));

		//Exercise_007: Adding a new Module
		FDummy003Module::HelloWorld();

		//Exercise_008: Plugin development
		FBasePlugin_01Module& BasePlugin01 = FModuleManager::LoadModuleChecked<FBasePlugin_01Module>("BasePlugin_01");
		BasePlugin01.HelloWorld();
	}

	void ShutdownModule() override
	{
		UE_LOG(LogTemp, Warning, TEXT("Goodbye from the new Game Module"));
	}

	//Exercise_001: Dynamic reloading of the Game Module
	bool SupportsDynamicReloading() override
	{
		return false;
	}

	
	bool Exec(UWorld* World, const TCHAR* Cmd, FOutputDevice& Ar)
	{
		//Exercise_006: A command for your Module
		if (FParse::Command(&Cmd, TEXT("foobar")))
		{
			TArray<FModuleStatus> Modules;
			FModuleManager::Get().QueryModules(Modules);
			Modules.Sort();
			for (FModuleStatus& Module : Modules)
			{
				if (Module.Name.StartsWith(Cmd))
				{
					UE_LOG(LogTemp, Warning, TEXT("Module Name: %s %s"), *Module.FilePath, *Module.Name);
				}
			}
			return true;
		}

		else if (FParse::Command(&Cmd, TEXT("Geo")))
		{
			if (FParse::Command(&Cmd, TEXT("Iterate")))
			{
				//Exercise_010: Asset Registry
				if (FParse::Command(&Cmd, TEXT("Objects")))
				{
					FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
					TArray<FAssetData> AssetsData;
					AssetRegistryModule.Get().GetAllAssets(AssetsData);
					for (const FAssetData& AssetData : AssetsData)
					{
						UE_LOG(LogTemp, Warning, TEXT("Asset %s: %s %d"), *AssetData.GetFullName(), *AssetData.AssetClass.ToString(), AssetData.IsAssetLoaded());
					}
					return true;
				}

				//Exercise_012: Iterate Worlds
				else if (FParse::Command(&Cmd, TEXT("Worlds")))
				{
					for (TObjectIterator<UWorld> It; It; ++It)
					{
						UWorld* World = *It;
						UE_LOG(LogTemp, Warning, TEXT("Found World %s"), *World->GetFullName());
						if (World->WorldType == EWorldType::Editor)
						{
							//Exercise_014: Actor spawning
							World->SpawnActor<AActor>(FVector::ZeroVector, FRotator::ZeroRotator);
							World->SpawnActor<AActor>(FVector::ZeroVector, FRotator::ZeroRotator);
							World->SpawnActor<AActor>(FVector::ZeroVector, FRotator::ZeroRotator);
							World->SpawnActor<AActor>(FVector::ZeroVector, FRotator::ZeroRotator);
						}
					}
					return true;
				}
			}

			//Exercise_017: Asset inspection from selection
			else if (FParse::Command(&Cmd, TEXT("AssetInspection")))
			{
				TArray<FAssetData> AssetsData;
				GEditor->GetContentBrowserSelections(AssetsData);
				for (const FAssetData& AssetData : AssetsData)
				{
					UObject* Instance = AssetData.GetAsset();
					UClass* AssetClass = Instance->GetClass();
					if (AssetClass->IsChildOf(UBlueprint::StaticClass()))
					{
						AssetClass = Cast<UBlueprint>(Instance)->GeneratedClass;
					}

					for (TFieldIterator<FProperty> It(AssetClass); It; ++It)
					{
						FProperty* Property = *It;
						if (FStrProperty* StrProperty = CastField<FStrProperty>(Property))
						{
							UE_LOG(LogTemp, Warning, TEXT("Asset %s: prop %s"), *Instance->GetFullName(), *Property->GetName());
						}
					}
				}
				return true;
			}

			//Exercise: Spawn BP from Asset
			else if (FParse::Command(&Cmd, TEXT("Spawn")))
			{
				if (FParse::Command(&Cmd, TEXT("BP")))
				{
					TArray<FAssetData>AssetsData;
					GEditor->GetContentBrowserSelections(AssetsData);
					for (const FAssetData& AssetData : AssetsData)
					{
						UObject* Instance = AssetData.GetAsset();
						UClass* AssetClass = Instance->GetClass();
						if (AssetClass->IsChildOf(UBlueprint::StaticClass()))
						{
							AssetClass = Cast<UBlueprint>(Instance)->GeneratedClass;
							if (AssetClass->IsChildOf(AActor::StaticClass()))
							{
								UWorld* World = GEditor->GetEditorWorldContext().World();
								World->SpawnActor(AssetClass);
								World->SpawnActor(AssetClass);
								World->SpawnActor(AssetClass);
								World->SpawnActor(AssetClass);
								UE_LOG(LogTemp, Error, TEXT("I'm a Blueprint Actor son and I'm spawning in the Editor World"));
							}
						}
					}
					return true;
				}

				else if (FParse::Command(&Cmd, TEXT("Actor")))
				{
					TArray<FAssetData>AssetsData;
					GEditor->GetContentBrowserSelections(AssetsData);
					for (const FAssetData& AssetData : AssetsData)
					{
						UObject* Instance = AssetData.GetAsset();
						UClass* AssetClass = Cast<UClass>(Instance);
						if (AssetClass->IsChildOf(AActor::StaticClass()))
						{
							UWorld* World = GEditor->GetEditorWorldContext().World();
							World->SpawnActor(AssetClass);
							World->SpawnActor(AssetClass);
							World->SpawnActor(AssetClass);
							World->SpawnActor(AssetClass);
							UE_LOG(LogTemp, Error, TEXT("I'm an Actor son and I'm spawning in the Editor World"));
						}
					}
					return true;
				}
			}
		}

		return false;
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FGeotechGameModule, GeotechExercise, "GeotechExercise" );
