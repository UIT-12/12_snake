// --- Platform-specific definitions ---
//▄▀█
#include <cmath>
#include <iostream>     // For input/output operations (cout, cin)
#include <vector>       // For std::vector to create the 2D board
#include <chrono>       // For high-resolution time measurements (FPS calculation)
#include <thread>       // For std::thread to enable multithreading
#include <mutex>        // For std::mutex to protect shared data (the board)
#include <random>       // For std::random_device, std::mt19937, std::uniform_int_distribution
#include <string>       // For std::string
#include <iomanip>      // For std::fixed and std::setprecision for FPS display
#include <sstream>      // For std::stringstream to build the output buffer
#include <atomic>       // For std::atomic<bool> to signal thread termination
#include <algorithm>    // For std::max, std::min
#include "intro.cpp"

// Platform-specific includes for non-blocking keyboard input
#ifndef _WIN32
#include <termios.h>    // For tcgetattr, tcsetattr on Linux/macOS
#include <unistd.h>     // For read, STDIN_FILENO
#include <fcntl.h>      // For fcntl
#else
#include <conio.h> // Explicitly include for _kbhit and _getch
#endif

// Define PI if not already defined (e.g., from <cmath> or <numbers> in C++20)
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



// --- Constants ---
const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 42;
const std::string CELL_CHAR = "▄"; // Each cell displays this two-character block
float intro_frame = 1;
int game_state = 0;
int show_sprite = 0;
std::mutex intro_frame_mutex;
std::mutex game_state_mutex;

#ifdef _WIN32
#include <windows.h>
void enable_virtual_terminal_processing()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    SetConsoleOutputCP(CP_UTF8);
}
#else
void enable_virtual_terminal_processing() {}
#endif

class AiString
{
private:

public:
	static std::string set_fg_rgb(int r, int g, int b)
	{
		return "\x1b[38;2;" + std::to_string(r) + ";" +
			   std::to_string(g) + ";" + std::to_string(b) + "m";
	}
	
	static std::string set_bg_rgb(int r, int g, int b) 
	{
		return "\x1b[48;2;" + std::to_string(r) + ";" +
			   std::to_string(g) + ";" + std::to_string(b) + "m";
	}
	
	static std::string reset_color()
	{
		return "\x1b[0m";
	}
	
	static std::string set_pos(int row, int col) 
	{
		return "\x1b[" + std::to_string(row + 1) + ";" + std::to_string(col + 1) + "H";
	}
	
	static std::string cls()
	{
		return "\x1b[2J\x1b[H";
	}
	
	static std::string hide_cur()
	{
		return "\x1b[?25l";
	}
	
	static std::string show_cur()
	{
		return "\x1b[?25h";
	}
};

// --- Cell Data Structure ---
struct Cell {
    std::string character; // Stores the character (e.g., "██")
    int r, g, b;           // Stores RGB color components (0-255)
};

// --- Sprite Data Structure ---
struct Sprite {
    // The sprite shape defined in terms of 'X' for solid cells and ' ' for transparent.
    // Each string represents a row of cells.
    std::vector<std::string> shape_cells = {
        {"X"," ","X"} // A 1x1 sprite (one '██' block)
    };
    int width_cells = 3;  // Width in terms of BOARD_WIDTH units (number of '██' blocks)
    int height_cells = 1; // Height in terms of BOARD_HEIGHT units (number of rows)

    int x; // Top-left X coordinate on the board (in CELL_CHAR units)
    int y; // Top-left Y coordinate on the board (in CELL_CHAR units)

    int r = 75; // White color for the sprite
    int g = 75;
    int b = 75;
};

// Global board (background data) and mutex
std::vector<std::vector<Cell>> board_background(BOARD_HEIGHT, std::vector<Cell>(BOARD_WIDTH));
std::mutex board_background_mutex; // Mutex to protect 'board_background'

// Global sprite object and mutex
Sprite player_sprite;
std::mutex sprite_mutex; // Mutex to protect 'player_sprite'

// Atomic flag to signal input thread to stop
std::atomic<bool> terminate_input_thread(false);

// --- Platform-specific terminal setup/teardown for non-blocking input ---
#ifndef _WIN32
// For Linux/macOS: Set terminal to non-canonical mode
struct termios original_termios;

void setNonBlockingInput() {
    tcgetattr(STDIN_FILENO, &original_termios); // Save original terminal settings
    struct termios new_termios = original_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode (line buffering) and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios); // Apply new settings immediately

    // Set stdin to non-blocking
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

