#include "xu/core/Context.hpp"

#include <iostream> // Temporary

namespace xu {

void Context::PushEvent(MouseMoveEvent const &evt) {
  Event event;
  event.type = EventType::MOUSE_MOVE;
  event.data.mouseMove = evt;
  eventQueue.push_back(event);
}

void Context::PushEvent(WindowResizeEvent const &evt) {
  Event event;
  event.type = EventType::WINDOW_RESIZE;
  event.data.windowResize = evt;
  eventQueue.push_back(event);
}

void Context::ProcessEvents() {
  // Temporary
  for (auto const &evt : eventQueue) {
    switch (evt.type) {
    case EventType::MOUSE_MOVE:
      std::cout << "Mouse move: " << evt.data.mouseMove.position.x << " "
                << evt.data.mouseMove.position.y << std::endl;
      break;
    case EventType::WINDOW_RESIZE:
      std::cout << "Window resize: " << evt.data.windowResize.size.x << " "
                << evt.data.windowResize.size.y << std::endl;
      break;
    }
  }
  eventQueue.clear();
}

} // namespace xu