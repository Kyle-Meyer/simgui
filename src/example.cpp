#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>  // GLFW for window/context creation
#include <ostream>
#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define GLFW_INCLUDE_ES3
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>


GLFWwindow* g_window;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
bool show_demo_window = true;
bool show_another_window = false;
int g_width;
int g_height;

ImU32 darkBlue  = IM_COL32(16, 22, 51, 255);  //darkblue
ImU32 darkGrey   = IM_COL32(169, 169, 169, 255); //grey


// you may be wondering what the "EM_JS" stuff here is, well a cool thing with emscripten 3
// is that you can write JS code directly in your C++ code, since WASM uses a Just in Time (JIT)
// linker, it can compile down JS code as well as C++ code, I only use it sparingly here
// to get the width and height of the HTML page

// Function used by c++ to get the width of the html canvas
EM_JS(int, canvas_get_width, (), {
  return Module.canvas.width;
});

// Function used by c++ to get the size of the html canvas
EM_JS(int, canvas_get_height, (), {
  return Module.canvas.height;
});

// this is used in the JS file tbh it doesnt really need to be here
// because I had a different vision for waht this would be, but I'm too lazy to remove it
EM_JS(void, resizeCanvas, (), {
  js_resizeCanvas();
});

void listFiles(const char* path) {
    struct stat info;
    if (stat(path, &info) != 0) {
      std::cout << "1" << std::endl;
        std::cerr << "[ERROR] Path does not exist in VFS: " << path << std::endl;
        return;
    }
    if (!(info.st_mode & S_IFDIR)) {
    std::cout << "1" << std::endl;
        std::cerr << "[ERROR] Not a directory: " << path << std::endl;
        return;
    }

    DIR* dir = opendir(path);
    if (!dir) {
      std::cout << "1" << std::endl;
        std::cerr << "[ERROR] Could not open directory: " << path << std::endl;
        return;
    }
   std::cout << "what the hell" << std::endl;
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        std::cout << "[VFS] Found file: " << entry->d_name << std::endl;
    }
    closedir(dir);
}
void on_size_changed()
{
  glfwSetWindowSize(g_window, g_width, g_height);

  ImGui::SetCurrentContext(ImGui::GetCurrentContext());
}


void setButtonStyle(ImDrawList* draw_list, float buttons_start_x, float buttons_start_y, float button_width, float button_height)
{
   float button_spacing = 20.0f; // Space between buttons
   // Define button styles (matching main window)
   ImVec4 button_bg_color = ImVec4(0.168f, 0.219f, 0.337f, 1.0f);   // #2B3856 (Main Window Background)
   ImVec4 button_border_color = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);  // Grey border color
   ImVec4 button_hover_color = ImVec4(0.2f, 0.3f, 0.5f, 1.0f);      // Slightly lighter on hover
   ImVec4 button_active_color = ImVec4(0.15f, 0.18f, 0.3f, 1.0f);   // Darker when clicked

   // Push styles before rendering buttons
   ImGui::PushStyleColor(ImGuiCol_Button, button_bg_color);
   ImGui::PushStyleColor(ImGuiCol_ButtonHovered, button_hover_color);
   ImGui::PushStyleColor(ImGuiCol_ButtonActive, button_active_color);
   ImGui::PushStyleColor(ImGuiCol_Border, button_border_color);

   // Set cursor position for Button 1 (slightly to the left)
   ImGui::SetCursorPosX(buttons_start_x);
   ImGui::SetCursorPosY(buttons_start_y);
   // Set cursor for Button 1 (slightly to the left)
   ImVec2 button1_pos = ImGui::GetCursorScreenPos(); // Get screen-space position of the button
   // Draw border for Button 1
   draw_list->AddRectFilled(ImVec2(button1_pos.x, button1_pos.y),
                            ImVec2(button1_pos.x + button_width, button1_pos.y + 3),
                            darkGrey);    // Top border
   draw_list->AddRectFilled(ImVec2(button1_pos.x, button1_pos.y),
                               ImVec2(button1_pos.x + 3, button1_pos.y + button_height),
                               darkGrey);  // Left border
   draw_list->AddRectFilled(ImVec2(button1_pos.x + button_width - 3, button1_pos.y),
                               ImVec2(button1_pos.x + button_width, button1_pos.y + button_height),
                               darkBlue); // Right border
   draw_list->AddRectFilled(ImVec2(button1_pos.x, button1_pos.y + button_height - 3),
                               ImVec2(button1_pos.x + button_width, button1_pos.y + button_height),
                               darkBlue); // Bottom border

   if (ImGui::Button("Button 1", ImVec2(button_width, button_height))) 
   {
         // Handle Button 1 click
   }

   // Move to the right for Button 2
   ImGui::SameLine(0, button_spacing);  // Apply spacing
   ImVec2 button2_pos = ImGui::GetCursorScreenPos();
   // Draw border for Button 2
   draw_list->AddRectFilled(ImVec2(button2_pos.x, button2_pos.y),
                               ImVec2(button2_pos.x + button_width, button2_pos.y + 3),
                               darkGrey);    // Top border
   draw_list->AddRectFilled(ImVec2(button2_pos.x, button2_pos.y),
                               ImVec2(button2_pos.x + 3, button2_pos.y + button_height),
                               darkGrey);  // Left border
   draw_list->AddRectFilled(ImVec2(button2_pos.x + button_width - 3, button2_pos.y),
                               ImVec2(button2_pos.x + button_width, button2_pos.y + button_height),
                               darkBlue); // Right border
   draw_list->AddRectFilled(ImVec2(button2_pos.x, button2_pos.y + button_height - 3),
                               ImVec2(button2_pos.x + button_width, button2_pos.y + button_height),
                               darkBlue); // Bottom border


   if (ImGui::Button("Button 2", ImVec2(button_width, button_height))) 
   {
      // Handle Button 2 click
   }
     
   ImGui::PopStyleColor(4);
}

