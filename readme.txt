Find HWNDs of windows.

Here's an example. This finds Chrome renderer's window handle:

HwndFinder hf;
HWND rendererHwnd = hf.find("Chrome_WidgetWin_1 > Chrome_WidgetWin_0 > Chrome_RenderWidgetHostHWND");

Here Chrome_RenderWidgetHostHWND is a child of Chrome_WidgetWin_0 is a child of Chrome_WidgetWin_1 which is the top window.

