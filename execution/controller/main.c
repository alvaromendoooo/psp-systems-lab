#include <pspkerneltypes.h>
#include <psploadexec.h>
#include <pspmoduleinfo.h>
#include <pspthreadman.h>
#include <pspuser.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <stdio.h>
#include <string.h>

PSP_MODULE_INFO("Controller", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

int done = 0;

int exit_callback(int arg1, int arg2, void *common) {
  done = 1;
  return 0;
}

int callback_thread(SceSize args, void *argp) {
  int cbid = sceKernelCreateCallback("Exit Callback", 
      exit_callback, NULL);
  sceKernelRegisterExitCallback(cbid);
  sceKernelSleepThreadCB();
  return 0;
}

int setup_callback(void) {
  int thid = sceKernelCreateThread("update_thread", 
      callback_thread, 0x11, 0xFA0, 0, 0);
  
  if (thid >= 0) {
    sceKernelStartThread(thid, 0, 0);
  }
  return thid;
}

int main(void) {
  SceCtrlData pad; // stores data from Controller - current buttons and axis states
  
  pspDebugScreenInit();
  setup_callback();

  sceCtrlSetSamplingCycle(0); // sets current controller cycle, normally 0
  sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG); // we want to read states of physical psp buttons

  while (!done) { // If we press HOME buttom, prgram will exits
    pspDebugScreenSetXY(0, 0);

    /*
     * Reads controller data from controller service
     *
     * count = 1 because we chose analog, if it was digital,
     * count's value would be different
     * */
    sceCtrlReadBufferPositive(&pad, 1);

    printf("Analog X = %3d, ", pad.Lx);
    printf("Analog Y = %3d, ", pad.Ly);
    
    if (pad.Buttons != 0) { // Check if s buttom was pressed
      switch (pad.Buttons) {
        case PSP_CTRL_SQUARE:
          printf("Square pressed! \n");
          break;
        case PSP_CTRL_CIRCLE:
          printf("Circle pressed! \n");
          break;
        case PSP_CTRL_TRIANGLE:
          printf("Triangle pressed! \n");
          break;
        case PSP_CTRL_CROSS:
          printf("Cross pressed! \n");
          break;
        case PSP_CTRL_UP:
        printf("Up direction pad pressed! \n");
          break;
        case PSP_CTRL_DOWN:
          printf("Down direction pad pressed! \n");
          break;
        case PSP_CTRL_LEFT:
          printf("Left direction pad pressed! \n");
          break;
        case PSP_CTRL_RIGHT:
          printf("Right direction pad pressed! \n");
          break;
        default:
          printf("Not supported buttom");
          break;
      }
    }
  }

  sceKernelExitGame();
  return 0;
}
