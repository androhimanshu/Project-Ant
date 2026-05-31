#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_ANT_ANT_CONTEXT_LIFECYCLE_OBSERVER_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_ANT_ANT_CONTEXT_LIFECYCLE_OBSERVER_H_

#include "third_party/blink/renderer/core/execution_context/execution_context_lifecycle_observer.h"
#include "third_party/blink/renderer/platform/heap/garbage_collected.h"

namespace blink {

class ExecutionContext;

class AntContextLifecycleObserver final
    : public GarbageCollected<AntContextLifecycleObserver>,
      public ExecutionContextLifecycleObserver {
 public:
  explicit AntContextLifecycleObserver(ExecutionContext* context);
  ~AntContextLifecycleObserver() override = default;
  void Trace(Visitor* visitor) const override;
  void ContextDestroyed() override {}
};

}  // namespace blink

#endif
