#ifndef WebsocketPlayer_h
#define WebsocketPlayer_h

#include "../player.h"
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

class WebsocketPlayer : Player {
private:
  std::stringstream _buffer;
  websocketpp::server<websocketpp::config::asio> *server;
  websocketpp::connection_hdl hdl;
  char id;

public:
  WebsocketPlayer(websocketpp::server<websocketpp::config::asio> *,
                  websocketpp::connection_hdl, Element, const Board &);

  /**
   * Receives move of the remote opponent
   * @return Move of the remote opponent
   */
  [[nodiscard]] std::pair<std::size_t, std::size_t> playOn() override;

  /**
   * Sends information about your move to the opponent, who will receive it
   * through playOn. Invoked by Game class
   * @param Coordinates of your move
   * @param Element you played with
   */
  void onOpponentMove(std::pair<std::size_t, std::size_t> pair,
                      Element element) override;

  void sendMove(std::pair<std::size_t, std::size_t>) noexcept;
};

#endif