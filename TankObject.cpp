#include "TankObject.h"

TankObject::TankObject(){
    x_val = 0;
    y_val = 0;

    pos.x = (SCREEN_WIDTH - WIDTH_TANK_OBJECT) / 2;
    pos.y = (SCREEN_HEIGHT - HEIGHT_TANK_OBJECT) / 2;
    pos.w = WIDTH_TANK_OBJECT;
    pos.h = HEIGHT_TANK_OBJECT;

    degrees = 0;
    flipType = SDL_FLIP_NONE;

    speed_bullet = SPEED_BULLET_MAIN_TANK;

    currentRocket = 0;
}
unsigned int TankObject::tank_speed = DEFAULT_SPEED;

int TankObject::bullet_style = TankObject::NORMAL;

TankObject::~TankObject() {
    for (int i = 0; i < bulletOfTankList.size(); ++i) {
        BulletObject* p_bullet = bulletOfTankList.at(i);
        if(p_bullet != NULL){
            delete p_bullet;
            p_bullet = NULL;            
        }
    }
    bulletOfTankList.clear();

    for (int i = 0; i < rocketOfTankList.size(); ++i) {
        BulletObject* p_rocket = rocketOfTankList.at(i);
        if (p_rocket != nullptr) {
            if(gRocketSound != NULL){
                Mix_FreeChunk(gRocketSound);
                gRocketSound = NULL;
            }
            delete p_rocket;
            p_rocket = NULL;
        }
    }
    rocketOfTankList.clear();
}


