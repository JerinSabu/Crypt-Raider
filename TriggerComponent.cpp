

#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
	
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Get an acceptable actor that is overlapping this component
    AActor* actor = GetAcceptableActor();
    if(actor != nullptr)
    {
        // If there is an acceptable actor, get its root component and disable physics simulation
        UPrimitiveComponent* component = Cast<UPrimitiveComponent>(actor->GetRootComponent());
        if(component != nullptr)
        {
            component->SetSimulatePhysics(false);

        }

        // Attach the actor to this component
        actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

        // Set the mover to start moving
        mover->SetShouldMove(true);
    }
    else
    {
        // If there is no acceptable actor, set the mover to stop moving
         mover->SetShouldMove(false);
    }
    
    
}

// Set the mover reference
void UTriggerComponent::SetMover(UMover* newMover)
{
    mover = newMover;


}

// Find an acceptable actor that is overlapping this component
AActor* UTriggerComponent::GetAcceptableActor() const
{
    // Get all overlapping actors
    TArray<AActor*> actors;
    GetOverlappingActors(actors);
  
    // Iterate through the overlapping actors to find an acceptable one
    for (AActor* actor : actors)
    {
        // Check if the actor has the acceptable tag and is not currently grabbed
        if(actor->ActorHasTag(acceptableActorTag) && !actor->ActorHasTag("Grabbed"))
        {
            return actor; // Return the first acceptable actor found
        }
        

    }
    return nullptr; // Return nullptr if no acceptable actor is found

}