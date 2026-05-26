# 🎮 Ultimate Sports Game - Player Model System Documentation

## Player Model & Character System

Complete player character system with realistic models, customization, and gameplay mechanics.

---

## 📋 Core Components

### **1. PlayerCharacter.h** - In-Game Player Actor

Player character with full gameplay integration.

#### Player States:
```cpp
enum EPlayerState {
    Idle,           // Standing still
    Running,        // Normal speed movement
    Sprinting,      // Maximum speed (drains stamina)
    Jumping,        // In the air
    Shooting,       // Shooting/Kicking
    Passing,        // Passing to teammate
    Defending,      // Defending against opponent
    Tackling,       // Aggressive tackle
    Injured,        // Injured/Slow
    Tired           // Low stamina
};
```

#### Key Functions:

**Initialization:**
```cpp
InitializePlayer(FPlayerData PlayerData)      // Create from data
LoadPlayerModel()                             // Load skeletal mesh
ApplyPlayerAppearance()                       // Apply colors/materials
```

**Movement:**
```cpp
Run(FVector Direction)                        // Run at RunSpeed (600 units/sec)
Sprint(FVector Direction)                     // Sprint at SprintSpeed (1000 units/sec)
Jump()                                        // Jump with height based on Agility stat
StopMovement()                                // Stop moving
```

**Actions:**
```cpp
Shoot(FVector TargetLocation)                 // Shoot/kick toward target
PassBall(AActor* Teammate)                    // Pass to teammate
Defend(AActor* Opponent)                      // Defensive stance
Tackle(AActor* Opponent)                      // Aggressive tackle
ReceiveBall()                                 // Catch incoming ball
```

**Stats Management:**
```cpp
GetCurrentStamina()                           // Stamina 0-100
DrainStamina(float Amount)                    // Deplete stamina
RestoreStamina(float Amount)                  // Restore stamina
GetPlayerHealth()                             // Health 0-100
TakeDamage(float Damage)                      // Take injury damage
GetPlayerStats()                              // FPlayerStats struct
```

**Animations:**
```cpp
PlayShootingAnimation()                       // Play shoot animation
PlayPassingAnimation()                        // Play pass animation
PlayDefendingAnimation()                      // Play defend animation
PlayInjuryAnimation()                         // Play injury animation
```

#### Stamina System:
- **Sprint depletes** at `StaminaDrainRate` (0.5 per frame)
- **Running depletes slower** at 0.25 per frame
- **Standing restores** at `StaminaRestoreRate` (0.3 per frame)
- **Below 50% stamina** → Player automatically switches to running
- **Below 20% stamina** → Player state = Tired (reduced speed)

#### Health System:
- Players start with 100 HP
- Taking tackles/collisions causes damage
- Injuries accumulate during game
- Low health reduces speed and agility
- Game can end if player is KO'd (0 HP)

---

### **2. PlayerModelManager.h** - Character Customization

Static utility class for loading, creating, and customizing player models.

#### Model Loading:
```cpp
// Load assets
LoadPlayerMesh(const FString& MeshPath)       // Load skeletal mesh
LoadPlayerMaterial(const FString& MaterialPath) // Load material

// Create complete player
CreatePlayerCharacter(PlayerData, World)      // Spawn in world with full setup
```

#### Appearance Customization:
```cpp
// Colors & Materials
ApplyTeamColors(Player, PrimaryColor, SecondaryColor)
ApplyMaterial(Player, Material)
SetJerseyNumber(Player, 23)

// Morphing & Scale
ApplyFaceMorphTargets(Player, HeadMorphPath, 0.75f) // Blend between faces
SetBodyTypeFromStats(Player, Stats)                 // Build based on stats
CustomizePlayerScale(Player, Height, Weight)       // Physical dimensions
```

**Body Type Logic:**
- **Low Weight (< 75kg)**: Lean build (guard, receiver)
- **Medium Weight (75-95kg)**: Athletic build (midfield, wing)
- **High Weight (> 95kg)**: Muscular build (tackles, centers)
- **Height Factor**: Affects jump height, reach, and standing animations