void TankObject::handleInputAction(SDL_Event e, Mix_Chunk* gBulletSound[NUMBER_OF_BULLET_SOUND], const string& gNameBulletOfMainTank, const string& gNameRocket, const int& currentMethod) {
    if (e.type == SDL_KEYDOWN && currentMethod == ControllerMethod::KEYBOARD) {
        switch (e.key.keysym.sym) {
            case SDLK_w:
                y_val = -1;
                if (x_val == 1) {
                    degrees = 45;
                } else if (x_val == -1) {
                    degrees = -45; 
                } else {
                    degrees = 0; 
                }
                break;
            case SDLK_d:
                x_val = 1;
                if (y_val == -1) {
                    degrees = 45; 
                } else if (y_val == 1) {
                    degrees = 135; 
                } else {
                    degrees = 90; 
                }
                break;
            case SDLK_a:
                x_val = -1;
                if (y_val == -1) {
                    degrees = -45; 
                } else if (y_val == 1) {
                    degrees = -135; 
                } else {
                    degrees = -90; 
                }
                break;
            case SDLK_s:
                y_val = 1;
                if (x_val == 1) {
                    degrees = 135; 
                } else if (x_val == -1) {
                    degrees = -135;
                } else {
                    degrees = 180;
                }
                break;
            default:
                break;
        }
    }else if (e.type == SDL_KEYUP && currentMethod == ControllerMethod::KEYBOARD) {
        switch (e.key.keysym.sym) {
            case SDLK_w:
            case SDLK_s:
                y_val = 0;
                if (x_val == 1) {
                    degrees = 90; 
                } else if (x_val == -1) {
                    degrees = -90; 
                }
                break;
            case SDLK_d:
            case SDLK_a:
                x_val = 0;
                // Cập nhật lại góc xoay khi thả phím di chuyển theo trục x
                if (y_val == 1) {
                    degrees = 180; // Hướng xuống dưới
                } else if (y_val == -1) {
                    degrees = 0; // Hướng lên trên
                }
                break;
            default:
                break;
        }
    }
    if(e.type == SDL_JOYAXISMOTION && currentMethod == ControllerMethod::JOYSTICK){
        if( e.jaxis.which == 0 )
        {                        
            switch(e.jaxis.axis) {
                case 0: // X axis motion
                    if( e.jaxis.value < -JOYSTICK_DEAD_ZONE )
                    {
                        x_val = -1;
                        if (y_val == -1) {
                            degrees = -45; 
                        } else if (y_val == 1) {
                            degrees = -135; 
                        } else {
                            degrees = -90; 
                        }
                    }
                    //Right of dead zone
                    else if( e.jaxis.value > JOYSTICK_DEAD_ZONE )
                    {
                        x_val =  1;
                        if (y_val == -1) {
                            degrees = 45; 
                        } else if (y_val == 1) {
                            degrees = 135; 
                        } else {
                            degrees = 90; 
                        }
                    }
                    else
                    {
                        x_val = 0;
                        if (y_val == 1) {
                            degrees = 180; 
                        } else if (y_val == -1) {
                            degrees = 0; 
                        }
                    }
                    break;
                case 1: // Y axis motion
                    //Below of dead zone
                    if( e.jaxis.value < -JOYSTICK_DEAD_ZONE )
                    {
                        y_val = -1;
                        if (x_val == 1) {
                            degrees = 45; 
                        } else if (x_val == -1) {
                            degrees = -45; 
                        } else {
                            degrees = 0; 
                        }
                    }
                    //Above of dead zone
                    else if( e.jaxis.value > JOYSTICK_DEAD_ZONE )
                    {
                        y_val =  1;
                        if (x_val == 1) {
                            degrees = 135; 
                        } else if (x_val == -1) {
                            degrees = -135;
                        } else {
                            degrees = 180;
                        }
                    }
                    else
                    {
                        y_val = 0;
                        if (x_val == 1) {
                            degrees = 90; 
                        } else if (x_val == -1) {
                            degrees = -90; 
                        }
                    }
                    break;
                default:
                    break;
            }
        }
    }
    if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_CONTROLLERBUTTONDOWN){
        if((currentMethod ==  ControllerMethod::KEYBOARD) ? e.button.button == SDL_BUTTON_LEFT : e.cbutton.button == SDL_CONTROLLER_BUTTON_A){
        
            if(bulletType == TankObject::SPHERE1){
                width_bullet = WIDTH_SPHERE;   height_bullet = HEIGHT_SPHERE;  speed_bullet = SPEED_BULLET_MAIN_TANK;
                Mix_PlayChannel(-1, gBulletSound[1], 0);
            }
            else if(bulletType == TankObject::NEW_1){
                width_bullet = WIDTH_NEW_1;    height_bullet = HEIGHT_NEW_1;   speed_bullet = SPEED_BULLET_MAIN_TANK + 2;
                Mix_PlayChannel(-1, gBulletSound[2], 0);
            }
            else if(bulletType == TankObject::ROUNDABOUT){
                width_bullet = WIDTH_ROUNDABOUT;    height_bullet = HEIGHT_ROUNDABOUT;   speed_bullet = SPEED_BULLET_MAIN_TANK + 1;
                Mix_PlayChannel(-1, gBulletSound[0], 0);
            }
            else if(bulletType == TankObject::TIA_BULLET){
                width_bullet = 38;    height_bullet = 130;   speed_bullet = SPEED_BULLET_MAIN_TANK + 3;
                Mix_PlayChannel(-1, gBulletSound[4], 0);
            }
            else if(bulletType == TankObject::PHI_TIEU){
                width_bullet = 30;    height_bullet = 30;   speed_bullet = SPEED_BULLET_MAIN_TANK + 5;
                Mix_PlayChannel(-1, gBulletSound[3], 0);
            }
            else if(bulletType == TankObject::SUPER_LASER){
                width_bullet = 30;    height_bullet = 60;   speed_bullet = SPEED_BULLET_MAIN_TANK + 7;
                Mix_PlayChannel(-1, gBulletSound[3], 0);                
            }
            else if(bulletType == TankObject::SPEED_BULLET){
                width_bullet = 60;    height_bullet = 60;   speed_bullet = SPEED_BULLET_MAIN_TANK + 3;
                Mix_PlayChannel(-1, gBulletSound[5], 0);
            }
            if(bullet_style == TankObject::NORMAL){
                BulletObject* bullet = new BulletObject();
                bullet->loadIMG(gNameBulletOfMainTank); 
                bullet->setWidthHeight(width_bullet, height_bullet);
                bullet->setx_val(speed_bullet);
                bullet->setDegrees(degrees);
                bullet->setIsMove(true);

                int bullet_start_x = pos.x + WIDTH_TANK_OBJECT / 2 - bullet->getPos().w / 2;
                int bullet_start_y = pos.y + HEIGHT_TANK_OBJECT / 2 - bullet->getPos().h / 2;
                bullet->setPos(bullet_start_x, bullet_start_y); 

                bulletOfTankList.push_back(bullet);                
            }
            else if(bullet_style == TankObject::FOUR_DIRECTIONS_BULLET){
                four_directions_bullet(gNameBulletOfMainTank);
            }
            else if(bullet_style == TankObject::SUPER_BULLET){
                super_bullet(gNameBulletOfMainTank);
            }
            else if(bullet_style == TankObject::BULLET_SPREAD){
                bullet_spread(gNameBulletOfMainTank);
            }
            else if(bullet_style == TankObject::STRAIGHT_BEAM){
                straight_beam(gNameBulletOfMainTank);
            }
            else if(bullet_style == TankObject::TRAP){
                trap(gNameBulletOfMainTank);
            }
        }

        else if((currentMethod == ControllerMethod::KEYBOARD) ? e.button.button == SDL_BUTTON_RIGHT : e.cbutton.button == SDL_CONTROLLER_BUTTON_B){
            if (!rocketOfTankList.empty()) {
                return; 
            }
            if (currentRocket <= 0) {  
                return; 
            }
            currentRocket--;  

            gRocketSound = Mix_LoadWAV(gNameRocketSoundOfTank);
            Mix_VolumeChunk(gRocketSound, 48);
            Mix_PlayChannel(-1, gRocketSound, 0);

            if(rocketType == TankObject::ROCKET){
                width_rocket = 60; height_rocket = 149; speed_rocket = SPEED_ROCKET_MAIN_TANK;
            }
            else if(rocketType == TankObject::ROCKET_2){
                width_rocket = 100; height_rocket = 98; speed_rocket = SPEED_ROCKET_MAIN_TANK + 2;
            }

            BulletObject* rocket = new BulletObject();
            rocket->loadIMG(gNameRocket);
            rocket->setWidthHeight(width_rocket, height_rocket);
            if(rocketType == TankObject::ROCKET)    rocket->setRocketTexture();
            else if(rocketType == TankObject::ROCKET_2)  rocket->set_rocket_2();

            rocket->setx_val(speed_rocket);
            rocket->setDegrees(degrees);
            rocket->setIsMove(true);

            int rocket_start_x = pos.x + WIDTH_TANK_OBJECT / 2 - rocket->getPos().w / 2;
            int rocket_start_y = pos.y + HEIGHT_TANK_OBJECT / 2 - rocket->getPos().h / 2;
            rocket->setPos(rocket_start_x, rocket_start_y);

            rocketOfTankList.push_back(rocket);
        }
    } 
}

