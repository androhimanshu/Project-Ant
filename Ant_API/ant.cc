#include "third_party/blink/renderer/modules/ant/ant.h"

#include "third_party/blink/public/platform/browser_interface_broker_proxy.h"
#include "third_party/blink/renderer/bindings/core/v8/script_promise_resolver.h"
#include "third_party/blink/renderer/core/context_features/context_feature_settings.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/platform/bindings/script_state.h"
#include "third_party/blink/renderer/platform/heap/persistent.h"
#include "third_party/blink/renderer/platform/wtf/functional.h"
#include "third_party/blink/renderer/platform/wtf/std_lib_extras.h"

namespace blink {

// POD statics — no constructor/destructor issues
const char Ant::kSupplementName[] = "Ant";
int32_t Ant::cached_value_    = 0;
int32_t Ant::cached_cpu_      = 0;
int32_t Ant::cached_screen_h_ = 0;
int32_t Ant::cached_screen_w_ = 0;

// String caches — DEFINE_STATIC_LOCAL avoids -Wglobal-constructors
// and -Wexit-time-destructors by storing only a pointer at global scope.
namespace {
String& CachedString()   { DEFINE_STATIC_LOCAL(String, s, ()); return s; }
String& CachedGpu()      { DEFINE_STATIC_LOCAL(String, s, ()); return s; }
String& CachedModel()    { DEFINE_STATIC_LOCAL(String, s, ()); return s; }
String& CachedAversion() { DEFINE_STATIC_LOCAL(String, s, ()); return s; }
String& CachedBversion() { DEFINE_STATIC_LOCAL(String, s, ()); return s; }
String& CachedVendor()   { DEFINE_STATIC_LOCAL(String, s, ()); return s; }
String& CachedBrand()    { DEFINE_STATIC_LOCAL(String, s, ()); return s; }
}  // namespace

// Origin guard — JS methods only. FetchAllFromBrowser runs for ALL origins.
static bool IsAntEnabled(ExecutionContext* ctx) {
  if (!ctx) return false;
  ContextFeatureSettings* s = ContextFeatureSettings::From(
      ctx, ContextFeatureSettings::CreationMode::kDontCreateIfNotExists);
  return s && s->isAntEnabled();
}

// Supplement lifecycle
Ant::Ant(ExecutionContext& context)
    : Supplement<ExecutionContext>(context),
      service_(&context),
      observer_receiver_(this, &context) {
  FetchAllFromBrowser();
  RegisterObserver();
}
Ant::~Ant() = default;

// static
Ant* Ant::From(ExecutionContext& context) {
  Ant* s = Supplement<ExecutionContext>::From<Ant>(context);
  if (!s) { s = MakeGarbageCollected<Ant>(context); ProvideTo(context, s); }
  return s;
}

// static
void Ant::InitializeForContext(ExecutionContext& context) {
  Ant::From(context);
}

// Observer
void Ant::RegisterObserver() {
  GetService()->AddObserver(
      observer_receiver_.BindNewPipeAndPassRemote(
          GetSupplementable()->GetTaskRunner(TaskType::kMiscPlatformAPI)));
}
void Ant::OnValueChanged(int32_t v)        { cached_value_ = v; }
void Ant::OnStringChanged(const String& s) { CachedString() = s; }

// Fetch all fields on page load.
// Mojo blink-side binding maps mojom string -> WTF::String,
// so all callbacks use const String& NOT const std::string&.
void Ant::FetchAllFromBrowser() {
  auto* svc = GetService();
  svc->GetValue   (WTF::BindOnce([](int32_t v)        { Ant::SetCachedValue(v); }));
  svc->GetString  (WTF::BindOnce([](const String& t)  { CachedString()   = t; }));
  svc->GetGpu     (WTF::BindOnce([](const String& t)  { CachedGpu()      = t; }));
  svc->GetModel   (WTF::BindOnce([](const String& t)  { CachedModel()    = t; }));
  svc->GetAversion(WTF::BindOnce([](const String& t)  { CachedAversion() = t; }));
  svc->GetBversion(WTF::BindOnce([](const String& t)  { CachedBversion() = t; }));
  svc->GetVendor  (WTF::BindOnce([](const String& t)  { CachedVendor()   = t; }));
  svc->GetCpu     (WTF::BindOnce([](int32_t v)        { Ant::cached_cpu_      = v; }));
  svc->GetBrand   (WTF::BindOnce([](const String& t)  { CachedBrand()    = t; }));
  svc->GetScreenH (WTF::BindOnce([](int32_t v)        { Ant::cached_screen_h_ = v; }));
  svc->GetScreenW (WTF::BindOnce([](int32_t v)        { Ant::cached_screen_w_ = v; }));
}

// ANT_GUARD — uses 'p' to match variable name in every method
#define ANT_GUARD(resolver, context)                              \
  if (!IsAntEnabled(context)) {                                   \
    resolver->RejectWithDOMException(                             \
        DOMExceptionCode::kNotAllowedError,                       \
        "Ant API is not available on this origin.");               \
    return p;                                                     \
  }

// getTag
// static
String Ant::getTag(ScriptState* ss) {
  if (!IsAntEnabled(ExecutionContext::From(ss))) return String();
  return "Ant";
}

// setValue / getValue
// static
ScriptPromise<IDLString> Ant::setValue(ScriptState* ss, int32_t v) {
  auto* r = MakeGarbageCollected<ScriptPromiseResolver<IDLString>>(ss);
  auto  p = r->Promise();
  auto* ctx = ExecutionContext::From(ss);
  ANT_GUARD(r, ctx)
  cached_value_ = v;
  Ant::From(*ctx)->GetService()->SetValue(v, WTF::BindOnce(
      [](ScriptPromiseResolver<IDLString>* r, const String&) {
        r->Resolve("ok");
      }, WrapPersistent(r)));
  return p;
}

// static
ScriptPromise<IDLLong> Ant::getValue(ScriptState* ss) {
  auto* r = MakeGarbageCollected<ScriptPromiseResolver<IDLLong>>(ss);
  auto  p = r->Promise();
  auto* ctx = ExecutionContext::From(ss);
  ANT_GUARD(r, ctx)
  Ant::From(*ctx)->GetService()->GetValue(WTF::BindOnce(
      [](ScriptPromiseResolver<IDLLong>* r, int32_t v) {
        Ant::SetCachedValue(v); r->Resolve(v);
      }, WrapPersistent(r)));
  return p;
}

// setString / getString
// static
ScriptPromise<IDLString> Ant::setString(ScriptState* ss, const String& t) {
  auto* r = MakeGarbageCollected<ScriptPromiseResolver<IDLString>>(ss);
  auto  p = r->Promise();
  auto* ctx = ExecutionContext::From(ss);
  ANT_GUARD(r, ctx)
  CachedString() = t;
  Ant::From(*ctx)->GetService()->SetString(t, WTF::BindOnce(
      [](ScriptPromiseResolver<IDLString>* r, const String&) {
        r->Resolve("ok");
      }, WrapPersistent(r)));
  return p;
}

// static
ScriptPromise<IDLString> Ant::getString(ScriptState* ss) {
  auto* r = MakeGarbageCollected<ScriptPromiseResolver<IDLString>>(ss);
  auto  p = r->Promise();
  auto* ctx = ExecutionContext::From(ss);
  ANT_GUARD(r, ctx)
  Ant::From(*ctx)->GetService()->GetString(WTF::BindOnce(
      [](ScriptPromiseResolver<IDLString>* r, const String& t) {
        r->Resolve(t);
      }, WrapPersistent(r)));
  return p;
}

// Macros for all remaining named fields.
// Setters: return IDLString with "ok" message.
// Getters: return IDLString or IDLLong.
// Callbacks use const String& (blink binding) NOT const std::string&.
// Strings passed directly to Mojo — no .Utf8() needed.

#define ANT_SET_STR(jsName, mojoName, cacheRef)                                \
ScriptPromise<IDLString> Ant::jsName(ScriptState* ss, const String& v) {      \
  auto* r = MakeGarbageCollected<ScriptPromiseResolver<IDLString>>(ss);        \
  auto  p = r->Promise();                                                       \
  auto* ctx = ExecutionContext::From(ss);                                       \
  ANT_GUARD(r, ctx)                                                            \
  cacheRef = v;                                                                 \
  Ant::From(*ctx)->GetService()->mojoName(v, WTF::BindOnce(                   \
      [](ScriptPromiseResolver<IDLString>* r, const String&) {                \
        r->Resolve("ok");                                                       \
      }, WrapPersistent(r)));                                                   \
  return p;                                                                     \
}

#define ANT_GET_STR(jsName, mojoName)                                          \
ScriptPromise<IDLString> Ant::jsName(ScriptState* ss) {                       \
  auto* r = MakeGarbageCollected<ScriptPromiseResolver<IDLString>>(ss);        \
  auto  p = r->Promise();                                                       \
  auto* ctx = ExecutionContext::From(ss);                                       \
  ANT_GUARD(r, ctx)                                                            \
  Ant::From(*ctx)->GetService()->mojoName(WTF::BindOnce(                      \
      [](ScriptPromiseResolver<IDLString>* r, const String& t) {              \
        r->Resolve(t);                                                          \
      }, WrapPersistent(r)));                                                   \
  return p;                                                                     \
}

#define ANT_SET_INT(jsName, mojoName, cache)                                   \
ScriptPromise<IDLString> Ant::jsName(ScriptState* ss, int32_t v) {            \
  auto* r = MakeGarbageCollected<ScriptPromiseResolver<IDLString>>(ss);        \
  auto  p = r->Promise();                                                       \
  auto* ctx = ExecutionContext::From(ss);                                       \
  ANT_GUARD(r, ctx)                                                            \
  cache = v;                                                                    \
  Ant::From(*ctx)->GetService()->mojoName(v, WTF::BindOnce(                   \
      [](ScriptPromiseResolver<IDLString>* r, const String&) {                \
        r->Resolve("ok");                                                       \
      }, WrapPersistent(r)));                                                   \
  return p;                                                                     \
}

#define ANT_GET_INT(jsName, mojoName, cache)                                   \
ScriptPromise<IDLLong> Ant::jsName(ScriptState* ss) {                         \
  auto* r = MakeGarbageCollected<ScriptPromiseResolver<IDLLong>>(ss);         \
  auto  p = r->Promise();                                                       \
  auto* ctx = ExecutionContext::From(ss);                                       \
  ANT_GUARD(r, ctx)                                                            \
  Ant::From(*ctx)->GetService()->mojoName(WTF::BindOnce(                      \
      [](ScriptPromiseResolver<IDLLong>* r, int32_t v) {                      \
        cache = v; r->Resolve(v);                                              \
      }, WrapPersistent(r)));                                                   \
  return p;                                                                     \
}

ANT_SET_STR(setGpu,      SetGpu,      CachedGpu())
ANT_GET_STR(getGpu,      GetGpu)
ANT_SET_STR(setModel,    SetModel,    CachedModel())
ANT_GET_STR(getModel,    GetModel)
ANT_SET_STR(setAversion, SetAversion, CachedAversion())
ANT_GET_STR(getAversion, GetAversion)
ANT_SET_STR(setBversion, SetBversion, CachedBversion())
ANT_GET_STR(getBversion, GetBversion)
ANT_SET_STR(setVendor,   SetVendor,   CachedVendor())
ANT_GET_STR(getVendor,   GetVendor)
ANT_SET_STR(setBrand,    SetBrand,    CachedBrand())
ANT_GET_STR(getBrand,    GetBrand)

ANT_SET_INT(setCpu,     SetCpu,     Ant::cached_cpu_)
ANT_GET_INT(getCpu,     GetCpu,     Ant::cached_cpu_)
ANT_SET_INT(setScreenH, SetScreenH, Ant::cached_screen_h_)
ANT_GET_INT(getScreenH, GetScreenH, Ant::cached_screen_h_)
ANT_SET_INT(setScreenW, SetScreenW, Ant::cached_screen_w_)
ANT_GET_INT(getScreenW, GetScreenW, Ant::cached_screen_w_)

// Native C++ accessors — usable from blink/renderer files only
int32_t Ant::GetCachedValue()    { return cached_value_;    }
void    Ant::SetCachedValue(int32_t v) { cached_value_ = v; }
int32_t Ant::GetCachedCpu()      { return cached_cpu_;      }
int32_t Ant::GetCachedScreenH()  { return cached_screen_h_; }
int32_t Ant::GetCachedScreenW()  { return cached_screen_w_; }
String  Ant::GetCachedBrand()    { return CachedBrand();    }
String  Ant::GetCachedString()   { return CachedString();   }
String  Ant::GetCachedGpu()      { return CachedGpu();      }
String  Ant::GetCachedModel()    { return CachedModel();     }
String  Ant::GetCachedAversion() { return CachedAversion();  }
String  Ant::GetCachedBversion() { return CachedBversion();  }
String  Ant::GetCachedVendor()   { return CachedVendor();    }

mojom::blink::AntService* Ant::GetService() {
  if (!service_.is_bound()) {
    auto* ctx = GetSupplementable();
    ctx->GetBrowserInterfaceBroker().GetInterface(
        service_.BindNewPipeAndPassReceiver(
            ctx->GetTaskRunner(TaskType::kMiscPlatformAPI)));
  }
  return service_.get();
}

void Ant::Trace(Visitor* visitor) const {
  visitor->Trace(service_);
  visitor->Trace(observer_receiver_);
  Supplement<ExecutionContext>::Trace(visitor);
}

}  // namespace blink
