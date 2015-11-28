
#include "allegro5/allegro.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"

#include "alguichan.hpp"
#include "al5gcn.hpp"



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
    al_init();
    al_install_keyboard();
    al_install_mouse();

    al_init_font_addon();
    al_init_image_addon();
    al_init_primitives_addon();

    // create display
    al_set_new_display_flags(ALLEGRO_WINDOWED|ALLEGRO_RESIZABLE);
    ALLEGRO_DISPLAY* p_display = al_create_display(640, 480);

    // init guichan and backend
    gcn::Allegro5ImageLoader image_loader;
    gcn::Image::setImageLoader(&image_loader);
    gcn::Allegro5Input input;
    gcn::Allegro5Graphics graphics;

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
    gcn::Button tab1Button("A button in tab 1");
    tabbedArea.addTab("Tab 1", &tab1Button);
    tab1Button.setPosition(6,6);
    gcn::CheckBox tab2CheckBox("A check box in tab 2");
    tabbedArea.addTab("Tab 2", &tab2CheckBox);
    tab2CheckBox.setPosition(6,6);
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

    // now we need to setup allegro for a simple game loop
    ALLEGRO_EVENT_QUEUE* p_eventQueue = al_create_event_queue();
    al_register_event_source(p_eventQueue, al_get_keyboard_event_source());
    al_register_event_source(p_eventQueue, al_get_mouse_event_source());
    ALLEGRO_DISPLAY*const pdisplay = al_get_current_display();
    al_register_event_source(p_eventQueue, al_get_display_event_source(pdisplay));
    // timer gives 30 fps, we draw at every timer event
    ALLEGRO_TIMER* p_timer = al_create_timer(1.0/30);
    al_register_event_source(p_eventQueue, al_get_timer_event_source(p_timer));
    al_start_timer(p_timer);

    // this is a simple game loop, that assumes that both logic() and draw() below execute quite fast
    while(true)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(p_eventQueue, &event);
        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            break;
        if(event.type < ALLEGRO_EVENT_TIMER)
        {
            // intercept ESC to quit the game loop
            if(event.type == ALLEGRO_EVENT_KEY_DOWN)
                if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    break;
            input.pushInput(event);
            continue;
        }
        if(event.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
        {
            al_resize_display(al_get_current_display(), event.display.width, event.display.height);
            gui.getTop()->setSize(event.display.width, event.display.height);
        }
        if(event.type == ALLEGRO_EVENT_TIMER && event.timer.source == p_timer)
        {
            tab3ImageButton.setEnabled(tab2CheckBox.isSelected());
            // Now we let the Gui object perform its logic.
            gui.logic();
            // Now we let the Gui object draw itself.
            gui.draw();

            al_flip_display();
        }
    }

    // cleanup
    al_destroy_timer(p_timer);
    al_destroy_event_queue(p_eventQueue);
    al_destroy_display(p_display);
    delete p_darkbitsImage;
    delete p_image;

    return 0;
}
