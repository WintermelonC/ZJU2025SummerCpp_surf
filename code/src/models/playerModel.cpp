#include "models/playerModel.h"

PlayerModel::PlayerModel() 
    : m_position(Config::Player::PLAYER_POS),
      m_velocity({0.0f, 0.0f}),
      m_angle(sf::degrees(0.0f)),
      m_state(PlayerState::Center),
      m_hp(Config::Player::PLAYER_HP),
      m_power(Config::Player::PLAYER_POWER),
      m_isTurn(false),
      m_isPower(false) {
}

void PlayerModel::usePower() {
    if (m_power > 0) {
        m_power--;
        m_isPower = true;
        if (m_onPowerChanged) {
            m_onPowerChanged(m_power);
        }
    }
}

void PlayerModel::takeDamage() {
    if (m_hp > 0) {
        m_hp--;
        if (m_onHPChanged) {
            m_onHPChanged(m_hp);
        }
    }
}

void PlayerModel::resetPlayer() {
    m_position = Config::Player::PLAYER_POS;
    m_velocity = {0.0f, 0.0f};
    m_angle = sf::degrees(0.0f);
    m_state = PlayerState::Center;
    m_hp = Config::Player::PLAYER_HP;
    m_power = Config::Player::PLAYER_POWER;
    m_isTurn = false;
    m_isPower = false;
}
