// Copyright Epic Games, Inc. All Rights Reserved.


#include "Variant_Shooter/AI/EnvQueryContext_Target.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "ShooterAIController.h"

void UEnvQueryContext_Target::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	// get the controller from the query instance
	if (AShooterAIController* Controller = Cast<AShooterAIController>(QueryInstance.Owner))
	{
		// ensure the target is valid
		if (IsValid(Controller->GetCurrentTarget()))
		{
			// add the controller's target actor to the context
			UEnvQueryItemType_Actor::SetContextHelper(ContextData, Controller->GetCurrentTarget());
		}
	}

}
