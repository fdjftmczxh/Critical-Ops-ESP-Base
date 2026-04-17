# Critical Ops ESP Base

[![C++](https://img.shields.io/badge/C++-blue.svg)](https://en.cppreference.com/w/cpp/)
[![Android](https://img.shields.io/badge/Platform-Android-green.svg)](https://developer.android.com/)
[![Target](https://img.shields.io/badge/Version-1.60.0.f3149-red.svg)](#)
[![Arch](https://img.shields.io/badge/Arch-x86__64-orange.svg)](#)

**Onyx** is a basic base project for **Critical Ops**. It provides a simple foundation for creating ESP and understanding IL2CPP structures in a heavily obfuscated environment.

---

## Target Specification

This project is specifically configured for:
* **Game Version:** 1.60.0.f3149
* **Architecture:** Android x86_64

**Note:** Using this on different versions or ARM-based architectures (armeabi-v7a/arm64-v8a) will result in crashes.

---

## Project Status

This is a learning base, not a finished product. It is currently missing many features (such as names, team identification, and proper health bars) because of the game's obfuscation. The goal is to provide a clean starting point that is easy to understand and modify.

---

## Basic Features

The project currently includes a simple Skeleton ESP setup:
* **Basic Bone Rendering**: Projects 3D world coordinates of bones onto the 2D screen.
* **Root Markers**: Simple circle markers drawn at the player's base (feet) position.
* **Overlay Rendering**: Uses a transparent ImGui background for drawing lines and markers.

---

![Screenshot](png/Screenshot%202026-04-15%20165955.png)

---

## External Implementation Logic

If you want to transition this base to a fully external, memory-reading architecture, you will need to manually traverse Unity’s internal structures to obtain elements such as the ViewMatrix, the static instance of the GameplayModule, and other relevant components.

Here is a working example on how you woul extract the vp matrix from the obfuscated `CameraSystem`:

```cpp
// 1. Read Camera object instance from CameraSystem
const auto unity_camera = Memory::read_safe<uintptr_t>(camera_system_instance + 0xa8);
if (!Memory::is_valid_ptr(unity_camera)) return;

// 2. Read cachedPtr (internal Unity pointer)
const auto cached_ptr = Memory::read_safe<uintptr_t>(unity_camera + 0x10);
if (!Memory::is_valid_ptr(cached_ptr)) return;

// 3. Read the matrix directly from the internal unity struct
auto view_matrix = Memory::read_safe<il2cpp::Matrix4x4>(cached_ptr + 0xF0);
```

---

## Reverse Engineering Reference

To overcome the obfuscation barrier, the core C# class structures required for ESP components are outlined below. The primary fields have been identified; however, resolving the remaining unknown offsets will require further independent analysis. A recommended approach is to cross-reference these classes with earlier, unobfuscated versions of the game (for example, version 1.30.0.f1670 or other newer builds that were not obfuscated).

For those who require a complete x86_64 dump, it is available in my GitHub repository:  
https://github.com/fdjftmczxh/Critical-Ops-Dumps

### C# Classes
```csharp

// GameplayModule Class
// Namespace: O2498FCB79
// Token: 0x20000ed
// TypeInfo Offset: 0x2f8d930
public class OECFB7135E : Object
	// Fields
	private const System.Single OE5C974128; // 0x0
	private O2498FCB79.OACC1C6383 O95B3F2CE0; // 0x10
	private O2498FCB79.O1CCD2E2C6 O7BC949864; // 0x18
	private O2498FCB79.O0EE32537F O34B4589C9; // 0x20
	private O0ABBAEBD6.O90551D01D O9357416DF; // 0x28
	private O2498FCB79.O6F692BC0C OE92D655B7; // 0x30 <--- GameSystem
	private O2498FCB79.O48D52ECEB OC751CCC01; // 0x38 <--- CameraSystem
	private O2498FCB79.OA41ED8008 O09AE5F507; // 0x40
	private O2498FCB79.O40E8AF793 O55FA68191; // 0x48
	private O2498FCB79.O7E9DACB73 O7C4FC5A90; // 0x50
	private O2498FCB79.O90111FA0D OCD8096658; // 0x58
	private readonly OC0ACAE5F0.O46CAB6546 O7B7D9BEBE; // 0x60
	private readonly OD622EEF6F.O012439827<O2498FCB79.O5DDA40B96> O485BE1E2A; // 0x68
	private readonly OD622EEF6F.O012439827<O2498FCB79.OFD544F985> OEF31C689F; // 0x70
	private readonly OD622EEF6F.O012439827<O2498FCB79.O4500F555C> OF9514E3C1; // 0x78
	private readonly OD622EEF6F.O012439827<O2498FCB79.O55FDE1D57> OAC79187D9; // 0x80
	private readonly OD622EEF6F.O012439827<O2498FCB79.OFFEB1114C> OE9FC3AEF5; // 0x88
	private System.Single O24762571D; // 0x90
	private static O2498FCB79.OECFB7135E O00CFC476D; // 0x0 <--- Static Instance
	private O2498FCB79.O25E2550E2 OF269BF5E4; // 0x98
	private O2498FCB79.O6C5D39D2D O7E18563F7; // 0xa0
	private O2498FCB79.O8530DE5FC O77EE9D173; // 0xa8
	private O1EA5A3818.OB9A10AAB5 O32EA938FE; // 0xb0
	private O2498FCB79.O015563378 O0D5C3E1DC; // 0xb8
	private O050B9AAC6.O1379E7885 O29E2BC7CF; // 0xc0
}

// CameraSystem Class
// Namespace: O2498FCB79
// Token: 0x2000418
// TypeInfo Offset: 0x2f8aa20
public class O48D52ECEB : Object, IRenderable
	// Fields
	private readonly O2498FCB79.O5FE9603BE O4E643BBD2; // 0x10
	private readonly O2498FCB79.O73CC765BC OC7C5DA6D6; // 0x18
	private readonly OC0ACAE5F0.O864782FC2<O2498FCB79.O933E5D5EE> O92B627BE4; // 0x20
	private readonly O7E65B40CD.O25C2D379C<O2498FCB79.O582BEB564> OE3A5F85FD; // 0x28
	private readonly UnityEngine.Transform O2BB75E896; // 0x30
	private readonly O2498FCB79.O6CA963200 O768A03469; // 0x38
	private readonly O2498FCB79.OD4E72217C OA80B6A09D; // 0x40
	private O2498FCB79.O5EF6CBE7B OEDD8309D8; // 0x48
	private UnityEngine.Camera O2ADE70DAF; // 0x50
	private O2498FCB79.O239D0D692 O42A68691E; // 0x58
	private readonly UnityEngine.WaitForSeconds OF864AB8D1; // 0x60
	private System.Single O5F46F25C8; // 0x68
	private System.Single O17CF4C544; // 0x6c
	private System.Single O1D55EC604; // 0x70
	private System.Single OA3ABA1BD6; // 0x74
	private System.Single OF18973770; // 0x78
	private System.Int32 O5EADCCF5D; // 0x7c
	private O85D8E43A6.OC8987E931<System.Int32> O2B350B115; // 0x80
	private O2498FCB79.O582BEB564 OEE013C50F; // 0x8c
	private UnityEngine.Quaternion OE9382A160; // 0x90
	private readonly CFRenderLib.Unity.Structs.Handle<CFRenderLib.Unity.Structs.RenderableData> O7DC953A3F; // 0xa0
	protected UnityEngine.Camera O4D9D04E00; // 0xa8 <--- Camera m_camera
	protected O2498FCB79.O6F692BC0C OE92D655B7; // 0xb0
	protected O0ABBAEBD6.O90551D01D O9357416DF; // 0xb8
	protected O2498FCB79.O0F1FDA7F3 O4A762CE27; // 0xc0
	protected O2498FCB79.O582BEB564 O7399960D5; // 0xc8
	private System.Boolean O613EF07E7; // 0xcc
	private static UnityEngine.Matrix4x4 O58858354C; // 0x0 
	private static UnityEngine.Matrix4x4 OF27937633; // 0x40
	private static UnityEngine.Matrix4x4 O27FCFF639; // 0x80
	private System.Boolean OA42829C8D; // 0xcd
	private readonly O2498FCB79.O6D29532E8 OC77F3A41A; // 0xd0
	private O85D8E43A6.OC8987E931<System.Single> OF8D3523E2; // 0xd8
}

// GameSystem Class
// Namespace: O2498FCB79
// Token: 0x20002d1
// TypeInfo Offset: 0x2f8d8e0
public class O6F692BC0C : Object
	// Fields
	private const System.Int32 OA591F1FA9; // 0x0
	private const System.Int32 OD2C18C12A; // 0x0
	private static System.Boolean O29184C17C; // 0x0
	private readonly O2498FCB79.O15933040E[] OA8C1A1FE8; // 0x10
	private readonly OC0ACAE5F0.O2997D0728[] OA2787BA21; // 0x18
	private readonly System.Collections.Generic.Dictionary<UnityEngine.PhysicsMaterial,O2498FCB79.O1CC78FAAE> O5A9D92B95; // 0x20
	private readonly OD622EEF6F.O16C9C7984<System.Int32,O2498FCB79.O0F1FDA7F3> O3471C73E7; // 0x28
	private readonly System.Collections.Generic.Dictionary<System.Int32,O2498FCB79.O0F1FDA7F3> OE54334CA1; // 0x30 <--- Dictionary<int, Character> AllCharacters
	private readonly O2498FCB79.O0F1FDA7F3[] O6ADC2C632; // 0x38
	private readonly UnityEngine.RaycastHit[] O673FAEF88; // 0x40
	private readonly OD622EEF6F.O16C9C7984<System.Int32,OC79B500DC.OF3CCF3A3B> OC4FA22DDC; // 0x48
	private readonly O2498FCB79.OFF46ED04D OCEB275F6B; // 0x50
	private readonly O2498FCB79.O277F1FF76 O8B3A413D6; // 0x58
	private readonly O050B9AAC6.O1379E7885 OA235A93E5; // 0x60
	private System.Collections.Generic.Queue<O2498FCB79.O0F1FDA7F3> OE52BC4AFF; // 0x68
	private System.Collections.Generic.Queue<O2498FCB79.O0F1FDA7F3> O609AFBDC2; // 0x70
	private readonly O2498FCB79.OA2B174EB2 O77D7D9BAB; // 0x78
	protected O2498FCB79.O6C5D39D2D O406FAFFE9; // 0x80
	protected O2498FCB79.O25E2550E2 O6DD90AC9C; // 0x88
	protected System.Collections.Generic.List<OC0ACAE5F0.O86F9C1B83> O061359203; // 0x90
	protected OD622EEF6F.O16C9C7984<System.Int32,OC0ACAE5F0.O86F9C1B83> O82FA4D0D8; // 0x98
	protected O0ABBAEBD6.O90551D01D O9357416DF; // 0xa0
	protected O2498FCB79.OECFB7135E OA254AC38D; // 0xa8
	private O2498FCB79.O21E5D44AB O4EDE19490; // 0xb0
	private readonly System.Collections.Generic.Dictionary<System.Int32,O2498FCB79.OEF414FACE> O8F0DD17B8; // 0xb8
	private System.Action<UnityEngine.RaycastHit> O569FE4CCC; // 0xc0
	private System.Single O73F2836C7; // 0xc8
	private readonly OC0ACAE5F0.ODC5BEA348 O128CA99F9; // 0xd0
	private readonly OC0ACAE5F0.O8FD10FE69 O82CF70C06; // 0xd8
	private readonly O789384CC2.O9DEC1BE8F OE24D7428D; // 0xe0
	private System.Boolean O3F3A96E4F; // 0xe8
}

// Character Class
// Namespace: O2498FCB79
// Token: 0x200010c
// TypeInfo Offset: 0x2f8aba8
public abstract class O0F1FDA7F3 : MonoBehaviour, O38C51199E, O4500F555C, O5DDA40B96
	// Fields
	protected static O2498FCB79.O6CA963200 O6CA963200; // 0x0
	private static O2498FCB79.O946725FD1 O9C784FB1A; // 0x8
	public O85D8E43A6.OC8987E931<System.Boolean> O1CB751B7B; // 0x20
	public O85D8E43A6.OC8987E931<System.Single> OD4B8D7589; // 0x2c
	public O85D8E43A6.OC8987E931<System.Single> OA067B5C14; // 0x38
	public O85D8E43A6.OC8987E931<System.Boolean> O43368F586; // 0x44
	public O85D8E43A6.OC8987E931<System.Boolean> O264888D27; // 0x50
	protected O2498FCB79.O6F692BC0C O6F692BC0C; // 0x60
	protected O2498FCB79.OECFB7135E O892EFEC70; // 0x68
	protected O2498FCB79.O6C5D39D2D O538C3BCC0; // 0x70
	protected O0ABBAEBD6.O90551D01D O90551D01D; // 0x78
	protected O85D8E43A6.OC8987E931<System.Single> O5F9331CEC; // 0x80
	protected OC79B500DC.OF3CCF3A3B O2F1D96AD9; // 0x90
	protected O2498FCB79.OE4843789E OE4843789E; // 0x98
	private System.Single O739ADD53A; // 0xa0
	private UnityEngine.Rigidbody O1D97A9F4A; // 0xa8
	private O2498FCB79.OBAB4F7E31 OACB3E44BD; // 0xb0
	private O2498FCB79.O3D1D8088B O42227FCAC; // 0xb8
	private System.Int32 O774FB2BDF; // 0xd0
	private System.Int32 O73A93472D; // 0xd4
	private System.Int32 O4F5A418C5; // 0xd8
	private System.Int32 ODCC7FC453; // 0xdc
	private System.Int32 O79E92FBEC; // 0xe0
	private OC79B500DC.OF3CCF3A3B ODA84DEA62; // 0xe8
	private UnityEngine.Transform O9C2100645; // 0xf0
	private UnityEngine.Transform O51B8E93B2; // 0xf8
	private UnityEngine.Transform O7571B9AEF; // 0x100
	private OC0ACAE5F0.O0AAA8CD33 O3EC3B2448; // 0x108
	private System.Int32 O52FE5729C; // 0x110
	private System.Single O9F22923D4; // 0x114
	private System.Boolean OF6884B228; // 0x118
	private System.Single O64A692619; // 0x11c
	private readonly O2498FCB79.O923509608[] OF39ADC4CB; // 0x120
	private readonly System.Collections.Generic.List<OC79B500DC.OF3CCF3A3B> OE0502E3A1; // 0x128
	private O2498FCB79.O1CC78FAAE O9958CE445; // 0x130
	private UnityEngine.CapsuleCollider OAE12B49B2; // 0x138
	private O2498FCB79.OBF48665A2 O43C257BCC; // 0x140
	private OC0ACAE5F0.O86F9C1B83 O511AD95FE; // 0x148
	private System.Single OE9CE2EC96; // 0x150
	private System.Single OFF927E504; // 0x154
	private System.Single O36DBCB0B0; // 0x158
	private System.Int64 O755AF4E4B; // 0x160
	private O2498FCB79.O5EF6CBE7B OD3B38604D; // 0x168 <--- CharacterModel k_model
	private UnityEngine.Transform O4975BC371; // 0x170
	private System.Boolean O3CF6951DD; // 0x178
	private System.Boolean OE7A63ACE4; // 0x179
}

// CharacterModel Class
// Namespace: O2498FCB79
// Token: 0x20003f6
// TypeInfo Offset: 0x2f8ac08
public class O5EF6CBE7B : MonoBehaviour, IRenderable, OE30E1B733
	// Fields
	private const System.Single ODE8791810; // 0x0
	private UnityEngine.SkinnedMeshRenderer O82DE28BAB; // 0x20  <--- SkinnedMeshRenderer
	private UnityEngine.Animator O7F5286B39; // 0x28
	private UnityEngine.Transform OB386BB56B; // 0x30
	private UnityEngine.Transform O08A3E0E36; // 0x38
	private O2498FCB79.OFBC1E55B3[] O06B80D789; // 0x40
	private UnityEngine.Material OE862BEA32; // 0x48
	private UnityEngine.Mesh OF809AD547; // 0x50
	private CFRenderLib.Unity.Structs.Handle<CFRenderLib.Unity.Structs.RenderableData> O7DC953A3F; // 0x58
	private UnityEngine.MaterialPropertyBlock O1A4A7AE5D; // 0x60
	private CFRenderLib.Core.Structs.SphericalHarmonicsL2 O05C71103C; // 0x68
	private O1623AB561.O11D28E99C O0C6E8A026; // 0xd8
	private O1623AB561.O7FED00083 O991B6D997; // 0xe0
	private readonly OD622EEF6F.O012439827<O2498FCB79.O3097CB002<O2498FCB79.OAD61ABD63>> O55D5880D2; // 0xe8
	private O2498FCB79.O5EF6CBE7B.OB009383FF[] O53378EC00; // 0xf0
	private UnityEngine.Coroutine OD174454A8; // 0xf8
	private readonly UnityEngine.WaitForSeconds O1D0A5F6C0; // 0x100
	private static O2498FCB79.O5EF6CBE7B.O2BD788E3D OF190A7D33; // 0x0
	private System.Boolean O9ABD2F042; // 0x108
	private System.Boolean O750858949; // 0x109
	private UnityEngine.Color OF49B687B3; // 0x10c
	private OC79B500DC.OF3CCF3A3B ODA84DEA62; // 0x120
	private readonly OD622EEF6F.O012439827<OC79B500DC.OF3CCF3A3B> O925C9DFC8; // 0x128
	private System.Single O2930CDCD4; // 0x130
	private UnityEngine.Color O774581175; // 0x134
}

```

---

## Tools Used

This project uses several community tools:

- **[KittyMemory](https://github.com/MJx0/KittyMemory)** by [MJx0](https://github.com/MJx0) — memory scanning and pattern finding for Android.
- **[AndKittyInjector](https://github.com/MJx0/AndKittyInjector)** by [MJx0](https://github.com/MJx0) — shared library injector for Android.
- **[Dobby](https://github.com/jmpews/Dobby)** by [jmpews](https://github.com/jmpews) — lightweight function hooking framework.

---

## Socials

- Signal: `feng_xingzhe_cn_.86`
- Discord: `feng_xingzhe`
