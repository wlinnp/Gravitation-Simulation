//Wai Phyo
//Robert Harris
//CS256-HW5
//Completed on 007/2016

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "Game.h"

//Helper functions

double colliVelX(double m1, double m2, double v1, double v2, double theta1, double theta2, double phi)
{
	return ( ((( (v1 * cos(theta1 - phi) * (m1 - m2)) + (2 * m2 * v2 * cos(theta2 - phi)) ) / (m1 + m2)) * cos( phi )) + ( v1 * sin(theta1 - phi) * cos(phi + (PI / 2.0)) ));
}

double colliVelY(double m1, double m2, double v1, double v2, double theta1, double theta2, double phi)
{
	return ( ((( (v1 * cos(theta1 - phi) * (m1 - m2)) + (2 * m2 * v2 * cos(theta2 - phi)) ) / (m1 + m2)) * sin( phi )) + ( v1 * sin(theta1 - phi) * sin(phi + (PI / 2.0)) ));
}
//End of Helper functions
Game::Game() 
    : width(DEFAULT_WIDTH), height(DEFAULT_HEIGHT), 
      start(0), last(0), current(0), 
      good(true), running(false), 
      particles(std::vector<Particle>())
{
    // Seed the random number generator
    srand(time(0));
    
    // initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        good = false;
        return;
    }
    
    // initialize SDL window
    window = SDL_CreateWindow("Gravity", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        good = false;
        return;
    }
    
    // initialize SDL renderer
    renderer = SDL_CreateRenderer(window, -1, 
            SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        good = false;
        return;
    }
    
    // initialize particle texture
    SDL_Surface* bmp = SDL_LoadBMP("particle.bmp");
    if (bmp == NULL)
    {
        good = false;
        return;
    }
    particleTexture = SDL_CreateTextureFromSurface(renderer, bmp);
    SDL_FreeSurface(bmp);
    if (particleTexture == NULL)
    {
        good = false;
        return;
    }
    
    // initialize our particles
    for (int i = 0; i < PARTICLE_COUNT; ++i)
    {
        particles.push_back(randomParticle());
    }
}

Game::~Game()
{
    if (!good)
    {
        std::cout << "SDL Error: " << SDL_GetError() << std::endl;
    }
    if (particleTexture != NULL)
    {
        SDL_DestroyTexture(particleTexture);
    }
    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

int Game::operator()()
{
    if (!good)
    {
        return -1;
    }
    running = true;
    SDL_Event event;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    start = SDL_GetTicks();
    last = start;

    while (running) // every iteration is 1 frame
    {
        current = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            handleEvent(event);
        }
        update((current - last) / 1000.0);
        render();
        last = current;
    }

    return 0;
}