void RenderSubWindow(ImVec2 win_size, ImVec2 sub_size) {
    static int selected_row = -1;  // Track selected row

    // Define colors
    ImVec4 rowColor = ImVec4(0.094f, 0.129f, 0.302f, 1.0f);      // Dark blue for rows
    ImVec4 headerColor = ImVec4(0.168f, 0.219f, 0.337f, 1.0f);   // Darker blue for header

    // Center the sub-window
    ImGui::SetCursorPosX((win_size.x - sub_size.x) * 0.5f);
    ImGui::SetCursorPosY((win_size.y - sub_size.y - 90) * 0.5f);

    // Create the sub-window
    ImGui::BeginChild("SubWindow", sub_size, false);

    // Retrieve draw list
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    // Get child window position & size **after** it has been drawn
    ImVec2 sub_min = ImGui::GetWindowPos();
    ImVec2 sub_max = ImVec2(sub_min.x + sub_size.x, sub_min.y + sub_size.y);

    // Override row colors (ensures all rows have same background color)
    ImGui::PushStyleColor(ImGuiCol_TableRowBg, rowColor);
    ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, rowColor);

    // Start the table (Disable borders inside the table)
    if (ImGui::BeginTable("USV Table", 6, ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoBordersInBody)) {
        
        // **Draw the header background manually**
        ImVec2 header_min = ImGui::GetCursorScreenPos();
        ImVec2 header_max = ImVec2(header_min.x + sub_size.x, header_min.y + 25.0f); // Assuming 25px height for header
        draw_list->AddRectFilled(header_min, header_max, ImGui::GetColorU32(headerColor));

        // Define column headers
        ImGui::TableSetupColumn("USV ID");
        ImGui::TableSetupColumn("USV-TN");
        ImGui::TableSetupColumn("Weapon Control Status");
        ImGui::TableSetupColumn("Status");
        ImGui::TableSetupColumn("Inv");
        ImGui::TableSetupColumn("Comm Status");

        // Render the header row **after drawing the background**
        ImGui::TableHeadersRow();

        // Populate table rows
        for (int i = 0; i < 50; i++) {
            ImGui::TableNextRow();

            // Make the row selectable
            ImGui::TableSetColumnIndex(0);
            bool selected = (selected_row == i);
            char usv_id[16];  
            std::snprintf(usv_id, sizeof(usv_id), "USV-%d", i);
            if (ImGui::Selectable(usv_id, selected, ImGuiSelectableFlags_SpanAllColumns)) {
                selected_row = i;
            }

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("80001");

            ImGui::TableSetColumnIndex(2);
            ImGui::Text("Ownship/Controlled");

            ImGui::TableSetColumnIndex(3);
            ImGui::Text("X");

            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%d", i);

            ImGui::TableSetColumnIndex(5);
            ImGui::Text("Down");
        }

        ImGui::EndTable();
    }

    // Restore original row colors
    ImGui::PopStyleColor(2);

    ImGui::EndChild();

    // Draw the custom border after the child window
    ImU32 subRightColor  = IM_COL32(169, 169, 169, 255);   // Grey
    ImU32 subLeftColor   = IM_COL32(16, 22, 51, 255);      // Blue
    ImU32 subTopColor    = IM_COL32(16, 22, 51, 255);      // Blue
    ImU32 subBottomColor = IM_COL32(169, 169, 169, 255);   // Grey
    int32_t borderOffset = 2;
    float thickness = 2.0f;

    // Top border
    draw_list->AddRectFilled(ImVec2(sub_min.x, sub_min.y - borderOffset),
                             ImVec2(sub_max.x, sub_min.y + thickness - borderOffset),
                             subTopColor);
    // Left border 
    draw_list->AddRectFilled(ImVec2(sub_min.x - borderOffset, sub_min.y - borderOffset),
                             ImVec2(sub_min.x + thickness - borderOffset, sub_max.y),
                             subLeftColor);
    // Right border
    draw_list->AddRectFilled(ImVec2(sub_max.x - thickness + borderOffset, sub_min.y - borderOffset),
                             ImVec2(sub_max.x + borderOffset, sub_max.y),
                             subRightColor);
    // Bottom border
    draw_list->AddRectFilled(ImVec2(sub_min.x - borderOffset, sub_max.y - thickness + borderOffset), 
                             ImVec2(sub_max.x - borderOffset + (borderOffset * 2), sub_max.y + borderOffset), 
                             subBottomColor);
}