void TankObject::handleMove() {
    pos.x += x_val * tank_speed;
    pos.y += y_val * tank_speed;

    if (pos.x < 0) {
        pos.x = SCREEN_WIDTH - pos.w;
    } else if (pos.x + pos.w > SCREEN_WIDTH) {
        pos.x = 0;
    }

    if (pos.y < 0) {
        pos.y = SCREEN_HEIGHT - pos.h;
    } else if (pos.y + pos.h > SCREEN_HEIGHT) {
        pos.y = 0;
    }

    flipType = SDL_FLIP_NONE;
}

void TankObject::runBullet(){  
    for(int i = 0; i < bulletOfTankList.size(); i++){
        BulletObject* p_bullet = bulletOfTankList.at(i);
        if(p_bullet != NULL){
            if(p_bullet->getIsMove()){
                SDL_Rect posBullet = p_bullet->getPos();
                double flipBullet = p_bullet->getDegrees();
                p_bullet->renderCopy(posBullet, flipBullet, NULL, SDL_FLIP_NONE);
                p_bullet->handleMove(SCREEN_WIDTH, SCREEN_HEIGHT); 
            }
            else{
                if(p_bullet != NULL){
                    if(gRocketSound != NULL){
                        Mix_FreeChunk(gRocketSound);
                        gRocketSound = NULL;
                    }
                    delete p_bullet; 
                    p_bullet = NULL;
                    bulletOfTankList.erase(bulletOfTankList.begin() + i);
                }
            }
        }
    }  
}