void restoreBlockingInput() {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios); // Restore original terminal settings
    // Restore stdin to blocking
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
}
#endif

void renderIntro(int x, int y)
{
	board_background[y][x].r = get_r((int)intro_frame,y,x);
	board_background[y][x].g = get_g((int)intro_frame,y,x);
	board_background[y][x].b = get_b((int)intro_frame,y,x);
	board_background[y][x].character = CELL_CHAR;
}

void renderMenu(int x, int y)
{
	board_background[y][x].r = 25;
	board_background[y][x].g = 25;
	board_background[y][x].b = 25;
	board_background[y][x].character = CELL_CHAR;
}

// --- Thread 1: Board Data Updater (Calculates RGB colors using sine waves) ---
void updateBoardData() {
	/*
    // Parameters for the sine waves for each color component
    const double x_freq_r = 0.08;
    const double y_freq_r = 0.04;
    const double horizontal_shift_speed_r = 5.0; // Units per second
    const double phase_offset_r = 0.0;

    const double x_freq_g = 0.07;
    const double y_freq_g = 0.05;
    const double horizontal_shift_speed_g = 6.0; // Slightly different speed for green
    const double phase_offset_g = M_PI / 2.0; // Offset green by 90 degrees

    const double x_freq_b = 0.06;
    const double y_freq_b = 0.06;
    const double horizontal_shift_speed_b = 7.0; // Slightly different speed for blue
    const double phase_offset_b = M_PI; // Offset blue by 180 degrees
	*/
    // auto start_time = std::chrono::high_resolution_clock::now();
    while (!terminate_input_thread.load()) {
        // Calculate elapsed time for driving the horizontal shift
        // auto current_time = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double> elapsed_seconds = current_time - start_time;
		
		intro_frame+=0.001;
        // double time_val = elapsed_seconds.count();

        // Lock the mutex before modifying the board_background
        std::lock_guard<std::mutex> lock(board_background_mutex);

        for (int y = 0; y < BOARD_HEIGHT; ++y) {
            for (int x = 0; x < BOARD_WIDTH; ++x) {
				/*
                // Calculate phase for each color component, incorporating horizontal shift
                double phase_r = ((x - (time_val * horizontal_shift_speed_r)) * x_freq_r) + (y * y_freq_r) + phase_offset_r;
                double phase_g = ((x - (time_val * horizontal_shift_speed_g)) * x_freq_g) + (y * y_freq_g) + phase_offset_g;
                double phase_b = ((x - (time_val * horizontal_shift_speed_b)) * x_freq_b) + (y * y_freq_b) + phase_offset_b;
				

                // Map sine wave output [-1, 1] to RGB range [0, 255]
                board_background[y][x].r = static_cast<int>((std::sin(phase_r) + 1.0) / 2.0 * 255.0);
                board_background[y][x].g = static_cast<int>((std::sin(phase_g) + 1.0) / 2.0 * 255.0);
                board_background[y][x].b = static_cast<int>((std::sin(phase_b) + 1.0) / 2.0 * 255.0);
                board_background[y][x].character = CELL_CHAR; // Ensure background character is always correct
				*/
				if (intro_frame > 63 && game_state != 2)
				{
					game_state = 2;
					std::lock_guard<std::mutex> lock(game_state_mutex);
					show_sprite = 1;
				}
				if (intro_frame < 63)
				{
					renderIntro(x,y);
				}
				else
				{
					renderMenu(x,y);
				}
            }
        }
    }
}

