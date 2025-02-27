#include <CtrlLib/CtrlLib.h>

using namespace Upp;

#define IMAGECLASS MenuImage
#define IMAGEFILE <HelloGuiGiantInlineMenu/icon.iml>
#include <Draw/iml.h>

struct HelloGuiGiantInlineMenu : TopWindow {

	MenuBar menu;
	ToolBar buttons;
	StatusBar status;

	typedef HelloGuiGiantInlineMenu CLASSNAME; // so that we can use THISFN shortcut

	void SubBar(Bar& bar)
	{

		bar.AddMenu("Function", MenuImage::Open(),
		            [=] { // AddMenu - only in menu
						PromptOK("open menu activated!");
					})
			.Help("This invokes MenuFn method of tutorial example");

		bar.Add(MenuImage::Open(),
		        [=] { // does not have image - not in toolbar
					PromptOK("Open tool activated!");
				})
			.Help("This invokes BarFn method of tutorial example");

		bar.Add("Exit", MenuImage::Close(), [=] { // in both toolbar and menu
			if(PromptOKCancel("Exit MyApp?"))
				Break();
		}).Help("Closes the app");
	}

	HelloGuiGiantInlineMenu()
	{

		Title("My application with menu").Sizeable();

		AddFrame(menu);
		AddFrame(TopSeparatorFrame());
		AddFrame(buttons);
		AddFrame(status);
		AddFrame(InsetFrame());

		menu.Set([=](Bar& bar) {
			bar.Sub("Menu", [=](Bar& bar) {
				bar.Add("Open", MenuImage::Open(), [=] {});
				bar.Add("Save", [=] {});
				bar.Add("Save As", [=] {});
				bar.Add("Settings", [=] {});
				bar.Add("Exit", MenuImage::Close(), [=] {});
			});
			bar.Sub("Edit", [=](Bar& bar) {
				bar.Add("Copy", [=] {});
				bar.Add("Paste", [=] {});
				bar.Add("Delete", [=] {});
				bar.Sub("Insert", [=](Bar& bar) {
					bar.Add("Item", [=] {});
					bar.Add("Template", [=] {});
				});
				bar.Add("Update", [=] {});
			});
			bar.Sub("Info", [=](Bar& bar) {
				bar.Add("About", [=] {});
				bar.Add("Search for Updates", [=] {});
				bar.Add("Register", [=] {});
				bar.Add("Help", [=] {});
			});
		    bar.Sub("Menu2", THISFN(SubBar));


		});

		buttons.Set([=](Bar& bar) {
			bar.Add("Open", MenuImage::Open(), [=] {}).Help("First button");
			bar.Add("Close", MenuImage::Close(), [=] {}).Help("Second button");
		});
		
		menu.WhenHelp = status;
		buttons.WhenHelp = status;
	}
};

GUI_APP_MAIN
{
	HelloGuiGiantInlineMenu app;

	app.Run();
}