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
   float button_spacing = 60.0f; // Space between buttons
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

   if (ImGui::Button("Take Control", ImVec2(button_width, button_height))) 
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


   if (ImGui::Button("Release Control", ImVec2(button_width, button_height))) 
   {
      // Handle Button 2 click
   }
     
   ImGui::PopStyleColor(4);
}


void RenderSubWindow(ImVec2 win_size, ImVec2 sub_size) {
    static int selected_row = -1;  // Track selected row
    static int right_clicked_row = -1; // Track right-clicked row
    // Define colors
    ImVec4 rowColor = ImVec4(0.094f, 0.129f, 0.302f, 1.0f);      // Dark blue for rows
    ImVec4 headerColor = ImVec4(0.168f, 0.219f, 0.337f, 1.0f);   // Darker blue for header

    // Center the sub-window
    ImGui::SetCursorPosX((win_size.x - sub_size.x) * 0.5f);
    ImGui::SetCursorPosY((win_size.y - sub_size.y - 90) * 0.5f);

    // Create the sub-window
    ImGui::BeginChild("SubWindow", sub_size, false);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Define column widths
    float columnWidths[] = { 70.0f, 70.0f, 200.0f, 60.0f, 50.0f, 100.0f };
    float totalTableWidth = 0.0f;
    for (float w : columnWidths) totalTableWidth += w;

    // Center header buttons
    float availableWidth = ImGui::GetContentRegionAvail().x;
    float offsetX = (availableWidth - totalTableWidth) * 0.5f;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX - 10);

    // Render buttons (header)
    ImGui::PushStyleColor(ImGuiCol_Button, headerColor);
    if (ImGui::Button("USV ID", ImVec2(columnWidths[0], 0))) {  }
    ImGui::SameLine();
    if (ImGui::Button("USV-TN", ImVec2(columnWidths[1], 0))) {  }
    ImGui::SameLine();
    if (ImGui::Button("Weapon Control Status", ImVec2(columnWidths[2], 0))) {  }
    ImGui::SameLine();
    if (ImGui::Button("Status", ImVec2(columnWidths[3], 0))) {  }
    ImGui::SameLine();
    if (ImGui::Button("Inv", ImVec2(columnWidths[4], 0))) {  }
    ImGui::SameLine();
    if (ImGui::Button("Comm Status", ImVec2(columnWidths[5], 0))) { }
    ImGui::PopStyleColor();

    ImGui::Spacing();

    // Override row colors
    ImGui::PushStyleColor(ImGuiCol_TableRowBg, rowColor);
    ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, rowColor);
    ImVec2 table_min;
    ImVec2 table_max;
    // Capture table position BEFORE rendering
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offsetX - 5);
    

    
    if (ImGui::BeginTable("USV Table", 6, ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY, ImVec2(totalTableWidth + 40, 300))) {
    ImGui::TableSetupColumn("USV ID", ImGuiTableColumnFlags_WidthFixed, columnWidths[0]);
    ImGui::TableSetupColumn("USV-TN", ImGuiTableColumnFlags_WidthFixed, columnWidths[1]);
    ImGui::TableSetupColumn("Weapon Control Status", ImGuiTableColumnFlags_WidthFixed, columnWidths[2]);
    ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed, columnWidths[3]);
    ImGui::TableSetupColumn("Inv", ImGuiTableColumnFlags_WidthFixed, columnWidths[4]);
    ImGui::TableSetupColumn("Comm Status", ImGuiTableColumnFlags_WidthFixed, columnWidths[5]);

    for (int i = 0; i < 50; i++) {
        ImGui::TableNextRow();

        bool rowClicked = false;  // Flag to check if the row was clicked
        for (int col = 0; col < 6; col++) {
            ImGui::TableSetColumnIndex(col);

            // Use the actual column width instead of GetContentRegionAvail()
            float cellWidth = columnWidths[col];  

            // Format text content
            const char* text = nullptr;
            char buffer[16];
            switch (col) {
                case 0: std::snprintf(buffer, sizeof(buffer), "USV-%d", i); text = buffer; break;
                case 1: text = "80001"; break;
                case 2: text = "Ownship/Controlled"; break;
                case 3: text = "X"; break;
                case 4: std::snprintf(buffer, sizeof(buffer), "%d", i); text = buffer; break;
                case 5: text = "Down"; break;
            }

            // **Fix: Reset cursor to align text properly**
            ImVec2 textSize = ImGui::CalcTextSize(text);
            float textOffsetX = (cellWidth - textSize.x) * 0.5f;
            textOffsetX = (textOffsetX > 0) ? textOffsetX : 0; // Prevent negative offset

            // **Fix: Reset cursor before text is drawn**
            float initialCursorX = ImGui::GetCursorPosX();
            ImGui::SetCursorPosX(initialCursorX + textOffsetX); 

            // **Make row selectable**
            if (ImGui::Selectable(text, selected_row == i, ImGuiSelectableFlags_SpanAllColumns)) {
                selected_row = i; // Update selected row
            }

            // **Only trigger once per row, not per column**
            if (!rowClicked && ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
              right_clicked_row = i;
              ImGui::OpenPopup("RightClickMenu");
              std::cout << "open the pop up damnit " << right_clicked_row << std::endl;
              rowClicked = true;  // Prevent multiple popups per row
            } 
        }
        if (rowClicked) {
          ImGui::SetNextWindowPos(ImGui::GetMousePos());  // Position the popup at cursor
          std::cout << "set next pos" << std::endl;
        }
    }
    // **Keep the popup inside the loop for proper scope**
    // this is such a pain, I'm not gonna bother styling this for right now
    if (ImGui::BeginPopup("RightClickMenu")) {
      ImGui::Text("Options for USV-%d", right_clicked_row);
      ImGui::Separator();
      if (ImGui::MenuItem("Pri Hook")) {
        printf("Pri Hook selected for USV-%d\n", right_clicked_row);
      }
      if (ImGui::MenuItem("Sec Hook")) {
        printf("Sec Hook selected for USV-%d\n", right_clicked_row);
      }
      ImGui::EndPopup();
    }
    ImGui::EndTable();
    
  }
    

    ImGui::PopStyleColor(2);
    ImGui::EndChild();

    // Border color scheme (unchanged)
    ImU32 subRightColor  = IM_COL32(169, 169, 169, 255);   // Grey
    ImU32 subLeftColor   = IM_COL32(16, 22, 51, 255);      // Blue
    ImU32 subTopColor    = IM_COL32(16, 22, 51, 255);      // Blue
    ImU32 subBottomColor = IM_COL32(169, 169, 169, 255);   // Grey
    //ImU32 subRightColor  = IM_COL32(255, 0, 0, 255);   // Grey
    //ImU32 subLeftColor   = IM_COL32(0, 255, 0, 255);      // Blue
    //ImU32 subTopColor    = IM_COL32(0, 0, 255, 255);      // Blue
    //ImU32 subBottomColor = IM_COL32(50, 50, 50, 255);   // Grey

    int borderOffset = 2;
    float thickness = 3.0f;

    // Draw borders **only around the table**
    float leftOffset = -10;
    float topOffset = -10;
    float rightOffset = -10;
    float bottomOffset = -10;
    table_max = ImGui::GetItemRectMax();
    table_min = ImGui::GetItemRectMin();
    
    //TODO make this better
    table_max = ImVec2(table_max.x - 20, table_max.y - 27);
    table_min = ImVec2(table_min.x + 48 , table_min.y + 28);
    // Compute the other corners
    ImVec2 topLeft = ImVec2(table_min.x, table_min.y);
    ImVec2 topRight = ImVec2(table_max.x, table_min.y);
    ImVec2 bottomLeft = ImVec2(table_min.x, table_max.y);
    ImVec2 bottomRight = ImVec2(table_max.x, table_max.y);
    
    //just in case the table is not the full size 
    draw_list->AddRectFilled(topLeft, bottomRight, IM_COL32(24, 33, 77, 255));
    draw_list->AddLine(topLeft, topRight, subTopColor, thickness);        // Top border (blue)
    draw_list->AddLine(topLeft, bottomLeft, subLeftColor, thickness);      // Left border (blue)
    draw_list->AddLine(topRight, bottomRight, subRightColor, thickness);   // Right border (grey)
    draw_list->AddLine(bottomLeft, bottomRight, subBottomColor, thickness); // Bottom border (grey)
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

      // Set the default size of the window before calling Begin()
      ImGui::SetNextWindowSize(ImVec2(1100, 600), ImGuiCond_FirstUseEver); 
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
     // ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.094f, 0.129f, 0.302f, 1.0f)); // #18214D
      // Calculate the position for the text above the sub-window
      ImGui::SetCursorPosX((win_size.x - sub_size.x) * 0.5f); // Center horizontally
      ImGui::SetCursorPosY((win_size.y - sub_size.y - 90) * 0.5f - 25); // Move up a bit
 
      RenderSubWindow(win_size, sub_size);
      ImVec2 sub_min = ImGui::GetWindowPos();
      ImVec2 sub_max = ImVec2(sub_min.x + sub_size.x, sub_min.y + sub_size.y);
      //first set of Buttons

      float button_width = 180.0f;  // Width of each button
      float button_height = 30.0f;
      float button_spacing = 60.0f; // Space between buttons

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

   ImGui::PopStyleColor(); // Restore styles

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