// --- Thread 2: Input Handler ---
void inputHandler() {
#ifndef _WIN32
    setNonBlockingInput(); // Set terminal to non-blocking mode on Linux/macOS
#endif

    while (!terminate_input_thread.load()) {
        int key = 0;
#ifdef _WIN32
        if (_kbhit()) { // Check if a key has been pressed
            key = _getch(); // Get the character without waiting for Enter
            if (key == 0 || key == 224) { // Special key (like arrow keys) are often 0 or 224
                key = _getch(); // Get the actual key code
            }
        }
#else
        // Linux/macOS non-blocking input
        char c;
        // Read the first byte of a potential escape sequence
        if (read(STDIN_FILENO, &c, 1) > 0) {
            key = c;
            // Handle arrow keys which are multi-byte sequences (e.g., ESC [ A for Up)
            if (key == '\033') { // ESC character
                char buffer[2]; // Buffer to read the next two characters
                // Try to read '[' and then the actual key character ('A', 'B', 'C', 'D')
                // Use non-blocking read for the subsequent characters too
                if (read(STDIN_FILENO, &buffer[0], 1) > 0 && buffer[0] == '[') {
                    if (read(STDIN_FILENO, &buffer[1], 1) > 0) {
                        switch(buffer[1]) {
                            case 'A': key = 72; break; // Up arrow (map to Windows _getch equivalent)
                            case 'B': key = 80; break; // Down arrow
                            case 'C': key = 77; break; // Right arrow
                            case 'D': key = 75; break; // Left arrow
                            default: key = 0; break; // Unknown escape sequence, ignore
                        }
                    } else { key = 0; } // Failed to read second char, incomplete sequence
                } else { key = 0; } // Failed to read '[', not an arrow key sequence
            }
        }
#endif

        if (key != 0) {
            std::lock_guard<std::mutex> lock(sprite_mutex); // Lock sprite before modifying
            switch (key) {
                case 72: // Up arrow (unified code for both platforms)
                    player_sprite.y = std::max(0, player_sprite.y - 1);
                    break;
                case 80: // Down arrow
                    player_sprite.y = std::min(BOARD_HEIGHT - player_sprite.height_cells, player_sprite.y + 1);
                    break;
                case 77: // Right arrow
                    player_sprite.x = std::min(BOARD_WIDTH - player_sprite.width_cells, player_sprite.x + 1);
                    break;
                case 75: // Left arrow
                    player_sprite.x = std::max(0, player_sprite.x - 1);
                    break;
                case 'q': // 'q' to quit
                case 'Q':
                    terminate_input_thread.store(true); // Signal main loop to terminate
                    break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Small delay to prevent busy-waiting
    }

#ifndef _WIN32
    restoreBlockingInput(); // Restore original terminal settings on Linux/macOS
#endif
}

// --- Renderer (Main thread in this case) ---
void renderFrame(double fps) {
    // Create a temporary frame buffer for this frame's rendering
    std::vector<std::vector<Cell>> frame_buffer(BOARD_HEIGHT, std::vector<Cell>(BOARD_WIDTH));

    // Step 1: Populate the frame_buffer with the current background state
    {
        std::lock_guard<std::mutex> board_lock(board_background_mutex);
        for (int y = 0; y < BOARD_HEIGHT; ++y) {
            for (int x = 0; x < BOARD_WIDTH; ++x) {
                frame_buffer[y][x] = board_background[y][x];
            }
        }
    } // board_lock goes out of scope, releasing board_background_mutex

    // Step 2: Draw the sprite onto the frame_buffer, overwriting background where needed
    {
        std::lock_guard<std::mutex> sprite_lock(sprite_mutex);
        for (int sy = 0; sy < player_sprite.height_cells; ++sy) {
            for (int sx = 0; sx < player_sprite.width_cells; ++sx) {
                // Calculate absolute coordinates on the board
                int board_x = player_sprite.x + sx;
                int board_y = player_sprite.y + sy;

                // Check if this sprite pixel is within board bounds and is not a transparent space
                if (board_x >= 0 && board_x < BOARD_WIDTH &&
                    board_y >= 0 && board_y < BOARD_HEIGHT &&
                    player_sprite.shape_cells[sy][sx] != ' ') { // Check the 'X' or ' '
                    // Overwrite the cell in the frame_buffer with sprite data
                    frame_buffer[board_y][board_x].character = CELL_CHAR; // Always use CELL_CHAR for solid sprite parts
					std::lock_guard<std::mutex> lock(game_state_mutex);
					if (show_sprite == 1)
					{
						frame_buffer[board_y][board_x].r = player_sprite.r;
						frame_buffer[board_y][board_x].g = player_sprite.g;
						frame_buffer[board_y][board_x].b = player_sprite.b;
					}
                    
                }
            }
        }
    } // sprite_lock goes out of scope, releasing sprite_mutex

    // Step 3: Build the final output string from the composed frame_buffer
    std::stringstream output_stream;
    output_stream << AiString::set_pos(0, 0); // Move cursor to home position

    // Variables to track the last applied RGB color to avoid redundant escape codes
	int last_bg_r = -1, last_bg_g = -1, last_bg_b = -1;
	int last_fg_r = -1, last_fg_g = -1, last_fg_b = -1;
	
    for (int y = 0; y < BOARD_HEIGHT; y+=2) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            const Cell& cell = frame_buffer[y][x];
			const Cell& cell1 = frame_buffer[y+1][x];

            // Only apply RGB color code if it's different from the last applied color
            if (cell.r != last_bg_r || cell.g != last_bg_g || cell.b != last_bg_b) {
                output_stream << AiString::set_bg_rgb(cell.r, cell.g, cell.b);
                last_bg_r = cell.r;
                last_bg_g = cell.g;
                last_bg_b = cell.b;
            }
			if (cell1.r != last_fg_r || cell1.g != last_fg_g || cell1.b != last_fg_b) {
                output_stream << AiString::set_fg_rgb(cell1.r, cell1.g, cell1.b);
                last_fg_r = cell1.r;
                last_fg_g = cell1.g;
                last_fg_b = cell1.b;
            }
            output_stream << cell.character;
        }
        output_stream << AiString::reset_color(); // Reset color at end of line
        output_stream << "\n"; // New line after each row
        // Reset last colors to force re-application on the next line (ensures correct starting color)
        last_bg_r = -1, last_bg_g = -1, last_bg_b = -1;
		last_fg_r = -1, last_fg_g = -1, last_fg_b = -1;
    }

    // Display FPS in the top-right corner
    // Position adjusted for BOARD_WIDTH * 2 (since CELL_CHAR is 2 chars wide)
    int fps_row = BOARD_HEIGHT/2 + 2;
    output_stream << AiString::set_pos(fps_row, 1);
    output_stream << AiString::set_fg_rgb(255, 255, 255); // White for FPS text
    output_stream << "FPS: " << std::fixed << std::setprecision(2) << fps << "   "; // Added spaces for clear overwrite

    output_stream << AiString::reset_color();

    // Move cursor below the board to prevent flickering if user types
    output_stream << AiString::set_pos(BOARD_HEIGHT + 1, 0);

    // Print the entire buffered frame to the console in one go
    std::cout << output_stream.str();
    std::cout.flush();
}

