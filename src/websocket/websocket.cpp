#include <iostream>
#include <regex>
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

unsigned int counter = 0;

std::string getID() {
  std::srand(counter++);
  const size_t len = 10;
  std::string result = "";
  for (size_t i = 0; i < len; i++) {
    result += 97 + std::rand() % (123 - 97);
  }

  return std::move(result);
}

std::tuple<std::string, std::string, std::string> parse(std::string payload) {
  static const std::regex regex("([^;]*);([^;]*);(.*)", std::regex::ECMAScript);
  std::smatch m;
  std::regex_match(payload, m, regex);
  std::string id = m[1].str();
  std::string query = m[2].str();
  std::string params = m[3].str();

  return {id, std::string(query), std::string(params)};
}

std::shared_ptr<Game>
createWebsocketGameObject(websocketpp::server<websocketpp::config::asio> *s,
                          websocketpp::connection_hdl hdlA,
                          websocketpp::connection_hdl hdlB, Board &b) {
  Element elems[] = {X, O};
  const int start = rand() % 2;
  return std::make_shared<Game>(
      (Player *)new WebsocketPlayer(s, hdlA, elems[start], b),
      (Player *)new WebsocketPlayer(s, hdlB, elems[(start + 1) % 2], b), b);
}

void create_game(Identifier playerA, Identifier playerB, server *s) {
  s->send(pool[playerA].hdl, playerB + ";play_with;",
          websocketpp::frame::opcode::CONTINUATION);
  std::thread t([&]() {
    websocketpp::connection_hdl hdlA = pool[playerA].hdl;
    websocketpp::connection_hdl hdlB = pool[playerB].hdl;
    Board b(3);
    std::shared_ptr<Game> g = createWebsocketGameObject(s, hdlA, hdlB, b);
    g->setHasFrontend(false);
    pool[playerA].game = g;
    pool[playerA].playsWith = playerB;
    pool[playerB].game = g;
    pool[playerB].playsWith = playerA;
    g->resolve();
  });
  t.detach();
}

void on_play_on(server *s, websocketpp::connection_hdl hdl,
                server::message_ptr msg) {
  std::string payload = msg->get_payload();
  std::string query, params, identifier;
  std::tie(identifier, query, params) = parse(payload);
  const WebsocketPoolElement elem = pool[identifier];
  unsigned int x, y;
  std::sscanf(params.c_str(), "%ud,%ud", &x, &y);

  ((WebsocketPlayer *)elem.game->getPlayer(elem.game->getRoundNumber()))
      ->sendMove({x, y});
}

void end_game(server *s, Identifier idA) {
  WebsocketPoolElement element = pool[idA];
  Identifier idB = element.playsWith;
  auto hdl = element.hdl;
  s->send(hdl, idA + ";game_ended;", websocketpp::frame::opcode::CLOSE);
  pool.erase(idA);
  pool.erase(idB);
}

void on_message(server *s, websocketpp::connection_hdl hdl,
                server::message_ptr msg) {
  std::string payload = msg->get_payload();
  std::clog << payload << '\n';
  std::string query, params, identifier;
  std::tie(identifier, query, params) = parse(payload);

  if (payload == "requests_id") {
    const auto id = getID();
    std::string response = id + ";id;";
    std::clog << "Sent " << response << '\n';
    s->send(hdl, response, msg->get_opcode());
    pool.insert({id, {id, "", hdl, std::shared_ptr<Game>()}});
  } else if (query == "disconnect") {
    pool.erase(identifier);
  } else if (query == "play_with") {
    create_game(params, identifier, s);
  } else if (query == "play_on") {
    on_play_on(s, hdl, msg);
  } else if (query == "game_ended") {
    end_game(s, identifier);
  } else if (query == "set_username") {
    pool[identifier].username = params;
    std::clog << "Username of " << identifier << " is " << params << '\n';
  } else if (query == "username_of") {
    const std::string response = pool[params].username;
    s->send(hdl, response, msg->get_opcode());
  } else {
    std::clog << "No idea what to do with query " << query << '\n';
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