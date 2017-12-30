#pragma once
#include <string>

// Forward decl
struct SDL_Renderer;
struct SDL_Window;
struct KW_RenderDriver;
struct KW_Surface;
struct KW_GUI;
struct KW_Widget;

/**
 * @brief Responsible for command input.
 *
 */
class CommandInput
{
public:
  CommandInput(SDL_Renderer* renderer, SDL_Window* window);
  ~CommandInput();
  std::string input(const std::string& prompt) const;

private:
  SDL_Renderer*    renderer;
  SDL_Window*      window;
  KW_RenderDriver* driver;
  KW_Surface*      set;
  KW_GUI*          gui;
  KW_Widget*       editbox;
  KW_Widget*       promptlabel;
};