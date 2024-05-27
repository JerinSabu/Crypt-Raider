

#include "Mover.h"

#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	// Store the original location of the actor this component is attached to
	originalLocation = GetOwner()->GetActorLocation();

	
}


// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Determine the target location based on whether the actor should move
	FVector targetLocation = originalLocation;
	if(shouldMove)
	{
		// Calculate the new target location by adding the move offset to the original location
		targetLocation = originalLocation + moveOffset;

	}

	// Get the current location of the actor
	FVector currentLocation = GetOwner()->GetActorLocation();

	// Calculate the speed of movement	
	float speed = moveOffset.Length()/ moveTime;

	// Interpolate the actor's location towards the target location at a constant speed
	FVector newLocation = FMath::VInterpConstantTo(currentLocation,targetLocation, DeltaTime, speed);

	// Update the actor's location
	GetOwner()->SetActorLocation(newLocation);
	
	

	// ...
}

	// Set whether the actor should move
	void UMover::SetShouldMove(bool newShouldMove)
	{
		shouldMove = newShouldMove;
	}
