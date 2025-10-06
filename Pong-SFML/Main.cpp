

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iomanip>
#include <iostream>
#include <cmath>

int getRandomNumber(int fromA, int toB) {
    return fromA + (std::rand() % (toB - fromA + 1));
}

int main() {

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    float BotSpeed = 5.0f;
    bool hadSpecialEffect = false;
  
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pong");
    window.setFramerateLimit(120);

    sf::Music BallCollide;
    if (!BallCollide.openFromFile("Assets/ball.wav")) { std::cout << "Music error"; }
    sf::Music ButtonMusic;
    if (!ButtonMusic.openFromFile("Assets/button1.wav")) { std::cout << "Music error"; }
    sf::Music NewLevel;
    if (!NewLevel.openFromFile("Assets/newlevl.wav")) { std::cout << "MUsic error"; }    

    sf::Texture paddleTex, ballTex;
    paddleTex.loadFromFile("Assets/paddle.png");
    ballTex.loadFromFile("Assets/ball.png");

    sf::RectangleShape paddle1(sf::Vector2f(20.0f, 100.0f));
    sf::RectangleShape paddle2(sf::Vector2f(20.0f, 100.0f));
    sf::CircleShape ball(10.0f);

    paddle1.setTexture(&paddleTex);
    paddle2.setTexture(&paddleTex);
    ball.setTexture(&ballTex);


    enum class GameState {Menu, ChooseDifficulty, Playing, GameOver};
    GameState currentState = GameState::Menu;

    paddle1.setPosition(50.0f, 250.0f);
    paddle2.setPosition(730.0f, 250.0f);
    ball.setPosition(390.0f, 290.0f);

    sf::Font font;
    if (!font.loadFromFile("Assets/Font/Pixellari.ttf")) {
        std::cout << "What the heck" << std::endl;
    }

    sf::Text effectText;
    effectText.setFont(font);
    effectText.setCharacterSize(50);
    effectText.setFillColor(sf::Color::Green);
    effectText.setPosition(250, 200);
    effectText.setString("");

    int Score1 = 0;
    sf::Text scoreText1;
    scoreText1.setFont(font);
    scoreText1.setString("P1       " + std::to_string(Score1) + "   -");
    scoreText1.setCharacterSize(30);
    scoreText1.setFillColor(sf::Color::White);
    scoreText1.setPosition(240.0f, 10.0f);

    int Score2 = 0;
    sf::Text scoreText2;
    scoreText2.setFont(font);
    scoreText2.setString("-   " + std::to_string(Score2) + "       P2");
    scoreText2.setCharacterSize(30);
    scoreText2.setFillColor(sf::Color::White);
    scoreText2.setPosition(400.0f, 10.0f);

    float enduranceClock = 0.0f;
    sf::Text enduranceClockText;
    enduranceClockText.setFont(font);
    enduranceClockText.setCharacterSize(40);
    enduranceClockText.setFillColor(sf::Color::Green);
    enduranceClockText.setPosition(250.0f, 10.0f);

    sf::Text title("PONG", font, 50);
    title.setPosition(325, 50);
    title.setFillColor(sf::Color::White);

sf::FloatRect bounds;
    sf::Text startText("Start Game", font, 30);
    startText.setPosition(320, 200);

    sf::Text exitText("Exit", font, 30);
    exitText.setPosition(370, 300);

    sf::Text normalDifficulty("Normal", font, 30);
    normalDifficulty.setPosition(320, 200);

    sf::Text hardDifficulty("Hard", font, 30);
    hardDifficulty.setPosition(320, 300);

    sf::Text impossibleDifficulty("Impossible", font, 30);
    impossibleDifficulty.setPosition(320, 400);

    sf::Text enduranceDifficulty("Endurance", font, 30);
    enduranceDifficulty.setPosition(320, 500);

        bounds = normalDifficulty.getLocalBounds();
        normalDifficulty.setPosition(400 - bounds.width / 2, 200);

        bounds = hardDifficulty.getLocalBounds();
        hardDifficulty.setPosition(400 - bounds.width / 2, 300);

        bounds = impossibleDifficulty.getLocalBounds();
        impossibleDifficulty.setPosition(400 - bounds.width / 2, 400);

        bounds = enduranceDifficulty.getLocalBounds();
        enduranceDifficulty.setPosition(400 - bounds.width / 2, 500);

        bounds = title.getLocalBounds();
        title.setPosition(400 - (bounds.left + bounds.width / 2), 50);

        bounds = effectText.getLocalBounds();
        effectText.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
        effectText.setPosition(400, 200);

        bounds = enduranceClockText.getLocalBounds();
        enduranceClockText.setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2);
        enduranceClockText.setPosition(400, 30);

        sf::RectangleShape background(sf::Vector2f(800, 600));
            
        sf::Texture backgroundTex;
        backgroundTex.loadFromFile("Assets/background.png");
        background.setTexture(&backgroundTex);


    int selectedDifficulty = 0;
    int selectedOption = 0;

    bool isPlayPressed = false;
    float EnterCooldown = 0.0f;
    bool effectActive = false;
    bool enduranceModebool = false;

    sf::Vector2f BallVelocity(300.0f, 300.0f);

    sf::Clock clock;
    float PaddleSpeed = 450.0f;

    //WHile loop

    while (window.isOpen()) {
        sf::Event event;

        float dt = clock.restart().asSeconds();
        EnterCooldown -= dt;
        if (EnterCooldown < 0.0f) {
            EnterCooldown = 0.0f;
        }

        if (enduranceModebool) {
            enduranceClock += dt;
            enduranceClock = std::round(enduranceClock * 100.0f) / 100.0f;
            
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(2) << enduranceClock;
            enduranceClockText.setString("Time: " + ss.str());
        }

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
            window.close();

    if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Escape) {
        switch (currentState) {
            case GameState::Playing:
                currentState = GameState::ChooseDifficulty;
                break;
            case GameState::ChooseDifficulty:
                currentState = GameState::Menu;
                break;
            case GameState::Menu:
                window.close();
                break;
            }
        }
        }
        }

        if (Score1 % 5 == 0 && !hadSpecialEffect && currentState == GameState::Playing) {
            int r = getRandomNumber(0, 1); // 0 or 1
            if (r == 0) {
                effectText.setString("Longer Bar!");
                paddle1.setSize(sf::Vector2f(20.0f, 300.0f)); // apply effect
                EnterCooldown = 10.0f;    // effect lasts 10 seconds
                effectActive = true;
            }
            hadSpecialEffect = true;
        }

        if (effectActive) {
            if (EnterCooldown <= 0.0f) {
                effectActive = false;
                effectText.setString("");
                paddle1.setSize(sf::Vector2f(20.0f, 100.0f)); // revert to normal
            }
        }
        if (Score1 % 5 != 0) {
            hadSpecialEffect = false;
        }

        if (!enduranceModebool) {
        float BotSpeedDiff = ball.getPosition().y - paddle2.getPosition().y;
        paddle2.move(0, BotSpeedDiff * dt * BotSpeed);
        } else {
            paddle2.setPosition(paddle2.getPosition().x, ball.getPosition().y);
        }

        ball.move(BallVelocity * dt);
        if (ball.getPosition().y <= 0 || ball.getPosition().y + ball.getRadius() * 2 >= 600) {
            BallVelocity.y = -BallVelocity.y;
        }



