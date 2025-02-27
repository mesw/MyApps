#include <CtrlLib/CtrlLib.h>

using namespace Upp;

// This program is just for experimenting and learning the U++ framework.

struct HelloGuiNoLayout : TopWindow {

	Point p;
	String text;
	MenuBar menu;

	void Close() override { delete this; }
	void Exit()
	{
		if (PromptOKCancel("Are you sure you want to quit?")) 
			Break();
	}
	void SubMenu(Bar& bar) 
	{
		bar.Add("Info",[=] { PromptOK("This is the info."); });	
		bar.Add("Exit",[=] { Exit(); });	
	}

	void MainMenu(Bar& bar)
	{
		bar.Sub("Menu", [=] (Bar& bar) { SubMenu(bar); });
	 	bar.Sub("Info", [=] (Bar& bar) { SubMenu(bar); });
	}

	void LeftDown(Point pos, dword flags) override
	{
		p.x = pos.x;
		p.y = pos.y;
		text = Format("Position clicked[%d:%d]", pos.x, pos.y);

		Refresh();
		DUMP("Left Down" << pos << flags);
	}

	void RightDown(Point pos, dword flags) override
	{
		DUMP("Right Down - opening new window");
		(new HelloGuiNoLayout)->OpenMain();
	}

	void LeftUp(Point pos, dword flags) override
	{
		p.x = pos.x;
		p.y = pos.y;
		text = Format(" Position [%d:%d]", pos.x, pos.y);

		Refresh();
		DUMP("Left Up" << pos << flags);
	}

	void MouseMove(Point pos, dword flags) override
	{
		p = pos;
		text = Format("Position[%d:%d]", pos.x, pos.y);
		DUMP("MouseMove" << pos << flags << text);
		Refresh();
	}

	void Paint(Draw& w) override
	{
		w.DrawRect(GetSize(), SBlack());
		w.DrawText(20, 20, "Hello U++", Arial(80), Red);
		w.DrawText(20, 100, "This is awesome!", Arial(60), Green);

		// this calculates the text size for the cursor position and prevents that the text is
		// extending beyond the window
		Upp::Font font = Upp::Arial(20);
		Upp::Size text_size = GetTextSize(text, font);
		if(p.x + text_size.cx > 800)
			p.x = 800 - text_size.cx;
		if(p.y + text_size.cy > 480)
			p.y = 480 - text_size.cy;

		w.DrawText(p.x, p.y, text, font, LtBlue);

		// this increases the font size of a text until it fits the window by one for each
		// paint event. It is interesting to see that there are only paint events when Refresh
		// is called.
		static int ptSize = 20;
		Upp::Font font2 = Upp::Arial(ptSize);
		Upp::String text2 = Upp::String() << "Popcorn";
		if(GetTextSize(text2, font2).cx < 800)
			ptSize++;
		if(GetTextSize(text2, font2).cx > 800)
			ptSize--;
		w.DrawText(0, 0, text2, font2, LtGreen);
	}

	HelloGuiNoLayout()
	{
		Title("My application");
		Sizeable(false);
		Zoomable(false);
		// FrameLess(true);
		SetRect(0, 0, 800, 480);
		AddFrame(menu);
		menu.Set([=](Bar& bar){MainMenu(bar);});
	}
};

GUI_APP_MAIN
{
	//// multiple windows
	(new HelloGuiNoLayout)->OpenMain();
	Ctrl::EventLoop();

	//// single window
	// HelloGuiNoLayout app;
	// app.Run();
}
