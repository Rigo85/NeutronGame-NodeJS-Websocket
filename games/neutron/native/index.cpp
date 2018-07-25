#include <nan.h>
#include <MinimaxAsyncBinding.h>

NAN_MODULE_INIT(InitModule) {
  MinimaxAsyncBinding::Init(target);
}

NODE_MODULE(NativeMinimax, InitModule);