if (currentState == GameState::Menu) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        selectedOption = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        selectedOption = 1;

    if (!isPlayPressed) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            if (selectedOption == 0) {
                currentState = GameState::ChooseDifficulty;
                isPlayPressed = true;
                EnterCooldown += 0.2f;
            } else {
                window.close();
            }
        }
    } else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        isPlayPressed = false;
    }
}


if (currentState == GameState::ChooseDifficulty) {

    if (EnterCooldown <= 0.0f) {
        // DOWN key
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            selectedDifficulty = std::min(selectedDifficulty + 1, 3);
            EnterCooldown += 0.2f; // add small delay between moves
        }

        // UP key
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            selectedDifficulty = std::max(selectedDifficulty - 1, 0);
            EnterCooldown += 0.2f; // add small delay between moves
        }

        // ENTER key
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            switch (selectedDifficulty) {
                case 0: BotSpeed = 5.0f; enduranceModebool = false; break;
                case 1: BotSpeed = 13.5f; enduranceModebool = false; break;
                case 2: BotSpeed = 18.0f; enduranceModebool = false; break;
                case 3: BotSpeed = 0.0f; enduranceModebool = true; enduranceClock = 0.0f; break;
            }
            currentState = GameState::Playing;
            EnterCooldown += 2.0f; // prevent accidental skipping through menu
        }
    }
}



        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            paddle1.move(0.0f, -PaddleSpeed * dt);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            paddle1.move(0.0f, PaddleSpeed * dt);
        }


        if (paddle1.getPosition().y < 0) {
            paddle1.setPosition(paddle1.getPosition().x, 0);
        }
        if (paddle1.getPosition().y + paddle1.getSize().y > 600) {
            paddle1.setPosition(paddle1.getPosition().x, 600 - paddle1.getSize().y);
        }
        if (paddle2.getPosition().y < 0) {
            paddle2.setPosition(paddle2.getPosition().x, 0);
        }
        if (paddle2.getPosition().y + paddle2.getSize().y > 600) {
            paddle2.setPosition(paddle2.getPosition().x, 600 - paddle2.getSize().y);
        }

    if (currentState == GameState::Playing) {
        //Collision stuff
        if (ball.getGlobalBounds().intersects(paddle1.getGlobalBounds())) {
            BallVelocity.x = std::abs(BallVelocity.x);
            BallCollide.play();
            float PaddleCenter = paddle1.getPosition().y + paddle1.getSize().y / 2;
            float BallCenter = ball.getPosition().y + ball.getRadius();
            float Diff = BallCenter - PaddleCenter;
            BallVelocity.y = Diff * 5;
            BallVelocity.x += 10;
            BallVelocity.y += 10;
        }
        if (ball.getGlobalBounds().intersects(paddle2.getGlobalBounds())) {
            BallVelocity.x = -std::abs(BallVelocity.x);
            BallCollide.play();
            float PaddleCenter = paddle2.getPosition().y + paddle2.getSize().y / 2;
            float BallCenter = ball.getPosition().y + ball.getRadius();
            float Diff = BallCenter - PaddleCenter;
            BallVelocity.y = Diff * 5;
            BallVelocity.x -= 10;
            BallVelocity.y -= 10;
        }   

        if (ball.getPosition().x > 800) {
            ball.setPosition(390.0f, 290.0f);

            BallVelocity.x = (ball.getPosition().x < 0) ? 300.0f : -300.0f;
            BallVelocity.y = 300.0f;
            if (!enduranceModebool) {
            Score1 += 1;
            scoreText1.setString("P1       " + std::to_string(Score1) + "   -");
            scoreText2.setFillColor(sf::Color::White);
            scoreText1.setFillColor(sf::Color::Yellow);
            } else { enduranceClock = 0.0f; }
            BallVelocity.x = 300.0f;
            BallVelocity.y = 300.0f;

        } else if (ball.getPosition().x < 0) {
            ball.setPosition(390.0f, 290.0f);

            BallVelocity.x = (ball.getPosition().x < 0) ? 300.0f : -300.0f;
            BallVelocity.y = 300.0f;
            if (!enduranceModebool) {
            Score2 += 1;
            scoreText2.setString("-   " + std::to_string(Score2) + "       P2");
            scoreText2.setFillColor(sf::Color::Yellow);
            scoreText1.setFillColor(sf::Color::White);
            } else { enduranceClock = 0.0f; }
            BallVelocity.x = 300.0f;
            BallVelocity.y = 300.0f;

        }
    }

        switch (selectedDifficulty) {
            case 0:
            normalDifficulty.setFillColor(sf::Color::Green);
            hardDifficulty.setFillColor(sf::Color::White);
            impossibleDifficulty.setFillColor(sf::Color::White);
            enduranceDifficulty.setFillColor(sf::Color::White);
            break;
            case 1:
            normalDifficulty.setFillColor(sf::Color::White);
            hardDifficulty.setFillColor(sf::Color::Yellow);
            impossibleDifficulty.setFillColor(sf::Color::White);
            enduranceDifficulty.setFillColor(sf::Color::White);
            break;
            case 2:
            normalDifficulty.setFillColor(sf::Color::White);
            hardDifficulty.setFillColor(sf::Color::White);
            impossibleDifficulty.setFillColor(sf::Color::Red);
            enduranceDifficulty.setFillColor(sf::Color::White);
            break;
            case 3:
            normalDifficulty.setFillColor(sf::Color::White);
            hardDifficulty.setFillColor(sf::Color::White);
            impossibleDifficulty.setFillColor(sf::Color::White);
            enduranceDifficulty.setFillColor(sf::Color(180, 0, 255));
            
        }

        if (currentState == GameState::Menu) {

            window.clear();

            window.draw(background);
            window.draw(title);
            window.draw(startText);
            window.draw(exitText);

        if (selectedOption == 0) {
            startText.setFillColor(sf::Color::Yellow);
            exitText.setFillColor(sf::Color::White);
        } else if (selectedOption == 1) {
            startText.setFillColor(sf::Color::White);
            exitText.setFillColor(sf::Color::Yellow);
        }

        } else if (currentState == GameState::Playing) {
            window.clear();

            window.draw(background);
            window.draw(paddle1);
            window.draw(ball);
            window.draw(paddle2);

            if (!enduranceModebool) {
            window.draw(scoreText1);
            window.draw(scoreText2);
            } else {
                window.draw(enduranceClockText);
            }
            window.draw(effectText);
        } else if (currentState == GameState::ChooseDifficulty) {
            window.clear();
            window.draw(background);
            window.draw(title);

            window.draw(normalDifficulty);
            window.draw(hardDifficulty);
            window.draw(impossibleDifficulty);
            window.draw(enduranceDifficulty);

        }

        window.display();
    }

    return 0;
}