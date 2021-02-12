#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <game.h>
#include <map>
#include <memory>
#include <websocketpp/server.hpp>

typedef std::string Identifier;

struct WebsocketClient {
  Identifier identifier;
  websocketpp::connection_hdl hdl;
  std::shared_ptr<Game> game;
  Identifier playsWith;
};

typedef WebsocketClient WebsocketPoolElement;
typedef std::map<Identifier, WebsocketPoolElement> WebsocketPool;

extern WebsocketPool pool;

#endif