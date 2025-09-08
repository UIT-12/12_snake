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

/*
 * Hàm dùng để tính khoảng thời gian đã trôi qua kể từ last_time
 * Trả về giá trị double tính theo millisecond (1/1000 giây)
 * Sử dụng thư viện "chrono" để bắt được thời gian chuẩn theo millisecond
 */
double Game::timeNow()
{
    currentTime = std::chrono::high_resolution_clock::now();
    auto t_dif = currentTime.time_since_epoch() - lastTime.time_since_epoch();
    return std::chrono::duration_cast<t_cast>(t_dif).count();
}

/*
 * Ghi nhận thao tác bàn phím từ người dùng và xuất ra tên phím tương ứng
 * Hàm xử lý được các phím: Enter, ESC, 4 phím mũi tên, W, A, S, D
 * Nếu không có phím nào được nhập, hàm sẽ trả về NONE
 *
 * sử dụng thư viện conio.h
 *      _kbhit() trả về true nếu có phím được nhấn
 *      _getch() trả về ký tự được nhấn, tuy nhiên phím đặc biệt cần phải gọi 2 lần để biết chính xác phím nhấn là gì
 *              ký tự đặc biệt bắt đầu với char(-32) hoặc char(
 */
KeyInput Game::getKey()
{
    char key = '\0';
    KeyInput res = KeyInput::NONE;

    if (_kbhit())
    {
        key = _getch();  // Đọc ký tự đầu tiên
        if (key == -32 || key == 224) // Kiểm tra xem đó có phải là phím đặc biệt (như mũi tên) không
        {
            key = _getch();              // Đọc ký tự thứ hai để xác định phím cụ thể
            switch (key)                 // Chuyển đổi mã phím mũi tên thành ký tự tương ứng
            {
            case 72: return KeyInput::UP;
            case 80: return KeyInput::DOWN;
            case 75: return KeyInput::LEFT;
            case 77: return KeyInput::RIGHT;
            }
        }
    }

    switch (key)
    {
    case 'w':
    case 'W': return KeyInput::UP;
    case 's':
    case 'S': return KeyInput::DOWN;
    case 'a':
    case 'A': return KeyInput::LEFT;
    case 'd':
    case 'D': return KeyInput::RIGHT;
    case 13: return KeyInput::ENTER;
    case 27: return KeyInput::ESC;
    }

    return res;
}

/*
 * Hàm dùng để kiểm tra các va chạm xảy ra
 *
 * Nếu đầu rắn chạm tường hoặc tự chạm bản thân thì chuyển trạng thái game thành GAME_OVER
 */
void Game::checkCollision(const Point& point)
{
    if (point.x < 0 || point.x >= BOARD_WIDTH || point.y < 0 || point.y >= BOARD_HEIGHT)
    {
        currentState = State::GAME_OVER;
        return;
    }

    if (canvas[point.y][point.x] == CELL_TYPE::WALL)
    {
        currentState = State::GAME_OVER;
        return;
    }

    if (snake->checkSelfCollision())
    {
        currentState = State::GAME_OVER;
    }
}

/*
 * Tùy vào trạng thái trò chơi, hàm sẽ xử lý các phím nhấn theo các cách khác nhau
 *
 * Trạng thái MENU (màn hình menu):
 *      gọi hàm xử lý input menu
 *
 * Trạng thái PLAYING (màn hình chơi game):
 *      gọi hàm xử lý input trò chơi
 *
 * Trạng thái DIFFICULT_SELECTION (màn hình lựa chọn độ khó):
 *      gọi hàm xử lý input lựa chọn độ khó
 *
 * Trạng thái HIGH_SCORES (màn hình điểm cao), ABOUT (màn hình giới thiệu), GAME_OVER (màn hình kết thúc trò chơi):
 *      ấn enter hoặc esc để đổi sang trạng thái menu
 *
 * Trạng thái PAUSED (tạm dừng):
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
 * Hàm xử lý input trong màn hình menu
 *
 * Nhấn phím lên trên hoặc xuống dưới để đổi lựa chọn
 *
 * Nhấn phím enter:
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

/*
 * Hàm xử lý input trong màn hình thay đổi độ khó
 *
 * Phím mũi tên lên và xuống để thay đổi lựa chọn độ khó (difficultSelect) và không tác động đến độ khó game (currentDifficulty)
 * Phím enter để lưu lựa chọn độ khó vào currentDifficulty để thay đổi tốc độ chơi game thực và trở lại màn hình menu
 * Phím esc để đổi trạng thái thành menu và gán lại giá trị độ khó gốc cho difficultSelect để người dùng biết được độ khó mặc định
 */
