#include "Game.h"

void Game::gameLoop()
{
    while (true)
    {
        handleInput();
        double deltaTime = timeNow();
        if (deltaTime > currentSpeed / currentDifficulty)
        {
            updateLogic(deltaTime);
            lastTime = currentTime;
        }
        render();
    }
}

double Game::timeNow()
{
	return 0.0;
}

KeyInput Game::getKey()
{
	return KeyInput();
}

void Game::checkCollision(const Point& point)
{
}

/*
 * tùy vào trạng thái trò chơi, hàm sẽ xử lý các phím nhấn theo các cách khác nhau
 *
 * trạng thái MENU (màn hình menu):
 *      gọi hàm xử lý input menu
 *
 * trạng thái PLAYING (màn hình chơi game):
 *      gọi hàm xử lý input trò chơi
 *
 * trạng thái DIFFICULT_SELECTION (màn hình lựa chọn độ khó):
 *      gọi hàm xử lý input lựa chọn độ khó
 *
 * trạng thái HIGH_SCORES (màn hình điểm cao), ABOUT (màn hình giới thiệu), GAME_OVER (màn hình kết thúc trò chơi):
 *      ấn enter hoặc esc để đổi sang trạng thái menu
 *
 * trạng thái PAUSED (tạm dừng):
 *      ấn enter hoặc esc để đổi sang trạng thái chơi game
 *
 */
void Game::handleInput()
{
    KeyInput key;

    while ((key = getKey()) != KeyInput::NONE)
    {
        switch (currentState)
        {
        case State::MENU: return handleMenuInput(key);
        case State::PLAYING: return handlePlayingInput(key);
        case State::DIFFICULTY_SELECTION: return handleDifficultSelectionInput(key);
        case State::HIGH_SCORES:
        case State::ABOUT:
        case State::GAME_OVER:
            if (key == KeyInput::ENTER || key == KeyInput::ESC) currentState = State::MENU;
            break;
        case State::PAUSED:
            if (key == KeyInput::ENTER || key == KeyInput::ESC) currentState = State::PLAYING;
            break;
        }
    }
}

/*
 * hàm xử lý input trong màn hình menu
 *
 * nhấn phím lên trên hoặc xuống dưới để đổi lựa chọn
 *
 * nhấn phím enter:
 *      trò chơi mới: gọi hàm resetGame() để đưa các giá trị trong game thành mặc định và đổi trạng thái sang PLAYING
 *      điểm cao, giới thiệu, độ khó: đổi sang trạng thái HIGH_SCORES hoặc ABOUT hoặc DIFFICULTY_SELECTION
 *      thoát: gọi hàm exit(0) để thoát game và tắt console
 */
void Game::handleMenuInput(KeyInput key)
{
    switch (key)
    {
    case KeyInput::UP:
        this->menuSelection = (this->menuSelection - 1 + 5) % 5;
        break;
    case KeyInput::DOWN:
        this->menuSelection = (this->menuSelection + 1) % 5;
        break;
    case KeyInput::ENTER:
        switch (this->menuSelection)
        {
        case 0: // Trò chơi mới
            resetGame();
            this->currentState = State::PLAYING;
            break;
        case 1: // Điểm cao
            this->currentState = State::HIGH_SCORES;
            break;
        case 2: // Độ khó
            this->currentState = State::DIFFICULTY_SELECTION;
            break;
        case 3: // Giới thiệu
            this->currentState = State::ABOUT;
            break;
        case 4: // Thoát
            exit(0);
        }
    }
}

void Game::handleDifficultSelectionInput(KeyInput key)
{
}

void Game::handlePlayingInput(KeyInput key)
{
}

void Game::updateLogic(double deltaTime)
{
}

void Game::updatePlayingLogic(double deltaTime)
{
}

/*
 * tùy trạng thái mà render các màn hình khác nhau
 *
 * xóa màn hình
 * render các thành phần
 * việt hóa các ký tự tiếng việt
 * hiển thị nội dung vừa render lên màn hình
 */
void Game::render()
{
    renderer->clearScreen();
    switch (currentState)
    {
    case State::MENU: renderMenu(); break;
    case State::PLAYING: renderPlaying(); break;
    case State::GAME_OVER: renderGameOver(); break;
    case State::HIGH_SCORES: renderHighScores(); break;
    case State::DIFFICULTY_SELECTION: renderDifficulty(); break;
    case State::ABOUT: renderAbout(); break;
    case State::PAUSED:
        renderer->drawText("Tam Dung !", { BOARD_WIDTH / 2 - 5, BOARD_HEIGHT / 2 - 1 }, DEFAULT_HIGHLIGHT_COLOR);
        break;
    }
    vietSub();
    renderer->present();
}

void Game::renderMenu()
{
}

void Game::renderPlaying()
{
}

void Game::renderGameOver()
{
}

void Game::renderHighScores()
{
}

void Game::renderDifficulty()
{
}

void Game::renderAbout()
{
}

void Game::resetGame()
{
}

void Game::vietSub()
{
}

bool Game::writeDataToBinaryFile(const std::string& filename, const std::vector<std::pair<std::string, int>>& data)
{
	return false;
}

std::vector<std::pair<std::string, int>> Game::readDataFromBinaryFile(const std::string& filename)
{
	return std::vector<std::pair<std::string, int>>();
}