// dt is time since last frame in seconds
void Game::update(double dt)
{
    	std::cout << dt << " sec since last frame.\n";
	//std::fstream fout ("log.txt");
	bool colliArr[PARTICLE_COUNT][PARTICLE_COUNT];//array to keep which ones are collided
	
	for(int a=0; a< PARTICLE_COUNT; ++a)
	{
		for(int b=0; b< PARTICLE_COUNT; ++b)
		{
			if(a != b)//check that not the particle itself
			{
				double diffX = particles.at(b).getPos().getX() - particles.at(a).getPos().getX();
				double diffY = particles.at(b).getPos().getY() - particles.at(a).getPos().getY();
				double distance = sqrt((diffX*diffX) +  (diffY*diffY));//distance between 2 objects
				double minDistance = particles.at(a).getRadius() + particles.at(b).getRadius(); //equal or less, then they are touching
				double theta = atan2 (diffY , diffX);//the angle of the particle
				if (minDistance < distance)
				{

					double acceleration = G * particles.at(b).getMass() / distance / distance;

					double veloY = acceleration * sin (theta) * dt;
					double veloX = acceleration * cos (theta) * dt;

					Point tempP(particles.at(a).getVelocity().getX() + veloX, particles.at(a).getVelocity().getY() + veloY);
					particles.at(a).setVelocity(tempP);
					colliArr[a][b] = false;
				}
				else
				{
					colliArr[a][b] = true;

					//splitting particles apart
					double overlappedDis = minDistance - distance;
					double overlapX = overlappedDis * cos(theta);
					double overlapY = overlappedDis * sin(theta);
					if( particles.at(b).getPos().getX() > particles.at(a).getPos().getX() )
					{
						overlapX= particles.at(a).getPos().getX() - overlapX;
					}
					else
					{
						overlapX= particles.at(a).getPos().getX() + overlapX;
					}

					if( particles.at(b).getPos().getY() > particles.at(a).getPos().getY() )
					{
						overlapY= particles.at(a).getPos().getY() - overlapY;
					}
					else
					{
						overlapY= particles.at(a).getPos().getY() + overlapY;
					}
					Point tempP( overlapX, overlapY);
					particles.at(a).setPos(tempP);
				}
			}
		}
	}

	for(int x=0;x< PARTICLE_COUNT; ++x)
	{
		for(int y = (x+1); y < PARTICLE_COUNT; ++y)//look only upper half of array
		{
			if(colliArr[x][y] == true)
			{
				double theta1 = atan2 ( particles.at(x).getVelocity().getY() , particles.at(x).getVelocity().getX() );// angle of 1st particle
				double theta2 = atan2 ( particles.at(y).getVelocity().getY() , particles.at(y).getVelocity().getX() );// angle of 2nd particle

				double veloMag1 = sqrt((particles.at(x).getVelocity().getX() * particles.at(x).getVelocity().getX()) +  (particles.at(x).getVelocity().getY() * particles.at(x).getVelocity().getY()));//velocity 1 magnitude
				double veloMag2 = sqrt((particles.at(y).getVelocity().getX() * particles.at(y).getVelocity().getX()) +  (particles.at(y).getVelocity().getY() * particles.at(y).getVelocity().getY()));//velocity 2 magnitude

				double diffPhiX = particles.at(y).getPos().getX() - particles.at(x).getPos().getX();
				double diffPhiY = particles.at(y).getPos().getY() - particles.at(x).getPos().getY();
				double phi = atan2 ( diffPhiY , diffPhiX );// angle of contact

				double newVelX = colliVelX(particles.at(x).getMass(), particles.at(y).getMass(), veloMag1, veloMag2, theta1, theta2, phi);
				double newVelY = colliVelY(particles.at(x).getMass(), particles.at(y).getMass(), veloMag1, veloMag2, theta1, theta2, phi);
				Point tempVel( newVelX , newVelY );


				newVelX = colliVelX(particles.at(y).getMass(), particles.at(x).getMass(), veloMag2, veloMag1, theta2, theta1, (phi+PI));
				newVelY = colliVelY(particles.at(y).getMass(), particles.at(x).getMass(), veloMag2, veloMag1, theta2, theta1, (phi+PI));
				Point tempVel1( newVelX, newVelY );

				particles.at(x).setVelocity(tempVel);
				particles.at(y).setVelocity(tempVel1);
			}
		}
	}
	for (Particle& p : particles)//calculate for all particles
	{
		Point tempV = p.getVelocity();//get velocity vector

		Point tempP(tempV.getX() * dt, tempV.getY() * dt);//temporary position based on velocity
		double minSide = p.getRadius()*2;//top and left border
		double maxWidth = DEFAULT_WIDTH - minSide;//right border
		double maxHeight = DEFAULT_HEIGHT - minSide;//bottom border
		if ((p.getPos().getX() + tempP.getX() ) <= minSide || (p.getPos().getX() + tempP.getX() ) >= maxWidth)
		{
			tempP.setX(tempP.getX() * -1);//change direction
			tempV.setX(tempV.getX() * -1);//change direction
		}
		if ((p.getPos().getY() + tempP.getY() ) <= minSide || (p.getPos().getY() + tempP.getX() ) >= maxHeight)
		{
			tempP.setY(tempP.getY() * -1);//change direction
			tempV.setY(tempV.getY() * -1);//change direction
		}
		p.setVelocity(tempV);
		tempP.setX(tempP.getX() + p.getPos().getX());//update position X
		tempP.setY(tempP.getY() + p.getPos().getY());//update position Y		
		p.setPos(tempP);//update particle's position	
	}
}

void Game::render()
{
    SDL_RenderClear(renderer);
    
    // rendering here would place objects beneath the particles
    
    for (const Particle& p : particles)
    {
        drawParticle(p);
    }
    
    // rendering here would place objects on top of the particles
    
    SDL_RenderPresent(renderer);
}

void Game::handleEvent(const SDL_Event& event)
{
    switch (event.type)
    {
    // Add your own event handling here if desired
    case SDL_QUIT:
        running = false;
        break;
    default:
        break;
    }
}

void Game::drawParticle(const Particle& p)
{
    SDL_Rect dst;
    double shift = p.getRadius();
    dst.x = (int) (p.getPos().getX() - shift);
    dst.y = (int) (p.getPos().getY() - shift);
    dst.w = shift * 2;
    dst.h = shift * 2;
    SDL_RenderCopy(renderer, particleTexture, NULL, &dst);
}

Particle Game::randomParticle() const
{
    Point pos(rand() % width, rand() % height);
    
    // using random percentage of MAX_MASS
    double mass = ((double) rand() / RAND_MAX) * MAX_MASS + 1.0;
    
    return Particle(pos, mass);
}