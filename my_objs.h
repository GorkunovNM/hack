#ifndef $MYOBJS$H
#define $MYOBJS$H

#include <stdio.h>
#include <sys/stat.h>
#include <cstdlib>

//==================================BUTTON!==================================//
class my_button {
public:
    struct parameters {
        struct clr_theme {
            int chanel_r;
            int chanel_g;
            int chanel_b;
            int delta;
            int max;

            clr_theme(int chanel_r_in, 
                      int chanel_g_in, 
                      int chanel_b_in, 
                      int delta_in, 
                      int max_in);
        };

        struct proportions {
            int up_left_corner_x;
            int up_left_corner_y;
            int width;
            int height;
            int outline_width;
            int text_size;

            proportions(int up_left_corner_x_in, 
                        int up_left_corner_y_in, 
                        int width_in, 
                        int height_in, 
                        int outline_width_in, 
                        int text_size_in);
        };
        
        clr_theme *clr_thm;
        proportions *props;

        parameters(clr_theme *clr_thm_in, 
                   proportions *props_in);
    };

    struct graphic_options {
        struct option {
            int is_loading;
            const char *text_str;
            sf::Color fill_color;

            option(int is_loading_in, 
                   const char *text_str_in, 
                   sf::Color fill_color_in);
        };

        int options_cnt;
        option **options;

        graphic_options(int options_cnt_in);

        ~graphic_options();
    };

    int is_button_pressed;
    graphic_options *graphic_ops;

    my_button(struct parameters *params_in, sf::Font &font);

    void get_press_update(sf::Event &event);

    int graphic_update(int option_num);

    int update(int is_loading, const char *text, 
               const sf::Color &fill_color);

    void draw(sf::RenderWindow &window);
private:
    parameters *params;
    sf::Text text;
    sf::RectangleShape rectangle;
};
//===========================================================================//

//================================BACKGROUND!================================//
class my_background {
public:
    my_background(int width_in, int height_in, sf::Texture &texture);

    void draw(sf::RenderWindow &window);
private:
    int width;
    int height;
    sf::RectangleShape background;
};
//===========================================================================//

my_button::parameters::clr_theme::clr_theme(int chanel_r_in, 
                                            int chanel_g_in, 
                                            int chanel_b_in, 
                                            int delta_in, 
                                            int max_in) {
    chanel_r = chanel_r_in;
    chanel_g = chanel_g_in;
    chanel_b = chanel_b_in;
    delta = delta_in;
    max = max_in;
}

my_button::parameters::proportions::proportions(int up_left_corner_x_in, 
                                                int up_left_corner_y_in, 
                                                int width_in, 
                                                int height_in, 
                                                int outline_width_in, 
                                                int text_size_in) {
    up_left_corner_x = up_left_corner_x_in;
    up_left_corner_y = up_left_corner_y_in;
    width = width_in;
    height = height_in;
    outline_width = outline_width_in;
    text_size = text_size_in;
}

my_button::parameters::parameters(clr_theme *clr_thm_in, 
                                  proportions *props_in) {
    clr_thm = clr_thm_in;
    props = props_in;
}

my_button::graphic_options::option::option(int is_loading_in, 
                                           const char *text_str_in, 
                                           sf::Color fill_color_in) {
    is_loading = is_loading_in;
    text_str = text_str_in;
    fill_color = fill_color_in;
}

my_button::graphic_options::graphic_options(int options_cnt_in) {
    options_cnt = options_cnt_in;
    options = (option **) calloc(options_cnt, sizeof(option *));
}

my_button::graphic_options::~graphic_options() {
    free(options);
}

