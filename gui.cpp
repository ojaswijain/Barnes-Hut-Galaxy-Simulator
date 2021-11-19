#include "particle.cpp"
#include "tree.cpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <Windows.h>

/**
 * @brief GUI version to visualise the Barnes-Hut algorithm
 * Has a runtime of 90 seconds due to SFML limitations 
 */

void HideConsole()
{
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

using std::cout;

signed main(){

    HideConsole();

    /**
     * The code snippet below is for the splash screen
     * Press enter to continue, or wait 5 seconds 
     * Credits: Stack OverFlow
     */

    sf::Texture logo;

    if (!logo.loadFromFile("data/galaxy.jpg")){
        exit(1);
    }

    sf::Sprite sp;
    sp.setTexture(logo);
    sp.scale(0.5, 0.5);

    int logoWidth = sp.getGlobalBounds().width;
    int logoHeight = sp.getGlobalBounds().height;

    sf::RenderWindow window1(sf::VideoMode(logoWidth, logoHeight), "SFML", sf::Style::None); // <- Important!! Style=None removes title

    sf::Clock timer;
    sf::Time time = timer.restart();

    while (window1.isOpen()){
        sf::Event event;
        while (window1.pollEvent(event)){
        }

        time += timer.restart();
        if (time >= sf::seconds(3.f)){
            window1.close();
        }

        window1.clear();
        window1.draw(sp);
        window1.display();
    }


    vector <particle*> ps;

    /**
     * Creating a random set of particles 
     * Inserting them in the particle set ps defined above
     */

    for(int i=0;i<1001;i++){
        particle* p = new particle (610+rand()%700, 240+rand()%600, 0.005*(1+rand()%1000));
        ps.push_back(p);
    }

    /**
     * Defining the window to be of the same size as the screen 
     * Declaring the shapes of the particles and central mass
     */

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Barnes-Hut Implementation");
    sf::CircleShape circle(1.5);
    circle.setFillColor(sf::Color::White);
    sf::CircleShape circle2(20.f);
    circle2.setFillColor(sf::Color::Black);

    /**
     * The code snippet below is to apply the background texture 
     * credits: Official documentation samples
     */

    sf::Texture BackgroundTexture;
    sf::Sprite background;
    sf::Vector2u TextureSize;  //Added to store texture size.
    sf::Vector2u WindowSize;   //Added to store window size.

    if(!BackgroundTexture.loadFromFile("data/bg2.jpg"))
    {
        return -1;
    }
    else
    {
        TextureSize = BackgroundTexture.getSize(); //Get size of texture.
        WindowSize = window.getSize();             //Get size of window.

        float ScaleX = (float) WindowSize.x / TextureSize.x;
        float ScaleY = (float) WindowSize.y / TextureSize.y;     //Calculate scale.

        background.setTexture(BackgroundTexture);
        background.setScale(ScaleX, ScaleY);      //Set scale.  
    }

        /**
         * blank refers to the central particle
         * parameters of blank can be changed to observe different evolution patterns
         * Creating a new tree at the beginning of each iteration (time unit)
         * inserting the particleset in the tree
        */

    particle* blank = new particle (960,540,1e4);
    tree* Tree = new tree(0,2000,0,2000,blank);

    /**
     * SFML code snippet to open and close windows 
     * credits: Official documentation
     */
    
    while (window.isOpen())
    {
        // Defining an event
        sf::Event event;

        // Waiting for the window to be closed
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        {
        
        /**
         * Adding the particle set to the tree
         *Using updateAcc to iteratively update the force on each particle 
         * Using equations of motion for newLoc and newVel 
         */

        Tree->add(ps);
        
        for (auto x : ps){
            Tree->updateAcc(x);
        }

        for (auto x : ps){
            x->newLoc();
            x->newVel();
        }

        /**
         * clearing the screen at the beginning of each iteration 
         * Adding the background now
         */

         window.clear();
         window.draw(background);
         
         /**
          * Setting the position of the central mass 
          */
         float p=blank->X();
         float q=blank->Y();
         circle2.setPosition(p-20.0, q-20.0);
         
         for (auto p : ps){

             /**
              * Drawing the particle on window only if it will be visible on screen (SFML limitation)
              * Resetting the force of the particle 
              */

             float a=p->X();
             float b=p->Y();
             if(a<1920&&b<1080){
                circle.setPosition(a, b);
                window.draw(circle);
             }
            p->resetForce();
        }

        /**
         * Deleting the pointer to the tree  
         * Recursively deleting all child pointers as well.
         */

        Tree->clear();
        delete Tree;

        /**
         * drawing the central particle to overlay on top 
         * display the final canvas at the end of the iteration
         */

        window.draw(circle2);    
        window.display();
     }
    }
    
    return 0;
}

