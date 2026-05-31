#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_ANT_ANT_MODULE_INITIALIZER_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_ANT_ANT_MODULE_INITIALIZER_H_

namespace blink {
class Document;

class AntModuleInitializer {
 public:
  static void Initialize();
  static void OnDocumentCreated(Document& document);
};

}  // namespace blink
#endif
