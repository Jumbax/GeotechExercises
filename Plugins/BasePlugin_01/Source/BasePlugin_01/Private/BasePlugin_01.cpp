// Copyright Epic Games, Inc. All Rights Reserved.

#include "BasePlugin_01.h"

#define LOCTEXT_NAMESPACE "FBasePlugin_01Module"

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
}

void FBasePlugin_01Module::HelloWorld()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	UE_LOG(LogTemp, Warning, TEXT("Hello World from Plugin001"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBasePlugin_01Module, BasePlugin_01)