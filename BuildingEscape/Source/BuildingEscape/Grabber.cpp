// Copyright Oti Oritsejafor 2017

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
    Super::BeginPlay();
    FindPhysicsHandleComponent();
    SetupInputComponent();
}

/// Look for attached physics handle
void UGrabber::FindPhysicsHandleComponent()
{
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if(PhysicsHandle == NULL)
    {
        UE_LOG(LogTemp, Error, TEXT("Physics handle component of %s not found"), *GetOwner()->GetName())
    }
}

/// Look for the attached input component at run time
void UGrabber::SetupInputComponent()
{
    // Look for input component (only available at run time)
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if(InputComponent)
    {
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
        
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("input component of %s not found"), *GetOwner()->GetName())
    }
}


void UGrabber::Grab() {
    /// LINE TRACE and see if we reach any actors with physics body collision channel set
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent();  // Gets the mesh in our case
    auto ActorHit = HitResult.GetActor();
    
    if(ActorHit) {
    /// If we hit something, then attach a physics handle
    // attach physics handle
    PhysicsHandle->GrabComponentAtLocation(ComponentToGrab,  // the mesh
                                 NAME_None,  // No bones needed
                                 ComponentToGrab->GetOwner()->GetActorLocation()
                                 );
    }
}

void UGrabber::Release() {
    PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    FVector LineTraceEnd = GetReachLineEnd();
    
    // if the physics handle is attached,
    if(PhysicsHandle->GrabbedComponent)
    {
    // move the object that we're holding
        PhysicsHandle->SetTargetLocation(GetReachLineEnd());
    }
    
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
    
    // Setup query parameters
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    
    /// Line-trace (AKA Ray-cast) out to reach distance
    FHitResult HitResult;
    GetWorld()->LineTraceSingleByObjectType(
                                            OUT HitResult,
                                            GetReachLineStart(),
                                            GetReachLineEnd(),
                                            FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                                            TraceParameters
                                            );
    return HitResult;
}

FVector UGrabber::GetReachLineStart() const
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT PlayerViewPointLocation,
                                                               OUT PlayerViewPointRotation);

    return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() const
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT PlayerViewPointLocation,
                                                               OUT PlayerViewPointRotation);
    
    // FVector LineTraceEnd = PlayerViewPointLocation + FVector(0.f, 0.f, 50.f);
    FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
    
    return LineTraceEnd;
}

