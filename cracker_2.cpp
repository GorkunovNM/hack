/*
COMPILE INFO
if debug uncomment "#define DEBUG_MODE"
if release comment "#define DEBUG_MODE"
use: "g++ -c cracker_2.cpp" 
and
if debug use: "g++ cracker_2.o -o cracker_2 -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system"
if release use: "g++ cracker_2.o -o cracker_2 -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system font_memory.h texture_memory.h sound_buffer_memory.h"
*/

#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <string.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "file_commander.h"

//===============================DEBUG  SWITCH===============================//
#define DEBUG_MODE
//---------------------------------------------------------------------------//
#ifndef DEBUG_MODE
#define RELEASE
#else
#define DEBUG
#endif
//===========================================================================//

#ifdef RELEASE
//==================================SOURCE!==================================//
#include "font_memory.h"
#include "texture_memory.h"
#include "sound_buffer_memory.h"
//===========================================================================//
#endif

//=================================CONSTANTS=================================//
const size_t hash_expected = 13842028284762129553;
const int PASS_CHECK_NAME_SIZE = 20;
const int POS_1 = 0x101b;
const unsigned char REPLACE_BYTE_1 = 0xeb;
const int POS_2 = 0x101c;
const unsigned char REPLACE_BYTE_2 = 0x51;

#ifdef DEBUG
const int FONT_NAME_SIZE    = 20;
const int TEXTURE_NAME_SIZE = 20;
const int SOUND_NAME_SIZE   = 20;
#endif

const int COLOR_THEME_R = 150;
const int COLOR_THEME_G = 50;
const int COLOR_THEME_B = 230;
const int COLOR_THEME_DELTA = 25;
const int COLOR_THEME_MAX = 255;

const int BUTTON_UPPER_LEFT_CORNER_X = 60;
const int BUTTON_UPPER_LEFT_CORNER_Y = 330;
const int BUTTON_WIDTH  = 600;
const int BUTTON_HEIGHT = 40;
const int BUTTON_OUTLINE = 10;

const int WINDOW_SIZE = 720;

const int TEXT_SIZE = BUTTON_HEIGHT;
const int MODES_CNT = 8;

const int SOUND_VOLUME = 10;

const double TIME_TO_WAIT = 1.0;
//===========================================================================//

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

//=============================SOURCE  FUNCTIONS=============================//
int source_load(sf::Font &font, sf::Texture &texture, 
                sf::SoundBuffer &sound_buffer);
//===========================================================================//

//==============================SOUND FUNCTIONS==============================//
void my_play(sf::Sound &sound, sf::SoundBuffer &sound_buffer, 
             int sound_volume);
//===========================================================================//

//==============================TIMER FUNCTIONS==============================//
int timer_checker(double control_point, double sec_to_wait);
//===========================================================================//

//==============================HASH  FUNCTIONS==============================//
size_t hash_func(unsigned char *word, size_t word_len);
//===========================================================================//

//==============================HACK  FUNCTIONS==============================//
int crack(FILE *code_in, unsigned char *buf);
//===========================================================================//

