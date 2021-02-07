#include <websocket/WebsocketPlayer.h>

WebsocketPlayer::WebsocketPlayer(
    websocketpp::server<websocketpp::config::asio> *s,
    websocketpp::connection_hdl h, Element e, const Board &b)
    : Player(e, b), server(s), hdl(h) {}

std::pair<std::size_t, std::size_t> WebsocketPlayer::playOn() {
  std::size_t x, y;
  _buffer >> x >> y;
  return {x, y};
}

void WebsocketPlayer::onOpponentMove(std::pair<std::size_t, std::size_t> move,
                                     Element element) {
  std::stringstream s;
  s << this->id << ";played;" << move.first << "," << move.second;
  server->send(hdl, s.str().c_str(), websocketpp::frame::opcode::CONTINUATION);
}

void WebsocketPlayer::sendMove(std::pair<std::size_t, std::size_t> a) noexcept {
  _buffer << a.first << a.second;
}