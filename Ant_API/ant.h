#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_ANT_ANT_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_ANT_ANT_H_

#include "third_party/blink/public/mojom/ant/ant_service.mojom-blink.h"
#include "third_party/blink/renderer/bindings/core/v8/script_promise.h"
#include "third_party/blink/renderer/bindings/core/v8/script_promise_resolver.h"
#include "third_party/blink/renderer/platform/heap/garbage_collected.h"
#include "third_party/blink/renderer/platform/mojo/heap_mojo_receiver.h"
#include "third_party/blink/renderer/platform/mojo/heap_mojo_remote.h"
#include "third_party/blink/renderer/platform/supplementable.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace blink {

class ExecutionContext;
class ScriptState;

class Ant final : public GarbageCollected<Ant>,
                  public Supplement<ExecutionContext>,
                  public mojom::blink::AntValueObserver {
 public:
  static const char kSupplementName[];

  explicit Ant(ExecutionContext& context);
  ~Ant();

  static Ant* From(ExecutionContext& context);
  static void InitializeForContext(ExecutionContext& context);

  // JS-exposed methods — setters return Promise<DOMString> with "ok" message
  static String getTag(ScriptState*);

  static ScriptPromise<IDLString> setValue(ScriptState*, int32_t);
  static ScriptPromise<IDLLong>   getValue(ScriptState*);

  static ScriptPromise<IDLString> setString(ScriptState*, const String&);
  static ScriptPromise<IDLString> getString(ScriptState*);

  static ScriptPromise<IDLString> setGpu(ScriptState*, const String&);
  static ScriptPromise<IDLString> getGpu(ScriptState*);

  static ScriptPromise<IDLString> setModel(ScriptState*, const String&);
  static ScriptPromise<IDLString> getModel(ScriptState*);

  static ScriptPromise<IDLString> setAversion(ScriptState*, const String&);
  static ScriptPromise<IDLString> getAversion(ScriptState*);

  static ScriptPromise<IDLString> setBversion(ScriptState*, const String&);
  static ScriptPromise<IDLString> getBversion(ScriptState*);

  static ScriptPromise<IDLString> setVendor(ScriptState*, const String&);
  static ScriptPromise<IDLString> getVendor(ScriptState*);

  static ScriptPromise<IDLString> setCpu(ScriptState*, int32_t);
  static ScriptPromise<IDLLong>   getCpu(ScriptState*);

  static ScriptPromise<IDLString> setBrand(ScriptState*, const String&);
  static ScriptPromise<IDLString> getBrand(ScriptState*);

  static ScriptPromise<IDLString> setScreenH(ScriptState*, int32_t);
  static ScriptPromise<IDLLong>   getScreenH(ScriptState*);

  static ScriptPromise<IDLString> setScreenW(ScriptState*, int32_t);
  static ScriptPromise<IDLLong>   getScreenW(ScriptState*);

  // Native C++ accessors — include ant.h in blink/renderer files only
  static int32_t GetCachedValue();
  static void    SetCachedValue(int32_t);
  static int32_t GetCachedCpu();
  static int32_t GetCachedScreenH();
  static int32_t GetCachedScreenW();
  static String  GetCachedBrand();
  static String  GetCachedString();
  static String  GetCachedGpu();
  static String  GetCachedModel();
  static String  GetCachedAversion();
  static String  GetCachedBversion();
  static String  GetCachedVendor();

  // AntValueObserver
  void OnValueChanged(int32_t new_value) override;
  void OnStringChanged(const String& new_string) override;

  void Trace(Visitor* visitor) const override;

 private:
  mojom::blink::AntService* GetService();
  void FetchAllFromBrowser();
  void RegisterObserver();

  // POD statics only — String statics use DEFINE_STATIC_LOCAL in ant.cc
  static int32_t cached_value_;
  static int32_t cached_cpu_;
  static int32_t cached_screen_h_;
  static int32_t cached_screen_w_;

  HeapMojoRemote<mojom::blink::AntService>              service_;
  HeapMojoReceiver<mojom::blink::AntValueObserver, Ant> observer_receiver_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_ANT_ANT_H_
