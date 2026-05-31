SRC=~/projects/chromium/src

mkdir -p $SRC/third_party/blink/public/mojom/ant
mkdir -p $SRC/content/browser/ant
mkdir -p $SRC/third_party/blink/renderer/modules/ant

cp ant_service.mojom                 $SRC/third_party/blink/public/mojom/ant/ant_service.mojom
cp ant_service_impl.h                $SRC/content/browser/ant/ant_service_impl.h
cp ant_service_impl.cc               $SRC/content/browser/ant/ant_service_impl.cc
cp browser_ant_BUILD.gn              $SRC/content/browser/ant/BUILD.gn
cp ant_values.h                      $SRC/base/ant_values.h
cp ant_values.cc                     $SRC/base/ant_values.cc
cp ant.idl                           $SRC/third_party/blink/renderer/modules/ant/ant.idl
cp ant.h                             $SRC/third_party/blink/renderer/modules/ant/ant.h
cp ant.cc                            $SRC/third_party/blink/renderer/modules/ant/ant.cc
cp ant_context_lifecycle_observer.h  $SRC/third_party/blink/renderer/modules/ant/ant_context_lifecycle_observer.h
cp ant_context_lifecycle_observer.cc $SRC/third_party/blink/renderer/modules/ant/ant_context_lifecycle_observer.cc
cp ant_module_initializer.h          $SRC/third_party/blink/renderer/modules/ant/ant_module_initializer.h
cp ant_module_initializer.cc         $SRC/third_party/blink/renderer/modules/ant/ant_module_initializer.cc
cp renderer_ant_BUILD.gn             $SRC/third_party/blink/renderer/modules/ant/BUILD.gn
