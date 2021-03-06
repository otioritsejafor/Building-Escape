// Copyright Oti Oritsejafor 2017

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
    if(!PressurePlate)
    {
        UE_LOG(LogTemp, Error, TEXT("%s Missing Pressure Plate component"), *GetOwner()->GetName())
    }
	Super::BeginPlay();
	// Find owning actor
    Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
    // Create a rotator
    FRotator NewRotation = FRotator(0.f, OpenAngle, 0.f);
    
    // Set the door rotation
    Owner->SetActorRotation(NewRotation);
}

void UOpenDoor::CloseDoor()
{
    Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume
    if(GetTotalMassOfActorsOnPlate() > 30.f) // TODO make into a parameter
    {
        OpenDoor();
        LastDoorOpenTime = GetWorld()->GetTimeSeconds();    // This gets when the door was open
    }
    
   // Check if it's time to close the door
    if(GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
    {
        CloseDoor();
    }
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
    float TotalMass = 0.f;
    
    // Find all the overlapping actors
    TArray<AActor*> OverlappingActors;
    if(!PressurePlate) { return TotalMass; }
    PressurePlate->GetOverlappingActors(OUT OverlappingActors);
    // Iterate through them adding their masses
    
    for(auto& Actor: OverlappingActors)
    {
        TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
        UE_LOG(LogTemp, Warning, TEXT("%s on pressure pad"), *Actor->GetName())

    }
    
    return TotalMass;
}

