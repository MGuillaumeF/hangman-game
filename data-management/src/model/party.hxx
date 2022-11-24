
/**
 * @brief party.hxx DO NOT MODIFY THIS FILE, this file is a generated model
 * class
 */

#ifndef __GENERATED_MODEL_OBJECT_PARTY_HXX__
#define __GENERATED_MODEL_OBJECT_PARTY_HXX__

#include "./chat.hxx"
#include "./root_model_object.hxx"
#include "./word.hxx"

#include <ctime>
#include <memory>
#include <string>
#include <vector>

class user;
class chat;
class word;

/**
 * @brief class of party object in model
 *
 */
#pragma db object session
#pragma db object pointer(std::shared_ptr)
class party final : public root_model_object {
private:
  friend class odb::access;
  std::time_t m_start_date;
  std::time_t m_end_date;
  bool m_win;
#pragma db value_not_null inverse(m_parties)
  std::vector<std::weak_ptr<user>> m_members;
#pragma db not_null
  std::shared_ptr<chat> m_chat;
#pragma db not_null
  std::shared_ptr<word> m_word;

public:
  /**
   * @brief Construct a new party object
   *
   */
  party() = default;

  /**
   * @brief Set the start_date of object
   *
   * @param start_date The start_date of object
   */
  void setStartDate(const std::time_t &start_date) {
    m_start_date = start_date;
  };

  /**
   * @brief Get the start_date of object
   *
   * @return const std::time_t& the start_date of object
   */
  const std::time_t &getStartDate() const { return m_start_date; };

  /**
   * @brief Set the end_date of object
   *
   * @param end_date The end_date of object
   */
  void setEndDate(const std::time_t &end_date) { m_end_date = end_date; };

  /**
   * @brief Get the end_date of object
   *
   * @return const std::time_t& the end_date of object
   */
  const std::time_t &getEndDate() const { return m_end_date; };

  /**
   * @brief Set the win of object
   *
   * @param win The win of object
   */
  void setWin(const bool &win) { m_win = win; };

  /**
   * @brief Get the win of object
   *
   * @return const bool& the win of object
   */
  const bool &getWin() const { return m_win; };

  /**
   * @brief Set the members of object
   *
   * @param members The members of object
   */
  void setMembers(const std::vector<std::weak_ptr<user>> &members) {
    m_members = members;
  };

  /**
   * @brief Get the members of object
   *
   * @return const std::vector<std::weak_ptr<user>>& the members of object
   */
  const std::vector<std::weak_ptr<user>> &getMembers() const {
    return m_members;
  };

  /**
   * @brief Set the chat of object
   *
   * @param chat The chat of object
   */
  void setChat(const std::shared_ptr<chat> &chat) { m_chat = chat; };

  /**
   * @brief Get the chat of object
   *
   * @return const std::shared_ptr<chat>& the chat of object
   */
  const std::shared_ptr<chat> &getChat() const { return m_chat; };

  /**
   * @brief Set the word of object
   *
   * @param word The word of object
   */
  void setWord(const std::shared_ptr<word> &word) { m_word = word; };

  /**
   * @brief Get the word of object
   *
   * @return const std::shared_ptr<word>& the word of object
   */
  const std::shared_ptr<word> &getWord() const { return m_word; };

  /**
   * @brief method to extract object from property tree
   *
   * @return The party found
   */
  static std::unique_ptr<party>
  parse(const boost::property_tree::ptree &property_tree) {
    std::unique_ptr<party> parsedObject =
        root_model_object::parse<party>(property_tree);
    const boost::optional<std::time_t> start_date =
        property_tree.get_optional<std::time_t>("start_date");
    if (start_date) {
      parsedObject->setStartDate(*start_date);
    }
    const boost::optional<std::time_t> end_date =
        property_tree.get_optional<std::time_t>("end_date");
    if (end_date) {
      parsedObject->setEndDate(*end_date);
    }
    const boost::optional<bool> win = property_tree.get_optional<bool>("win");
    if (win) {
      parsedObject->setWin(*win);
    }

    return parsedObject;
  }

  /**
   * @brief method to get object's errors
   *
   * @return The error vector
   */
  std::vector<model_error> getErrors() {
    std::vector<model_error> errors;
    // TODO add implementation
    return errors;
  }

  /**
   * @brief method to get object type
   *
   * @return The object type
   */
  static std::string getObjectType() { return "party"; }

  /**
   * @brief method to get plurial object type
   *
   * @return The plurial object type
   */
  static std::string getPlurialObjectType() { return "parties"; }
};

#pragma db object(party)

#pragma db view object(party)
struct party_stat {
#pragma db column("count(" + party::m_id + ")")
  std::size_t count;
};

#endif // end __GENERATED_MODEL_OBJECT_PARTY_HXX__

#ifdef ODB_COMPILER
#include "./user.hxx"
#endif
