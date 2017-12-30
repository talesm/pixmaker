#pragma once
#include <string>

// Forward decl
struct SDL_Renderer;
struct SDL_Window;

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
  SDL_Renderer* renderer;
  SDL_Window*   window;
};