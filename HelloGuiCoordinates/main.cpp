#include <CtrlLib/CtrlLib.h>

using namespace Upp;

struct MyAppWindow : TopWindow {
	Button button;
	
	MyAppWindow() {
		Title("Experimenting with coordinates");
		button.SetLabel("Click me");
		Add(button.LeftPos(10,200).TopPos(10,200));
		button.WhenAction = [=]{PromptOK("Button clicked");};
	};
};

GUI_APP_MAIN
{
	MyAppWindow app;
	app.Run();
}
