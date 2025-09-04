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

/*
 * render màn hình kết thúc
 *
 * hiển thị thông tin
 * nếu người chơi có điểm và điểm đủ để vào bảng điểm cao thì yêu cầu nhập tên
 */
void Game::renderGameOver()
{
    int xOffset = 25;
    int yOffset = 20;
    std::string score = "Diem: " + std::to_string(scoreManager.score);
    renderer->drawText("Tro choi ket thuc!", { xOffset + 5, yOffset }, DEFAULT_HIGHLIGHT_COLOR);
    renderer->drawText(score, { (int)(BOARD_WIDTH / 2 - score.length() / 2 - 2), yOffset + 4 }, DEFAULT_TEXT_COLOR);
    if (scoreManager.score > 0 && (scoreManager.highScore.size() < 10 || scoreManager.score > scoreManager.highScore.back().second))
    {
        renderer->drawText("Ky luc moi! Nhap ten cua ban: ", { xOffset, yOffset + 24 }, DEFAULT_HIGHLIGHT_COLOR);
    }
    else
    {
        renderer->drawText("Nhan ENTER de tro lai menu.", { xOffset + 1, yOffset + 24 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawText("ENTER", { xOffset + 6, yOffset + 24 }, DEFAULT_SELECTED_COLOR);
    }
}

/*
 * render thông tin điểm cao
 * nếu chưa có điểm cao thì hiển thị thông báo
 */
void Game::renderHighScores()
{
    int x_offset = 15;
    int y_offset = 22;
    renderer->fillMenu();
    renderer->drawText("Diem Cao", { x_offset + 2, y_offset - 2 }, DEFAULT_TITLE_COLOR);
    if (scoreManager.highScore.empty())
    {
        renderer->drawText("Chua co diem cao", { x_offset + 2, y_offset + 6 }, DEFAULT_TEXT_COLOR);
    }
    else
    {
        Color textColor;
        textColor.r = 255;
        textColor.g = 25;
        for (size_t i = 0; i < scoreManager.highScore.size(); ++i)
        {
            std::pair<std::string, int> hi = scoreManager.highScore[i];
            renderer->drawText(std::to_string(i + 1) + ". " + hi.first, { x_offset + 2, y_offset + 2 + (int)i * 2 }, textColor);
            renderer->drawText(": " + std::to_string(hi.second), { 17 + x_offset, y_offset + 2 + (int)i * 2 }, textColor);
            textColor.g += 60;
            textColor.b += 60;
            if (i > 2) textColor = DEFAULT_TEXT_COLOR;
        }
    }
    renderer->drawText("> Tro Lai", { x_offset, y_offset + 14 }, DEFAULT_SELECTED_COLOR);
}

/*
 * render màn hình lựa chọn độ khó
 * thay đổi màu và thêm dấu ">" vào lựa chọn đang được chọn (difficultSelect)
 */
void Game::renderDifficulty()
{
    renderer->fillMenu();
    int x_offset = 15;
    int y_offset = 22;
    std::vector<std::string> options = { "", "Don Gian", "Trung Binh", "Kho" };
    renderer->drawText("Do Kho", { x_offset + 2, y_offset - 2 }, DEFAULT_TITLE_COLOR);

    Color color;

    for (size_t i = 1; i < options.size(); ++i)
    {
        std::string prefix = (i == difficultSelect) ? "> " : "  ";
        color = (i == difficultSelect) ? DEFAULT_SELECTED_COLOR : DEFAULT_UNSELECTED_COLOR;
        renderer->drawText(prefix + options[i], { x_offset, y_offset + (int)i * 2 }, color);
    }
}

/*
 * render thông tin giới thiệu trò chơi
 *
 */
void Game::renderAbout()
{
    int x_offset = 10;
    int y_offset = 4;
    renderer->drawText("Gioi Thieu Tro Choi", { x_offset, y_offset + 4 }, DEFAULT_TITLE_COLOR);
    renderer->drawText("Chao mung ban den voi the gioi ran san moi!", { x_offset, y_offset + 8 }, DEFAULT_HIGHLIGHT_COLOR);
    renderer->drawText("Muc tieu cua ban rat don gian:", { x_offset, y_offset + 12 }, DEFAULT_HIGHLIGHT_COLOR);
    renderer->drawText("Dieu khien ran di san cang nhieu moi cang tot.", { x_offset, y_offset + 14 }, DEFAULT_TEXT_COLOR);
    renderer->drawText("Khi an duoc moi, ran cua ban se dai va nhanh hon mot chut.", { x_offset, y_offset + 16 }, DEFAULT_TEXT_COLOR);
    renderer->drawText("Nhung hay can than voi tuong va chinh ban than minh.", { x_offset, y_offset + 18 }, DEFAULT_TEXT_COLOR);
    renderer->drawText("Cach dieu khien:", { x_offset, y_offset + 22 }, DEFAULT_HIGHLIGHT_COLOR);
    renderer->drawText("                                               D  A  W  S", { x_offset, y_offset + 24 }, DEFAULT_SELECTED_COLOR);
    renderer->drawText("Su dung phim mui ten de di chuyen:  ,  ,  ,", { x_offset, y_offset + 24 }, DEFAULT_TEXT_COLOR);
    renderer->drawText("(", { x_offset + 46, y_offset + 24 }, DEFAULT_TEXT_COLOR);
    renderer->drawText(").", { x_offset + 57, y_offset + 24 }, DEFAULT_TEXT_COLOR);
    renderer->drawChar("↓", { x_offset + 44, y_offset + 24 }, DEFAULT_SELECTED_COLOR);
    renderer->drawChar("↑", { x_offset + 41, y_offset + 24 }, DEFAULT_SELECTED_COLOR);
    renderer->drawChar("→", { x_offset + 38, y_offset + 24 }, DEFAULT_SELECTED_COLOR);
    renderer->drawChar("←", { x_offset + 35, y_offset + 24 }, DEFAULT_SELECTED_COLOR);
    renderer->drawText("Su dung phim ESC de tam dung tro choi.", { x_offset, y_offset + 26 }, DEFAULT_TEXT_COLOR);
    renderer->drawText("ESC", { x_offset + 13, y_offset + 26 }, DEFAULT_SELECTED_COLOR);
    renderer->drawText("Chuc ban choi game vui ve!", { x_offset, y_offset + 30 }, DEFAULT_HIGHLIGHT_COLOR);
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
