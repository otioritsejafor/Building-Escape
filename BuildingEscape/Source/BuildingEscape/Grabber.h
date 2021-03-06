// Copyright Oti Oritsejafor 2017

#pragma once

#include "Components/ActorComponent.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()
private:
    // How far ahead of the player can we reach in cm
    float Reach = 100.f;
    
    UPhysicsHandleComponent* PhysicsHandle = nullptr;
    
    UInputComponent* InputComponent = nullptr;
    
    //Ray cast and grab what is in reach
    void Grab();
    
    // Called when grab is released
    void Release();
    
    // Find attached physics handle
    void FindPhysicsHandleComponent();
    
    // Setup (assumed) attached input component
    void SetupInputComponent();
    
    // Return hit for first physics body in reach
    FHitResult GetFirstPhysicsBodyInReach() const;
    
    // Return current start of reach line
    FVector GetReachLineStart() const;
    
    // Return current end of reach line
    FVector GetReachLineEnd() const;
    
public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	
};
