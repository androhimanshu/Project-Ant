#include "third_party/blink/renderer/modules/ant/ant_module_initializer.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/execution_context/execution_context.h"
#include "third_party/blink/renderer/modules/ant/ant.h"
#include "third_party/blink/renderer/modules/ant/ant_context_lifecycle_observer.h"

namespace blink {

// static
void AntModuleInitializer::Initialize() {}

// static
void AntModuleInitializer::OnDocumentCreated(Document& document) {
  ExecutionContext* context = document.GetExecutionContext();
  if (!context)
    return;
  MakeGarbageCollected<AntContextLifecycleObserver>(context);
}

}  // namespace blink
