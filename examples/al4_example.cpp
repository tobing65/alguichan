
#include "allegro.h"

#include "alguichan.hpp"
#include "al4gcn.hpp"



/*
 * List boxes and drop downs need an instance of a list model
 * in order to display a list.
 */
class DemoListModel : public gcn::ListModel
{
public:
    int getNumberOfElements()
    {
        return 5;
    }

    std::string getElementAt(int i)
    {
        switch(i)
        {
          case 0:
              return std::string("zero");
          case 1:
              return std::string("one");
          case 2:
              return std::string("two");
          case 3:
              return std::string("three");
          case 4:
              return std::string("four");
          default: // Just to keep warnings away
              return std::string("");
        }
    }
};

DemoListModel demoListModel;



int main(int argc, char **argv)
{
    // init allegro
    // We simply initialise Allegro as we would do with any Allegro application.
    allegro_init();

    int bpp = desktop_color_depth();
    if (bpp == 0)
    {
        bpp = 16;
    }

    set_color_depth(bpp);

    if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0))
    {
        if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0))
        {
            throw GCN_EXCEPTION("Unable to set graphics mode");
        }
    }

    BITMAP* screenBuffer = create_bitmap(SCREEN_W, SCREEN_H);

    if (screenBuffer == NULL)
    {
        throw GCN_EXCEPTION("Unable to create a screen buffer");
    }

    install_keyboard();
    install_mouse();
    install_timer();

    // init guichan and backend
    gcn::AllegroImageLoader image_loader;
    gcn::Image::setImageLoader(&image_loader);
    gcn::AllegroInput input;
    gcn::AllegroGraphics graphics;
    graphics.setTarget(screenBuffer);

    gcn::Gui gui;
    gui.setGraphics(&graphics);
    gui.setInput(&input);

    // setup font
    gcn::ImageFont font("fixedfont.bmp", " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    gcn::Widget::setGlobalFont(&font);

    // now setup a few widgets
    gcn::Container top;
    gcn::Label label("Label");
    gcn::Image* p_image = gcn::Image::load("gui-chan.bmp");
    gcn::Icon icon(p_image);
    gcn::Button button("Button");
    gcn::TextField textField("TextField");
    textField.setAlignment(gcn::Graphics::Center);
    textField.setBackgroundColor(gcn::Color(200,100,100));
    gcn::TextBox textBox("Multiline\nText box");
    textBox.setBackgroundColor(gcn::Color(200,100,110));
    gcn::ScrollArea textBoxScrollArea(&textBox);
    textBoxScrollArea.setWidth(200);
    textBoxScrollArea.setHeight(100);
    gcn::ListBox listBox(&demoListModel);
    listBox.setWidth(80);
    gcn::DropDown dropDown(&demoListModel);
    dropDown.setWidth(110);
    gcn::CheckBox checkBox1("Checkbox 1");
    gcn::CheckBox checkBox2("Checkbox 2");
    gcn::RadioButton radioButton1("RadioButton 1", "radiogroup", true);
    gcn::RadioButton radioButton2("RadioButton 2", "radiogroup");
    gcn::RadioButton radioButton3("RadioButton 3", "radiogroup");
    gcn::Slider slider(0, 10);
    slider.setSize(100, 10);
    gcn::Window window("I am a window  Drag me");
    window.setBaseColor(gcn::Color(255, 150, 200, 190));
    gcn::Image* p_darkbitsImage = gcn::Image::load("darkbitslogo_by_haiko.bmp");
    gcn::Icon darkbitsIcon(p_darkbitsImage);
    window.add(&darkbitsIcon);
    window.resizeToContent();
    gcn::TabbedArea tabbedArea;
    tabbedArea.setSize(200, 100);
    gcn::Button tabOneButton("A button in tab 1");
    tabbedArea.addTab("Tab 1", &tabOneButton);
    tabOneButton.setPosition(6,6);
    gcn::CheckBox tabTwoCheckBox("A check box in tab 2");
    tabbedArea.addTab("Tab 2", &tabTwoCheckBox);
    tabTwoCheckBox.setPosition(6,6);
    gcn::ImageButton tab3ImageButton;
    tab3ImageButton.setImage(p_image);
    tab3ImageButton.adjustSize();
    gcn::ScrollArea tab3ScrollArea;
    tab3ScrollArea.setSize(160,60);
    tab3ScrollArea.setContent(&tab3ImageButton);
    tabbedArea.addTab("Tab 3", &tab3ScrollArea);
    // add all widgets to the top container
    top.add(&label, 10, 10);
    top.add(&icon, 10, 30);
    top.add(&button, 200, 10);
    top.add(&textField, 250, 10);
    top.add(&textBoxScrollArea, 200, 50);
    top.add(&listBox, 200, 200);
    top.add(&dropDown, 500, 10);
    top.add(&checkBox1, 500, 130);
    top.add(&checkBox2, 500, 150);
    top.add(&radioButton1, 500, 200);
    top.add(&radioButton2, 500, 220);
    top.add(&radioButton3, 500, 240);
    top.add(&slider, 500, 300);
    top.add(&window, 50, 350);
    top.add(&tabbedArea, 400, 350);
    top.setSize(640, 480);
    // and set the top container to be the top widget for the gui
    gui.setTop(&top);

    // this is a simple game loop without exact timing
    while(!key[KEY_ESC])
    {
        // Now we let the Gui object perform its logic.
        gui.logic();
        // Now we let the Gui object draw itself.
        gui.draw();

        // We draw the mouse pointer manually, as Allegro's mouse
        // drawing code is so wierd.
        draw_sprite(screenBuffer, mouse_sprite, mouse_x-mouse_x_focus, mouse_y-mouse_y_focus);

        // Finally we update the screen.
        blit(screenBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

        rest(10);
    }

    // cleanup
    destroy_bitmap(screenBuffer);
    delete p_darkbitsImage;
    delete p_image;

    return 0;
}
END_OF_MAIN()
