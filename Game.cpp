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

void Game::handleInput()
{
}

void Game::handleMenuInput(KeyInput key)
{
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

/*
 * render màn hình menu
 *
 * render các lựa chọn lên màn hình, thay đổi lại màu nếu lựa chọn đó đang được chọn
 */
void Game::renderMenu()
{
    renderer->fillMenu();
    std::vector<std::string> options = { "Tro Choi Moi", "Diem Cao", "Do Kho", "Gioi Thieu", "Thoat" };
    for (size_t i = 0; i < options.size(); ++i)
    {
        std::string prefix = (i == menuSelection) ? "> " : "  ";
        Color color = (i == menuSelection) ? DEFAULT_SELECTED_COLOR : DEFAULT_UNSELECTED_COLOR;
        renderer->drawText(prefix + options[i], { 15, 22 + (int)i * 2 }, color);
    }
}

/*
 * render màn hình chơi game
 *
 * vẽ các thành phần trong bảng lên màn hình
 * ghi đè lên bonus point nếu có vì bonus dùng chung khối với point
 * hiển thị điểm người chơi
 */
void Game::renderPlaying()
{
    // Vẽ bảng
    for (int y = 0; y < BOARD_HEIGHT; ++y)
        for (int x = 0; x < BOARD_WIDTH; ++x)
            renderer->drawBlock({ x, y }, canvas[y][x]);

    if (food->isBonusActive()) renderer->drawBlock(food->getBonusPosition(), CELL_TYPE::BONUS);
    else canvas[food->getBonusPosition().y][food->getBonusPosition().x] = CELL_TYPE::NONE;

    renderer->drawText("Diem: " + std::to_string(scoreManager.score), { 1, 0 }, DEFAULT_TITLE_COLOR);
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