void TankObject::removeBullet(const int& idx){
    for(int i = 0; i < bulletOfTankList.size(); i++){
        if(idx < bulletOfTankList.size()){

            BulletObject* aBullet = bulletOfTankList.at(idx);
            bulletOfTankList.erase(bulletOfTankList.begin() + idx);

            if(aBullet != NULL){
                delete aBullet;
                aBullet = NULL;
            } 
        }
    }
}

void TankObject::runRocket() {
    for (int i = rocketOfTankList.size() - 1; i >= 0; i--){
        BulletObject* aRocket = rocketOfTankList.at(i);
        if (aRocket != NULL && aRocket->getIsMove()) {
            if(rocketType == TankObject::ROCKET)    aRocket->runRocket();
            else if(rocketType == TankObject::ROCKET_2) aRocket->run_rocket_2(); 
            aRocket->handleMove(SCREEN_WIDTH, SCREEN_HEIGHT); 
            
            if (!aRocket->getIsMove()) {
                removeRocket(i);
            }
        }
    }
}

void TankObject::removeRocket(const int& idx){
    for(int i = 0; i < rocketOfTankList.size(); i++){
        if(idx < rocketOfTankList.size()){
            BulletObject* aRocket = rocketOfTankList.at(idx);
            rocketOfTankList.erase(rocketOfTankList.begin() + idx);
            if(aRocket != NULL){
                aRocket->free();
                aRocket = NULL;
            } 
        }
    }
}

void TankObject::removeAllBullets(){
    for (int i = 0; i < bulletOfTankList.size(); ++i) {
        BulletObject* p_bullet = bulletOfTankList.at(i);
        if(p_bullet != NULL){
            delete p_bullet;
            p_bullet = NULL;            
        }
    }
    bulletOfTankList.clear();

    for (int i = 0; i < rocketOfTankList.size(); ++i) {
        BulletObject* p_rocket = rocketOfTankList.at(i);
        if (p_rocket != nullptr) {
            if(gRocketSound != NULL){
                Mix_FreeChunk(gRocketSound);
                gRocketSound = NULL;
            }
            delete p_rocket;
            p_rocket = NULL;
        }
    }
    rocketOfTankList.clear();
}
int TankObject::frame = 0;

void TankObject::Set_sprite_clips(){
    spriteTank_1[0].x = 0;
    spriteTank_1[0].y = 0;
    spriteTank_1[0].w = 70;
    spriteTank_1[0].h = 67;

    spriteTank_1[1].x = 70;
    spriteTank_1[1].y = 0;
    spriteTank_1[1].w = 70;
    spriteTank_1[1].h = 67;

    spriteTank_1[2].x = 140;
    spriteTank_1[2].y = 0;
    spriteTank_1[2].w = 70;
    spriteTank_1[2].h = 67;
    
    spriteTank_1[3].x = 210;
    spriteTank_1[3].y = 0;
    spriteTank_1[3].w = 70;
    spriteTank_1[3].h = 67;

    spriteTank_1[4].x = 280;
    spriteTank_1[4].y = 0;
    spriteTank_1[4].w = 70;
    spriteTank_1[4].h = 67;
}

void TankObject::runMainTank(){
    SDL_Rect* currentClip = &spriteTank_1[frame / TANK_1_FRAME];
    SDLCommonFunc::render_for_sprite(p_object, pos.x, pos.y, currentClip, degrees, NULL, SDL_FLIP_NONE);
    SDL_RenderPresent(gRenderer);
       frame++; 
    if( (frame / TANK_1_FRAME) >= TANK_1_FRAME )
    {
        frame = 0;
    }
}

