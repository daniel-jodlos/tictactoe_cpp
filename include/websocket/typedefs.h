#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <game.h>
#include <map>
#include <memory>
#include <websocketpp/server.hpp>

struct WebsocketClient {
  char identifier;
  websocketpp::connection_hdl hdl;
  Game *game;
  Element _elem;
};

typedef WebsocketClient WebsocketPoolElement;
typedef std::map<char, WebsocketPoolElement> WebsocketPool;

extern WebsocketPool pool;

#endif