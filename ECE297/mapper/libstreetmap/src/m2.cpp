/* 
 * Copyright 2019 University of Toronto
 *
 * Permission is hereby granted, to use this software and associated 
 * documentation files (the "Software") in course work at the University 
 * of Toronto, or for personal use. Other uses are prohibited, in 
 * particular the distribution of the Software either publicly or to third 
 * parties.
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "m1.h"
#include "m2.h"
#include "draw_functions.h"
#include "ezgl/application.hpp"
#include "ezgl/graphics.hpp"
#include "StreetsDatabaseAPI.h"

#include "callback_definitions.h"
#include "find_in_range.h"
#include "find_intersection.h"

int colour_theme;

//Draws the map which has been loaded with load_map().
//Your main() program should do the same.
void draw_map() 
{
    colour_theme = 0;
    //Setup for drawing map
    ezgl::application::settings settings; 
    settings.main_ui_resource = "libstreetmap/resources/main.ui"; 
    settings.window_identifier = "MainWindow";
    settings.canvas_identifier = "MainCanvas";

    ezgl::application application(settings); 
    
    ezgl::rectangle initial_world({x_from_lon(min_lon), y_from_lat(min_lat)}, 
                                  {x_from_lon(max_lon), y_from_lat(max_lat)});
    application.add_canvas("MainCanvas", draw_main_canvas, initial_world);
    
    application.run(nullptr, act_on_mouse_click, nullptr, nullptr, act_on_key_release);
}