void TankObject::four_directions_bullet(string gNameBulletOfMainTank) {
    for (int i = 0; i < 4; i++) {
        BulletObject* new_bullet = new BulletObject();
        bool check = new_bullet->loadIMG(gNameBulletOfMainTank);
        new_bullet->setIsMove(true);
        new_bullet->setWidthHeight(width_bullet, height_bullet);
        new_bullet->setDegrees((i*90)%360);
        new_bullet->setx_val(speed_bullet);

        int bullet_start_x = pos.x + pos.w / 2 - new_bullet->getPos().w / 2;
        int bullet_start_y = pos.y + pos.h / 2 - new_bullet->getPos().h / 2;
        new_bullet->setPos(bullet_start_x, bullet_start_y ); 

        bulletOfTankList.push_back(new_bullet);
    }
}

void TankObject::run_four_bullet(const int& x_limit, const int& y_limit) {
    for(int i = 0; i < bulletOfTankList.size(); i++) {
        BulletObject* aBullet = bulletOfTankList.at(i);
        if(aBullet != NULL) {
            if(aBullet->getIsMove()) {
                aBullet->renderCopy(aBullet->getPos(), aBullet->getDegrees(), NULL, SDL_FLIP_NONE);
                aBullet->handleMove(x_limit, y_limit);
            }
            else {
                if(aBullet != NULL) {
                    delete aBullet;
                    aBullet = NULL;
                    bulletOfTankList.erase(bulletOfTankList.begin() + i);
                }
            }
        }
    }        
}

void TankObject::super_bullet(string gNameBulletOfMainTank) {
    for (int i = 0; i < 4; i++) {
        BulletObject* new_bullet = new BulletObject();
        bool check = new_bullet->loadIMG(gNameBulletOfMainTank);
        new_bullet->setIsMove(true);
        new_bullet->setWidthHeight(width_bullet, height_bullet);
        new_bullet->setx_val(speed_bullet);

        int bullet_start_x = pos.x + pos.w / 2 - new_bullet->getPos().w / 2;
        int bullet_start_y = pos.y + pos.h / 2 - new_bullet->getPos().h / 2;
        new_bullet->setPos(bullet_start_x, bullet_start_y); 

        bulletOfTankList.push_back(new_bullet);
    }
}

void TankObject::run_super_bullet(const int& x_limit, const int& y_limit) {
    for(int i = 0; i < bulletOfTankList.size(); i++) {
        BulletObject* aBullet = bulletOfTankList.at(i);
        if(aBullet != NULL) {
            if(aBullet->getIsMove()) {
                aBullet->setDegrees(i*90 + 90);
                aBullet->renderCopy(aBullet->getPos(), aBullet->getDegrees());
                aBullet->handleMove(x_limit, y_limit);
            }
            else {
                if(aBullet != NULL) {
                    delete aBullet;
                    aBullet = NULL;
                    bulletOfTankList.erase(bulletOfTankList.begin() + i);
                }
            }
        }
    }        
}

void TankObject::bullet_spread(string gNameBulletOfMainTank){
    for (int i = 0; i < 3; i++) {
        BulletObject* new_bullet = new BulletObject();
        bool check = new_bullet->loadIMG(gNameBulletOfMainTank);
        new_bullet->setIsMove(true);
        new_bullet->setWidthHeight(width_bullet, height_bullet); 
        new_bullet->setx_val(speed_bullet);

        new_bullet->setDegrees(degrees + 30 * (i - 1));

        int bullet_start_x = pos.x + pos.w / 2 - new_bullet->getPos().w / 2;
        int bullet_start_y = pos.y + pos.h / 2 - new_bullet->getPos().h / 2;
        new_bullet->setPos(bullet_start_x, bullet_start_y); 

        bulletOfTankList.push_back(new_bullet);
    }
}

void TankObject::run_bullet_spread(const int& x_limit, const int& y_limit){
    for(int i = 0; i < bulletOfTankList.size(); i++) {
        BulletObject* aBullet = bulletOfTankList.at(i);
        if(aBullet != NULL) {
            if(aBullet->getIsMove()) {
                aBullet->renderCopy(aBullet->getPos(), aBullet->getDegrees());
                aBullet->handleMove(x_limit, y_limit);
            }
            else {
                if(aBullet != NULL) {
                    delete aBullet;
                    aBullet = NULL;
                    bulletOfTankList.erase(bulletOfTankList.begin() + i);
                }
            }
        }
    }         
}