void Game::handleDifficultSelectionInput(KeyInput key)
{
    switch (key)
    {
    case KeyInput::UP:
        if (--difficultSelect == 0) difficultSelect = 3;
        break;
    case KeyInput::DOWN:
        if (++difficultSelect == 4) difficultSelect = 1;
        break;
    case KeyInput::ENTER:
        currentDifficulty = difficultSelect;
        currentState = State::MENU;
        break;
    case KeyInput::ESC:
        difficultSelect = currentDifficulty;
        currentState = State::MENU;
    }
}

/*
 * Xử lý input trong trạng thái chơi game (PLAYING)
 *
 * Người dùng ấn phím mũi tên thì sẽ được lưu vào bộ nhớ tạm (inputQueue)
 *      bộ nhớ sẽ lưu lại 3 phím nhấn mới nhất từ người dùng
 *      nhằm fix lỗi mất phím khi người dùng ấn quá nhanh và hệ thống xử lý không kịp
 *      đồng thời chỉ lưu trữ 3 phím để người dùng có thể dự đoán được hướng đi của rắn
 *          nếu lưu trữ không giới hạn thì nếu người dùng ấn quá nhiều phím, thì rắn sẽ đi đến khi nào xử lý hết phím mới điều khiển tiếp được)
 *
 * Người dùng nhấn phím ESC thì sẽ đưa sang trạng thái tạm dừng
 *
 */
void Game::handlePlayingInput(KeyInput key)
{
    if (key == KeyInput::UP) inputQueue.push_back(Direction::UP);
    if (key == KeyInput::DOWN) inputQueue.push_back(Direction::DOWN);
    if (key == KeyInput::LEFT) inputQueue.push_back(Direction::LEFT);
    if (key == KeyInput::RIGHT) inputQueue.push_back(Direction::RIGHT);
    while (inputQueue.size() > 3) inputQueue.pop_front();
    if (key == KeyInput::ESC) currentState = State::PAUSED;
}

/*
 * Hàm cập nhật logic của trò chơi (nhận dữ liệu deltaTime để tính toán trong hàm updatePlayingLogic)
 *
 * Nếu đang ở trạng thái chơi game (PLAYING) thì gọi hàm updatePlayingLogic
 *
 * Nếu đang ở trạng thái kết thúc trò chơi (GAME_OVER):
 *      nếu người chơi không có điểm thì không thực hiện gì cả
 *      nếu bảng điểm cao chưa đầy hoặc điểm người chơi lớn hơn điểm cuối cùng thì:
 *          lưu tên người dùng (tối đa 10 ký tự),
 *          sắp xếp lại bảng điểm cao giảm dần,
 *          xóa tên người chơi cuối nếu bảng đầy
 *          chuyển sang trạng thái điểm cao để xem bảng điểm
 *
 */
