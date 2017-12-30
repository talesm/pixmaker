#include "CommandInput.hpp"
#include <KW_button.h>
#include <KW_editbox.h>
#include <KW_frame.h>
#include <KW_gui.h>
#include <KW_label.h>
#include <KW_renderdriver_sdl2.h>
#include "Driver.hpp"

using namespace std;

CommandInput::CommandInput(SDL_Renderer* renderer, SDL_Window* window)
  : renderer(renderer)
  , window(window)
{}

CommandInput::~CommandInput() {}

/* Callback for when the OK button is clicked */
static bool quit = false;
static void
OKClicked(KW_Widget* widget, int b)
{
  quit = true;
}

static void
EnterPressed(KW_Widget* widget, SDL_Keycode sym, SDL_Scancode code)
{
  if (sym == SDLK_KP_ENTER || sym == SDLK_RETURN) {
    quit = true;
  }
}
string
CommandInput::input(const string& prompt) const
{
  quit = false;
  /* Initialize KiWi */
  KW_RenderDriver* driver = KW_CreateSDL2RenderDriver(renderer, window);
  KW_Surface*      set    = KW_LoadSurface(driver, "res/tileset.png");
  KW_GUI*          gui    = KW_Init(driver, set);

  /* Create the top-level framve */
  KW_Rect windowrect = {0, 0, 800, 600};
  KW_Rect framerect  = {10, 10, 300, 220};
  KW_RectCenterInParent(&windowrect, &framerect);
  KW_Widget* frame = KW_CreateFrame(gui, NULL, &framerect);

  /* Create the title, label and edibox widgets */
  KW_Rect titlerect   = {10, 10, 280, 30};
  KW_Rect labelrect   = {10, 50, 280, 30};
  KW_Rect editboxrect = {10, 100, 280, 40};
  KW_CreateLabel(gui, frame, "Input command", &titlerect);
  KW_CreateLabel(gui, frame, prompt.c_str(), &labelrect);

  auto editbox = KW_CreateEditbox(gui, frame, "", &editboxrect);
  KW_AddWidgetKeyUpHandler(editbox, EnterPressed);
  KW_SetFocusedWidget(editbox);

  KW_Rect    buttonrect = {250, 170, 40, 40};
  KW_Widget* okbutton = KW_CreateButtonAndLabel(gui, frame, "OK", &buttonrect);
  KW_AddWidgetMouseDownHandler(okbutton, OKClicked);

  /* Main loop */
  while (!SDL_QuitRequested() && !quit) {
    SDL_RenderClear(renderer);
    KW_ProcessEvents(gui);
    // redraw();
    KW_Paint(gui);
    SDL_RenderPresent(renderer);
    SDL_Delay(1);
  }
  string input = KW_GetEditboxText(editbox);

  /* free stuff */
  KW_Quit(gui);
  // KW_ReleaseSurface(driver, set);
  // KW_ReleaseRenderDriver(driver);
  return input;
}