void loop()
{
  int width = canvas_get_width();
  int height = canvas_get_height();

  if (width != g_width || height != g_height)
  {
    g_width = width;
    g_height = height;
    on_size_changed();
  }

  glfwPollEvents();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // 1. Show a simple window.
  // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
  {
      static float f = 0.0f;
      static int counter = 0;
      ImGui::Text("Hello, world! Dingo");                           // Display some text (you can use a format string too)
      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

      ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
      ImGui::Checkbox("Another Window", &show_another_window);

      if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
          counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  }

   //control esque window
   {


      static float f = 0.0f;
      static int counter = 0;
            // Draw main window borders
      ImDrawList* draw_list = ImGui::GetForegroundDrawList();
      float thickness = 3.0f;
      // Apply styles
      ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0)); // Hide default border
      ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.168f, 0.219f, 0.337f, 1.0f)); // #2B3856

      ImGui::Begin("Control Service", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

      // Get window dimensions
      ImVec2 win_pos = ImGui::GetWindowPos();
      ImVec2 win_size = ImGui::GetWindowSize();

      // Define sub-window size
      ImVec2 sub_size = ImVec2(win_size.x * 0.6f, win_size.y * 0.6f);
      ImVec2 sub_pos = ImVec2(
         win_pos.x + (win_size.x - sub_size.x) * 0.5f,
         win_pos.y + (win_size.y - sub_size.y) * 0.5f
      );

      // Push background color for sub-window
      ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.094f, 0.129f, 0.302f, 1.0f)); // #18214D
      // Calculate the position for the text above the sub-window
      ImGui::SetCursorPosX((win_size.x - sub_size.x) * 0.5f); // Center horizontally
      ImGui::SetCursorPosY((win_size.y - sub_size.y - 90) * 0.5f - 25); // Move up a bit

      // Render the text
      /*
      ImGui::Text("\tUSV ID\tUSV-TN\tWeapon Control Status\tStatus Inv\tComm Status");
      // Create the sub-window
      ImGui::SetCursorPosX((win_size.x - sub_size.x) * 0.5f);  // Center horizontally
      ImGui::SetCursorPosY((win_size.y - sub_size.y - 90) * 0.5f);  // Center vertically
      ImGui::BeginChild("SubWindow", sub_size, false, ImGuiWindowFlags_AlwaysVerticalScrollbar);

      // UI inside sub-window
      for (int i = 0; i < 50; i++) 
      {
          ImGui::Text("\tUSV-123\t80001\tOwnship/Controlled\tX\t%d\tDown", i);
      }

      // Get sub-window min/max for borders
      ImVec2 sub_min = ImGui::GetWindowPos();
      ImVec2 sub_max = ImVec2(sub_min.x + sub_size.x, sub_min.y + sub_size.y);

      // End sub-window
      ImGui::EndChild();
      */ 
      RenderSubWindow(win_size, sub_size);
      ImVec2 sub_min = ImGui::GetWindowPos();
      ImVec2 sub_max = ImVec2(sub_min.x + sub_size.x, sub_min.y + sub_size.y);
      //first set of Buttons

      float button_width = 120.0f;  // Width of each button
      float button_height = 30.0f;
      float button_spacing = 20.0f; // Space between buttons

      // Calculate total width occupied by buttons
      float total_buttons_width = (button_width * 2) + button_spacing;


      // Compute starting X position to center the buttons below the subwindow
      float buttons_start_x = (win_size.x - total_buttons_width) * 0.5f; 
      float buttons_start_y = (win_size.y - sub_size.y - 90) * 0.5f + sub_size.y + 15;  // Position right below subwindow

      setButtonStyle(draw_list, buttons_start_x, buttons_start_y, button_width, button_height);
      //end of buttons

      ImGui::PopStyleColor(); // Restore sub-window background color

      ImGui::End(); // Finish the main window

      

      // Main window border colors
      ImU32 rightColor  = IM_COL32(16, 22, 51, 255);  //darkblue
      ImU32 leftColor   = IM_COL32(169, 169, 169, 255); //grey
      ImU32 topColor    = IM_COL32(169, 169, 169, 255);
      ImU32 bottomColor = IM_COL32(16, 22, 51, 255);

      // Draw borders for the main window
      draw_list->AddRectFilled(ImVec2(win_pos.x, win_pos.y), 
                               ImVec2(win_pos.x + win_size.x, win_pos.y + thickness), 
                               topColor);
      draw_list->AddRectFilled(ImVec2(win_pos.x, win_pos.y), 
                               ImVec2(win_pos.x + thickness, win_pos.y + win_size.y), 
                               leftColor);
      draw_list->AddRectFilled(ImVec2(win_pos.x + win_size.x - thickness, win_pos.y), 
                               ImVec2(win_pos.x + win_size.x, win_pos.y + win_size.y), 
                               rightColor);
      draw_list->AddRectFilled(ImVec2(win_pos.x, win_pos.y + win_size.y - thickness), 
                               ImVec2(win_pos.x + win_size.x, win_pos.y + win_size.y), 
                               bottomColor);

   ImGui::PopStyleColor(2); // Restore styles

   } //end of control esque window
  // 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
  if (show_another_window)
  {
      ImGui::Begin("Another Window", &show_another_window);
      ImGui::Text("Hello from another window!");
      if (ImGui::Button("Close Me"))
          show_another_window = false;
      ImGui::End();
  }

  // 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
  if (show_demo_window)
  {
      ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
      ImGui::ShowDemoWindow(&show_demo_window);
  }
  
  //main render call
  ImGui::Render();

  int display_w, display_h;
  glfwMakeContextCurrent(g_window);
  glfwGetFramebufferSize(g_window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  glfwMakeContextCurrent(g_window);
}


