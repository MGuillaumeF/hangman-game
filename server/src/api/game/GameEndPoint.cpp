#include "GameEndPoint.hpp"

#include <boost/property_tree/ptree.hpp>

#include "../../common/HTTP/Exception/ParsingException.hpp"
#include "../../common/HTTP/Utils.hpp"
#include "../../common/Logger/Logger.hpp"
#include <regex>

/**
 * @brief Construct a new Http Game Endpoint object
 *
 * @param req The request to handle
 */
GameEndPoint::GameEndPoint(
    const boost::beast::http::request<boost::beast::http::string_body> &req)
    : http::RestrictiveEndpoint(
          req,
          // Only POST and DELETE method are allowed
          {boost::beast::http::verb::post, boost::beast::http::verb::patch}) {}

/**
 * @brief Method to overwrite POST method
 *
 */
void GameEndPoint::doPost() {
  m_logger->info("HTTP_DATA_READ", "GameEndPoint - doPost - first call");
  // get HTTP request
  const boost::beast::http::request<boost::beast::http::string_body> l_request =
      this->getRequest();

  // get ptree of body by content-type parsing
  const boost::property_tree::ptree l_requestBodyTree =
      http::Utils::getBodyTree(l_request);

  const std::string l_target = std::string(l_request.target());
  if (l_target.compare("/api/game/start") == 0) {

    m_logger->info("HTTP_DATA_READ",
                   "GameEndPoint - doPost - start party asked");

    // READ property tree content
    try {
      const uint64_t l_user_id = l_requestBodyTree.get<uint64_t>("user_id");
    } catch (
        const boost::wrapexcept<boost::property_tree::ptree_bad_path> &ex) {
      // ERROR TYPE : ENOKEY, mandatory key not found
      // FIELD : [NAME, ERROR]
      m_logger->error("HTTP_DATA_READ",
                      "GameEndPoint - doPost - body fail reading "
                      "mandatory field : " +
                          std::string(ex.what()));
      throw ParsingException("body fail reading mandatory field");
    } catch (const boost::wrapexcept<boost::property_tree::ptree_bad_data> &) {
      // ERROR TYPE : EVALUETYPE, key found with bad value type
      // FIELD : [NAME, ERROR]
      throw ParsingException("body has bad value type");
    }
    // read from db min and max id word
    // select random id word (in interval [min, max])
    // read word from database
    // create party object in database
    // return party id and word pattern hided
  } else if (l_target.rfind("/api/game/party/", 0) == 0) {
    std::smatch l_m;
    // search the party number from URL
    std::regex l_re("/api/game/party/(\\d+)/(\\S*)/");
    std::regex_search(l_target, l_m, l_re);
    if (l_m.length() >= 1) {
      // only one parameters to seach from url path
      const uint8_t l_partyIndex = 1;
      const uint8_t l_orderIndex = 2;
      std::string l_order = "none";
      const std::string l_partyNumberStr = l_m[l_partyIndex].str();
      const uint64_t l_partyNumber = std::stoul(l_partyNumberStr, nullptr, 0);
      m_logger->info("HTTP_DATA_READ",
                     "La party à mettre à jour est la partie : " +
                         l_partyNumberStr);
      if (l_m.length() >= 2) {
        l_order = l_m[l_orderIndex].str();
        m_logger->info("HTTP_DATA_READ",
                       "L'ordre à appliquer est : " + l_order);
        if (std::string("letter").compare(l_order) == 0) {
          const std::string l_letter =
              l_requestBodyTree.get<std::string>("letter");
          m_logger->info("HTTP_DATA_READ",
                         "L'ordre à appliquer est : " + l_order +
                             " avec la letter :" + l_letter);
          // search party in database

          // (if party found in database)

          // if party is not failed
          // add letter submitted to played letters list
          // verify if letter is in word or not
          // --> (false) increment failure counter

          // verify if party is failed
          // --> (true) reveal word as pattern
          // --> (false) generate actual pattern of word

          // if party not found in database return 404 error
        }
      }
    } else {
      m_logger->error("HTTP_DATA_READ",
                      "La party à mettre à jour est non trouvé");
    }

    m_logger->info("HTTP_DATA_READ",
                   "GameEndPoint - doPost - update party asked");
  }

  // create http response
  boost::beast::http::response<boost::beast::http::string_body> res{
      boost::beast::http::status::ok, l_request.version()};
  res.set(boost::beast::http::field::server, BOOST_BEAST_VERSION_STRING);
  res.set(boost::beast::http::field::content_type, "text/plain");
  res.keep_alive(l_request.keep_alive());
  res.body() = "OK";
  res.prepare_payload();
  // set generated response
  setResponse(res);
}

/**
 * @brief Destroy the Game Endpoint object
 *
 */
GameEndPoint::~GameEndPoint() = default;
