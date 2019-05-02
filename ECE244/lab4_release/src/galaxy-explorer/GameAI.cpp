#include <galaxy-explorer/GameAI.hpp>
#include <galaxy-explorer/MyAIData.hpp>

#include <SFML/Graphics.hpp>

#include <iostream>
using namespace std;

GameAI::GameAI(const GameInfo& game_info, sf::RenderTarget* debug_rt) {
    this->debug_rt = debug_rt;
    this->game_info = game_info;
    this->asteroid_observer = AsteroidsObserver(this);
    this->my_game_ai = new MyAIData();
    this->my_game_ai->turn = 0; // Turning CCW
    // customState().debug_on = false;
}

GameAI::~GameAI() {
    delete my_game_ai;
}

SuggestedAction GameAI::suggestAction(const ShipState& ship_state) {
    debug_rt->clear(sf::Color::Transparent);

    if (customState().debug_on) {
        if (not asteroidsObserver().asteroids().isEmpty()) {
            const sf::IntRect first_ast_hb = asteroidsObserver().asteroids().front().getCurrentHitbox();
            sf::RectangleShape ast_rect(sf::Vector2f(first_ast_hb.width, first_ast_hb.height));
            ast_rect.setPosition(first_ast_hb.left, first_ast_hb.top);
            ast_rect.setOutlineThickness(33.0f); // if lines are too thin, they won't show up sometimes
            ast_rect.setOutlineColor(sf::Color::Yellow);
            ast_rect.setFillColor(sf::Color::Transparent);
            debug_rt->draw(ast_rect);
        }
    }
    
    int param = 30000;
    
    if(this->my_game_ai->turn==1) {
        if(ship_state.millidegree_rotation >= -param) {
            if(ship_state.frames_until_phaser_is_ready==0) return SimpleActions::STOP_YAWING_AND_FIRE;
            else return SimpleActions::START_YAWING_ACW;
        } else {
            this->my_game_ai->turn = -1;
            return SimpleActions::START_YAWING_CW;
        }
    } else if(this->my_game_ai->turn==-1) {
        if(ship_state.millidegree_rotation <= param) {
            if(ship_state.frames_until_phaser_is_ready==0) return SimpleActions::STOP_YAWING_AND_FIRE;
            else return SimpleActions::START_YAWING_CW;
        } else {
            this->my_game_ai->turn = 1;
            return SimpleActions::START_YAWING_ACW;
        }
    } else {
        this->my_game_ai->turn=1;
        return SimpleActions::START_YAWING_ACW;
    }
    // return SimpleActions::NO_CHANGE;
    // return SimpleActions::START_YAWING_CW;
    // return SimpleActions::START_YAWING_ACW;
    // return SimpleActions::STOP_YAWING_AND_FIRE;
    // return SuggestedAction{ SuggestedAction::YawingClockwise, SuggestedAction::FiringTry };
}