#### Animation Setup:
```cpp
// Animation blueprints
SetupAnimationBlueprint(Player, AnimBlueprintPath)
SetupMovementAnimations(Player)                   // Walk/Run/Sprint
SetupPositionAnimations(Player, Position)        // Position-specific

// Position-specific animations:
// - Goalkeeper: Diving, jumping saves
// - Forward: Striking animations
// - Defender: Blocking, sliding tackles
// - Pitcher: Wind-up animations
```

#### Uniform & Gear:
```cpp
ApplyUniform(Player, Team)                    // Apply team uniform
AddEquipment(Player, Position)                // Add position-specific gear
ApplyPositionGear(Player, Position)           // Helmets, gloves, etc.
```

**Equipment by Position:**
- **Goalkeeper**: Gloves, padding
- **Defender**: Shin guards, compression
- **Midfielder**: Socks, wrist bands
- **Forward**: Cleats, sleeves
- **Pitcher**: Glove, cap
- **Catcher**: Helmet, chest protector, shin guards
- **Defenseman**: Helmet, elbow pads
- **Quarterback**: Helmet, shoulder pads

#### Batch Creation:
```cpp
// Create entire roster
CreateTeam(Team, World)                       // All players
CreateTeamStarters(Team, World)               // 11/9/5 starters
CreateBench(Team, World)                      // Substitute players
```

---

## 📊 Player Data Flow

```
FPlayerData
├─ Identity
│  ├─ First/Last Name
│  ├─ Jersey Number
│  └─ Nationality
│
├─ Physical
│  ├─ Height (cm)
│  ├─ Weight (kg)
│  └─ Position
│
├─ Stats (1-100)
│  ├─ Speed
│  ├─ Strength
│  ├─ Stamina
│  ├─ Agility
│  ├─ Technical (Passing, Shooting, etc.)
│  └─ Mental (Leadership, Consistency)
│
└─ Appearance
   ├─ Skeletal Mesh Path
   ├─ Material Path
   ├─ Jersey Colors
   └─ Morph Target Data

         ↓

APlayerCharacter (In-Game)
├─ Model & Mesh
├─ Material & Colors
├─ Position-Specific Gear
├─ Animations (Based on position)
├─ Movement (Speed = PlayerStats.Speed)
├─ Stamina (Capacity = PlayerStats.Stamina)
└─ Actions (Accuracy = PlayerStats.Shooting, etc.)
```

---

## 🎯 Implementation Example

```cpp
// Create player from data
FPlayerData PlayerData;
PlayerData.FirstName = "Luka";
PlayerData.LastName = "Doncic";
PlayerData.JerseyNumber = 77;
PlayerData.Position = EPlayerPosition::Forward;
PlayerData.Stats.Speed = 85;
PlayerData.Stats.Strength = 80;
PlayerData.Stats.Shooting = 88;
PlayerData.SkeletalMeshPath = "/Game/Characters/NBA/Doncic/Mesh";
PlayerData.MaterialPath = "/Game/Characters/Materials/NBA_Jersey";
PlayerData.JerseyPrimaryColor = FLinearColor::Blue;
PlayerData.JerseySecondaryColor = FLinearColor::White;

// Spawn player
APlayerCharacter* Player = FPlayerModelManager::CreatePlayerCharacter(PlayerData, GetWorld());

// Apply team colors
FPlayerModelManager::ApplyTeamColors(Player, 
    FLinearColor::Blue,      // Dallas Mavericks primary
    FLinearColor::White);    // Secondary

// Apply uniform
FTeamData DallasTeam = /* ... */;
FPlayerModelManager::ApplyUniform(Player, DallasTeam);

// Setup position-specific gear and animations
FPlayerModelManager::ApplyPositionGear(Player, EPlayerPosition::Forward);
FPlayerModelManager::SetupPositionAnimations(Player, EPlayerPosition::Forward);

// Player is now ready for gameplay!
// In-game stats affect gameplay:
// - Speed affects movement speed
// - Shooting affects accuracy
// - Stamina affects endurance
```

