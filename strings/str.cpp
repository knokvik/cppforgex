#include <iostream>
#include <string.h>
#include <string_view>

using std::cout;

void strhere() {
  std::string dynamic_url = "https://example.100.12.42";
  size_t pos = dynamic_url.find("example");
  cout << pos;

  // string_view
  cout << std::string_view(dynamic_url);
}

// ==========================================
// CRITICAL CONCEPT: DANGLING STRING_VIEW
// ==========================================
// std::string_view does NOT own the string data, it just points to it.
// If the underlying string is destroyed, the string_view becomes a dangling
// pointer.

std::string_view get_dangling_view() {
  std::string temp_string = "Hello World";
  // ❌ Returning a view to a local string!
  // temp_string is destroyed when this function ends.
  return std::string_view(temp_string);
}

void string_view_dangling_demo() {
  std::string_view bad_view = get_dangling_view();
  // std::cout << bad_view; // 💥 UNDEFINED BEHAVIOR! Prints garbage or crashes.

  // ✅ Fix: Only use string_view when you are absolutely sure the underlying
  // string will outlive the string_view. Useful mainly for function parameters.
}