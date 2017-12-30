#pragma once
#include <string>

// Forward decl
struct SDL_Renderer;
struct SDL_Window;
struct SDL_Texture;
struct _TTF_Font;

/**
 * @brief Responsible for command input.
 *
 */
class CommandInput
{
public:
  CommandInput(SDL_Window* window);
  ~CommandInput();
  std::string input(const std::string& prompt) const;

private:
  SDL_Window* parentWindow;
  _TTF_Font*  font;
};