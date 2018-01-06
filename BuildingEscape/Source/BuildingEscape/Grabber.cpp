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

void UGrabber::FindPhysicsHandleComponent()
{
    // Look for attached physics handle
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if(PhysicsHandle)
    {
        // Physics handle is found
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Physics handle component of %s not found"), *GetOwner()->GetName())
    }
}

void UGrabber::SetupInputComponent()
{
    // Look for input component (only available at run time)
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if(InputComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("Input component found"));
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
        
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("input component of %s not found"), *GetOwner()->GetName())
    }
}

void UGrabber::Grab() {
    UE_LOG(LogTemp, Warning, TEXT("Grab pressed"))
    
    /// LINE TRACE and see if we reach any actors with physics body collision channel set
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent();
    auto ActorHit = HitResult.GetActor();
    
    if(ActorHit) {
    /// If we hit something, then attach a physics handle
    // attach physics handle
    PhysicsHandle->GrabComponentAtLocation(ComponentToGrab,
                                 NAME_None,
                                 ComponentToGrab->GetOwner()->GetActorLocation()
                                 );
    }
}

void UGrabber::Release() {
    UE_LOG(LogTemp, Warning, TEXT("Grab key released"))
    PhysicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT PlayerViewPointLocation,
                                                               OUT PlayerViewPointRotation);
    
    // FVector LineTraceEnd = PlayerViewPointLocation + FVector(0.f, 0.f, 50.f);
    FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);

    
    // if the physics handle is attached,
    if(PhysicsHandle->GrabbedComponent)
    {
    // move the object that we're holding
        PhysicsHandle->SetTargetLocation(LineTraceEnd);
    }
    
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
    // Get player view point this tick
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT PlayerViewPointLocation,
                                                               OUT PlayerViewPointRotation);
    
    // FVector LineTraceEnd = PlayerViewPointLocation + FVector(0.f, 0.f, 50.f);
    FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
    
    // Setup query parameters
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    
    /// Line-trace (AKA Ray-cast) out to reach distance
    FHitResult Hit;
    GetWorld()->LineTraceSingleByObjectType(
                                            OUT Hit,
                                            PlayerViewPointLocation,
                                            LineTraceEnd,
                                            FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                                            TraceParameters
                                            );
    
    
    // See what we hit
    AActor* ActorHit = Hit.GetActor();
    if (ActorHit) {
        UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()))
    }
    
    return Hit;
}


