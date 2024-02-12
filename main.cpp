#include <SDL2/SDL.h>
#include <algorithm>
#include <deque>

const int WIDTH = 1000;
const int HEIGHT = 1000;

enum Direction {
  DOWN,
  LEFT,
  RIGHT,
  UP
};

int main() {
  SDL_Init(SDL_INIT_EVERYTHING);
  auto window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
  auto renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_Event e;

  // Snake head position
  //
  SDL_Rect head {WIDTH/2, HEIGHT/2, 10, 10};

  // Snake body container
  //
  std::deque<SDL_Rect> rq;
  int size = 5;

  // Init apple position
  //
  SDL_Rect apple{rand()%(WIDTH/10)*10, rand()%(HEIGHT/10)*10, 10, 10};
  bool eat = false;

  bool running = true;
  int dir = UP;

  while (running) {
    // Check Input
    //
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) running = false;
      if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_DOWN && dir != UP)  { dir = DOWN; }
        if (e.key.keysym.sym == SDLK_UP && dir != DOWN)    { dir = UP; }
        if (e.key.keysym.sym == SDLK_LEFT && dir != RIGHT)  { dir = LEFT; }
        if (e.key.keysym.sym == SDLK_RIGHT && dir != LEFT) { dir = RIGHT; }
        if (e.key.keysym.sym == SDLK_q) { running = false; } // Quit the game
        if (e.key.keysym.sym == SDLK_r) { // Reset the game
          head = SDL_Rect{WIDTH/2, HEIGHT/2, 10, 10};
          rq.clear();
          size = 5;
          eat = false;
          running = true;
          dir = UP;
          apple = SDL_Rect{rand()%(WIDTH/10)*10, rand()%(HEIGHT/10)*10, 10, 10};
        }
      }
    }

    // Move
    //
    switch (dir) {
      case DOWN:
        head.y = (head.y + 10 + HEIGHT) % HEIGHT; break;
      case UP:
        head.y = (head.y - 10 + HEIGHT) % HEIGHT; break;
      case LEFT:
        head.x = (head.x- 10 + WIDTH) % WIDTH; break;
      case RIGHT:
        head.x = (head.x + 10 + WIDTH) % WIDTH; break;
    }

    // Collision detection with apple
    //
    if (head.x == apple.x && head.y == apple.y) {
      size += 3;
      apple.x = -10;
      apple.y = -10;
      eat = true;
    }

    // Collision detection with snake body
    //
    std::for_each(rq.begin(), rq.end(), [&](auto &snake_segment) {
      if (head.x == snake_segment.x && head.y == snake_segment.y) {
        bool game_over = true;
        while (game_over) {
          while (SDL_PollEvent(&e)) {
            if (e.key.keysym.sym == SDLK_q) {
              game_over = false;
              running = false;
            }
            if (e.key.keysym.sym == SDLK_r) {
              game_over = false;
              head = SDL_Rect{WIDTH/2, HEIGHT/2, 10, 10};
              rq.clear();
              size = 5;
              eat = false;
              running = true;
              dir = UP;
              apple = SDL_Rect{rand()%(WIDTH/10)*10, rand()%(HEIGHT/10)*10, 10, 10};
            }
          }
        }
      }
    });

    // Update snake body
    //
    rq.push_front(head);

    while (rq.size() > size) {
      rq.pop_back();
    }

    // Clear Window
    //
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw Body
    //
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    std::for_each(rq.begin(), rq.end(), [&](auto &snake_segment) {
      SDL_RenderFillRect(renderer, &snake_segment);
    });

    // Draw Apples
    //
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    if (eat) {
      apple = SDL_Rect{rand()%(WIDTH/10)*10, rand()%(HEIGHT/10)*10, 10, 10};
      eat = false;
    }
    SDL_RenderFillRect(renderer, &apple);

    // Display
    //
    SDL_RenderPresent(renderer);
    SDL_Delay(30);
  }

  return 0;
}
