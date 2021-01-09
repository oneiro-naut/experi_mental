#include <iostream>
#include <vector>
#include <unordered_set>
#include <SDL2/SDL.h>

using namespace std;

struct point_hash {
  inline std::size_t operator()(const std::pair<int, int> &v) const {
      return v.first*31+v.second;
  }
};

void draw_points(unordered_set<std::pair<int, int>, point_hash> &points, SDL_Renderer *s) {
  // We clear what we draw before
  SDL_RenderClear(s);
  // Set our color for the draw functions
  SDL_SetRenderDrawColor(s, 0xFF, 0xFF, 0xFF, 0xFF);
  // Now we can draw our point
  for (auto i : points) {
    SDL_RenderDrawPoint(s, i.first, i.second);
    // Set the color to what was before
  }
}

void draw_line_between_points(unordered_set<std::pair<int, int>, point_hash> &points, SDL_Renderer *s) {
  // We do not clear here
  //SDL_RenderClear(s);
  // Set our color for the draw functions
  SDL_SetRenderDrawColor(s, 0xFF, 0x00, 0x00, 0xFF);
  // Now we can draw our point
  for (auto i : points) 
    for (auto j : points) 
      SDL_RenderDrawLine(s, i.first, i.second, j.first, j.second);
}

void print_points(unordered_set<std::pair<int, int>, point_hash> &points) {
  for (auto i : points) {
    cout << i.first << ", " << i.second << "\n";
  }
}

int main(int argc, char **argv) {
  const int windowHeight = 600;
  const int windowWidth = 800;
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      return 1;
      cout << "Initialization failed" << endl;
  }
  SDL_Window *window = SDL_CreateWindow("Diagonals of Polygon",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth,
      windowHeight, SDL_WINDOW_SHOWN);

  if (window == NULL) {
      SDL_Quit();
      return 2;
  }

  // We create a renderer with hardware acceleration, we also present according with the vertical sync refresh.
  SDL_Renderer *s = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) ;
  //set of points
  unordered_set<std::pair<int, int>, point_hash> points;
  bool quit = false;
  SDL_Event event;
  while (!quit) {
      //drawing particles
      //setting up objects
      //repeated over and over again
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            quit = true;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
          std::pair<int, int> mouse_coord;
          SDL_GetMouseState(&mouse_coord.first,&mouse_coord.second);
          if (points.find(mouse_coord) == points.end()) points.insert(mouse_coord);
          cout << mouse_coord.first << ", " << mouse_coord.second << "\n";
          cout << "No. of points = " << points.size() << "\n";
        }
    }
    SDL_SetRenderDrawColor(s, 0x00, 0x00, 0x00, 0xFF);
    // .. you could do some other drawing here
    // And now we present everything we draw after the clear.
    draw_points(points, s);
    draw_line_between_points(points, s);
    SDL_RenderPresent(s);
  }
  SDL_DestroyWindow(window);
  // We have to destroy the renderer, same as with the window.
  SDL_DestroyRenderer(s);
  SDL_Quit();
  return 0;
}