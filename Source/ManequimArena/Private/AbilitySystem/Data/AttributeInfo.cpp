// Copyright to Bue.


#include "AbilitySystem/Data/AttributeInfo.h"

FManequimAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FManequimAttributeInfo info : AttributeInformation)
	{
		if (info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return info;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FManequimAttributeInfo();
}
