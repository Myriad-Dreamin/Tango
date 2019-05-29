#include "GameAutomationRelayer.h"

#include "RemoteClient.h"

GameAutomationRelayer::GameAutomationRelayer(RemoteClient *client, QObject *parent) : AbstractGameAutomation(parent)
{
    this->client = client;
    connect(client, &RemoteClient::game_signal_failed, this, &GameAutomationRelayer::emit_failed);
    connect(client, &RemoteClient::game_signal_elasped, this, &GameAutomationRelayer::emit_elasped);
    connect(client, &RemoteClient::game_signal_success, this, &GameAutomationRelayer::emit_success);
    connect(client, &RemoteClient::game_signal_new_tango, this, &GameAutomationRelayer::emit_new_tango);
    connect(client, &RemoteClient::game_signal_start_game, this, &GameAutomationRelayer::emit_start_game);
    connect(client, &RemoteClient::game_signal_tango_faded, this, &GameAutomationRelayer::emit_tango_faded);
    connect(client, &RemoteClient::game_signal_answer_failed, this, &GameAutomationRelayer::emit_answer_failed);
}

GameAutomationRelayer::~GameAutomationRelayer()
{
    disconnect(client, nullptr, this, nullptr);
    disconnect(this, nullptr, client, nullptr);
}

bool GameAutomationRelayer::start()
{
    this->client->game_start();
    emit start_game();
    return true;
}

void GameAutomationRelayer::set_error(QString error)
{
    this->_last_error = error;
}



void GameAutomationRelayer::answer_tango(QString tango)
{
    this->client->game_answer(tango);
}

bool GameAutomationRelayer::stop()
{
    this->client->game_stop();
    return true;
}

void GameAutomationRelayer::emit_start_game()
{
    emit start_game();
}

void GameAutomationRelayer::emit_elasped()
{
    emit elasped();
}

void GameAutomationRelayer::emit_new_tango(TangoPair tango, int fade_time)
{
    emit new_tango(tango, fade_time);
}

void GameAutomationRelayer::emit_tango_faded(int answer_time)
{
    emit tango_faded(answer_time);
}

void GameAutomationRelayer::emit_answer_failed()
{
    emit answer_failed();
}

void GameAutomationRelayer::emit_success()
{
    emit success();
    disconnect(client, nullptr, this, nullptr);
    disconnect(this, nullptr, client, nullptr);
}

void GameAutomationRelayer::emit_failed()
{
    emit failed();
    disconnect(client, nullptr, this, nullptr);
    disconnect(this, nullptr, client, nullptr);
}
