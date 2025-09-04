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