void Game::updateLogic(double deltaTime)
{
    switch (currentState)
    {
    case State::PLAYING:
        updatePlayingLogic(deltaTime);
        break;
    case State::GAME_OVER:
        if (scoreManager.score == 0)
            return;
        if (scoreManager.highScore.size() < scoreManager.maxHighScore || scoreManager.score > scoreManager.highScore.back().second)
        {
            std::string playerName;
            std::cin >> playerName;
            std::cin.clear();
            char ch;
            while (std::cin.get(ch) && ch != '\n');
            if (playerName.length() > 10)
            {
                playerName = playerName.substr(0, 10);
                playerName += "..";
            }
            scoreManager.highScore.push_back({ playerName, scoreManager.score });
            std::sort(scoreManager.highScore.begin(), scoreManager.highScore.end(), [](const auto& a, const auto& b) {
                return a.second > b.second; // Sắp xếp giảm dần
                });
            if (scoreManager.highScore.size() > scoreManager.maxHighScore)
            {
                scoreManager.highScore.pop_back();
            }
            currentState = State::HIGH_SCORES;
            // Lưu điểm cao vào file
            writeDataToBinaryFile(highScoreFilename, scoreManager.highScore);
        }
        break;
    default:
        // Các trạng thái khác không cần cập nhật logic game
        break;
    }
}

/*
 * Hàm xử lý logic trong khi chơi game
 *
 * trong hàng đợi hướng, lấy hướng hợp lệ đầu tiên (khác với hướng hiện tại và hướng ngược lại hướng rắn đi)
 * nếu có hướng hợp lệ mới thì cập nhật hướng vào class rắn
 *
 * lấy vị trí đuôi rắn -> cho rắn di chuyển -> lấy vị trí đầu rắn
 * kiểm tra va chạm rắn sau khi di chuyển
 * nếu rắn không lớn mà đi tiếp thì xóa đuôi rắn cũ
 * cập nhật vị trí đầu rắn và đổi tên đầu rắn cũ thành thân rắn
 *
 * nếu rắn ăn được thức ăn thì rắn cộng 1 điểm, tăng tổng điểm người chơi, tăng tốc rắn, hiển thị thức ăn mới
 *      hiển thị điểm cộng (bonus) nếu có
 *
 * nếu rắn ăn được điểm cộng thì rắn cộng 2 điểm, tăng tổng điểm người chơi, hủy điểm cộng cũ trên màn hình
 *
 * nếu hết thời gian mà chưa ăn được điểm cộng thì xóa điểm cộng trên màn hình
 */
void Game::updatePlayingLogic(double deltaTime)
{
    Direction currentDirection = snake->getDirection();
    Direction wrongDirection = Direction::LEFT;
    Direction newDirection = Direction::NONE;

    switch (currentDirection)   // lấy hướng ngược lại hướng hiện tại của rắn
    {
    case Direction::UP: wrongDirection = Direction::DOWN; break;
    case Direction::DOWN: wrongDirection = Direction::UP; break;
    case Direction::LEFT: wrongDirection = Direction::RIGHT; break;
    }
    // Lấy hướng hợp lệ gần nhất từ hàng đợi
    while (!inputQueue.empty()) {
        Direction tempDirection = inputQueue.front();
        inputQueue.pop_front();
        // nếu hướng lấy ra khác hướng hiện tại hoặc khác hướng sai thì sử dụng hướng đó và thoát vòng lặp 
        if (tempDirection != wrongDirection && tempDirection != currentDirection)
        {
            newDirection = tempDirection;
            break;
        }
    }
    // Nếu có hướng hợp lệ mới, cập nhật hướng của rắn
    if (newDirection != Direction::NONE) {
        snake->setDirection(newDirection);
    }

    // Cho rắn di chuyển
    Point tail = snake->getTail();  // lấy đuôi rắn
    snake->move();                  // di chuyển rắn (cập nhật lại đầu rắn)
    Point head = snake->getHead();  // lấy đầu rắn

    // kiểm tra va chạm khi rắn di chuyển
    checkCollision(head);

    // nếu đuôi rắn có di chuyển thì xóa đuôi rắn
    if (snake->isSnakeMove())
    {
        canvas[tail.y][tail.x] = CELL_TYPE::NONE;
        snake->moveDone();
    }

    // Cập nhật lại vị trí đầu rắn mới và đổi đầu rắn cũ thành thân rắn
    canvas[head.y][head.x] = CELL_TYPE::SNAKE_HEAD;
    canvas[snake->getBody().at(1).y][snake->getBody().at(1).x] = CELL_TYPE::SNAKE;

    // Kiểm tra tình trạng rắn ăn thức ăn 
    if (head == food->getFoodPosition())
    {
        snake->addPoint(2);
        if (currentSpeed > 1) currentSpeed -= 1;
        scoreManager.score += scoreManager.foodScore * currentDifficulty;

        food->spawnFood(canvas);
        canvas[food->getFoodPosition().y][food->getFoodPosition().x] = CELL_TYPE::POINT;

        if (scoreManager.score % scoreManager.bonusScore * currentDifficulty == 0)
        {
            food->spawnBonus(canvas);
            canvas[food->getBonusPosition().y][food->getBonusPosition().x] = CELL_TYPE::POINT;
        }
    }

    // kiểm tra tình trạng rắn ăn điểm cộng
    if (food->isBonusActive() && head == food->getBonusPosition())
    {
        snake->addPoint(4);
        food->deactivateBonus();
        scoreManager.score += scoreManager.bonusScore * currentDifficulty;
    }

    // kiểm tra bonus point hết thời gian chưa để xóa
    Point bonus_pos = food->refreshBonus(deltaTime);
    if (bonus_pos.x != -1) canvas[bonus_pos.y][bonus_pos.x] = CELL_TYPE::NONE;
}

