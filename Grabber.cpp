
#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UGrabber::UGrabber()
{

	PrimaryComponentTick.bCanEverTick = true;

	
}


void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	
	
}


void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get the physics handle component
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();

	// If the physics handle is valid and has a grabbed component
	if(physicsHandle && physicsHandle->GetGrabbedComponent())
	{
		// Calculate the target location for the grabbed component
		FVector targetLocation = GetComponentLocation() + GetForwardVector() * holdDistance;

		// Update the location and rotation of the grabbed component
		physicsHandle->SetTargetLocationAndRotation(targetLocation, GetComponentRotation());
	}
	


}

// Attempt to grab a physics object within reach
void UGrabber::Grab()
{
	// Get the physics handle component
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
	if(physicsHandle == nullptr)
	{
		return;

	} 

	// Get the component and actor that was hit
	FHitResult hitResult;
	bool hasHit = GetGrabbableInReach(hitResult);
	if(hasHit)
	{
		UPrimitiveComponent* hitComponent = hitResult.GetComponent();
		hitComponent->SetSimulatePhysics(true);
		hitComponent->WakeAllRigidBodies();
		AActor* hitActor =hitResult.GetActor();

		// Tag the actor as grabbed and detach it from any parent
		hitActor->Tags.Add("Grabbed");
		hitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		// Grab the component at the impact point with the current rotation
		physicsHandle->GrabComponentAtLocationWithRotation(
			hitComponent,
			NAME_None,
			hitResult.ImpactPoint,
			GetComponentRotation()
		);
	}
	
}

// Release the currently grabbed object
void UGrabber::Release()
{
	// Get the physics handle component
	UPhysicsHandleComponent* physicHandle = GetPhysicsHandle();

	// If the physics handle is valid and has a grabbed component
	if (physicHandle && physicHandle->GetGrabbedComponent())
	{
		// Get the actor of the grabbed component
		AActor* grabbedActor = physicHandle->GetGrabbedComponent()->GetOwner();

		// Remove the grabbed tag and release the component
		grabbedActor->Tags.Remove("Grabbed");
		physicHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		physicHandle->ReleaseComponent();
	}
	
}

// Get the physics handle component from the owner
UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(result == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No physics Handler  Found"));

	}
	return result;

}
	
// Check if there is a grabbable object within reach
bool UGrabber::GetGrabbableInReach(FHitResult& outHitResult) const
{
	 // Define the start and end points for the sweep
	FVector start = GetComponentLocation();
	FVector end = start + GetForwardVector() * maxGrabDistance;

	// Define a collision sphere for the sweep
	FCollisionShape sphere = FCollisionShape::MakeSphere(grabRadius);
	// Perform a sweep to check for a grabbable object within reach
	return GetWorld()->SweepSingleByChannel(
		outHitResult,
		start, end,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		sphere);
	
	

}