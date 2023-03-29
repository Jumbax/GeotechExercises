// Fill out your copyright notice in the Description page of Project Settings.

//Final Exercise: Build your importer for you custom UObject type

#include "DummyFactory.h"
#include "DummyObject.h"

UDummyFactory::UDummyFactory()
{
    bEditorImport = true;
    SupportedClass = UDummyObject::StaticClass();
    Formats.Add("foo;Foo file format");
}

UObject* UDummyFactory::FactoryCreateFile
(
    UClass* InClass,
    UObject* InParent,
    FName InName,
    EObjectFlags Flags,
    const FString& Filename,
    const TCHAR* Parms,
    FFeedbackContext* Warn,
    bool& bOutOperationCanceled
)
{
    UE_LOG(LogTemp, Error, TEXT("Filename: %s"), *Filename);
    UDummyObject* Object = NewObject<UDummyObject>(InParent, InName, Flags);
    return Object;
}