# Ant API — Complete Technical Documentation
# Custom Chromium JavaScript API with Origin Restriction

---

## Overview

Ant is a custom JavaScript API built into a modified Chromium browser that:
- Exposes a JS namespace `Ant` **visible only on allowed origins**
- Stores values in the **browser process** shared across all tabs
- Overrides native browser properties (`navigator.deviceMemory`, `screen.width`, etc.)
- Provides a **shared values store** accessible from `base/` and `content/` layers
- Uses **Mojo IPC** + **observer push** for instant cross-tab sync

```javascript
// On allowed origin only:
const msg = await Ant.setValue(16)    // → "ok"
await Ant.getValue()                  // → 16
navigator.deviceMemory                // → 16  (works on ALL origins)

// On any other site:
typeof Ant                            // → "undefined"  completely invisible
navigator.deviceMemory                // → 16  still gets the value
```

---

## Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                      Browser Process                             │
│  ┌───────────────────────────────────────────────────────────┐  │
│  │  AntServiceImpl (Singleton)                               │  │
│  │  - stored_value_, stored_gpu_, stored_brand_, etc.        │  │
│  │  - On Set: stores + pushes OnValueChanged to all tabs     │  │
│  │  - Also writes to base::AntValues (shared C++ store)      │  │
│  └───────────────────────────────────────────────────────────┘  │
│  ┌───────────────────────────────────────────────────────────┐  │
│  │  base::AntValues (Singleton)                              │  │
│  │  - Plain C++ — no Blink, no Mojo                         │  │
│  │  - Readable from base/**, content/**, chrome/**           │  │
│  └───────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────┘
                         ↑ ↓ Mojo IPC
┌──────────────────┐              ┌──────────────────┐
│  Renderer Tab 1  │              │  Renderer Tab 2  │
│  Ant supplement  │←─ push ─────→│  Ant supplement  │
│  cached values   │  observer    │  cached values   │
└──────────────────┘              └──────────────────┘
```

---

## All API Methods

| JS Method | Type | Returns | Native override |
|---|---|---|---|
| `setValue(n)` | int | `"ok"` | `navigator.deviceMemory` |
| `getValue()` | int | stored int | — |
| `setCpu(n)` | int | `"ok"` | `navigator.hardwareConcurrency` |
| `getCpu()` | int | stored int | — |
| `setScreenH(n)` | int | `"ok"` | `screen.height` |
| `getScreenH()` | int | stored int | — |
| `setScreenW(n)` | int | `"ok"` | `screen.width` |
| `getScreenW()` | int | stored int | — |
| `setString(s)` | string | `"ok"` | custom use |
| `getString()` | string | stored string | — |
| `setGpu(s)` | string | `"ok"` | custom use |
| `getGpu()` | string | stored string | — |
| `setModel(s)` | string | `"ok"` | custom use |
| `getModel()` | string | stored string | — |
| `setAversion(s)` | string | `"ok"` | custom use |
| `getAversion()` | string | stored string | — |
| `setBversion(s)` | string | `"ok"` | `base::Version` in version_info.cc |
| `getBversion()` | string | stored string | — |
| `setVendor(s)` | string | `"ok"` | custom use |
| `getVendor()` | string | stored string | — |
| `setBrand(s)` | string | `"ok"` | `navigator.userAgent` |
| `getBrand()` | string | stored string | — |
| `getTag()` | string | `"Ant"` | feature detection |

---

## How to use values in native browser functions (blink/renderer layer)

Include `ant.h` ONLY in files inside `third_party/blink/renderer/`.

### Integer override pattern:
```cpp
#include "third_party/blink/renderer/modules/ant/ant.h"

SomeType SomeClass::someProperty() const {
  int32_t v = Ant::GetCachedXxx();
  if (v > 0) return static_cast<SomeType>(v);
  // original code below
}
```

### String override pattern:
```cpp
#include "third_party/blink/renderer/modules/ant/ant.h"

String SomeClass::someProperty() const {
  String v = Ant::GetCachedXxx();
  if (!v.IsEmpty()) return v;
  // original code below
}
```

### All native accessors available:
```cpp
Ant::GetCachedValue()    // → int32_t  for navigator.deviceMemory
Ant::GetCachedCpu()      // → int32_t  for navigator.hardwareConcurrency
Ant::GetCachedScreenH()  // → int32_t  for screen.height
Ant::GetCachedScreenW()  // → int32_t  for screen.width
Ant::GetCachedBrand()    // → String   for navigator.userAgent
Ant::GetCachedString()   // → String   generic
Ant::GetCachedGpu()      // → String   generic
Ant::GetCachedModel()    // → String   generic
Ant::GetCachedAversion() // → String   generic
Ant::GetCachedBversion() // → String   generic
Ant::GetCachedVendor()   // → String   generic
```

---

## How to use values in base/ and content/ layers

Use `base::AntValues` — no Blink dependency:

```cpp
#include "base/ant_values.h"

// Read integer (atomic — thread safe)
int32_t cpu     = base::AntValues::GetInstance()->cpu.load();
int32_t screen_w = base::AntValues::GetInstance()->screen_w.load();

// Read string
std::string brand = base::AntValues::GetInstance()->brand;
std::string gpu   = base::AntValues::GetInstance()->gpu;

// Read parsed Version (for version_info.cc)
const base::Version& ver = base::AntValues::GetInstance()->bversion_parsed;
if (ver.IsValid()) { /* use ver */ }
```

### Which layer uses which:
```
blink/renderer/**   →  #include "ant.h"         → Ant::GetCachedXxx()
content/**          →  #include "base/ant_values.h" → AntValues::GetInstance()->xxx
base/**             →  #include "base/ant_values.h" → AntValues::GetInstance()->xxx
chrome/**           →  #include "base/ant_values.h" → AntValues::GetInstance()->xxx
```

---

## How to add a new method — complete recipe

Example: add `Ant.setOs("Android")` wired to `navigator.platform`.

### Step 1 — `ant_service.mojom`
```mojom
SetOs(string value) => (string message);
GetOs()             => (string value);
```

### Step 2 — `ant_service_impl.h`
```cpp
void SetOs(const std::string& v, SetOsCallback cb) override;
void GetOs(GetOsCallback cb) override;
private:
  std::string stored_os_;
```

### Step 3 — `ant_service_impl.cc`
```cpp
void AntServiceImpl::SetOs(const std::string& v, SetOsCallback cb) {
  stored_os_ = v;
  base::AntValues::GetInstance()->os = v;   // if you add it to AntValues
  std::move(cb).Run("ok");
}
void AntServiceImpl::GetOs(GetOsCallback cb) {
  std::move(cb).Run(stored_os_);
}
```

### Step 4 — `ant.idl`
```idl
[CallWith=ScriptState] Promise<DOMString> setOs(DOMString value);
[CallWith=ScriptState] Promise<DOMString> getOs();
```

### Step 5 — `ant.h`
```cpp
static ScriptPromise<IDLString> setOs(ScriptState*, const String&);
static ScriptPromise<IDLString> getOs(ScriptState*);
static String GetCachedOs();
```

### Step 6 — `ant.cc`
```cpp
// In anonymous namespace:
String& CachedOs() { DEFINE_STATIC_LOCAL(String, s, ()); return s; }

// In FetchAllFromBrowser():
svc->GetOs(WTF::BindOnce([](const String& t) { CachedOs() = t; }));

// Expand macros:
ANT_SET_STR(setOs, SetOs, CachedOs())
ANT_GET_STR(getOs, GetOs)

// Native accessor:
String Ant::GetCachedOs() { return CachedOs(); }
```

### Step 7 — Wire to native property
```cpp
// In navigator_id.cc or wherever navigator.platform is implemented:
#include "third_party/blink/renderer/modules/ant/ant.h"

String NavigatorID::platform() const {
  String v = Ant::GetCachedOs();
  if (!v.IsEmpty()) return v;
  // original code below
}
```

### Step 8 — `base/ant_values.h` (optional — only if needed in base/content layer)
```cpp
// Add to AntValues struct:
std::string os;
```

---

## Origin restriction — how it works

```
IDL: [RuntimeEnabled=Ant, ContextEnabled=Ant]

Generated V8 check (OR logic):
  RuntimeEnabledFeatures::AntEnabled()       → false  (no status in json5)
  ContextFeatureSettings::isAntEnabled()     → true only for allowed origins

On blocked origins:  false || false = false → Hub never installed on window
On allowed origins:  false || true  = true  → Hub installed on window
```

`WebV8Features::EnableAnt()` is called only for origins in `kAntAllowedOrigins`
inside `render_frame_impl.cc::DidCreateScriptContext()`.

To add or remove allowed sites, edit **only** `kAntAllowedOrigins` in
`content/renderer/render_frame_impl.cc`.

---

## Build commands

```bash
cd ~/projects/chromium/src
autoninja -C out/Default chrome

# If missing v8_ant.cc/v8_ant.h → apply PATCH_11 then rebuild
autoninja -C out/Default chrome

# Run with logging
./out/Default/chrome --enable-logging --v=1
```

---

## Bugs fixed during development

| Error | Cause | Fix |
|---|---|---|
| `~Ant() override` | GarbageCollected dtor not virtual | Remove `override` |
| `-Wglobal-constructors` on String statics | WTF::String has ctor/dtor | Use `DEFINE_STATIC_LOCAL` |
| Callback type mismatch `std::string` vs `WTF::String` | Blink Mojo binding maps string→WTF::String | Use `const String&` in all callbacks |
| `return promise` undeclared | Macro used `promise`, variable is `p` | Change macro to `return p` |
| `//third_party/blink/public/web` not loadable from modules | Wrong dep | Remove, register callback differently |
| `ant.h` included in `base/` layer | `ant.h` uses Blink types | Use `base/ant_values.h` instead |
| Stack address returned for `base::Version` | Returning local var by reference | Store `Version` in `AntValues::bversion_parsed` |
| Complex ctor/dtor inlined in header | Chromium style rule | Move to `.cc` file |
