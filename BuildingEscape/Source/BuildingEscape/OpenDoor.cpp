// Copyright Oti Oritsejafor 2017

#include "BuildingEscape.h"
#include "OpenDoor.h"


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
	Super::BeginPlay();
    
	// Find owning actor
    Owner = GetOwner();
    
    ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
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
    // If the actor that opens is in the volume
    if(PressurePlate->IsOverlappingActor(ActorThatOpens))
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

