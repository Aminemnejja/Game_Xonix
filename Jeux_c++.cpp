#include <SFML/Graphics.hpp>
#include <time.h>
#include <SFML/Audio.hpp>
using namespace sf;
const int M = 25;
const int N = 40;
int grid[M][N] = { 0 };
int ts = 18; //tile size

struct Enemy
{ int x, y, dx, dy;
    Enemy()
    {    x = y = 300;
        dx = 4 - rand() % 8;
        dy = 4 - rand() % 8;
    }

    void move()
    {
        x += dx; if (grid[y / ts][x / ts] == 1) { dx = -dx; x += dx; }
        y += dy; if (grid[y / ts][x / ts] == 1) { dy = -dy; y += dy; }
    }
};

void drop(int y, int x)
{
    if (grid[y][x] == 0) grid[y][x] = -1;
    if (grid[y - 1][x] == 0) drop(y - 1, x);
    if (grid[y + 1][x] == 0) drop(y + 1, x);
    if (grid[y][x - 1] == 0) drop(y, x - 1);
    if (grid[y][x + 1] == 0) drop(y, x + 1);
}

int main()
{
    srand(time(0));


    RenderWindow window(VideoMode(N * ts, M * ts), "Game");
    window.setFramerateLimit(60);

    sf::RectangleShape restartButton(sf::Vector2f(300.f, 50.f));
    restartButton.setFillColor(sf::Color::Transparent);
    restartButton.setPosition(200.f, 280.f);

    sf::Font font;
    if (!font.loadFromFile("images/cerial.ttf")) 
        return -1;

    sf::Text restartText("Restart Game", font, 24);
    restartText.setPosition(restartButton.getPosition().x + restartButton.getSize().x / 2.f - restartText.getGlobalBounds().width / 2.f,
        restartButton.getPosition().y + restartButton.getSize().y / 2.f - restartText.getGlobalBounds().height / 2.f);
    Music gameMusic;
    Music gameMusic1;
    Music gameMusic2;

    gameMusic1.openFromFile("audio/LETSATRT.ogg");
    gameMusic1.setVolume(100);
    gameMusic1.play();
    gameMusic.openFromFile("audio/Battle_in_the_Stars.ogg");
    gameMusic.setVolume(80);
    gameMusic.play();
    gameMusic.setLoop(true); //loop the audio

    Texture t1, t2, t3, t4;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");
    t4.loadFromFile("images/ball.png");
    Texture backgroundTexture;
    backgroundTexture.loadFromFile("images/backgroundd.png");
    Sprite backgrpundSprite(backgroundTexture);



    Sprite sTile(t1), sGameover(t2), sEnemy(t3), sball(t4);


    sGameover.setPosition(100, 100);
    sEnemy.setOrigin(20, 20);
    sball.setOrigin(20, 20);


    int enemyCount = 4 ;
    Enemy a[10];

    bool Game = true;
    int x = 0, y = 0, dx = 0, dy = 0;
    float timer = 0, delay = 0.07;
    Clock clock;


    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)  grid[i][j] = 1;



    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
  

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
         

            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Escape)
                {
                    for (int i = 1; i < M - 1; i++)
                        for (int j = 1; j < N - 1; j++)
                            grid[i][j] = 0;

                    x = 10; y = 0;
                    Game = true;
                }
            if (e.type == Event::MouseButtonPressed)
            {
                if (e.mouseButton.button == Mouse::Left)
                {
                    Vector2i mousePosition = Mouse::getPosition(window);
                    if (restartButton.getGlobalBounds().contains(mousePosition.x, mousePosition.y))
                    {
                        // Restart the game here
                        for (int i = 1; i < M - 1; i++)
                            for (int j = 1; j < N - 1; j++)
                                grid[i][j] = 0;

                        x = 0; y = 0;
                        Game = true;
                     

                        gameMusic.stop();

                        gameMusic1.openFromFile("audio/LETSATRT.ogg");
                        gameMusic1.setVolume(100);
                        gameMusic1.play();
                        gameMusic.openFromFile("audio/Battle_in_the_Stars.ogg");
                        gameMusic.setVolume(80);
                        gameMusic.play();
                       
                    }
                }
            }
        }

        

        if (Keyboard::isKeyPressed(Keyboard::Left)) { dx = -1; dy = 0; };
        if (Keyboard::isKeyPressed(Keyboard::Right)) { dx = 1; dy = 0; };
        if (Keyboard::isKeyPressed(Keyboard::Up)) { dx = 0; dy = -1; };
        if (Keyboard::isKeyPressed(Keyboard::Down)) { dx = 0; dy = 1; };


        if (!Game) continue;

        if (timer > delay)
        {
            x += dx;
            y += dy;

            if (x < 0) x = 0; if (x > N - 1) x = N - 1;
            if (y < 0) y = 0; if (y > M - 1) y = M - 1;

            if (grid[y][x] == 2) Game = false;
            if (grid[y][x] == 0) grid[y][x] = 2;
            timer = 0;
        }

        for (int i = 0; i < enemyCount; i++) a[i].move();

        if (grid[y][x] == 1)
        {
            dx = dy = 0;

            for (int i = 0; i < enemyCount; i++)
                drop(a[i].y / ts, a[i].x / ts);

            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++)
                    if (grid[i][j] == -1) grid[i][j] = 0;
                    else { grid[i][j] = 1; 
        
            }
        }

        for (int i = 0; i < enemyCount; i++)
            if (grid[a[i].y / ts][a[i].x / ts] == 2) Game = false;

        /////////draw//////////
        window.clear();

        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                if (grid[i][j] == 0) continue;
                if (grid[i][j] == 1) sTile.setTextureRect(IntRect(0, 0, ts, ts));
                if (grid[i][j] == 2) sTile.setTextureRect(IntRect(54, 0, ts, ts));
                sTile.setPosition(j * ts, i * ts);
                window.draw(sTile);
            
            }

        sTile.setTextureRect(IntRect(36, 0, ts, ts));
        sTile.setPosition(x * ts, y * ts);
        window.draw(sTile);

        sEnemy.rotate(10);
        sball.rotate(5);
        for (int i = 0; i < enemyCount; i++)
        {
            if (i % 2 == 0) {
                sEnemy.setPosition(a[i].x, a[i].y);
                window.draw(sEnemy);
            }
            else {
                sball.setPosition(a[i].x, a[i].y);
                window.draw(sball);
            }
        }


        if (!Game) {


            gameMusic.stop();

            gameMusic2.openFromFile("audio/gameOversound.ogg");
            gameMusic2.setVolume(100);
            gameMusic2.play();
            window.clear();
            window.draw(backgrpundSprite);
            window.draw(sGameover);
            window.draw(restartButton);
            window.draw(restartText);


        }
    
      
     
       
                window.display();
            }
            return 0;
        }          
     
   