---

## 🎨 Character Customization Pipeline

### Step 1: Load Base Model
```cpp
USkeletalMesh* BaseMesh = FPlayerModelManager::LoadPlayerMesh(
    "/Game/Characters/BasePlayer/Mesh"
);
```

### Step 2: Apply Morph Targets (Facial Features)
```cpp
FPlayerModelManager::ApplyFaceMorphTargets(
    Player,
    "/Game/Characters/FaceMorphs/PlayerFace_Data",
    0.75f  // 0-1 blend factor
);
```

### Step 3: Set Body Scale (Height/Weight)
```cpp
FPlayerModelManager::CustomizePlayerScale(
    Player,
    190.0f,  // Height in cm
    95.0f    // Weight in kg
);
```

### Step 4: Apply Material & Colors
```cpp
UMaterial* JerseyMaterial = FPlayerModelManager::LoadPlayerMaterial(
    "/Game/Characters/Materials/Jersey_Base"
);
FPlayerModelManager::ApplyMaterial(Player, JerseyMaterial);
FPlayerModelManager::ApplyTeamColors(
    Player,
    FLinearColor::Red,    // Primary color
    FLinearColor::White   // Secondary color
);
```

### Step 5: Add Equipment
```cpp
FPlayerModelManager::ApplyPositionGear(Player, EPlayerPosition::Forward);
```

### Result: 
- ✅ Unique player model with customized appearance
- ✅ Team colors applied
- ✅ Position-specific gear
- ✅ Ready for gameplay

---

## 📁 Asset Structure

```
Content/
├── Characters/
│   ├── BaseModels/
│   │   └── PlayerBase_Mesh (Skeletal Mesh)
│   │
│   ├── FaceMorphs/
│   │   ├── PlayerFace_01
│   │   ├── PlayerFace_02
│   │   └── PlayerFace_Data (Morph targets)
│   │
│   ├── Materials/
│   │   ├── Jersey_Base (Dynamic material)
│   │   ├── SkinTone_Base
│   │   └── Equipment_Base
│   │
│   ├── Animations/
│   │   ├── Idle_Base
│   │   ├── Run_Base
│   │   ├── Sprint_Base
│   │   ├── Shoot_Base
│   │   ├── Pass_Base
│   │   └── Position/
│   │       ├── Forward_Animations
│   │       ├── Defender_Animations
│   │       └── Goalkeeper_Animations
│   │
│   └── Equipment/
│       ├── Gloves_Base
│       ├── Helmet_Base
│       ├── ShinGuards_Base
│       └── Cleats_Base
│
└── Teams/
    ├── NBA/
    │   ├── Lakers_Jerseys
    │   └── Warriors_Jerseys
    └── FIFA/
        ├── ManCity_Jerseys
        └── RealMadrid_Jerseys
```

---

## 🚀 Performance Optimization

### Asset Caching:
- Meshes cached after first load
- Materials reused across players (GPU instancing)
- Animations streamed on demand
- LOD (Level of Detail) for crowd players

### Batching:
- Team creation uses pooling
- Bench players simplified models
- Inactive players use lower LOD

### Memory:
- ~2MB per unique player model
- ~5MB for full team (including variations)
- ~50MB total for all sport player assets

---

## 🎮 Gameplay Integration

### Stat-to-Gameplay Conversion:
```
Speed (50-99) → Movement Speed (300-1000 units/sec)
Strength (50-99) → Tackle Power & Jump Height
Stamina (50-99) → Stamina Capacity (50-150 points)
Agility (50-99) → Turn Speed & Animation Speed
Shooting (50-99) → Accuracy & Power (0-100%)
Passing (50-99) → Accuracy & Range (0-100%)
Defense (50-99) → Defensive Power & Reach
```

### Player Performance:
- Fatigue reduces speed/accuracy over time
- Injuries reduce effectiveness
- Formation affects player positioning
- AI uses stats for decision-making

---

**Complete player model system ready for all 9 sports with customization and gameplay!** 🎮

