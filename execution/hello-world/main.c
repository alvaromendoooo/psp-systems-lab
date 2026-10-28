#include <pspkerneltypes.h>
#include <psploadexec.h>
#include <pspmoduleinfo.h>
#include <pspthreadman.h>
#include <pspuser.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <time.h>

// PSP_MODULE_INFO is required
PSP_MODULE_INFO("Hello World!", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

int exit_callback(int arg1, int arg2, void *common) {
  sceKernelExitGame(); // Exists the game and goes back to PSP homebrowser. Need thread.
  return 0;
}

int callback_thread(SceSize args, void *argp) {
  int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
  sceKernelRegisterExitCallback(cbid);
  sceKernelSleepThreadCB();
  return 0;
}

int setup_callbacks(void) {
  int thid = sceKernelCreateThread("update thread", callback_thread, 0x11, 0xFA0, 0, 0);
  if (thid >= 0) {
    sceKernelStartThread(thid, 0, 0);
  }
  return thid;
}

int main(void) {
  // Use created functions to make existing possible
  setup_callbacks();

  // Prints hello world! on a debug screen on a laptop
  pspDebugScreenInit(); // Init a debug screen
  while(1) {
    pspDebugScreenSetXY(0, 0);
    pspDebugScreenPrintf("Hello World!");
    sceDisplayWaitVblank();
  }

  return 0;
}
