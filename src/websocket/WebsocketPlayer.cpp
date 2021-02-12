#include <websocket/WebsocketPlayer.h>

WebsocketPlayer::WebsocketPlayer(
    websocketpp::server<websocketpp::config::asio> *s,
    websocketpp::connection_hdl h, Element e, const Board &b,
    std::string opponent)
    : Player(e, b), server(s), hdl(h) {
  _send_game_info(opponent);
}

std::pair<std::size_t, std::size_t> WebsocketPlayer::playOn() {
  server->send(hdl, "0;your_turn;" + getPlaysWith(),
               websocketpp::frame::opcode::text);
  return _queue.pop();
}

void WebsocketPlayer::onOpponentMove(std::pair<std::size_t, std::size_t> move,
                                     Element element) {
  std::stringstream s;
  s << this->id << ";played;" << move.first << "," << move.second;
  server->send(hdl, s.str().c_str(), websocketpp::frame::opcode::text);
}

void WebsocketPlayer::sendMove(std::pair<std::size_t, std::size_t> a) noexcept {
  _queue.push(a);
}

void WebsocketPlayer::_send_game_info(std::string &opponent) noexcept {
  std::stringstream ss;
  ss << ";game_info;" << opponent << "," << (getPlaysWith() == X ? 1 : 2);
  server->send(hdl, ss.str(), websocketpp::frame::opcode::text);
}