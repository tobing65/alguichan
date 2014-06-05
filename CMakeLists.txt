cmake_minimum_required(VERSION 2.6)

include(FindPkgConfig)

pkg_check_modules(ALLEGRO REQUIRED
    allegro-5.0
    allegro_image-5.0
    allegro_font-5.0
    allegro_primitives-5.0
    allegro_ttf-5.0
    )

set(GUICHAN_SOURCES
    guichan/actionevent.cpp
    guichan/basiccontainer.cpp
    guichan/cliprectangle.cpp
    guichan/color.cpp
    guichan/containerevent.cpp
    guichan/defaultfont.cpp
    guichan/defaultstyle.cpp
    guichan/event.cpp
    guichan/exception.cpp
    guichan/focushandler.cpp
    guichan/font.cpp
    guichan/genericinput.cpp
    guichan/graphics.cpp
    guichan/gui.cpp
    guichan/guichan.cpp
    guichan/image.cpp
    guichan/imagefont.cpp
    guichan/inputevent.cpp
    guichan/key.cpp
    guichan/keyevent.cpp
    guichan/keyinput.cpp
    guichan/mouseevent.cpp
    guichan/mouseinput.cpp
    guichan/rectangle.cpp
    guichan/selectionevent.cpp
    guichan/style.cpp
    guichan/widget.cpp
    guichan/widgets/button.cpp
    guichan/widgets/buttonbar.cpp
    guichan/widgets/checkbox.cpp
    guichan/widgets/container.cpp
    guichan/widgets/dropdown.cpp
    guichan/widgets/icon.cpp
    guichan/widgets/imagebutton.cpp
    guichan/widgets/label.cpp
    guichan/widgets/listbox.cpp
    guichan/widgets/radiobutton.cpp
    guichan/widgets/scrollarea.cpp
    guichan/widgets/slider.cpp
    guichan/widgets/tab.cpp
    guichan/widgets/tabbedarea.cpp
    guichan/widgets/textbox.cpp
    guichan/widgets/textfield.cpp
    guichan/widgets/window.cpp
    )

set(AL5GCN_SOURCES
    al5gcn/a5_guichan_font.cpp
    al5gcn/a5_guichan_graphics.cpp
    al5gcn/a5_guichan_image.cpp
    al5gcn/a5_guichan_imageloader.cpp
    al5gcn/a5_guichan_input.cpp
    )

include_directories(.)

add_executable(al5_example
    examples/al5_example.cpp
    ${GUICHAN_SOURCES}
    ${AL5GCN_SOURCES}
    )

target_link_libraries(al5_example ${ALLEGRO_LIBRARIES})
