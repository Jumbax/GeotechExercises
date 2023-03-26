// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyActor.h"

// Sets default values
ADummyActor::ADummyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Exercise_016: Adding Components
	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>("Scene");
	RootComponent = SceneComponent;

	USceneComponent* AnotherComponent = CreateDefaultSubobject<USceneComponent>("AnotherScene");
	AnotherComponent->SetupAttachment(SceneComponent);
}

// Called when the game starts or when spawned
void ADummyActor::BeginPlay()
{
	Super::BeginPlay();
	//Exercise_016: Adding Components
	USceneComponent* SecondComponent = NewObject<USceneComponent>(this);
	SecondComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SecondComponent->RegisterComponent();
	AddInstanceComponent(SecondComponent);
}

// Called every frame
void ADummyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

