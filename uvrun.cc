#include <nan.h>
#include <uv.h>

using namespace v8;

NAN_METHOD(Run) {
#ifdef OLD_UV_RUN_SIGNATURE
  int r = uv_run(uv_default_loop());
#else
  int r = uv_run(uv_default_loop(), UV_RUN_DEFAULT);
#endif
  info.GetReturnValue().Set(r);
}

NAN_METHOD(RunOnce) {
#ifdef OLD_UV_RUN_SIGNATURE
  int r = uv_run_once(uv_default_loop());
#else
  int r = uv_run(uv_default_loop(), UV_RUN_ONCE);
#endif
  info.GetReturnValue().Set(r);
}

int runNoWait() {
#ifdef OLD_UV_RUN_SIGNATURE
  int r = uv_run_nowait(uv_default_loop());
#else
  int r = uv_run(uv_default_loop(), UV_RUN_NOWAIT);
#endif
  return r;
}

NAN_METHOD(RunNoWait) {
  int r = runNoWait();
  info.GetReturnValue().Set(r);
}

#ifdef _WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif
NAN_METHOD(DoSleep) {
  unsigned int ms = info[0]->Uint32Value();
  Sleep(ms);
}

NAN_METHOD(yield) {
  unsigned int ms = info[0]->Uint32Value();
  if (ms > 0) {
    Sleep(ms);
  }
  runNoWait();
}

NAN_MODULE_INIT(init) {
    using Nan::New;
    Nan::Set(target, New<String>("run").ToLocalChecked(),
        Nan::GetFunction(New<FunctionTemplate>(Run)).ToLocalChecked());
    Nan::Set(target, New<String>("runOnce").ToLocalChecked(),
        Nan::GetFunction(New<FunctionTemplate>(RunOnce)).ToLocalChecked());
    Nan::Set(target, New<String>("runNoWait").ToLocalChecked(),
        Nan::GetFunction(New<FunctionTemplate>(RunNoWait)).ToLocalChecked());
    Nan::Set(target, New<String>("sleep").ToLocalChecked(),
        Nan::GetFunction(New<FunctionTemplate>(DoSleep)).ToLocalChecked());
    Nan::Set(target, New<String>("yield").ToLocalChecked(),
        Nan::GetFunction(New<FunctionTemplate>(yield)).ToLocalChecked());
}

NODE_MODULE(uvrun, init);
