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

void Game::render()
{
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