// --- Main Function ---
int main() {
    // Optimize C++ streams for faster I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Enable Virtual Terminal Processing for Windows (if defined in library.h)
    enable_virtual_terminal_processing();

    // Initialize the board_background with default characters
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            board_background[y][x].character = CELL_CHAR;
            board_background[y][x].r = 0; 
			board_background[y][x].g = 0; 
			board_background[y][x].b = 0; 
        }
    }
	/*
    // Initialize sprite position (e.g., center bottom)
    player_sprite.x = BOARD_WIDTH / 2 - player_sprite.width_cells / 2;
    player_sprite.y = BOARD_HEIGHT - player_sprite.height_cells - 1; // One row from bottom
	*/

    // Clear the screen once at the start
    std::cout << AiString::cls();
    // Hide the cursor
    std::cout << AiString::hide_cur();
    std::cout.flush();

    // Start the board data update thread
    std::thread updater_thread(updateBoardData);

    // Start the input handling thread
    std::thread input_thread(inputHandler);

    // Variables for FPS calculation
    auto last_frame_time = std::chrono::high_resolution_clock::now();
    double current_fps = 0.0;
    long long frame_count = 0;
    auto fps_update_interval_start = std::chrono::high_resolution_clock::now();

    // Main rendering loop
    while (!terminate_input_thread.load()) { // Loop until input thread signals termination
        auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> delta_time = current_time - last_frame_time;
        last_frame_time = current_time;

        // Calculate FPS
        frame_count++;
        std::chrono::duration<double> elapsed_since_last_fps_update = current_time - fps_update_interval_start;

        if (elapsed_since_last_fps_update.count() >= 1.0) {
            current_fps = frame_count / elapsed_since_last_fps_update.count();
            frame_count = 0;
            fps_update_interval_start = current_time;
        }

        renderFrame(current_fps); // Render the board and display FPS

        // Small delay to yield CPU time. This is a minimum sleep, actual frame time
        // will be longer if rendering takes more than 1ms.
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // Clean up threads on exit
    updater_thread.join();
    input_thread.join(); // Wait for input thread to finish

    // Show the cursor before exiting
    std::cout << AiString::show_cur();
    // Ensure terminal colors are reset on exit
    std::cout << AiString::reset_color();
	std::cout << AiString::cls();
    std::cout.flush();
    return 0;
}
