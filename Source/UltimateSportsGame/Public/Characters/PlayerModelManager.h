#pragma once

#include "CoreMinimal.h"
#include "Data/PlayerData.h"
#include "Data/TeamData.h"
#include "PlayerCharacter.h"

/**
 * Static player model management utility
 * Handles loading, creating, and customizing player models
 */
class FPlayerModelManager
{
public:
	// ============ ASSET LOADING ============

	/** Load skeletal mesh for player */
	static USkeletalMesh* LoadPlayerMesh(const FString& MeshPath);

	/** Load material for player */
	static UMaterial* LoadPlayerMaterial(const FString& MaterialPath);

	// ============ CHARACTER CREATION ============

	/** Create and spawn player character in world */
	static APlayerCharacter* CreatePlayerCharacter(
		const FPlayerData& PlayerData,
		UWorld* World,
		const FVector& SpawnLocation = FVector::ZeroVector,
		const FRotator& SpawnRotation = FRotator::ZeroRotator
	);

	/** Create entire team */
	static TArray<APlayerCharacter*> CreateTeam(
		const FTeamData& Team,
		UWorld* World,
		bool bSpawnAll = false
	);

	/** Create team starters only */
	static TArray<APlayerCharacter*> CreateTeamStarters(
		const FTeamData& Team,
		UWorld* World
	);

	/** Create bench players */
	static TArray<APlayerCharacter*> CreateBench(
		const FTeamData& Team,
		UWorld* World
	);

	// ============ CUSTOMIZATION ============

	/** Apply team colors to player */
	static void ApplyTeamColors(
		APlayerCharacter* Player,
		const FLinearColor& PrimaryColor,
		const FLinearColor& SecondaryColor
	);

	/** Apply material to player */
	static void ApplyMaterial(
		APlayerCharacter* Player,
		UMaterial* Material
	);

	/** Apply face morph targets for unique appearance */
	static void ApplyFaceMorphTargets(
		APlayerCharacter* Player,
		const FString& MorphTargetPath,
		float BlendFactor = 0.5f
	);

	/** Set jersey number */
	static void SetJerseyNumber(
		APlayerCharacter* Player,
		int32 Number
	);

	/** Customize player body based on stats */
	static void SetBodyTypeFromStats(
		APlayerCharacter* Player,
		const FPlayerStats& Stats
	);

	/** Customize player scale (height/weight) */
	static void CustomizePlayerScale(
		APlayerCharacter* Player,
		float Height,
		float Weight
	);

	// ============ UNIFORM & EQUIPMENT ============

	/** Apply team uniform */
	static void ApplyUniform(
		APlayerCharacter* Player,
		const FTeamData& Team
	);

	/** Add position-specific equipment */
	static void ApplyPositionGear(
		APlayerCharacter* Player,
		EPlayerPosition Position
	);

	/** Add generic equipment */
	static void AddEquipment(
		APlayerCharacter* Player,
		const FString& EquipmentType
	);

	// ============ ANIMATIONS ============

	/** Setup animation blueprint for player */
	static void SetupAnimationBlueprint(
		APlayerCharacter* Player,
		const FString& AnimBlueprintPath
	);

	/** Setup position-specific animations */
	static void SetupPositionAnimations(
		APlayerCharacter* Player,
		EPlayerPosition Position
	);

	/** Setup movement animations (idle, run, sprint) */
	static void SetupMovementAnimations(
		APlayerCharacter* Player
	);

	// ============ STATS ============

	/** Get stat-based movement speed */
	static float GetSpeedFromStat(float SpeedStat);

	/** Get stat-based jump height */
	static float GetJumpHeightFromStat(float AgilityStat);

	/** Get stat-based stamina capacity */
	static float GetStaminaCapacityFromStat(float StaminaStat);

	// ============ CACHING & OPTIMIZATION ============

	/** Preload assets for team */
	static void PreloadTeamAssets(const FTeamData& Team);

	/** Clear asset cache */
	static void ClearCache();

	/** Get cached mesh */
	static USkeletalMesh* GetCachedMesh(const FString& MeshPath);

private:
	// Asset cache
	static TMap<FString, USkeletalMesh*> MeshCache;
	static TMap<FString, UMaterial*> MaterialCache;

	// Equipment paths by position
	static void GetEquipmentPaths(
		EPlayerPosition Position,
		TArray<FString>& OutEquipmentPaths
	);

	// Stat conversion
	static float ConvertStatToGameplay(float Stat, float MinValue, float MaxValue);
};