void TankObject::straight_beam(string gNameBulletOfMainTank) {
    int spacing = pos.w / 4;

    for (int i = 0; i < 4; i++) {
        BulletObject* new_bullet = new BulletObject();
        bool check = new_bullet->loadIMG(gNameBulletOfMainTank);
        new_bullet->setIsMove(true);
        new_bullet->setWidthHeight(width_bullet, height_bullet);
        new_bullet->setx_val(speed_bullet);
        new_bullet->setDegrees(degrees);
        // Tính toán vị trí bắt đầu của từng viên đạn
        int bullet_start_x = pos.x + pos.w / 2 - new_bullet->getPos().w / 2 + (i - 2) * spacing + spacing / 2;  
        int bullet_start_y = pos.y + pos.h / 2 - new_bullet->getPos().h / 2;
        if(degrees == 90 || degrees == -90){
            bullet_start_x = pos.x + pos.w / 2 - new_bullet->getPos().w / 2;
            bullet_start_y = pos.y + pos.h / 2 - new_bullet->getPos().h / 2 + (i - 2) * spacing + spacing / 2;    
        }
        new_bullet->setPos(bullet_start_x, bullet_start_y); 

        bulletOfTankList.push_back(new_bullet);
    }
}

void TankObject::run_straight_beam(const int& x_limit, const int& y_limit){
    for(int i = 0; i < bulletOfTankList.size(); i++) {
        BulletObject* aBullet = bulletOfTankList.at(i);
        if(aBullet != NULL) {
            if(aBullet->getIsMove()) {
                aBullet->renderCopy(aBullet->getPos(), aBullet->getDegrees());
                aBullet->handleMove(x_limit, y_limit);
            }
            else {
                if(aBullet != NULL) {
                    delete aBullet;
                    aBullet = NULL;
                    bulletOfTankList.erase(bulletOfTankList.begin() + i);
                }
            }
        }
    }         
}

void TankObject::trap(string gNameBulletOfMainTank) {
    int spacing = pos.w / 2;

    for (int i = 0; i < 2; i++) {
        BulletObject* new_bullet = new BulletObject();
        bool check = new_bullet->loadIMG(gNameBulletOfMainTank);
        new_bullet->setIsMove(true);
        new_bullet->setWidthHeight(width_bullet, height_bullet);
        new_bullet->setx_val(speed_bullet);
        new_bullet->setDegrees(degrees + (i*2 - 1)* 45);
        // Tính toán vị trí bắt đầu của từng viên đạn
        int bullet_start_x = pos.x + pos.w / 2 - new_bullet->getPos().w / 2 + (i - 1) * spacing + spacing / 2;  
        int bullet_start_y = pos.y + pos.h / 2 - new_bullet->getPos().h / 2;
        if(degrees == 90 || degrees == -90){
            bullet_start_x = pos.x + pos.w / 2 - new_bullet->getPos().w / 2;
            bullet_start_y = pos.y + pos.h / 2 - new_bullet->getPos().h / 2 + (i - 1) * spacing + spacing / 2;    
        }
        new_bullet->setPos(bullet_start_x, bullet_start_y); 

        bulletOfTankList.push_back(new_bullet);
    }
}

void TankObject::run_trap(const int& x_limit, const int& y_limit){
    for(int i = 0; i < bulletOfTankList.size(); i++) {
        BulletObject* aBullet = bulletOfTankList.at(i);
        if(aBullet != NULL) {
            if(aBullet->getIsMove()) {
                if(i % 2 == 0)
                    aBullet->setDegrees(aBullet->getDegrees() + (i*2 + 1) * 45 );
                else 
                    aBullet->setDegrees(aBullet->getDegrees() + (i*2 - 1) * 45 );
                aBullet->renderCopy(aBullet->getPos(), aBullet->getDegrees());
                aBullet->handleMove(x_limit, y_limit);
            }
            else {
                if(aBullet != NULL) {
                    delete aBullet;
                    aBullet = NULL;
                    bulletOfTankList.erase(bulletOfTankList.begin() + i);
                }
            }
        }
    }         
}
