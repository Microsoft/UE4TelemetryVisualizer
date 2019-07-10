// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.
//--------------------------------------------------------------------------------------
// TelemetryEvent.h
//
// Telemetry actor.  Provides interactive representation of an event
//
// Advanced Technology Group (ATG)
// Copyright (C) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma once

#include "Components/InstancedStaticMeshComponent.h"
#include "Query/TelemetryQuery.h"
#include "TelemetryEvent.generated.h"

struct STelemetryEvent;

//Type of mesh to draw for an event
static enum EventType
{
    Cone,
    Cube,
    Cylinder,
    Plane,
    Sphere
};

//Structure for rendering information
struct InstanceType
{
    UInstancedStaticMeshComponent* component;
    FColor color;
    EventType type;

    InstanceType(UInstancedStaticMeshComponent* component, FColor color, EventType type) : component(component), color(color), type(type) {}
};

UCLASS()
class ATelemetryEvent : public AActor
{
    GENERATED_BODY()

public:
    ATelemetryEvent(const FObjectInitializer& InInitializer);

    //Name and category of event
    UPROPERTY(VisibleAnywhere, Category = "TelemetryEvent", Meta = (ToolTip = "Event"))
    FString eventName;

    //Location of an event
    UPROPERTY(VisibleAnywhere, Category = "TelemetryEvent", Meta = (ToolTip = "Location"))
    FVector location;

    //Orientation of an event
    UPROPERTY(VisibleAnywhere, Category = "TelemetryEvent", Meta = (ToolTip = "Orientation"))
    FVector orientation;

    //Client time of the event
    UPROPERTY(VisibleAnywhere, Category = "TelemetryEvent", Meta = (ToolTip = "Time"))
    FDateTime time;

    //Value for an event
    UPROPERTY(VisibleAnywhere, Category = "TelemetryEvent", Meta = (ToolTip = "Value"))
    TMap<FString, FString> values;

    //Session of the event
    UPROPERTY(VisibleAnywhere, Category = "TelemetryEvent", Meta = (ToolTip = "Session ID"))
    FString session;

    //Build string of the event
    UPROPERTY(VisibleAnywhere, Category = "TelemetryEvent", Meta = (ToolTip = "Build"))
    FString build;

    TArray<InstanceType> InstanceTypes;

    //Populates event values based on an Telemetry event
    void SetEvent(const TSharedPtr<STelemetryEvent> inPointer);

    //Sets the emissive color of the mesh
    void SetColor(FColor inColor);

    //Set the shape of the mesh
    void SetShapeType(EventType inType);

    //Set size of the mesh
    void SetScale(float inScale);

    //Set the value and range
    void AddValue(FString inName, FString value) { values.Add(inName, value); }

    //Return the private name or category
    FString GetName() { return eventName; }
    FString GetCategory() { return category; }

    bool IsEditorOnly() const { return true; };

    //Build the render details of this actor based on event details
    void AddEvent(FVector inLocation, FVector inOrientation, FColor inColor, EventType inType)
    {
        AddEvent(inLocation, inOrientation, inColor, inType, FBox::BuildAABB(FVector::ZeroVector, scale), L"Mesh");
    };

    void AddEvent(FVector inLocation, FVector inOrientation, FColor inColor, EventType inType, float inScale, double ratio)
    {
        AddEvent(inLocation, inOrientation, inColor, inType, FBox::BuildAABB(FVector::ZeroVector, FVector(inScale, inScale, inScale)), ratio);
    };

    void AddEvent(FVector inLocation, FVector inOrientation, FColor inColor, EventType inType, FBox inScale, double ratio)
    {
        AddEvent(inLocation, inOrientation, inColor, inType, inScale, "HeatmapGroup_" + FString::SanitizeFloat(ratio));
    };

    void AddEvent(FVector inLocation, FVector inOrientation, FColor inColor, EventType inType, FBox inScale, FString inName);

private:
//Cached mesh and material instances
    UStaticMesh* MeshCone;
    UStaticMesh* MeshCube;
    UStaticMesh* MeshCylinder;
    UStaticMesh* MeshPlane;
    UStaticMesh* MeshSphere;
    UMaterial* MeshMaterial;

    FVector scale;
    FString name;
    FString category;
};
