

#pragma once

#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

//protected:
	// Called when the game starts
	virtual void BeginPlay() override;
    
    void OpenDoor();
    
    void CloseDoor();
    

//public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY(EditAnywhere)
    float OpenAngle = -90.f;
    
    UPROPERTY(EditAnywhere)
    ATriggerVolume* PressurePlate;
    
    UPROPERTY(EditAnywhere)
    float DoorCloseDelay = 1.f;
    
    float LastDoorOpenTime;

    AActor *Owner; // The owning door
    AActor* ActorThatOpens;      // Remember pawns inherit from actor
    
};