/*
 * tùy trạng thái mà render các màn hình khác nhau
 *
 * Xoá màn hình
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
    if (scoreManager.score > 0 && (scoreManager.highScore.size() < scoreManager.maxHighScore || scoreManager.score > scoreManager.highScore.back().second))
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


/*
 * hàm đặt lại các giá trị trong game về mặc định
 *
 * xóa rắn và bảng cũ nếu có
 * tạo rắn mới ở giữa bảng
 * tạo bảng mới với kích thước đã định
 * điền rắn vào bảng
 * sinh thức ăn mới và điền vào bảng
 * hủy điểm cộng nếu có
 * đặt lại điểm người chơi về 0
 * đặt lại tốc độ về tốc độ ban đầu
 * xóa bộ nhớ tạm hướng đi của người dùng
 */
void Game::resetGame()
{
    if (snake) delete snake;
    snake = new Snake(BOARD_WIDTH / 2, BOARD_HEIGHT / 2);

    canvas.resize(BOARD_HEIGHT, std::vector<CELL_TYPE>(BOARD_WIDTH, CELL_TYPE::NONE));
    for (int x = 0; x < BOARD_WIDTH; ++x)
        for (int y = 0; y < BOARD_HEIGHT; ++y)
            canvas[y][x] = CELL_TYPE::NONE;

    for (int x = 0; x < BOARD_WIDTH; ++x)
    {
        canvas[2][x] = CELL_TYPE::WALL;					// Tường trên
        canvas[0][x] = CELL_TYPE::WALL;					// Tường trên
        canvas[1][x] = CELL_TYPE::WALL;					// Tường trên
        canvas[BOARD_HEIGHT - 1][x] = CELL_TYPE::WALL;	// Tường dưới
    }
    for (int y = 2; y < BOARD_HEIGHT; ++y)
    {
        canvas[y][0] = CELL_TYPE::WALL;					// Tường trái
        canvas[y][BOARD_WIDTH - 1] = CELL_TYPE::WALL;		// Tường phải
    }

    for (const auto& segment : snake->getBody())
    {
        canvas[segment.y][segment.x] = CELL_TYPE::SNAKE;
    }
    canvas[snake->getBody().front().y][snake->getBody().front().x] = CELL_TYPE::SNAKE_HEAD;

    food->spawnFood(canvas);
    canvas[food->getFoodPosition().y][food->getFoodPosition().x] = CELL_TYPE::POINT;
    food->deactivateBonus();

    scoreManager.score = 0;
    currentSpeed = minSpeed;
    inputQueue.clear();
}

