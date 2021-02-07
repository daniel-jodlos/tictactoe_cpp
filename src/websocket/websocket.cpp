#include <iostream>
#include <thread>
#include <websocket/WebsocketPlayer.h>
#include <websocket/typedefs.h>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

WebsocketPool pool;

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
char id = 'a';

std::tuple<char, std::string, std::string> parse(std::string payload) {
  char id = 0;
  char query[20];
  char params[30];
  std::sscanf(payload.c_str(), "%c;%s;%s", &id, &query, &params);
  return {id, std::string(query), std::string(params)};
}

Game createWebsocketGameObject(
    websocketpp::server<websocketpp::config::asio> *s,
    websocketpp::connection_hdl hdlA, websocketpp::connection_hdl hdlB,
    Board &b) {
  Element elems[] = {X, O};
  const int start = rand() % 2;
  return Game((Player *)new WebsocketPlayer(s, hdlA, elems[start], b),
              (Player *)new WebsocketPlayer(s, hdlB, elems[(start + 1) % 2], b),
              b);
}

void create_game(char playerA, char playerB, server *s) {
  std::thread t([&]() {
    websocketpp::connection_hdl hdlA = pool[playerA].hdl;
    websocketpp::connection_hdl hdlB = pool[playerB].hdl;
    Board b(3);
    Game g = createWebsocketGameObject(s, hdlA, hdlB, b);
    g.resolve();
  });
  t.detach();
}

void on_message(server *s, websocketpp::connection_hdl hdl,
                server::message_ptr msg) {
  std::string payload = msg->get_payload();
  char identifier;
  std::string query, params;
  std::tie(identifier, query, params) = parse(payload);

  if (payload == "requests_id") {
    char response[] = {id++, 0};
    s->send(hdl, response, msg->get_opcode());
    pool.insert({identifier, {identifier, hdl, nullptr, Empty}});
  } else if (query == "disconnect") {
    pool.erase(identifier);
  } else if (query == "play_with") {
    create_game(params[0], identifier, s);
  } else if (query == "game_ended") {
    // end_game(identifier);
    std::clog << "Game requested to end, but we don't give a shit\n";
  }
}

void do_setup_game_server(int argc, char **args) {
  if (argc < 3) {
    std::cerr << "Didn't provide a port\n";
    return;
  }

  int port = std::atoi(args[2]);
  server echo_server;

  try {
    echo_server.set_access_channels(websocketpp::log::alevel::all);
    echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

    echo_server.init_asio();

    echo_server.set_message_handler(
        bind(&on_message, &echo_server, ::_1, ::_2));

    echo_server.listen(port);

    // Start the server accept loop
    echo_server.start_accept();

    // Start the ASIO io_service run loop
    echo_server.run();
  } catch (websocketpp::exception const &e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
    std::cout << "other exception" << std::endl;
  }

  echo_server.stop_listening();
}