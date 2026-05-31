#include "third_party/blink/renderer/modules/ant/ant_context_lifecycle_observer.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/modules/ant/ant.h"

namespace blink {

AntContextLifecycleObserver::AntContextLifecycleObserver(
    ExecutionContext* context)
    : ExecutionContextLifecycleObserver(context) {
  if (!context)
    return;
  // No isAntEnabled() check — runs for ALL origins so
  // FetchAllFromBrowser() fires on every page load and
  // navigator.deviceMemory etc. work everywhere.
  Ant::InitializeForContext(*context);
}

void AntContextLifecycleObserver::Trace(Visitor* visitor) const {
  ExecutionContextLifecycleObserver::Trace(visitor);
}

}  // namespace blink
