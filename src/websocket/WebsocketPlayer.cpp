#include <websocket/WebsocketPlayer.h>

#define wssend(msg) server->send(hdl, msg, websocketpp::frame::opcode::text)

WebsocketPlayer::WebsocketPlayer(
    websocketpp::server<websocketpp::config::asio> *s,
    websocketpp::connection_hdl h, Element e, const Board &b,
    std::string opponent)
    : Player(e, b), server(s), hdl(h) {
  _send_game_info(opponent);
}

std::pair<std::size_t, std::size_t> WebsocketPlayer::playOn() {
  wssend("your_turn");
  return _queue.pop();
}

void WebsocketPlayer::onOpponentMove(std::pair<std::size_t, std::size_t> move,
                                     Element element) {
  std::stringstream s;
  s << this->id << ";played;" << move.first << "," << move.second;
  wssend(s.str());
}

void WebsocketPlayer::sendMove(std::pair<std::size_t, std::size_t> a) noexcept {
  _queue.push(a);
}

void WebsocketPlayer::_send_game_info(std::string &opponent) noexcept {
  std::stringstream ss;
  ss << ";game_info;" << opponent << "," << (getPlaysWith() == X ? 1 : 2);
  wssend(ss.str());
}

void WebsocketPlayer::onFinish(Element result) const {
  const auto upload = [&](std::string status) {
    wssend(";finished;" + status);
    std::clog << "Sent status " << status << '\n';
  };
  if (result == Empty) {
    upload("drew");
  } else if (result == getPlaysWith()) {
    upload("won");
  } else {
    upload("lost");
  }
}

void WebsocketPlayer::onError(std::string error) const noexcept {
  wssend(";error;" + error);
}

#undef wssend