int init_gl()
{
  if( !glfwInit() )
  {
      fprintf( stderr, "Failed to initialize GLFW\n" );
      return 1;
  }

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

  // Open a window and create its OpenGL context
  int canvasWidth = g_width;
  int canvasHeight = g_height;
  g_window = glfwCreateWindow(canvasWidth, canvasHeight, "WebGui Demo", NULL, NULL);
  if( g_window == NULL )
  {
      fprintf( stderr, "Failed to open GLFW window.\n" );
      glfwTerminate();
      return -1;
  }
  glfwMakeContextCurrent(g_window); // Initialize GLEW

  return 0;
}


int init_imgui()
{
  // Setup Dear ImGui binding
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(g_window, true);
  ImGui_ImplOpenGL3_Init();

  // Setup style
  ImGui::StyleColorsDark();

  ImGuiIO& io = ImGui::GetIO();

  // Load Fonts
  /*
  io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 23.0f);
  io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 18.0f);
  io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 26.0f);
  io.Fonts->AddFontFromFileTTF("data/xkcd-script.ttf", 32.0f);
  io.Fonts->AddFontDefault();
*/
   std::cout << "HEY" << std::endl;
   listFiles("/resources");
   io.Fonts->AddFontFromFileTTF("/resources/LucidiaSansTypeWriter.ttf", 16.0f); // 16.0f is the font size
  resizeCanvas();

  return 0;
}


int init()
{
  init_gl();
  init_imgui();
  return 0;
}


void quit()
{
  glfwTerminate();
}


int main(int argc, char** argv)
{
  g_width = canvas_get_width();
  g_height = canvas_get_height();
  if (init() != 0) return 1;
  std::cout << "pimbo " << std::endl;
  #ifdef __EMSCRIPTEN__
  emscripten_set_main_loop(loop, 0, 1);
  #endif

  quit();

  return 0;
}