int main() {
    sf::Font font;
    sf::Texture texture;
    sf::SoundBuffer sound_buffer;
    int source_load_res = 0;
    const char* crash_reports[] = {"UNEXPECTED_ERROR\n",
                                   "Crash report: font load error\n",
                                   "Crash report: texture load error\n",
                                   "Crash report: sound load error\n",
                                   ""};
    if (source_load_res = source_load(font, texture, sound_buffer)) {
        printf("%s\n", crash_reports[source_load_res]);
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), 
                            "cracker_v0.2");

    my_button::parameters::clr_theme *main_clr_theme = new my_button::parameters::clr_theme(COLOR_THEME_R, 
                                                                                            COLOR_THEME_G, 
                                                                                            COLOR_THEME_B, 
                                                                                            COLOR_THEME_DELTA, 
                                                                                            COLOR_THEME_MAX);
    my_button::parameters::proportions *main_props = new my_button::parameters::proportions(BUTTON_UPPER_LEFT_CORNER_X, 
                                                                                            BUTTON_UPPER_LEFT_CORNER_Y, 
                                                                                            BUTTON_WIDTH, 
                                                                                            BUTTON_HEIGHT, 
                                                                                            BUTTON_OUTLINE,
                                                                                            TEXT_SIZE);
    my_button::parameters *main_params = new my_button::parameters(main_clr_theme, main_props);
    my_button *button = new my_button(main_params, font);
    my_button::graphic_options *main_graphic_ops = new my_button::graphic_options(MODES_CNT);
    const char* button_texts[] = {"Loading...",
                                  "Program not found!",
                                  "Cracked.",
                                  "Crack failed!",
                                  "Wrong program!",
                                  "Try again?",
                                  "More RAM required!",
                                  "Crack it.",
                                  "UNEXPECTED_ERROR"};
    for (int option_itt = 0; option_itt < main_graphic_ops->options_cnt; ++option_itt) {
        main_graphic_ops->options[option_itt] = new my_button::graphic_options::option(0, 
                                                                                       button_texts[option_itt], 
                                                                                       sf::Color::Red);
    }
    main_graphic_ops->options[0]->is_loading = 1;
    main_graphic_ops->options[2]->fill_color = sf::Color::Green;
    main_graphic_ops->options[5]->fill_color = sf::Color(COLOR_THEME_R, 
                                                         COLOR_THEME_G, 
                                                         COLOR_THEME_B);
    main_graphic_ops->options[7]->fill_color = sf::Color(COLOR_THEME_R, 
                                                         COLOR_THEME_G, 
                                                         COLOR_THEME_B);
    button->graphic_ops = main_graphic_ops;
    my_background *bg = new my_background(WINDOW_SIZE, WINDOW_SIZE, texture);
    sf::Sound sound;
    my_play(sound, sound_buffer, SOUND_VOLUME);

    //==============================PRE-LOGIC BLOCK==============================//
    int button_mode = 7;
    char pass_check[PASS_CHECK_NAME_SIZE] = "break2";
    FILE *code_in = NULL;
    double control_point = 0.0;
    //===========================================================================//

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else button->get_press_update(event);
        }

        //================================LOGIC BLOCK================================//
        switch (button_mode) {
            case 0: code_in = fopen(pass_check, "r+b");
                    if (!code_in) {
                        button_mode = 1; 
                        control_point = (double) clock() / CLOCKS_PER_SEC;
                    }
                    else {
                        size_t code_in_len = file_len(pass_check);
                        unsigned char *buf = (unsigned char *) calloc(code_in_len + 10, 
                                                                      sizeof(unsigned char));
                        if (!buf) {
                            button_mode = 6; 
                            control_point = (double) clock() / CLOCKS_PER_SEC;
                        }
                        else {
                            fread(buf, sizeof(unsigned char), code_in_len, code_in);
                            size_t hash = hash_func(buf, code_in_len);
                            if (hash == hash_expected) {
                                if (crack(code_in, buf)) button_mode = 2;
                                else {
                                    button_mode = 3; 
                                    control_point = (double) clock() / CLOCKS_PER_SEC;
                                }
                            }
                            else {
                                button_mode = 4; 
                                control_point = (double) clock() / CLOCKS_PER_SEC;
                            }
                            free(buf);
                            buf = NULL;
                            fclose(code_in);
                        }
                    }
                    break;
            case 1: if (timer_checker(control_point, TIME_TO_WAIT)) button_mode = 5; break;
            case 2:                                                                  break;
            case 3: if (timer_checker(control_point, TIME_TO_WAIT)) button_mode = 5; break;
            case 4: if (timer_checker(control_point, TIME_TO_WAIT)) button_mode = 5; break;
            case 5: if (button->is_button_pressed) button_mode = 0;                  break;
            case 6: if (timer_checker(control_point, TIME_TO_WAIT)) button_mode = 5; break;
            case 7: if (button->is_button_pressed) button_mode = 0;                  break;
            default: printf("Crash report: undefined button_mode = %d\n", button_mode);
        }
        //===========================================================================//

        button->graphic_update(button_mode);

        window.clear();
        
        bg->draw(window);
        button->draw(window);

        window.display();
    }
    
    delete main_clr_theme;
    delete main_props;
    delete main_params;
    for (int option_itt = 0; option_itt < main_graphic_ops->options_cnt; ++option_itt) {
        delete main_graphic_ops->options[option_itt];
    }
    delete main_graphic_ops;
    delete bg;
    delete button;
    return 0;
}

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

int source_load(sf::Font &font, sf::Texture &texture, sf::SoundBuffer &sound_buffer) {
    #ifdef RELEASE
    if (!font.loadFromMemory(FONT_MEM, FONT_MEM_SIZE)) return 1;
    if (!texture.loadFromMemory(TEXTURE_MEM, TEXTURE_MEM_SIZE)) return 2;
    if (!sound_buffer.loadFromMemory(SOUND_BUF_MEM, SOUND_BUF_MEM_SIZE)) return 3;
    #endif

    #ifdef DEBUG
    char font_name[FONT_NAME_SIZE] = "FORTSSH_.ttf";
    int is_font_loaded = font.loadFromFile(font_name);
    if (!is_font_loaded) {
        printf("Crash report: font %s not found\n", font_name); 
        return 4;
    }

    char texture_name[TEXTURE_NAME_SIZE] = "texture.png";
    if (!texture.loadFromFile(texture_name)) {
        printf("Crash report: texture %s not found\n", texture_name); 
        return 4;
    }

    char sound_name[SOUND_NAME_SIZE] = "Dubmood.flac";
    if (!sound_buffer.loadFromFile(sound_name)) {
        printf("Crash report: sound %s not found\n", sound_name); 
        return 4;
    }
    #endif
    return 0;
}

void my_play(sf::Sound &sound, sf::SoundBuffer &sound_buffer, int sound_volume) {
    sound.setBuffer(sound_buffer);
    sound.setLoop(true);
    sound.setVolume(sound_volume);
    sound.play();
}

int timer_checker(double control_point, double sec_to_wait) {
    if ((double) clock() / CLOCKS_PER_SEC - control_point >= sec_to_wait) return 1;
    return 0;
}

size_t hash_func(unsigned char *word, size_t word_len) {
    size_t ans = 0;
    size_t word_pos = 0;
    while (word_pos < word_len) {
        ans = left_cycl_shft_m_to_l(ans) ^ *(word + word_pos);
        ++word_pos;
    }
    //printf("%lu\n", ans);
    return ans;
}

int crack(FILE *code_in, unsigned char *buf) {
    fseek(code_in, POS_1, SEEK_SET);
    fputc(REPLACE_BYTE_1, code_in);
    fseek(code_in, POS_2, SEEK_SET);
    fputc(REPLACE_BYTE_2, code_in);
    return 1;
}