my_button::my_button(parameters *params_in, sf::Font &font) {
    params = params_in;
    rectangle.setSize(sf::Vector2f(params->props->width, params->props->height));
    rectangle.setFillColor(sf::Color(params->clr_thm->chanel_r, 
                                     params->clr_thm->chanel_g, 
                                     params->clr_thm->chanel_b));
    rectangle.setOutlineThickness(params->props->outline_width);
    rectangle.setOutlineColor(sf::Color(params->clr_thm->chanel_r + params->clr_thm->delta, 
                                        params->clr_thm->chanel_g + params->clr_thm->delta, 
                                        params->clr_thm->chanel_b + params->clr_thm->delta));
    rectangle.setPosition(params->props->up_left_corner_x, params->props->up_left_corner_y);
    text.setFont(font);
    text.setCharacterSize(params->props->text_size);
    text.setFillColor(sf::Color(params->clr_thm->max - params->clr_thm->chanel_r, 
                                params->clr_thm->max - params->clr_thm->chanel_g, 
                                params->clr_thm->max - params->clr_thm->chanel_b));
    text.setPosition(params->props->up_left_corner_x + params->props->outline_width, 
                     params->props->up_left_corner_y - params->props->outline_width / 2);
    is_button_pressed = 0;
}

void my_button::get_press_update(sf::Event &event) {
    is_button_pressed = 0;
    if ((event.type == sf::Event::MouseButtonPressed && 
         event.mouseButton.button == sf::Mouse::Left) && 
       ((params->props->up_left_corner_x < event.mouseButton.x && 
         event.mouseButton.x < params->props->up_left_corner_x + params->props->width) && 
        (params->props->up_left_corner_y < event.mouseButton.y && 
         event.mouseButton.y < params->props->up_left_corner_y + params->props->height))) 
        is_button_pressed = 1;
}

int my_button::graphic_update(int option_num) {
    if (option_num >= graphic_ops->options_cnt) option_num = graphic_ops->options_cnt - 1;
    text.setString(graphic_ops->options[option_num]->text_str);
    if (graphic_ops->options[option_num]->is_loading) {
        rectangle.setFillColor(sf::Color(params->clr_thm->chanel_r - params->clr_thm->delta * 2, 
                                         params->clr_thm->chanel_g - params->clr_thm->delta * 2, 
                                         params->clr_thm->chanel_b - params->clr_thm->delta * 2));
        rectangle.setOutlineColor(sf::Color(params->clr_thm->chanel_r - params->clr_thm->delta, 
                                            params->clr_thm->chanel_g - params->clr_thm->delta, 
                                            params->clr_thm->chanel_b - params->clr_thm->delta));
        return 0;
    }
    rectangle.setFillColor(graphic_ops->options[option_num]->fill_color);
    rectangle.setOutlineColor(sf::Color(params->clr_thm->chanel_r + params->clr_thm->delta, 
                                        params->clr_thm->chanel_g + params->clr_thm->delta, 
                                        params->clr_thm->chanel_b + params->clr_thm->delta));
    return 0;
}

int my_button::update(int is_loading, const char *text_str, const sf::Color &fill_color) {
    text.setString(text_str);
    if (is_loading) {
        rectangle.setFillColor(sf::Color(params->clr_thm->chanel_r - params->clr_thm->delta * 2, 
                                         params->clr_thm->chanel_g - params->clr_thm->delta * 2, 
                                         params->clr_thm->chanel_b - params->clr_thm->delta * 2));
        rectangle.setOutlineColor(sf::Color(params->clr_thm->chanel_r - params->clr_thm->delta, 
                                            params->clr_thm->chanel_g - params->clr_thm->delta, 
                                            params->clr_thm->chanel_b - params->clr_thm->delta));
        return 0;
    }
    rectangle.setFillColor(fill_color);
    rectangle.setOutlineColor(sf::Color(params->clr_thm->chanel_r + params->clr_thm->delta, 
                                        params->clr_thm->chanel_g + params->clr_thm->delta, 
                                        params->clr_thm->chanel_b + params->clr_thm->delta));
    return 0;
}

void my_button::draw(sf::RenderWindow &window) {
    window.draw(rectangle);
    window.draw(text);
}

my_background::my_background(int width_in, int height_in, sf::Texture &texture) {
    width = width_in;
    height = height_in;
    background.setSize(sf::Vector2f(width, height));
    background.setFillColor(sf::Color::White);
    background.setTexture(&texture);
    background.setTextureRect(sf::IntRect(0, 0, width, height));
}

void my_background::draw(sf::RenderWindow &window) {
    window.draw(background);
}

#endif