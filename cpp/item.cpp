#include "../hpp/libs.hpp"

Item::Item(float x, float y, float sizet, float speedb, float jumpb, std::string description, std::string name, std::string fname) : sizet(sizet),
                                                                                                                   speedb(speedb),
                                                                                                                   jumpb(jumpb),
                                                                                                                   name(name),
                                                                                                                   description(description)
{
    this->texture.loadFromFile(fname);
    this->sprite.setTexture(this->texture);
    this->sprite.setPosition(x,y);
}

void Item::applyStats(Player *player)
{
    player->jumpForce *= this->jumpb;
    player->moveSpeed *= this->speedb;
    player->sprite.scale(sizet, sizet);
}

void Item::addStats(Player *player, float speed, float jump)
{
    player->jumpForce += jump;
    player->moveSpeed += speed;
}

std::string Item::customText(){
    return "";
}

void Item::setOneHitInv(Player *player, bool val)
{
    player->onehitinvin = val;
    if (!val)
        player->gothitinv = false;
}

bool Item::getOneHitInv(Player *player)
{
    return player->gothitinv;
}

bool Item::isGrounded(Player* player)
{
    return player->isGrounded;
}

float Item::getJumpForce(Player* player){
    return player->jumpForce;
}

void Item::setStasis(Player* player, bool stasis){
    player->isStasis = stasis;
}

void Item::draw(sf::RenderWindow &window)
{
    window.draw(this->sprite);
}

HB::HB(float x, float y) : Item(x, y, 1, 1.25, 1.1, "Obtain a protective barrier once every 10 seconds,\nthat makes you immune to damage for a brief period of time", "Horus's Brogans", "../imgs/HorusBrogans.png")
{
    this->ctimer.restart();
    this->btimer.restart();
    this->fc = true;
}

void HB::updateO(Player *player)
{

    if (ctimer.getElapsedTime().asSeconds() >= 10.0f)
    {
        setOneHitInv(player, true);
        pinv = true;
    }
    if (pinv && getOneHitInv(player))
    {
        if (fc)
        {
            btimer.restart();
            fc = false;
        }
        else if (btimer.getElapsedTime().asSeconds() >= 0.2f)
        {
            setOneHitInv(player, false);
            fc = true;
            ctimer.restart();
        }
    }
}

bool HB::updateU(Player *player)
{
    if (player->getBounds().intersects(this->sprite.getGlobalBounds()))
    {
        applyStats(player);
        ctimer.restart();
        btimer.restart();
        return true;
    }

    return false;
}

RP::RP(float x, float y) : Item(x, y, 1.0f, 1.0f, 0.85f, "You gain 10 speed for every 300 units of distance traveled,\n up to a max of 200", "Runner's Pact", "../imgs/runnerspact.png"){
    this->xdis = 0;
    this->accsp = 0;
}

bool RP::updateU(Player* player){
    if (player->getBounds().intersects(this->sprite.getGlobalBounds()))
    {
        applyStats(player);
        px = player->getBounds().getPosition().x;
        return true;
    }

    return false;
}

void RP::updateO(Player* player){
    xdis += std::abs(px - player->getBounds().getPosition().x);
    px = player->getBounds().getPosition().x;
    if(accsp < 200 && xdis >= 300.0f){
        addStats(player, 10, 0);
        accsp += 10;
        xdis -= 300.0f;
    }
}

std::string RP::customText(){
    return "Accumulated Speed: "+std::to_string(int(accsp))+"/200";
}

GB::GB(float x, float y) : Item(x, y, 1.0f, 1.0f, 1.35f, "Every third jump has 50% bonus jump power", "Groundbreaker", "../imgs/groundbreaker.png"){
    this->jc = 0;
}

bool GB::updateU(Player* player){
    if (player->getBounds().intersects(this->sprite.getGlobalBounds()))
    {
        applyStats(player);
        isGroundedP = isGrounded(player);
        return true;
    }

    return false;
}

void GB::updateO(Player* player){
    if(!isGroundedP && isGrounded(player)){
        jc++;
        if(jc == 2){
            jpowerg = getJumpForce(player)*0.5f;
            addStats(player, 0, jpowerg);
        }
        else if(jc == 3){
            addStats(player, 0, -jpowerg);
            jc = 0;
        }
    }
    isGroundedP = isGrounded(player);
}

CTP::CTP(float x, float y) : Item(x,y,1,1,1,"Active(2 second cooldown):\n When used, you enter a stasis\nthat renders you invulnerable for 2.5 seconds", "Chronos's Time Piece", "../imgs/chronostimepiece.png"){
    activated = false;
    fc = true;
}

bool CTP::updateU(Player* player){
    if (player->getBounds().intersects(this->sprite.getGlobalBounds()))
    {
        applyStats(player);
        timer.restart();
        return true;
    }

    return false;
}

void CTP::updateO(Player* player){
    if(activated){
        if(fc){
            fc = false;
            setStasis(player, true);
            timer.restart();
        }
        else if(timer.getElapsedTime().asSeconds() >= 2.5f){
            fc = true;
            activated = false;
            setStasis(player, false);
            timer.restart();
        }
    }
}

void CTP::activate(){
    if(timer.getElapsedTime().asSeconds() >= 2.0f)
        activated = true;
}


