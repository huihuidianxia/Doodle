#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "DoodleCommandlet.generated.h"

UCLASS()
class DOODLEEDITOR_API UDoodleAssCreateCommandlet : public UCommandlet
{
    GENERATED_UCLASS_BODY()

    /** Parsed commandline tokens */
    TArray<FString> CmdLineTokens;

    /** Parsed commandline switches */
    TArray<FString> CmdLineSwitches;

    virtual int32 Main(const FString &Params) override;
};