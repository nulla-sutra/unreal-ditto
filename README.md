# Ditto

Ditto is a runtime Unreal Engine plugin for Combee-backed outfit and equipment state.

The plugin owns the outfit container, slot layout, equip and unequip transactions, and the
handoff from inventory state to avatar presentation. The actual visual implementation is
kept behind outfit part fragments, so a part can be driven by Mutable Customizable Objects,
direct skeletal mesh replacement, or another backend added later.

## Current Scope

Ditto currently provides:

- A replicated outfit container component: `UDittoOutfits`.
- A gameplay-tag based outfit layout: `FDittoOutfitLayout`.
- Item fragments for equip and unequip behavior: `UDittoFragment_OutfitPart`.
- Combee transactions for moving items between closets and outfit slots.
- Avatar callbacks through `IDittoOutfitAvatar`.
- Mutable Customizable Object outfit part implementations.
- Direct skeletal mesh attachment/replacement outfit part implementation.

Ditto does not implement a complete character customization UI, inventory database, or
animation policy. Those remain project-level or plugin-level concerns built on top of the
container and fragment flow.

## Dependencies

The plugin descriptor enables:

- [unreal-combee](https://github.com/nulla-sutra/unreal-combee)
- [UE5Coro](https://github.com/landelare/ue5coro)
- `Mutable`

The module also depends on Combee use, presets, and transaction modules, plus Unreal
`GameplayTags`, `MutableRuntime`, and `CustomizableObject`.

## Core Concepts

### Outfit Container

`UDittoOutfits` is a `UCombeeContainer` subclass that represents the currently equipped
outfit slots for an owner.

It stores:

- `Avatar`: the actor that receives visual outfit updates.
- `Layout`: the registered outfit slots and per-slot presentation data.

The component validates compatible items through `UDittoFragment_OutfitPart`. An item is
compatible with a slot when the item's `Part` tags include the registered slot tags.

### Outfit Layout

`FDittoOutfitLayout` is registered at runtime by calling:

```cpp
OutfitComponent->RegisterPart(PartTags, PartData);
```

Each registered part contains:

- `Part`: gameplay tags that describe the slot.
- `ContainerIndex`: the matching cell index in `UDittoOutfits`.
- `PartData`: an `FInstancedStruct` consumed by the outfit part implementation.

`PartData` is intentionally backend-specific. Mutable parts use
`FDittoOutfitPartData_Mutable`; attach parts use `FDittoOutfitPartData_Attach`.

### Outfit Avatar

An avatar actor can implement `IDittoOutfitAvatar` to configure and optionally intercept
outfit updates.

Important callbacks:

- `SetupOutfitAvatar`: called on authority when an avatar is assigned. Register layout
  slots here.
- `ReceiveOutfitUpdate`: called on authority after a cell changes. Return true to fully
  handle the update yourself.
- `ReceiveOutfitUpdateDeferred`: called from container mutation flow on server and client.
  Return true to fully handle the visual update yourself.

If the avatar callbacks return false, `UDittoOutfits` falls back to the fragment's
`Wear` and `TakeOff` implementations.

### Outfit Part Fragment

`UDittoFragment_OutfitPart` is the base fragment placed on an equippable Combee item.

It contains:

- `Part`: gameplay tags describing what slots the item can occupy.
- `Closets`: container component classes that can receive the item when it is unequipped.
- `Wear`: presentation hook for applying the item.
- `TakeOff`: presentation hook for removing the item.

`OnUse` decides whether the item is currently in an outfit container or another container:

- If used from `UDittoOutfits`, it requests `UDittoTransaction_Unequip`.
- If used from another container, it requests `UDittoTransaction_Equip`.

## Equip Flow

1. A player uses an item that owns a `UDittoFragment_OutfitPart`.
2. `OnUse` builds an equip or unequip payload.
3. `UCombeeTransactionSubsystem` runs the transaction.
4. `UDittoTransaction_Equip` swaps the item into the matching outfit slot.
5. `UDittoTransaction_Unequip` moves the item back into a configured closet container.
6. `UDittoOutfits` receives the cell/container mutation.
7. The avatar interface gets a chance to handle the update.
8. If not handled, Ditto calls the outfit part fragment's `Wear` or `TakeOff`.

`TakeOff` is multicast by fragment class because the previous item instance may not exist
on every client. Implementations should use the supplied `PartData` and class default
values instead of relying on per-instance runtime state.

## Built-In Outfit Part Backends

### Mutable Customizable Object

Files:

- `Outfit/Mutable/DittoFragment_OutfitPart_Mutable_CO.*`
- `Outfit/Mutable/DittoFragment_OutfitPart_Mutable_COI.*`
- `Outfit/Mutable/DittoOutfitPartData_Mutable.h`

Use this backend for body parts that should participate in a Mutable Customizable Object
graph, such as body, head, clothing, armor, hands, or feet.

`FDittoOutfitPartData_Mutable` supplies:

- `CustomizableComponent`
- `MeshComponent`
- `PartName`
- `DefaultLink`

`UDittoFragment_OutfitPart_Mutable_CO` selects the fragment's `ObjectName` for the Mutable
integer parameter named by `PartName`, updates the skeletal mesh asynchronously, and can
link an animation layer override. Taking the part off resets the Mutable parameter to its
default value and relinks `DefaultLink`.

`UDittoFragment_OutfitPart_Mutable_COI` follows the same parameter flow, but also copies
parameters from a configured `UCustomizableObjectInstance`.

Mutable parts must be compatible with the skeleton merge rules of Mutable. If a part owns
an independent skeleton with conflicting bone parents, prefer the attach backend instead
of forcing it into the Customizable Object graph.

### Attach Skeletal Mesh

Files:

- `Outfit/Attach/DittoFragment_OutfitPart_Attach.*`
- `Outfit/Attach/DittoOutfitPartData_Attach.h`

Use this backend for equipment that should not be merged by Mutable, such as weapons,
props, independent skeletal accessories, or socket-driven pieces.

`FDittoOutfitPartData_Attach` supplies:

- `MeshComponent`
- `DefaultMesh`
- `PartName`

The current implementation uses `MeshComponent` and `DefaultMesh`. Wearing a part sets the
component's skeletal mesh to the fragment's `Mesh` and optionally applies `AnimOverride`.
Taking it off restores `DefaultMesh`.

This backend is the safer choice for weapons or props with their own skeleton hierarchy.

## Recommended Setup

1. Add `UDittoOutfits` to the actor that owns the equipment state.
2. Assign an avatar actor with `SetAvatar`.
3. Implement `IDittoOutfitAvatar` on the avatar.
4. In `SetupOutfitAvatar`, call `RegisterPart` for each slot.
5. Build each slot's `PartData` with the backend data struct needed by that slot.
6. Put a `UDittoFragment_OutfitPart` subclass on each equippable item.
7. Set the fragment's `Part` tags so the item can match the registered slot.
8. Configure `Closets` so unequipped items have a valid destination container.

Example slot registration shape:

```cpp
const auto PartData = FInstancedStruct::Make(FDittoOutfitPartData_Attach{
    .MeshComponent = WeaponMeshComponent,
    .DefaultMesh = DefaultWeaponMesh,
    .PartName = TEXT("Weapon"),
});

OutfitComponent->RegisterPart(WeaponPartTags, PartData);
```

## Backend Selection Guide

Use Mutable CO or COI when:

- The part is a body/clothing module.
- It should affect the generated character mesh.
- It uses the same skeleton hierarchy expected by the Mutable graph.
- It needs Mutable parameters, material variants, or body-part composition.

Use Attach when:

- The part is a weapon, prop, or independent accessory.
- The part has its own skeletal hierarchy.
- The part should be attached to a socket or controlled by a dedicated component.
- Mutable skeleton merging reports conflicting bone parents.

## Replication Notes

- `UDittoOutfits::Layout` and `Avatar` are replicated.
- Layout changes are authority-side operations.
- The default avatar update path runs through Combee mutation events.
- `TakeOff` uses a reliable multicast helper because previous item instances may not be
  available on clients.

## Extension Points

New presentation backends should subclass `UDittoFragment_OutfitPart` and define a matching
`FInstancedStruct` data type for slot-specific runtime references.

Good candidates for future backends:

- Static mesh socket attachments.
- Niagara or effect-only outfit parts.
- Multiple component swaps from one outfit item.
- Gameplay ability or attribute driven outfit parts.

Keep new backend logic self-contained in its own folder under `Outfit/` and keep
cross-plugin dependencies explicit in the plugin descriptor and module rules.