/*
 * hàm việt hóa các ký tự tiếng việt trong các trạng thái khác nhau
 *
 * PAUSED: hiển thị chữ "Tạm Dừng"
 * MENU: hiển thị các lựa chọn trong menu
 * PLAYING: hiển thị tiêu đề "Điểm"
 * GAME_OVER: hiển thị thông tin kết thúc trò chơi, nếu người chơi có điểm và đủ để vào bảng điểm cao thì yêu cầu nhập tên
 * HIGH_SCORES: hiển thị tiêu đề "Điểm Cao"
 *
 * sử dụng hàm drawChar của Renderer để vẽ từng ký tự có dấu
 */
void Game::vietSub()
{
    switch (currentState)
    {
    case State::PAUSED:
        renderer->drawChar("ạ", { BOARD_WIDTH / 2 - 4, BOARD_HEIGHT / 2 - 1 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ừ", { BOARD_WIDTH / 2, BOARD_HEIGHT / 2 - 1 }, DEFAULT_HIGHLIGHT_COLOR);
        break;
    case State::MENU:
    {
        Color color = (0 == menuSelection) ? DEFAULT_SELECTED_COLOR : DEFAULT_UNSELECTED_COLOR;
        renderer->drawChar("ò", { 19, 22 }, color);
        renderer->drawChar("ơ", { 23, 22 }, color);
        renderer->drawChar("ớ", { 27, 22 }, color);
        color = (1 == menuSelection) ? DEFAULT_SELECTED_COLOR : DEFAULT_UNSELECTED_COLOR;
        renderer->drawChar("Đ", { 17, 24 }, color);
        renderer->drawChar("ể", { 19, 24 }, color);
        color = (2 == menuSelection) ? DEFAULT_SELECTED_COLOR : DEFAULT_UNSELECTED_COLOR;
        renderer->drawChar("Đ", { 17, 26 }, color);
        renderer->drawChar("ộ", { 18, 26 }, color);
        renderer->drawChar("ó", { 22, 26 }, color);
        color = (3 == menuSelection) ? DEFAULT_SELECTED_COLOR : DEFAULT_UNSELECTED_COLOR;
        renderer->drawChar("ớ", { 19, 28 }, color);
        renderer->drawChar("ệ", { 25, 28 }, color);
        color = (4 == menuSelection) ? DEFAULT_SELECTED_COLOR : DEFAULT_UNSELECTED_COLOR;
        renderer->drawChar("á", { 20, 30 }, color);
        break;
    }
    case State::PLAYING:
        renderer->drawChar("Đ", { 1, 0 }, DEFAULT_TITLE_COLOR);
        renderer->drawChar("ể", { 3, 0 }, DEFAULT_TITLE_COLOR);
        break;
    case State::GAME_OVER:
    {
        int xOffset = 25;
        int yOffset = 20;
        std::string score = "Diem: " + std::to_string(scoreManager.score);
        renderer->drawChar("ò", { xOffset + 7, yOffset }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ơ", { xOffset + 11, yOffset }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ế", { xOffset + 15, yOffset }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ú", { xOffset + 20, yOffset }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("Đ", { (int)(BOARD_WIDTH / 2 - score.length() / 2 - 2), yOffset + 4 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ể", { (int)(BOARD_WIDTH / 2 - score.length() / 2), yOffset + 4 }, DEFAULT_TEXT_COLOR);
        if (scoreManager.score > 0 && (scoreManager.highScore.size() < scoreManager.maxHighScore || scoreManager.score > scoreManager.highScore.back().second))
        {
            renderer->drawChar("ỷ", { xOffset + 1, yOffset + 24 }, DEFAULT_HIGHLIGHT_COLOR);
            renderer->drawChar("ụ", { xOffset + 4, yOffset + 24 }, DEFAULT_HIGHLIGHT_COLOR);
            renderer->drawChar("ớ", { xOffset + 8, yOffset + 24 }, DEFAULT_HIGHLIGHT_COLOR);
            renderer->drawChar("ậ", { xOffset + 14, yOffset + 24 }, DEFAULT_HIGHLIGHT_COLOR);
            renderer->drawChar("ê", { xOffset + 18, yOffset + 24 }, DEFAULT_HIGHLIGHT_COLOR);
            renderer->drawChar("ủ", { xOffset + 22, yOffset + 24 }, DEFAULT_HIGHLIGHT_COLOR);
            renderer->drawChar("ạ", { xOffset + 26, yOffset + 24 }, DEFAULT_HIGHLIGHT_COLOR);
        }
        else
        {
            renderer->drawChar("ấ", { xOffset + 3, yOffset + 24 }, DEFAULT_HIGHLIGHT_COLOR);
            renderer->drawChar("đ", { xOffset + 12, yOffset + 24 }, DEFAULT_HIGHLIGHT_COLOR);
            renderer->drawChar("ể", { xOffset + 13, yOffset + 24 }, DEFAULT_HIGHLIGHT_COLOR);
            renderer->drawChar("ở", { xOffset + 17, yOffset + 24 }, DEFAULT_HIGHLIGHT_COLOR);
            renderer->drawChar("ạ", { xOffset + 20, yOffset + 24 }, DEFAULT_HIGHLIGHT_COLOR);
        }
        break;
    }
    case State::HIGH_SCORES:
    {
        int x_offset = 15;
        int y_offset = 22;
        renderer->drawChar("ể", { x_offset + 4, y_offset - 2 }, DEFAULT_TITLE_COLOR);
        renderer->drawChar("Đ", { x_offset + 2, y_offset - 2 }, DEFAULT_TITLE_COLOR);
        renderer->drawChar("ở", { x_offset + 4, y_offset + 14 }, DEFAULT_SELECTED_COLOR);
        renderer->drawChar("ạ", { x_offset + 7, y_offset + 14 }, DEFAULT_SELECTED_COLOR);
        if (scoreManager.highScore.empty())
        {
            renderer->drawChar("ư", { x_offset + 4, y_offset + 6 }, DEFAULT_TEXT_COLOR);
            renderer->drawChar("ó", { x_offset + 8, y_offset + 6 }, DEFAULT_TEXT_COLOR);
            renderer->drawChar("đ", { x_offset + 10, y_offset + 6 }, DEFAULT_TEXT_COLOR);
            renderer->drawChar("ể", { x_offset + 12, y_offset + 6 }, DEFAULT_TEXT_COLOR);
        }
        break;
    }
    case State::DIFFICULTY_SELECTION:
    {
        int x_offset = 15;
        int y_offset = 22;
        /*việt hóa*/ renderer->drawChar("Đ", { x_offset + 2, y_offset - 2 }, DEFAULT_TITLE_COLOR);
        renderer->drawChar("ộ", { x_offset + 3, y_offset - 2 }, DEFAULT_TITLE_COLOR);
        renderer->drawChar("ó", { x_offset + 7, y_offset - 2 }, DEFAULT_TITLE_COLOR);
        Color color = (1 == difficultSelect) ? DEFAULT_SELECTED_COLOR : DEFAULT_UNSELECTED_COLOR;
        renderer->drawChar("Đ", { x_offset + 2, y_offset + 2 }, color);
        renderer->drawChar("ơ", { x_offset + 3, y_offset + 2 }, color);
        renderer->drawChar("ả", { x_offset + 8, y_offset + 2 }, color);
        color = (2 == difficultSelect) ? DEFAULT_SELECTED_COLOR : DEFAULT_UNSELECTED_COLOR;
        renderer->drawChar("ì", { x_offset + 9, y_offset + 4 }, color);
        color = (3 == difficultSelect) ? DEFAULT_SELECTED_COLOR : DEFAULT_UNSELECTED_COLOR;
        renderer->drawChar("ó", { x_offset + 4, y_offset + 6 }, color);
        break;
    }
    case State::ABOUT:
    {
        int x_offset = 10;
        int y_offset = 4;
        renderer->drawChar("ớ", { x_offset + 2, y_offset + 4 }, DEFAULT_TITLE_COLOR);
        renderer->drawChar("ệ", { x_offset + 8, y_offset + 4 }, DEFAULT_TITLE_COLOR);
        renderer->drawChar("ò", { x_offset + 13, y_offset + 4 }, DEFAULT_TITLE_COLOR);
        renderer->drawChar("ơ", { x_offset + 17, y_offset + 4 }, DEFAULT_TITLE_COLOR);
        renderer->drawChar("à", { x_offset + 2, y_offset + 8 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ừ", { x_offset + 6, y_offset + 8 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ạ", { x_offset + 11, y_offset + 8 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("đ", { x_offset + 14, y_offset + 8 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ế", { x_offset + 15, y_offset + 8 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ớ", { x_offset + 19, y_offset + 8 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ế", { x_offset + 24, y_offset + 8 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ớ", { x_offset + 28, y_offset + 8 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ắ", { x_offset + 32, y_offset + 8 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ă", { x_offset + 36, y_offset + 8 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ồ", { x_offset + 40, y_offset + 8 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ụ", { x_offset + 1, y_offset + 12 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ê", { x_offset + 6, y_offset + 12 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ủ", { x_offset + 10, y_offset + 12 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ạ", { x_offset + 14, y_offset + 12 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ấ", { x_offset + 18, y_offset + 12 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("đ", { x_offset + 21, y_offset + 12 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ơ", { x_offset + 22, y_offset + 12 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ả", { x_offset + 27, y_offset + 12 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("Đ", { x_offset, y_offset + 14 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ề", { x_offset + 2, y_offset + 14 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ể", { x_offset + 8, y_offset + 14 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ắ", { x_offset + 12, y_offset + 14 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("đ", { x_offset + 15, y_offset + 14 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ă", { x_offset + 19, y_offset + 14 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("à", { x_offset + 23, y_offset + 14 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ề", { x_offset + 30, y_offset + 14 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ồ", { x_offset + 34, y_offset + 14 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("à", { x_offset + 38, y_offset + 14 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ố", { x_offset + 43, y_offset + 14 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ă", { x_offset + 4, y_offset + 16 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("đ", { x_offset + 7, y_offset + 16 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ư", { x_offset + 8, y_offset + 16 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ợ", { x_offset + 9, y_offset + 16 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ồ", { x_offset + 13, y_offset + 16 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ắ", { x_offset + 18, y_offset + 16 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ủ", { x_offset + 22, y_offset + 16 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ạ", { x_offset + 26, y_offset + 16 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ẽ", { x_offset + 30, y_offset + 16 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("à", { x_offset + 33, y_offset + 16 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("à", { x_offset + 37, y_offset + 16 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ơ", { x_offset + 46, y_offset + 16 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ộ", { x_offset + 50, y_offset + 16 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ú", { x_offset + 55, y_offset + 16 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ư", { x_offset + 2, y_offset + 18 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ã", { x_offset + 7, y_offset + 18 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ẩ", { x_offset + 11, y_offset + 18 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ậ", { x_offset + 16, y_offset + 18 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ớ", { x_offset + 20, y_offset + 18 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ư", { x_offset + 24, y_offset + 18 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ờ", { x_offset + 25, y_offset + 18 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("à", { x_offset + 30, y_offset + 18 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("í", { x_offset + 34, y_offset + 18 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ả", { x_offset + 39, y_offset + 18 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("â", { x_offset + 44, y_offset + 18 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ì", { x_offset + 48, y_offset + 18 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("á", { x_offset + 1, y_offset + 22 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("đ", { x_offset + 5, y_offset + 22 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ề", { x_offset + 7, y_offset + 22 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ể", { x_offset + 13, y_offset + 22 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ử", { x_offset + 1, y_offset + 24 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ụ", { x_offset + 4, y_offset + 24 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("í", { x_offset + 10, y_offset + 24 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ũ", { x_offset + 14, y_offset + 24 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ê", { x_offset + 18, y_offset + 24 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("đ", { x_offset + 21, y_offset + 24 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ể", { x_offset + 22, y_offset + 24 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ể", { x_offset + 31, y_offset + 24 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ử", { x_offset + 1, y_offset + 26 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ụ", { x_offset + 4, y_offset + 26 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("í", { x_offset + 10, y_offset + 26 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("đ", { x_offset + 17, y_offset + 26 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ể", { x_offset + 18, y_offset + 26 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ạ", { x_offset + 21, y_offset + 26 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ừ", { x_offset + 25, y_offset + 26 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ò", { x_offset + 31, y_offset + 26 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ơ", { x_offset + 35, y_offset + 26 }, DEFAULT_TEXT_COLOR);
        renderer->drawChar("ú", { x_offset + 2, y_offset + 30 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ạ", { x_offset + 6, y_offset + 30 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ơ", { x_offset + 11, y_offset + 30 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawChar("ẻ", { x_offset + 24, y_offset + 30 }, DEFAULT_HIGHLIGHT_COLOR);
        renderer->drawText("> Tro Lai", { x_offset, y_offset + 34 }, DEFAULT_SELECTED_COLOR);
        renderer->drawChar("ở", { x_offset + 4, y_offset + 34 }, DEFAULT_SELECTED_COLOR);
        renderer->drawChar("ạ", { x_offset + 7, y_offset + 34 }, DEFAULT_SELECTED_COLOR);
        break;
    }
    }
}


/*
 * hàm ghi dữ liệu điểm cao vào file nhị phân
 *
 * nếu không mở được file thì trả về false
 * ghi từng cặp tên và điểm vào file
 * trả về true nếu ghi thành công
 */
bool Game::writeDataToBinaryFile(const std::string& filename, const std::vector<std::pair<std::string, int>>& data)
{
    std::ofstream outputFile(filename, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open file '" << filename << "' for binary writing." << std::endl;
        return false;
    }
    for (const auto& entry : data) {
        size_t nameLength = entry.first.length();
        outputFile.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        outputFile.write(entry.first.c_str(), nameLength);
        outputFile.write(reinterpret_cast<const char*>(&entry.second), sizeof(entry.second));
    }
    return true;
}

/*
 * hàm đọc dữ liệu điểm cao từ file nhị phân
 *
 * nếu không mở được file thì trả về vector rỗng
 * đọc từng cặp tên và điểm từ file
 * trả về vector các cặp tên và điểm đã đọc được
 */
std::vector<std::pair<std::string, int>> Game::readDataFromBinaryFile(const std::string& filename)
{
    std::ifstream inputFile(filename, std::ios::binary);
    std::vector<std::pair<std::string, int>> data;
    if (!inputFile.is_open()) {
        // Không cần hiển thị lỗi nếu file chưa tồn tại lần đầu
        return data;
    }
    while (inputFile.peek() != EOF) {
        size_t nameLength;
        inputFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        if (inputFile.gcount() != sizeof(nameLength)) break;

        std::string name(nameLength, '\0');
        inputFile.read(&name[0], nameLength);
        if (inputFile.gcount() != nameLength) break;

        int score;
        inputFile.read(reinterpret_cast<char*>(&score), sizeof(score));
        if (inputFile.gcount() != sizeof(score)) break;

        data.push_back({ name, score });
    }
    return data;
}
